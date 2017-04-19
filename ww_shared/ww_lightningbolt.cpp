#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_lightningbolt.h"


LINK_ENTITY_TO_CLASS( ww_spell_lightningbolt, WWLightningBoltSpell );


void WWLightningBoltSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_LIGHTNINGBOLT;
	m_iDefaultAmmo	= 0;

	m_bFiring		= false;
	m_flPulseTime	= 0.0f;
	m_pBeam1		= NULL;
	m_pBeam2		= NULL;

	FallInit();
}


void WWLightningBoltSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/bolt_loop.wav"	);
	PRECACHE_MODEL( "sprites/smoke.spr"	);
	m_usEvent1 = PRECACHE_EVENT( 1, "events/spells/lightningbolt1.sc" );
	m_usEvent2 = PRECACHE_EVENT( 1, "events/spells/lightningbolt2.sc" );
}


int WWLightningBoltSpell::iItemSlot( void )
{
	return SLOT_LIGHTNINGBOLT + 1;
}


int WWLightningBoltSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_LIGHTNINGBOLT;
	pInfo->iSlot		= SLOT_LIGHTNINGBOLT;
	pInfo->iPosition	= POS_LIGHTNINGBOLT;
	pInfo->iWeight		= WEIGHT_LIGHTNINGBOLT;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWLightningBoltSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < LIGHTNINGBOLT_COST )
		return FALSE;

	return TRUE;
}


BOOL WWLightningBoltSpell::Deploy( void )
{
	m_bFiring = false;
	m_flPulseTime = 0.0f;

	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_LIGHTNING],
		(char *)szWizardStaves[WWCLASS_LIGHTNING],
		HANDS_LIGHTNING_LIGHTNINGBOLT_IDLE, "zap" );
}


void WWLightningBoltSpell::Holster( int skiplocal )
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;
	EndAttack();

	CBasePlayerWeapon::Holster( skiplocal );
}


void WWLightningBoltSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < LIGHTNINGBOLT_COST )
	{
		RetireWeapon();
		return;
	}

	Attack();
}


void WWLightningBoltSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_bFiring )
	{
		EndAttack();
		return;
	}

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < LIGHTNINGBOLT_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent1, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0.0f, 0.0f, 2, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}


void WWLightningBoltSpell::Attack( void )
{
	// no lightning underwater
	if ( m_pPlayer->pev->waterlevel > 2 )
	{
		if( m_bFiring )
			EndAttack();

		return;
	}

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
		m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= LIGHTNINGBOLT_COST;
		m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + LIGHTNINGBOLT_MANABURN;
	}

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.1f;
}


void WWLightningBoltSpell::Fire( const Vector & vOrigin, const Vector & vAiming )
{
#ifndef CLIENT_DLL
	Vector vEndPos = vOrigin + vAiming * LIGHTNINGBOLT_RANGE;

	TraceResult tr;
	UTIL_TraceLine( vOrigin, vEndPos, dont_ignore_monsters, m_pPlayer->edict(), &tr );
#endif

	if( gpGlobals->time >= m_flPulseTime )
	{
		CBaseEntity * pVictim = NULL;

#ifndef CLIENT_DLL
		// if we are zaping water, do major area effect damage
		if( tr.fInWater )
		{
			while( ( pVictim = UTIL_FindEntityInSphere( pVictim, tr.vecEndPos, LIGHTNINGBOLT_AREA * 5 ) ) != NULL )
			{
				if( !pVictim->pev->takedamage )
					continue;

				if( pVictim->pev->waterlevel <= 1 )
					continue;

				ClearMultiDamage();
				pVictim->TakeDamage( pev, m_pPlayer->pev, LIGHTNINGBOLT_DAMAGE, DMG_SHOCK | DMG_NEVERGIB );
				ApplyMultiDamage( pev, m_pPlayer->pev );
			}
		}

		else
		{
			pVictim = CBaseEntity::Instance( tr.pHit );

			ClearMultiDamage();

			if( pVictim && pVictim->pev->takedamage )
				pVictim->TraceAttack( m_pPlayer->pev, LIGHTNINGBOLT_DAMAGE, vAiming, &tr, DMG_SHOCK );
			
			else
				::RadiusDamage( tr.vecEndPos, pev, m_pPlayer->pev, LIGHTNINGBOLT_DAMAGE, LIGHTNINGBOLT_AREA, CLASS_NONE, DMG_SHOCK | DMG_NEVERGIB );

			ApplyMultiDamage( pev, m_pPlayer->pev );
		}

		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );
#endif

		PLAYBACK_EVENT_FULL( 0, m_pPlayer->edict(), m_usEvent1, 0.0f,
			(float *)&vOrigin, (float *)&vAiming, 0.0f, 0.0f, m_bFiring, 0, 0, 0 );

		m_flPulseTime = gpGlobals->time + LIGHTNINGBOLT_DELAY;
	}

#ifndef CLIENT_DLL
	UpdateEffect( vOrigin, tr.vecEndPos );
#endif
}


void WWLightningBoltSpell::EndAttack( void )
{
	PLAYBACK_EVENT_FULL( FEV_GLOBAL | FEV_RELIABLE, m_pPlayer->edict(), m_usEvent2, 0,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 0, 0, 0, 0 );

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5f;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0f;

	m_bFiring = false;

	DestroyEffect();
}


void WWLightningBoltSpell::CreateEffect( void )
{
#ifndef CLIENT_DLL

	DestroyEffect();

	m_pBeam1 = CBeam::BeamCreate( "sprites/smoke.spr", 2 );
	m_pBeam1->PointEntInit( pev->origin, m_pPlayer->entindex() );
	m_pBeam1->SetEndAttachment( 1 );
	m_pBeam1->SetScrollRate( 50 );
	m_pBeam1->SetNoise( 10 );
	m_pBeam1->SetBrightness( 255 );
	m_pBeam1->SetColor( 200, 200, 100 );
	m_pBeam1->pev->spawnflags |= SF_BEAM_TEMPORARY;
	m_pBeam1->pev->flags |= FL_SKIPLOCALHOST;
	m_pBeam1->pev->owner = m_pPlayer->edict();

	m_pBeam2 = CBeam::BeamCreate( "sprites/smoke.spr", 2 );
	m_pBeam2->PointEntInit( pev->origin, m_pPlayer->entindex() );
	m_pBeam2->SetEndAttachment( 1 );
	m_pBeam2->SetScrollRate( 100 );
	m_pBeam2->SetNoise( 10 );
	m_pBeam2->SetBrightness( 255 );
	m_pBeam2->SetColor( 200, 200, 50 );
	m_pBeam2->pev->spawnflags |= SF_BEAM_TEMPORARY;
	m_pBeam2->pev->flags |= FL_SKIPLOCALHOST;
	m_pBeam2->pev->owner = m_pPlayer->edict();

#endif
}


void WWLightningBoltSpell::UpdateEffect( Vector vSrc, Vector vDest )
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


void WWLightningBoltSpell::DestroyEffect( void )
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

