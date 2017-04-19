#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_thornblast.h"


LINK_ENTITY_TO_CLASS( ww_spell_thornblast, WWThornBlastSpell );


void WWThornBlastSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_THORNBLAST;
	m_iDefaultAmmo	= 0;

	FallInit();
}


void WWThornBlastSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/thornblast.wav"	);
	PRECACHE_MODEL( "models/spellbook.mdl"	);
	m_usEvent = PRECACHE_EVENT( 1, "events/spells/thornblast.sc" );
}


int WWThornBlastSpell::iItemSlot( void )
{
	return SLOT_THORNBLAST + 1;
}


int WWThornBlastSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_THORNBLAST;
	pInfo->iSlot		= SLOT_THORNBLAST;
	pInfo->iPosition	= POS_THORNBLAST;
	pInfo->iWeight		= WEIGHT_THORNBLAST;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWThornBlastSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < THORNBLAST_COST )
		return FALSE;
	return TRUE;
}


BOOL WWThornBlastSpell::Deploy( void )
{
	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_NATURE],
		(char *)szWizardStaves[WWCLASS_NATURE],
		HANDS_NATURE_THORNBLAST_IDLE, "thornblast" );
}


void WWThornBlastSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < THORNBLAST_COST )
	{
		RetireWeapon();
		return;
	}

	m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= THORNBLAST_COST;
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + THORNBLAST_DELAY;
	m_flTimeWeaponIdle	  = m_flNextPrimaryAttack;

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );

	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecAiming = gpGlobals->v_forward;

	vecSrc = vecSrc - gpGlobals->v_right * 6.0f;

	Vector vDir = m_pPlayer->FireBulletsPlayer( THORNBLAST_COUNT, vecSrc, vecAiming,
		Vector( 0.08f, 0.08f, 0.0f ), 2048, BULLET_PLAYER_BUCKSHOT, -1,
		THORNBLAST_DAMAGE, m_pPlayer->pev, m_pPlayer->random_seed );

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent, 0.0f,
		(float *)&g_vecZero, (float *)&g_vecZero, vDir.x, vDir.y, 0, 0, 0, 0 );
}


void WWThornBlastSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < THORNBLAST_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}