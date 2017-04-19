#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_dragonbreath.h"


LINK_ENTITY_TO_CLASS( ww_spell_dragonbreath, WWDragonBreathSpell );


void WWDragonBreathSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_DRAGONBREATH;
	m_iDefaultAmmo	= 0;

	m_bFiring		= false;
	m_flPulseTime	= 0.0f;

	FallInit();
}


void WWDragonBreathSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/dragonbreath_loop.wav"	);
	PRECACHE_MODEL( "sprites/smoke.spr"	);

	m_usEvent1 = PRECACHE_EVENT( 1, "events/spells/dragonbreath1.sc" );
	m_usEvent2 = PRECACHE_EVENT( 1, "events/spells/dragonbreath2.sc" );
}


int WWDragonBreathSpell::iItemSlot( void )
{
	return SLOT_DRAGONBREATH + 1;
}


int WWDragonBreathSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_DRAGONBREATH;
	pInfo->iSlot		= SLOT_DRAGONBREATH;
	pInfo->iPosition	= POS_DRAGONBREATH;
	pInfo->iWeight		= WEIGHT_DRAGONBREATH;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWDragonBreathSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < DRAGONBREATH_COST )
		return FALSE;

	return TRUE;
}


BOOL WWDragonBreathSpell::Deploy( void )
{
	m_bFiring = false;
	m_flPulseTime = 0.0f;

	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_DRAGON],
		(char *)szWizardStaves[WWCLASS_DRAGON],
		HANDS_DRAGON_DRAGONBREATH_IDLE, "dragonbreath" );
}


void WWDragonBreathSpell::Holster( int skiplocal )
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;

	if( m_bFiring )
		EndAttack();

	CBasePlayerWeapon::Holster( skiplocal );
}


void WWDragonBreathSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < DRAGONBREATH_COST )
	{
		RetireWeapon();
		return;
	}

	Attack();
}


void WWDragonBreathSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_bFiring )
	{
		EndAttack();
		return;
	}

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < DRAGONBREATH_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( FEV_CLIENT, m_pPlayer->edict(),
		m_usEvent2, 0, m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 0, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}


void WWDragonBreathSpell::Attack( void )
{
	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );
	Vector vAiming = gpGlobals->v_forward;
	Vector vOrigin = m_pPlayer->GetGunPosition();

	if( !m_bFiring )
	{
		m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase();

		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.1f;

		PLAYBACK_EVENT_FULL( FEV_CLIENT, m_pPlayer->edict(), m_usEvent1, 0.0f,
			(float *)&g_vecZero, (float *)&g_vecZero, 0.0f, 0.0f, 1, 0, 0, 0 );

		m_flPulseTime = gpGlobals->time;
		m_bFiring = true;
	}
	else
	{
		Fire( vOrigin, vAiming );

		if( UTIL_WeaponTimeBase() >= m_pPlayer->m_flNextAmmoBurn && m_bFiring )
		{
			m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= DRAGONBREATH_COST;
			m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + DRAGONBREATH_MANABURN;
		}
	}
}


void WWDragonBreathSpell::Fire( const Vector & vOrigin, const Vector & vAiming )
{
	if( gpGlobals->time >= m_flPulseTime )
	{
#ifndef CLIENT_DLL
		Vector vEndPos = vOrigin + vAiming * DRAGONBREATH_RANGE;

		TraceResult tr;
		UTIL_TraceLine( vOrigin, vEndPos, dont_ignore_monsters, m_pPlayer->edict(), &tr );

		ClearMultiDamage();

		::RadiusDamage( tr.vecEndPos, pev, m_pPlayer->pev, DRAGONBREATH_DAMAGE, DRAGONBREATH_RADIUS,
			CLASS_NONE, DMG_BURN | DMG_SLOWBURN );

		ApplyMultiDamage( pev, m_pPlayer->pev );
#endif

		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

		PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent1, 0.0f,
			(float *)&g_vecZero, (float *)&g_vecZero, 0.0f, 0.0f, 0, 0, 0, 0 );

		m_flPulseTime = gpGlobals->time + DRAGONBREATH_DELAY;
	}
}


void WWDragonBreathSpell::EndAttack( void )
{
	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5f;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.5f;

	m_bFiring = false;
}

