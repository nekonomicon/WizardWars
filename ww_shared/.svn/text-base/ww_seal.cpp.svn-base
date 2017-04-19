#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "effects.h"
#include "customentity.h"
#include "gamerules.h"

#include "tf_globals.h"
#include "tf_globalinfo.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_seal.h"

#ifndef CLIENT_DLLS
#include "ww_seals.h"
#endif


LINK_ENTITY_TO_CLASS( ww_spell_seal, WWSealSpell );


void WWSealSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_SEAL;
	m_iDefaultAmmo	= 0;
	m_bCharging		= false;

#ifndef CLIENT_DLL
	m_hSeal			= NULL;
#endif

	FallInit();
}


void WWSealSpell::Precache( void )
{
	PRECACHE_MODEL( "models/spellbook.mdl"	);
	PRECACHE_MODEL( "models/v_seal.mdl"		);
	PRECACHE_MODEL( "models/p_seal.mdl"		);
	PRECACHE_MODEL( "sprites/smoke.spr"		);
	PRECACHE_SOUND( "spells/seal_start.wav"	);
	PRECACHE_SOUND( "spells/seal_loop.wav"	);
	PRECACHE_SOUND( "spells/seal_end.wav"	);
	m_usEvent = PRECACHE_EVENT( 1, "events/spells/seal.sc" );
}


int WWSealSpell::iItemSlot( void )
{
	return SLOT_SEAL + 1;
}


int WWSealSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->pszName		= STRING( pev->classname );
	pInfo->iId = m_iId	= WEAPON_SEAL;
	pInfo->iSlot		= SLOT_SEAL;
	pInfo->iPosition	= POS_SEAL;
	pInfo->iWeight		= WEIGHT_SEAL;
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWSealSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < SEAL_COST )
		return FALSE;
	return TRUE;
}


BOOL WWSealSpell::Deploy( void )
{
	return DefaultDeploy( "models/v_seal.mdl", "models/p_seal.mdl", 0, "seal" );
}


void WWSealSpell::Holster( int iSkipLocal )
{
#ifndef CLIENT_DLL
	if( m_bCharging )
	{
		m_pPlayer->SetSpeed( SPEED_NOMOVE, false );
		m_bCharging = false;

		if( (CBaseEntity *)m_hSeal != NULL )
		{
			if( m_hSeal->pev->renderfx == kRenderFxLightning )
				m_hSeal->pev->renderfx = 0;
		}
	}
#endif

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST | FEV_GLOBAL | FEV_RELIABLE, 0 ), m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 0, 1, 0, 0 );

	CBasePlayerWeapon::Holster( iSkipLocal ); 
}


void WWSealSpell::PrimaryAttack( void )
{
	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.1f;
	m_flTimeWeaponIdle    = UTIL_WeaponTimeBase() + 0.2f;

	if( m_pPlayer->pev->weaponanim != 1 )
		SendWeaponAnim( 1, 0 );

#ifndef CLIENT_DLL
	if( !m_pPlayer || !m_pPlayer->m_pClass )
		return;

	if( !FBitSet( m_pPlayer->pev->flags, FL_ONGROUND ) ||
		 FBitSet( m_pPlayer->pev->button, IN_DUCK ) )
		return;

	if( !m_bCharging )
	{
		m_hSeal = FindSeal();

		if( (CBaseEntity *)m_hSeal == NULL )
			return;

		WWSeal * pSeal = (WWSeal *)((CBaseEntity *)m_hSeal);

		if( pSeal == NULL )
			return;

		if( pSeal->m_flMana >= 100.0f )
			return;
	
		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );
		m_pPlayer->SetSpeed( SPEED_NOMOVE, true );
		m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase();

		m_bCharging = true;

		pSeal->m_bActive = false;
		pSeal->pev->framerate = 0;
		pSeal->pev->renderfx = kRenderFxLightning;

		PLAYBACK_EVENT_FULL( 0, m_pPlayer->edict(), m_usEvent, 0.0f,
			m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 0, 0, 0, 0 );

		PLAYBACK_EVENT_FULL( 0, m_pPlayer->edict(), m_usEvent, 1.5f,
			m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );
	}
	else if( (CBaseEntity *)m_hSeal != NULL )
	{
		if( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] < SEAL_COST )
		{
			RetireWeapon();
			return;
		}

		if( m_pPlayer->m_flNextAmmoBurn > UTIL_WeaponTimeBase() )
			return;

		m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + SEAL_MANABURN;

		WWSeal * pSeal = ((WWSeal *)((CBaseEntity*)m_hSeal));

		if( !pSeal )
			return;

		float flReturn = pSeal->GiveMana( SEAL_COST );

		if( flReturn == SEAL_COST )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#SealReady" );

			m_pPlayer->SetSpeed( SPEED_NOMOVE, false );
			m_pPlayer->pev->button &= ~IN_ATTACK;
			m_bCharging = false;

			if( m_pPlayer->m_pActiveItem )
			{
				m_pPlayer->m_pActiveItem->Holster();
				m_pPlayer->SelectItem( "ww_staff" );
			}

			PLAYBACK_EVENT_FULL( FEV_UPDATE, m_pPlayer->edict(), m_usEvent, 0,
				m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 2, 0, 0, 0 );

			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 2.0f;
		}
		else
			m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= SEAL_COST - (int)flReturn;
	}
#endif
}


void WWSealSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

#ifndef CLIENT_DLL
	if( m_bCharging )
	{
		m_pPlayer->SetSpeed( SPEED_NOMOVE, false );
		m_pPlayer->m_flNextAmmoBurn = 0.0f;

		if( (CBaseEntity *)m_hSeal != NULL )
		{
			if( m_hSeal->pev->renderfx == kRenderFxLightning )
				m_hSeal->pev->renderfx = 0;
		}

		m_bCharging	 = false;
		m_hSeal		 = NULL;

		PLAYBACK_EVENT_FULL( 0, m_pPlayer->edict(), m_usEvent, 0,
			m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 2, 0, 0, 0 );
	}
#endif

	SendWeaponAnim( 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}



CBaseEntity * WWSealSpell::FindSeal( void )
{
#ifndef CLIENT_DLL
	CBaseEntity * pSeal = (CBaseEntity *)m_pPlayer->m_pClass->m_hSeal;

	// see if my seal is near by
	if( pSeal != NULL )
	{
		if( ( pSeal->pev->origin - m_pPlayer->pev->origin ).Length2D() <= 64.0f )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#SealOwn" );
			return pSeal;
		}

		// if it was ok to use our seal, we wouldn't be here
		pSeal = NULL;
	}

	// find someone elses nearby seal
	if( pSeal == NULL )
	{
		// search entities in the area
		while( ( pSeal = UTIL_FindEntityInSphere( pSeal, m_pPlayer->pev->origin, 64.0f ) ) != NULL )
		{
			// if they aren't a sphere, keep looking
			if( pSeal->Classify() != SEAL_CLASSIFY )
				continue;

			// if we can't effec them, keep looking
			if( pSeal->pev->playerclass != m_pPlayer->pev->playerclass )
				continue;

			// if it doesn't belong to my team, keep looking
			if( !g_pTFGlobalInfo->IsFriendTeam( pSeal->pev->team, m_pPlayer->pev->team ) )
				continue;

			// all those passed? ok we are done.
			break;
		}

		// did we find an ent?
		if( pSeal != NULL )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#SealUsed" );
			return pSeal;
		}

		CBaseEntity * pSeal = (CBaseEntity *)m_pPlayer->m_pClass->m_hSeal;

		// move our seal if we have one
		if( pSeal != NULL )
		{
			WWSeal * pTemp = ((WWSeal *)((CBaseEntity*)pSeal));

			SET_ORIGIN( ENT( pSeal->pev ), m_pPlayer->pev->origin - Vector( 0, 0, 16 ) );
			pTemp->m_flNextProcess = UTIL_WeaponTimeBase() + 2.0f;
			pTemp->m_flMana *= 0.5f;
			pTemp->pev->movetype = MOVETYPE_TOSS;
			pTemp->pev->flags &= ~FL_ONGROUND;
			pTemp->pev->renderamt = 0;
			pTemp->pev->nextthink = gpGlobals->time + 0.25f;

			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#SealMove" );
			return pSeal;
		}

		// none found? ok make one...
		pSeal = Create(
			(char *)m_pPlayer->m_pClass->GetSealClass(),
			m_pPlayer->pev->origin,
			m_pPlayer->pev->angles,
			m_pPlayer->edict() );

		if( pSeal == NULL )
			return NULL;
		m_pPlayer->m_pClass->m_hSeal = pSeal;

		pSeal->pev->playerclass = m_pPlayer->pev->playerclass;
		pSeal->pev->team = m_pPlayer->pev->team;

		ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#SealNew" );
		return pSeal;
	}
#endif

	return NULL;
}