#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_levitate.h"


LINK_ENTITY_TO_CLASS( ww_spell_levitate, WWLevitateSpell );


void WWLevitateSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId				= WEAPON_LEVITATE;
	m_iDefaultAmmo		= 0;

	FallInit();
}


void WWLevitateSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/levitate.wav"	);
	PRECACHE_MODEL( "models/spellbook.mdl"	);

	m_usEvent = PRECACHE_EVENT( 1, "events/spells/levitate.sc" );
}


int WWLevitateSpell::iItemSlot( void )
{
	return SLOT_LEVITATE + 1;
}


int WWLevitateSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_LEVITATE;
	pInfo->iSlot		= SLOT_LEVITATE;
	pInfo->iPosition	= POS_LEVITATE;
	pInfo->iWeight		= WEIGHT_LEVITATE;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWLevitateSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < LEVITATE_COST )
		return FALSE;
	return TRUE;
}


BOOL WWLevitateSpell::Deploy( void )
{
	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 0, 0, 0, 0 );

	m_pPlayer->pev->gravity = 0.1f;

	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_WIND],
		(char *)szWizardStaves[WWCLASS_WIND],
		HANDS_WIND_LEVITATE_IDLE, "levitate" );
}


void WWLevitateSpell::Holster( int skiplocal )
{
	PLAYBACK_EVENT_FULL( FEV_GLOBAL | FEV_RELIABLE, m_pPlayer->edict(), m_usEvent, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );

	m_pPlayer->pev->gravity = 1.0f;

	CBasePlayerWeapon::Holster( skiplocal );
}



void WWLevitateSpell::PrimaryAttack( void )
{
	WeaponIdle();
}


void WWLevitateSpell::WeaponIdle( void )
{
	// if we don't have enough ammo, put the weapon away
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < LEVITATE_COST )
	{
		RetireWeapon();
		return;
	}

	// if it isn't time to idle
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	// clamp levitate speed if not on ground
	if( !FBitSet( m_pPlayer->pev->flags, FL_ONGROUND ) )
	{
		UTIL_MakeVectors( m_pPlayer->pev->v_angle );

		Vector vVelocity = m_pPlayer->pev->velocity;

		if( m_pPlayer->pev->button & IN_FORWARD )
			vVelocity = vVelocity + gpGlobals->v_forward * LEVITATE_VELOCITY;
		
		if( m_pPlayer->pev->button & IN_BACK )
			vVelocity = vVelocity - gpGlobals->v_forward * LEVITATE_VELOCITY;

		if( m_pPlayer->pev->button & IN_MOVERIGHT )
			vVelocity = vVelocity + gpGlobals->v_right * LEVITATE_VELOCITY;

		if( m_pPlayer->pev->button & IN_MOVELEFT )
			vVelocity = vVelocity - gpGlobals->v_right * LEVITATE_VELOCITY;

		if( m_pPlayer->pev->button & IN_JUMP )
			vVelocity = vVelocity + gpGlobals->v_up * LEVITATE_VELOCITY;

		if( m_pPlayer->pev->button & IN_DUCK )
			vVelocity = vVelocity - gpGlobals->v_up * LEVITATE_VELOCITY;

		if( vVelocity.Length() > LEVITATE_MAXVELOCITY )
			vVelocity = vVelocity.Normalize() * LEVITATE_MAXVELOCITY;

		m_pPlayer->pev->velocity = vVelocity;
	}

	
	// it only takes mana if its not on the ground
	if( UTIL_WeaponTimeBase() >= m_pPlayer->m_flNextAmmoBurn &&
		!FBitSet( m_pPlayer->pev->flags, FL_ONGROUND ) )
	{
		PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent, 0.0f,
			m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 0, 0, 0, 0 );

		m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= LEVITATE_COST;
		m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + LEVITATE_MANABURN;
	}

	m_pPlayer->pev->gaitsequence = m_pPlayer->LookupActivity( ACT_CROUCHIDLE );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + LEVITATE_DELAY;
}

