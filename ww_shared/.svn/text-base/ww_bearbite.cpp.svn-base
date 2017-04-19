#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_bearbite.h"


LINK_ENTITY_TO_CLASS( ww_bearbite, WWBearBite );


void WWBearBite::Spawn( void )
{
	Precache();

	m_iId = WEAPON_BEARBITE;

	FallInit();
}


void WWBearBite::Precache( void )
{
	PRECACHE_SOUND( "spells/bearbite_hitbod1.wav"	);
	PRECACHE_SOUND( "spells/bearbite_hitbod2.wav"	);
	PRECACHE_SOUND( "spells/bearbite_hitbod3.wav"	);
	PRECACHE_SOUND( "spells/bearbite_hitwall.wav"	);
	PRECACHE_SOUND( "spells/bearbite_miss.wav"		);
	PRECACHE_MODEL(	"models/hands/bear.mdl"	);

	m_usEvent = PRECACHE_EVENT( 1, "events/spells/bearbite.sc" );
}


int WWBearBite::iItemSlot( void )
{
	return SLOT_BEARBITE + 1;
}


int WWBearBite::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_BEARBITE;
	pInfo->iSlot		= SLOT_BEARBITE;
	pInfo->iPosition	= POS_BEARBITE;
	pInfo->iWeight		= WEIGHT_BEARBITE;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWBearBite::Deploy( void )
{
	return DefaultDeploy( "models/hands/bear.mdl", NULL, HANDS_EARTH_BEARBITE_IDLE, "bearbite" );
}


void WWBearBite::PrimaryAttack( void )
{
	if( m_flNextPrimaryAttack > UTIL_WeaponTimeBase() )
		return;

	Attack();

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + BEARBITE_DELAY;
	m_flTimeWeaponIdle	  = m_flNextPrimaryAttack;
}


void WWBearBite::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	PLAYBACK_EVENT_FULL( FEV_HOSTONLY | FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles,	0, 0, 0, 0, 0, 1 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}


bool WWBearBite::Attack( void )
{
	bool bHit = false;

	Vector vOrigin = m_pPlayer->GetGunPosition();
	Vector vAngles = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;

	UTIL_MakeVectors( vAngles );

	Vector vEndPos = vOrigin + gpGlobals->v_forward * BEARBITE_RANGE;

	TraceResult tr;
	UTIL_TraceLine( vOrigin, vEndPos, dont_ignore_monsters, m_pPlayer->edict(), &tr );

	if( tr.flFraction == 1.0f )
		UTIL_TraceHull( vOrigin, vEndPos, dont_ignore_monsters, head_hull, m_pPlayer->edict(), &tr );

	if( tr.flFraction != 1.0f )
	{
#ifndef CLIENT_DLL
		CBaseEntity * pHit = CBaseEntity::Instance( tr.pHit );

		if( pHit != NULL )
		{
			ClearMultiDamage();
			pHit->TraceAttack( m_pPlayer->pev, BEARBITE_DAMAGE, gpGlobals->v_forward, &tr, DMG_SLASH );
			ApplyMultiDamage( pev, m_pPlayer->pev );
		}
#endif
		bHit = true;
	}

	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 0, 0, bHit, 0 );

	return bHit;
}


