#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_freezeray.h"


LINK_ENTITY_TO_CLASS( ww_spell_freezeray, WWFreezeRaySpell );


void WWFreezeRaySpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_FREEZERAY;
	m_iDefaultAmmo	= 0;

	m_bFiring		= false;
	m_flPulseTime	= 0.0f;
	m_pBeam1		= NULL;
	m_pBeam2		= NULL;

	FallInit();
}


void WWFreezeRaySpell::Precache( void )
{
	PRECACHE_SOUND( "spells/freezeray_loop.wav"	);
	PRECACHE_MODEL( "sprites/smoke.spr"	);

	m_usEvent1 = PRECACHE_EVENT( 1, "events/spells/freezeray1.sc" );
	m_usEvent2 = PRECACHE_EVENT( 1, "events/spells/freezeray2.sc" );
}


int WWFreezeRaySpell::iItemSlot( void )
{
	return SLOT_FREEZERAY + 1;
}


int WWFreezeRaySpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_FREEZERAY;
	pInfo->iSlot		= SLOT_FREEZERAY;
	pInfo->iPosition	= POS_FREEZERAY;
	pInfo->iWeight		= WEIGHT_FREEZERAY;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWFreezeRaySpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < FREEZERAY_COST )
		return FALSE;

	return TRUE;
}


BOOL WWFreezeRaySpell::Deploy( void )
{
	m_bFiring = false;
	m_flPulseTime = 0.0f;

	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_ICE],
		(char *)szWizardStaves[WWCLASS_ICE],
		HANDS_ICE_FREEZERAY_IDLE, "rayspell" );
}


void WWFreezeRaySpell::Holster( int skiplocal )
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;

	if( m_bFiring )
		EndAttack();

	CBasePlayerWeapon::Holster( skiplocal );
}


void WWFreezeRaySpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < FREEZERAY_COST )
	{
		RetireWeapon();
		return;
	}

	Attack();
}


void WWFreezeRaySpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_bFiring )
	{
		EndAttack();
		return;
	}

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < FREEZERAY_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( FEV_CLIENT, m_pPlayer->edict(), m_usEvent1, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0.0f, 0.0f, 2, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}


void WWFreezeRaySpell::Attack( void )
{
	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );
	Vector vAiming = gpGlobals->v_forward;
	Vector vOrigin = m_pPlayer->GetGunPosition();

	if( !m_bFiring )
	{
		m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase();

		PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent1, 0.0f,
			vOrigin, vAiming, 0.0f, 0.0f, m_bFiring, 0, 0, 0 );

		m_flPulseTime = gpGlobals->time;
		m_bFiring = true;
	}

	Fire( vOrigin, vAiming );

	if( UTIL_WeaponTimeBase() >= m_pPlayer->m_flNextAmmoBurn && m_bFiring )
	{
		m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= FREEZERAY_COST;
		m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + FREEZERAY_MANABURN;
	}

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.1f;
}


void WWFreezeRaySpell::Fire( const Vector & vOrigin, const Vector & vAiming )
{
#ifndef CLIENT_DLL
	Vector vEndPos = vOrigin + vAiming * FREEZERAY_RANGE;

	TraceResult tr;
	UTIL_TraceLine( vOrigin, vEndPos, dont_ignore_monsters, m_pPlayer->edict(), &tr );

	if( tr.fAllSolid || tr.fStartSolid )
		return;

	if( gpGlobals->time >= m_flPulseTime )
	{
		CBaseEntity * pVictim = pVictim = CBaseEntity::Instance( tr.pHit );

		if( !pVictim )
			return;

		ClearMultiDamage();

		if( pVictim->pev->takedamage )
			pVictim->TraceAttack( m_pPlayer->pev, FREEZERAY_DAMAGE, vAiming, &tr, DMG_FREEZE );
		
		ApplyMultiDamage( pev, m_pPlayer->pev );

		if( pVictim->IsPlayer() )
		{
			CBasePlayer * pPlayer = (CBasePlayer *)pVictim;

			if( pPlayer->IRelationship( m_pPlayer ) >= R_NO )
			{
				if( pPlayer->GetTimer( TIMER_FROZEN ) )
					pPlayer->SetTimer( TIMER_FROZEN, FREEZERAY_FROSTTIME * 0.25f, true );
				else
					pPlayer->SetTimer( TIMER_FROSTED, FREEZERAY_FROSTTIME, true );
			}
		}

		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

		PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent1, 0.0f,
			(float *)&vOrigin, (float *)&vAiming, 0.0f, 0.0f, m_bFiring, 0, 0, 0 );

		m_flPulseTime = gpGlobals->time + WHITERAY_DELAY;
	}

	UpdateEffect( vOrigin, tr.vecEndPos );
#endif
}


void WWFreezeRaySpell::EndAttack( void )
{ 
	PLAYBACK_EVENT_FULL( SC( FEV_GLOBAL | FEV_RELIABLE, 0 ), m_pPlayer->edict(),
		m_usEvent2, 0, m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 0, 0, 0, 0 );

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5f;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.5f;

	m_bFiring = false;

	DestroyEffect();
}


void WWFreezeRaySpell::CreateEffect( void )
{
#ifndef CLIENT_DLL
	DestroyEffect();

	m_pBeam1 = CBeam::BeamCreate( "sprites/smoke.spr", 10 );
	m_pBeam1->PointEntInit( pev->origin, m_pPlayer->entindex() );
	m_pBeam1->SetEndAttachment( 1 );
	m_pBeam1->SetColor( 100, 100, 200 );
	m_pBeam1->SetNoise( 5 );
	m_pBeam1->pev->spawnflags |= SF_BEAM_TEMPORARY;
	m_pBeam1->pev->flags |= FL_SKIPLOCALHOST;
	m_pBeam1->pev->owner = m_pPlayer->edict();

	m_pBeam2 = CBeam::BeamCreate( "sprites/smoke.spr", 2 );
	m_pBeam2->PointEntInit( pev->origin, m_pPlayer->entindex() );
	m_pBeam2->SetEndAttachment( 1 );
	m_pBeam2->SetColor( 200, 200, 200 );
	m_pBeam2->pev->spawnflags |= SF_BEAM_TEMPORARY;
	m_pBeam2->pev->flags |= FL_SKIPLOCALHOST;
	m_pBeam2->pev->owner = m_pPlayer->edict();
#endif
}


void WWFreezeRaySpell::UpdateEffect( Vector vSrc, Vector vDest )
{
#ifndef CLIENT_DLL
	if( !m_pBeam1 || !m_pBeam2 )
		CreateEffect();

	if( m_pBeam1 != NULL )
	{
		m_pBeam1->SetStartPos( vDest );
		m_pBeam1->RelinkBeam();
	}

	if( m_pBeam2 != NULL )
	{
		m_pBeam2->SetStartPos( vDest );
		m_pBeam2->RelinkBeam();
	}
#endif
}


void WWFreezeRaySpell::DestroyEffect( void )
{
#ifndef CLIENT_DLL
	if( m_pBeam1 != NULL )
	{
		UTIL_Remove( m_pBeam1 );
		m_pBeam1 = NULL;
	}

	if( m_pBeam2 != NULL )
	{
		UTIL_Remove( m_pBeam2 );
		m_pBeam2 = NULL;
	}
#endif
}

