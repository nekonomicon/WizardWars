#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_doublemissile.h"


LINK_ENTITY_TO_CLASS( ww_spell_doublemissile, WWDoubleMissileSpell );


void WWDoubleMissileSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_DOUBLEMISSILE;
	m_iDefaultAmmo	= 0;
	m_fireState		= 0;

	FallInit();
}


void WWDoubleMissileSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/magicmissile.wav"	);
	PRECACHE_MODEL( "models/spellbook.mdl"	);

	m_usEvent = PRECACHE_EVENT( 1, "events/spells/missile1.sc" );
}



int WWDoubleMissileSpell::iItemSlot( void )
{
	return SLOT_MISSILE + 1;
}


int WWDoubleMissileSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_DOUBLEMISSILE;
	pInfo->iSlot		= SLOT_DOUBLEMISSILE;
	pInfo->iPosition	= POS_DOUBLEMISSILE;
	pInfo->iWeight		= WEIGHT_DOUBLEMISSILE;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWDoubleMissileSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < DOUBLEMISSILE_COST )
		return FALSE;
	return TRUE;
}


BOOL WWDoubleMissileSpell::Deploy( void )
{
	if( m_pPlayer->m_pClass != NULL )
	{
		return DefaultDeploy(
			(char *)m_pPlayer->m_pClass->GetWizardHands(),
			(char *)m_pPlayer->m_pClass->GetWizardStaff(),
			HANDS_MISSILE_IDLE, "dualmagicmissile" );
	}
	else
		return DefaultDeploy( "", "", HANDS_MISSILE_IDLE, "dualmagicmissile" );
}


void WWDoubleMissileSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < DOUBLEMISSILE_COST )
	{
		RetireWeapon();
		return;
	}

	m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= DOUBLEMISSILE_COST;
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	Vector vAngles = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;
	UTIL_MakeVectors( vAngles );

#ifndef CLIENT_DLL
	Vector vOrigin = m_pPlayer->GetGunPosition();
	vOrigin.z -= 8.0f;
	vOrigin = vOrigin + gpGlobals->v_forward * 8.0f;

	if( m_fireState )
		vOrigin = vOrigin + gpGlobals->v_right * 8.0f;
	else
		vOrigin = vOrigin - gpGlobals->v_right * 8.0f;

	CBaseEntity * pMissile = Create( "proj_missile", vOrigin, vAngles, m_pPlayer->edict() );

	if( pMissile != NULL )
	{
		pMissile->pev->angles = UTIL_VecToAngles( gpGlobals->v_forward );
		pMissile->pev->velocity = gpGlobals->v_forward * DOUBLEMISSILE_SPEED;
		pMissile->pev->flags |= FL_SKIPLOCALHOST;
	}
#endif

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, vAngles, 0, 0, 0, 0, m_fireState, 0 );

	m_fireState = !m_fireState;

	m_flNextPrimaryAttack	= UTIL_WeaponTimeBase() + DOUBLEMISSILE_DELAY;
	m_flTimeWeaponIdle		= m_flNextPrimaryAttack;
}


void WWDoubleMissileSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < DOUBLEMISSILE_COST )
	{
		RetireWeapon();
		return;
	}

	m_fireState = 0;

	PLAYBACK_EVENT_FULL( FEV_HOSTONLY | FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}
