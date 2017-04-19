#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_flamelick.h"


LINK_ENTITY_TO_CLASS( ww_spell_flamelick, WWFlameLickSpell );


void WWFlameLickSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_FLAMELICK;
	m_iDefaultAmmo	= 0;

	FallInit();
}


void WWFlameLickSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/flamelick_cast.wav"	);
	PRECACHE_SOUND( "spells/flamelick_hit.wav"	);
	PRECACHE_MODEL( "models/spellbook.mdl"	);
	PRECACHE_MODEL( "sprites/zerogxplode.spr"	);
	PRECACHE_MODEL( "sprites/mushroom.spr"		);
	PRECACHE_MODEL( "sprites/stmbal1.spr"		);

	m_usEvent1 = PRECACHE_EVENT( 1, "events/spells/flamelick1.sc" );
	m_usEvent2 = PRECACHE_EVENT( 1, "events/spells/flamelick2.sc" );
}


int WWFlameLickSpell::iItemSlot( void )
{
	return SLOT_FLAMELICK + 1;
}


int WWFlameLickSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_FLAMELICK;
	pInfo->iSlot		= SLOT_FLAMELICK;
	pInfo->iPosition	= POS_FLAMELICK;
	pInfo->iWeight		= WEIGHT_FLAMELICK;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWFlameLickSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < FLAMELICK_COST )
		return FALSE;
	return TRUE;
}


BOOL WWFlameLickSpell::Deploy( void )
{
	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_FIRE],
		(char *)szWizardStaves[WWCLASS_FIRE],
		HANDS_WIND_UPDRAFT_IDLE, "flamelick" );
}


void WWFlameLickSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < FLAMELICK_COST )
	{
		RetireWeapon();
		return;
	}

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + FLAMELICK_DELAY;
	m_flTimeWeaponIdle	  = m_flNextPrimaryAttack;

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );

	Vector vOrigin = m_pPlayer->GetGunPosition();
	Vector vEndPos = vOrigin + gpGlobals->v_forward * 8192.0f;

	TraceResult tr;
	UTIL_TraceLine( vOrigin, vEndPos, dont_ignore_monsters, m_pPlayer->edict(), &tr );

	if( tr.fInWater )
		return;

	m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= FLAMELICK_COST;
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

#ifndef CLIENT_DLL
	ClearMultiDamage();
	// 1.2.8 #30 Hurting Friends Fix
	::RadiusDamage( tr.vecEndPos, pev, m_pPlayer->pev, FLAMELICK_DAMAGE, FLAMELICK_RADIUS,
		CLASS_NONE, DMG_BURN | DMG_SLOWBURN );
	ApplyMultiDamage( pev, m_pPlayer->pev );

	Vector vDiff = m_pPlayer->pev->origin - tr.vecEndPos;
	float flFactor = ( 1.0 - vDiff.Length() / FLAMELICK_RADIUS ) * 2.0f;

	if( flFactor < 0.0f )
		flFactor = 0.0f;
	if( flFactor > 1.5f )
		flFactor = 1.5f;

	Vector vVelocity = vDiff.Normalize() * FLAMELICK_STRENGTH * flFactor;
	m_pPlayer->pev->velocity = m_pPlayer->pev->velocity + vVelocity;

	CBaseEntity * pEnt = NULL;
	while( ( pEnt = UTIL_FindEntityInSphere( pEnt, tr.vecEndPos, FLAMELICK_RADIUS ) ) != NULL )
	{
		if( !pEnt->CanRepel( m_pPlayer ) )
			continue;

		vDiff = pEnt->pev->origin - tr.vecEndPos;

		flFactor = 1.0 - vDiff.Length() / FLAMELICK_RADIUS;

		if( flFactor < 0.0f )
			flFactor = 0.0f;
		if( flFactor > 1.0f )
			flFactor = 1.0f;

		vVelocity = vDiff.Normalize() * FLAMELICK_STRENGTH * flFactor;
		pEnt->pev->velocity = pEnt->pev->velocity + vVelocity;
	}
#endif

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent1, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 0, 0, 0, 0 );

	PLAYBACK_EVENT_FULL( FEV_NOTHOST | FEV_SERVER, NULL, m_usEvent2, 0, tr.vecEndPos,
		( vOrigin - tr.vecEndPos ).Normalize(), 0, 0, 0, 0, 0, 0 );
}


void WWFlameLickSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < FLAMELICK_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( FEV_HOSTONLY | FEV_CLIENT, m_pPlayer->edict(), m_usEvent1, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}