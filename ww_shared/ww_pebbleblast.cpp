#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_pebbleblast.h"


LINK_ENTITY_TO_CLASS( ww_spell_pebbleblast, WWPebbleBlastSpell );


void WWPebbleBlastSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_PEBBLEBLAST;
	m_iDefaultAmmo	= 0;

	FallInit();
}


void WWPebbleBlastSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/pebbleblast.wav"	);
	PRECACHE_MODEL( "models/spellbook.mdl"		);
	m_usEvent = PRECACHE_EVENT( 1, "events/spells/pebbleblast.sc" );
}


int WWPebbleBlastSpell::iItemSlot( void )
{
	return SLOT_PEBBLEBLAST + 1;
}


int WWPebbleBlastSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_PEBBLEBLAST;
	pInfo->iSlot		= SLOT_PEBBLEBLAST;
	pInfo->iPosition	= POS_PEBBLEBLAST;
	pInfo->iWeight		= WEIGHT_PEBBLEBLAST;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWPebbleBlastSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < PEBBLEBLAST_COST )
		return FALSE;
	return TRUE;
}


BOOL WWPebbleBlastSpell::Deploy( void )
{
	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_EARTH],
		(char *)szWizardStaves[WWCLASS_EARTH],
		HANDS_EARTH_PEBBLEBLAST_IDLE, "thornblast" );
}


void WWPebbleBlastSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < PEBBLEBLAST_COST )
	{
		RetireWeapon();
		return;
	}

	m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= PEBBLEBLAST_COST;
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + PEBBLEBLAST_DELAY;
	m_flTimeWeaponIdle	  = m_flNextPrimaryAttack;

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );

	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecAiming = gpGlobals->v_forward;

	vecSrc = vecSrc - gpGlobals->v_right * 6.0f;

	Vector vDir = m_pPlayer->FireBulletsPlayer( PEBBLEBLAST_COUNT, vecSrc, vecAiming,
		Vector( 0.1f, 0.1f, 0.0f ), 2048, BULLET_PLAYER_BUCKSHOT, -1,
		PEBBLEBLAST_DAMAGE, m_pPlayer->pev, m_pPlayer->random_seed );

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent, 0.0f,
		(float *)&g_vecZero, (float *)&g_vecZero, vDir.x, vDir.y, 0, 0, 0, 0 );
}


void WWPebbleBlastSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < PEBBLEBLAST_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}