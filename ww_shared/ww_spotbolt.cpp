#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_spotbolt.h"


LINK_ENTITY_TO_CLASS( ww_spell_spotbolt, WWSpotBoltSpell );


void WWSpotBoltSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_SPOTBOLT;
	m_iDefaultAmmo	= 0;

	m_bCharging		= false;
	m_bReady		= false;
	m_bFull			= false;
	m_flChargeTime	= 0.0f;
	m_pSprite		= NULL;

	FallInit();
}


void WWSpotBoltSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/bolt_start.wav"	);
	PRECACHE_SOUND( "spells/bolt_loop.wav"	);
	PRECACHE_SOUND( "spells/bolt_end.wav"	);
	PRECACHE_SOUND( "ambience/thunder.wav"	);
	PRECACHE_MODEL( "sprites/dot.spr"		);
	PRECACHE_MODEL( "sprites/smoke.spr"		);
	m_usEvent1 = PRECACHE_EVENT( 1, "events/spells/spotbolt1.sc" );
	m_usEvent2 = PRECACHE_EVENT( 1, "events/spells/spotbolt2.sc" );
}


int WWSpotBoltSpell::iItemSlot( void )
{
	return SLOT_SPOTBOLT + 1;
}


int WWSpotBoltSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_SPOTBOLT;
	pInfo->iSlot		= SLOT_SPOTBOLT;
	pInfo->iPosition	= POS_SPOTBOLT;
	pInfo->iWeight		= WEIGHT_SPOTBOLT;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWSpotBoltSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < SPOTBOLT_COST )
		return FALSE;

	return TRUE;
}


BOOL WWSpotBoltSpell::Deploy( void )
{
	m_bCharging		= false;
	m_bReady		= false;
	m_bFull			= false;
	m_flChargeTime	= 0.0f;

	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_LIGHTNING],
		(char *)szWizardStaves[WWCLASS_LIGHTNING],
		HANDS_LIGHTNING_SPOTBOLT_IDLE, "spotbolt" );
}


void WWSpotBoltSpell::Holster( int skiplocal )
{
	if( m_bCharging )
		Fire();

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent1, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 0, 1, 0, 0 );

	CBasePlayerWeapon::Holster( skiplocal );
}


void WWSpotBoltSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < SPOTBOLT_COST )
	{
		if( m_bReady )
		{
			if( m_pPlayer->pev->waterlevel > 2 )
				MisFire();
			else
				Fire();
		}

		RetireWeapon();
		return;
	}

	if( !m_bCharging )
	{
		PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent1, 0.0f,
			m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );

		m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase();
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase();

		m_pPlayer->SetSpeed( SPEED_HALFSPEED, true );

		m_flChargeTime = gpGlobals->time;
		m_bCharging = true;
	}
	else
	{
		if( !m_bFull && UTIL_WeaponTimeBase() > m_pPlayer->m_flNextAmmoBurn )
		{
			m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= SPOTBOLT_COST;
			m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + SPOTBOLT_MANABURN;
		}

		// you need to charge for at least 1/2 a sec before ready
		if( gpGlobals->time - m_flChargeTime > SPOTBOLT_MINCHARGE )
		{
			if( !m_bReady )
			{
				PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent1, 0.0f,
					m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 2, 0, 0, 0 );

				CreateEffect();
			}

			m_bReady = true;
		}

		if( gpGlobals->time - m_flChargeTime > SPOTBOLT_MAXCHARGE )
		{
			if( !m_bFull )
			{
				PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent1, 0.0f,
					m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 3, 0, 0, 0 );
			}

			m_bFull = true;
		}

		if( gpGlobals->time - m_flChargeTime > SPOTBOLT_OVERCHARGE )
			MisFire();

		UpdateEffect();
	}
}


void WWSpotBoltSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_bCharging )
	{
		if( m_pPlayer->pev->waterlevel > 2 )
			MisFire();
		else
			Fire();

		return;
	}

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < SPOTBOLT_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( FEV_CLIENT, m_pPlayer->edict(), m_usEvent1, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 5, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}


void WWSpotBoltSpell::Fire( void )
{
	m_bCharging	= false;
	m_bFull		= false;

	DestroyEffect();

	m_pPlayer->SetSpeed( SPEED_HALFSPEED, false );
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	if( !m_bReady )
		return;
	m_bReady = false;

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 1.5f;
	m_flTimeWeaponIdle = m_flNextPrimaryAttack;

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent1, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 4, 0, m_bReady, 0 );

#ifndef CLIENT_DLL
	float flPercent = ( gpGlobals->time - m_flChargeTime ) / SPOTBOLT_MAXCHARGE;

	if( flPercent > 1.0f )
		flPercent = 1.0f;
	if( flPercent < 0.1f )
		flPercent = 0.1f;

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );

	Vector vSrc = m_pPlayer->GetGunPosition();
	Vector vDst = vSrc + gpGlobals->v_forward * 8192.0f;

	TraceResult tr;
	UTIL_TraceLine( vSrc, vDst, dont_ignore_monsters, m_pPlayer->edict(), &tr );

	if( tr.fAllSolid || tr.fStartSolid )
		return;

	PLAYBACK_EVENT_FULL( 0, NULL, m_usEvent2, 0.0f,
		tr.vecEndPos, m_pPlayer->pev->angles, flPercent, 0, 0, 0, 0, 0 );

	CBaseEntity * pVictim = NULL;

	// if we are zaping water, do major area effect damage
	if( tr.fInWater )
	{
		while( ( pVictim = UTIL_FindEntityInSphere( pVictim, tr.vecEndPos,
			SPOTBOLT_AREA * 5 * flPercent ) ) != NULL )
		{
			if( !pVictim->pev->takedamage )
				continue;

			if( pVictim->pev->waterlevel <= 1 )
				continue;

			ClearMultiDamage();
			pVictim->TakeDamage( pev, m_pPlayer->pev, SPOTBOLT_DAMAGE * flPercent, DMG_SHOCK );
			ApplyMultiDamage( pev, m_pPlayer->pev );
		}
	}
	else
	{
		pVictim = CBaseEntity::Instance( tr.pHit );

		if( !pVictim )
			return;

		ClearMultiDamage();

		if( pVictim->pev->takedamage )
			pVictim->TraceAttack( m_pPlayer->pev, SPOTBOLT_DAMAGE * flPercent, Vector( 0, 0, 1 ), &tr, DMG_SHOCK );

		::RadiusDamage( tr.vecEndPos, pev, m_pPlayer->pev,
			SPOTBOLT_DAMAGE * flPercent, SPOTBOLT_AREA * flPercent, CLASS_NONE, DMG_SHOCK );

		ApplyMultiDamage( pev, m_pPlayer->pev );
	}
#endif
}


void WWSpotBoltSpell::MisFire( void )
{
	m_bCharging	= false;
	m_bFull		= false;

	DestroyEffect();

	m_pPlayer->SetSpeed( SPEED_HALFSPEED, false );
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	DestroyEffect();

	PLAYBACK_EVENT_FULL( FEV_CLIENT, m_pPlayer->edict(), m_usEvent1, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 6, 0, 0, 0 );

	if( !m_bReady )
		return;
	m_bReady = false;

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 1.5f;
	m_flTimeWeaponIdle = m_flNextPrimaryAttack;

#ifndef CLIENT_DLL
//	CBaseEntity * pVictim = NULL;

	ClearMultiDamage();
	if( m_pPlayer->IsAlive() )
		m_pPlayer->TakeDamage( pev, m_pPlayer->pev, SPOTBOLT_DAMAGE * 0.1f, DMG_SHOCK );
	ApplyMultiDamage( pev, m_pPlayer->pev );
#endif
}



void WWSpotBoltSpell::CreateEffect( void )
{
#ifndef CLIENT_DLL
	DestroyEffect();

	m_pSprite = CSprite::SpriteCreate( "sprites/dot.spr", pev->origin, FALSE );
	m_pSprite->SetColor( 250, 150, 0 );
	m_pSprite->pev->spawnflags |= SF_SPRITE_TEMPORARY;
	m_pSprite->pev->flags |= FL_SKIPLOCALHOST;
	m_pSprite->pev->owner = m_pPlayer->edict();
	m_pSprite->pev->rendermode = kRenderGlow;
	m_pSprite->pev->renderamt = 255;
	m_pSprite->pev->renderfx = kRenderFxNoDissipation;
#endif
}


void WWSpotBoltSpell::UpdateEffect( void )
{
#ifndef CLIENT_DLL
	if( m_pSprite != NULL )
	{
		UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );

		Vector vSrc = m_pPlayer->GetGunPosition();
		Vector vDst = vSrc + gpGlobals->v_forward * 8192.0f;

		TraceResult tr;
		UTIL_TraceLine( vSrc, vDst, dont_ignore_monsters, m_pPlayer->edict(), &tr );

		SET_ORIGIN( m_pSprite->edict(), tr.vecEndPos );

		if( m_bFull )
			m_pSprite->SetColor( 250, 250, 0 );
	}
#endif
}


void WWSpotBoltSpell::DestroyEffect( void )
{
#ifndef CLIENT_DLL
	if( m_pSprite != NULL )
	{
		UTIL_Remove( m_pSprite );
		m_pSprite = NULL;
	}
#endif
}

