#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_whiteray.h"


LINK_ENTITY_TO_CLASS( ww_spell_whiteray, WWWhiteRaySpell );


void WWWhiteRaySpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_WHITERAY;
	m_iDefaultAmmo	= 0;

	m_bFiring		= false;
	m_flPulseTime	= 0.0f;
	m_pBeam1		= NULL;
	m_pBeam2		= NULL;

	FallInit();
}


void WWWhiteRaySpell::Precache( void )
{
	PRECACHE_SOUND( "spells/whiteray_loop.wav"	);
	PRECACHE_MODEL( "sprites/smoke.spr"			);
	m_usEvent1 = PRECACHE_EVENT( 1, "events/spells/whiteray1.sc" );
	m_usEvent2 = PRECACHE_EVENT( 1, "events/spells/whiteray2.sc" );
}


int WWWhiteRaySpell::iItemSlot( void )
{
	return SLOT_WHITERAY + 1;
}


int WWWhiteRaySpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_WHITERAY;
	pInfo->iSlot		= SLOT_WHITERAY;
	pInfo->iPosition	= POS_WHITERAY;
	pInfo->iWeight		= WEIGHT_WHITERAY;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWWhiteRaySpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < WHITERAY_COST )
		return FALSE;

	return TRUE;
}


BOOL WWWhiteRaySpell::Deploy( void )
{
	m_bFiring = false;
	m_flPulseTime = 0.0f;

	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_LIFE],
		(char *)szWizardStaves[WWCLASS_LIFE],
		HANDS_LIFE_WHITERAY_IDLE, "rayspell" );
}


void WWWhiteRaySpell::Holster( int skiplocal )
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;

	if( m_bFiring )
		EndAttack();

	CBasePlayerWeapon::Holster( skiplocal );
}


void WWWhiteRaySpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < WHITERAY_COST )
	{
		RetireWeapon();
		return;
	}

	Attack();
}


void WWWhiteRaySpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_bFiring )
	{
		EndAttack();
		return;
	}

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < WHITERAY_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( FEV_CLIENT, m_pPlayer->edict(), m_usEvent1, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0.0f, 0.0f, 2, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}


void WWWhiteRaySpell::Attack( void )
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

	if( UTIL_WeaponTimeBase() >= m_pPlayer->m_flNextAmmoBurn )
	{
		m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= WHITERAY_COST;
		m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + WHITERAY_MANABURN;
	}

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase();
}


void WWWhiteRaySpell::Fire( const Vector & vOrigin, const Vector & vAiming )
{
#ifndef CLIENT_DLL
	Vector vEndPos = vOrigin + vAiming * WHITERAY_RANGE;

	TraceResult tr;
	UTIL_TraceLine( vOrigin, vEndPos, dont_ignore_monsters, m_pPlayer->edict(), &tr );

	if( tr.fAllSolid || tr.fStartSolid )
		return;

	if( gpGlobals->time >= m_flPulseTime )
	{
		CBaseEntity * pVictim = pVictim = CBaseEntity::Instance( tr.pHit );

		if( pVictim && pVictim->pev->takedamage )
		{
			if( m_pPlayer->IRelationship( pVictim ) < R_NO )
				pVictim->TakeHealth( WHITERAY_DAMAGE * 0.5f, DMG_ENERGYBEAM );

			else
			{
				ClearMultiDamage();
				pVictim->TraceAttack( m_pPlayer->pev, WHITERAY_DAMAGE, vAiming, &tr, DMG_ENERGYBEAM );
				ApplyMultiDamage( pev, m_pPlayer->pev );
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


void WWWhiteRaySpell::EndAttack( void )
{
	PLAYBACK_EVENT_FULL( SC( FEV_GLOBAL | FEV_RELIABLE, 0 ), m_pPlayer->edict(),
		m_usEvent2, 0, m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 0, 0, 0, 0 );

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5f;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.5f;

	m_bFiring = false;

	DestroyEffect();
}


void WWWhiteRaySpell::CreateEffect( void )
{
#ifndef CLIENT_DLL
	DestroyEffect();

	m_pBeam1 = CBeam::BeamCreate( "sprites/smoke.spr", 5 );
	m_pBeam1->PointEntInit( pev->origin, m_pPlayer->entindex() );
	m_pBeam1->SetEndAttachment( 1 );
	m_pBeam1->SetColor( 0, 100, 200 );
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


void WWWhiteRaySpell::UpdateEffect( Vector vSrc, Vector vDest )
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


void WWWhiteRaySpell::DestroyEffect( void )
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

