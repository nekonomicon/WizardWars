#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_updraft.h"


LINK_ENTITY_TO_CLASS( ww_spell_updraft, WWUpdraftSpell );


void WWUpdraftSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_UPDRAFT;
	m_iDefaultAmmo	= 0;

	FallInit();
}


void WWUpdraftSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/updraft_cast.wav"		);
	PRECACHE_SOUND( "spells/updraft_hit.wav"		);
	PRECACHE_MODEL( "models/spellbook.mdl"			);
	PRECACHE_MODEL( "sprites/effects/updraft.spr"	);
	m_usEvent1 = PRECACHE_EVENT( 1, "events/spells/updraft1.sc" );
	m_usEvent2 = PRECACHE_EVENT( 1, "events/spells/updraft2.sc" );
}


int WWUpdraftSpell::iItemSlot( void )
{
	return SLOT_UPDRAFT + 1;
}


int WWUpdraftSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_UPDRAFT;
	pInfo->iSlot		= SLOT_UPDRAFT;
	pInfo->iPosition	= POS_UPDRAFT;
	pInfo->iWeight		= WEIGHT_UPDRAFT;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWUpdraftSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < UPDRAFT_COST )
		return FALSE;
	return TRUE;
}


BOOL WWUpdraftSpell::Deploy( void )
{
	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_WIND],
		(char *)szWizardStaves[WWCLASS_WIND],
		HANDS_WIND_UPDRAFT_IDLE, "updraft" );
}


void WWUpdraftSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < UPDRAFT_COST )
	{
		RetireWeapon();
		return;
	}

	m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= UPDRAFT_COST;
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + UPDRAFT_DELAY;
	m_flTimeWeaponIdle	  = m_flNextPrimaryAttack;

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );

	Vector vOrigin = m_pPlayer->GetGunPosition();
	Vector vEndPos = vOrigin + gpGlobals->v_forward * 8192.0f;

	TraceResult tr;
	UTIL_TraceLine( vOrigin, vEndPos, dont_ignore_monsters, m_pPlayer->edict(), &tr );

#ifndef CLIENT_DLL
	ClearMultiDamage();
	::RadiusDamage( tr.vecEndPos, pev, m_pPlayer->pev, UPDRAFT_DAMAGE, UPDRAFT_RADIUS, CLASS_NONE, DMG_GENERIC );
	ApplyMultiDamage( pev, m_pPlayer->pev );

	Vector vDiff = m_pPlayer->pev->origin - tr.vecEndPos;
	float flFactor = 1.0 - vDiff.Length() / UPDRAFT_RADIUS;

	if( flFactor < 0 )
		flFactor = 0;
	else if ( flFactor > 1)
		flFactor = 1;

	Vector vVelocity = vDiff.Normalize() * UPDRAFT_STRENGTH * flFactor;
	m_pPlayer->pev->velocity = m_pPlayer->pev->velocity + vVelocity;

	CBaseEntity * pEnt = NULL;
	while( ( pEnt = UTIL_FindEntityInSphere( pEnt, tr.vecEndPos, UPDRAFT_RADIUS ) ) != NULL )
	{
		if( !pEnt->CanRepel( m_pPlayer ) )
			continue;

		vDiff = pEnt->pev->origin - tr.vecEndPos;

		flFactor = 1.0 - vDiff.Length() / UPDRAFT_RADIUS;

		if( flFactor < 0 )
			flFactor = 0;
		else if ( flFactor > 1)
			flFactor = 1;

		vVelocity = vDiff.Normalize() * UPDRAFT_STRENGTH * flFactor;
		pEnt->pev->velocity = pEnt->pev->velocity + vVelocity;
	}
#endif

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent1, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 0, 0, 0, 0 );

	PLAYBACK_EVENT_FULL( FEV_NOTHOST | FEV_SERVER, NULL, m_usEvent2, 0, tr.vecEndPos,
		( vOrigin - tr.vecEndPos ).Normalize(), 0, 0, 0, 0, 0, 0 );
}


void WWUpdraftSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < UPDRAFT_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( FEV_HOSTONLY | FEV_CLIENT, m_pPlayer->edict(), m_usEvent1, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}