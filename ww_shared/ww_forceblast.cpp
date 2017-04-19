#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_forceblast.h"

#ifndef CLIENT_DLL
#include "ww_seals.h"
#endif


LINK_ENTITY_TO_CLASS( ww_spell_forceblast, WWForceBlastSpell );


void WWForceBlastSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_FORCEBLAST;
	m_iDefaultAmmo	= 0;

	FallInit();
}


void WWForceBlastSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/forceblast_cast.wav"	);
	PRECACHE_MODEL( "models/spellbook.mdl"			);
	PRECACHE_MODEL( "sprites/shockwave.spr"			);

	m_usEvent = PRECACHE_EVENT( 1, "events/spells/forceblast.sc" );
}


int WWForceBlastSpell::iItemSlot( void )
{
	return SLOT_FORCEBLAST + 1;
}


int WWForceBlastSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_FORCEBLAST;
	pInfo->iSlot		= SLOT_FORCEBLAST;
	pInfo->iPosition	= POS_FORCEBLAST;
	pInfo->iWeight		= WEIGHT_FORCEBLAST;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWForceBlastSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < FORCEBLAST_COST )
		return FALSE;
	return TRUE;
}


BOOL WWForceBlastSpell::Deploy( void )
{
	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_LIFE],
		(char *)szWizardStaves[WWCLASS_LIFE],
		HANDS_LIFE_FORCEBLAST_IDLE, "forcespell" );
}


void WWForceBlastSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < FORCEBLAST_COST )
	{
		RetireWeapon();
		return;
	}

	m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= FORCEBLAST_COST;
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + FORCEBLAST_DELAY;
	m_flTimeWeaponIdle	  = m_flNextPrimaryAttack;

	//	trepid_jon - 040523
	//	because this is fun to do
	m_pPlayer->pev->velocity.x *= FORCEBLAST_EXTRAVELOCITY;
	m_pPlayer->pev->velocity.y *= FORCEBLAST_EXTRAVELOCITY;
	m_pPlayer->pev->velocity.z *= FORCEBLAST_EXTRAVELOCITY;

#ifndef CLIENT_DLL
	CBaseEntity * pVictim = NULL;
	while( ( pVictim = UTIL_FindEntityInSphere( pVictim, m_pPlayer->pev->origin, FORCEBLAST_RADIUS ) ) != NULL )
	{
		Vector vDiff = pVictim->pev->origin - m_pPlayer->pev->origin;
		float flFactor = 1.0 - vDiff.Length() / FORCEBLAST_RADIUS;

		if( flFactor < 0 )
			flFactor = 0;
		else if ( flFactor > 1)
			flFactor = 1;

		if( pVictim->IsPlayer() && pVictim->IsAlive() )
		{
			if( m_pPlayer->IRelationship( pVictim ) < R_NO )
			{
				// placeholder for future team effect
			}
			else
			{
				Vector vVelocity = vDiff.Normalize() * FORCEBLAST_STRENGTH * flFactor;
				//vVelocity.z *= 0.5f;
				
				pVictim->pev->flags &= ~FL_ONGROUND;
				pVictim->pev->velocity = pVictim->pev->velocity + vVelocity;
				pVictim->TakeDamage( pev, m_pPlayer->pev, FORCEBLAST_DAMAGE, DMG_SONIC );
			}
		}
		else if( pVictim->CanRepel( m_pPlayer ) )
		{
			if( m_pPlayer->IRelationship( pVictim ) < R_NO )
			{
				// placeholder for future team effect
			}
			else
			{
				Vector vVelocity = vDiff.Normalize() * FORCEBLAST_STRENGTH * flFactor;
				//vVelocity.z *= 0.5f;

				pVictim->pev->flags &= ~FL_ONGROUND;
				pVictim->pev->nextthink = gpGlobals->time + 1.0f;	// stun monsters, etc... we don't want them to undo velocity
				pVictim->pev->velocity = vVelocity;					// monsters have velocity set, rather then modified
				pVictim->TakeDamage( pev, m_pPlayer->pev, FORCEBLAST_DAMAGE, DMG_SONIC );
			}
		}
		else if( pVictim->Classify() == SEAL_CLASSIFY )
		{
			WWSeal * pSeal = (WWSeal *)pVictim;
			
			if( pSeal != NULL )
			{
				if( m_pPlayer->IRelationship( pVictim ) < R_NO )
				{
					// placeholder for future team effect
				}
				else
				{
					pSeal->m_flShowTime		 = gpGlobals->time + 5.0f;
					pSeal->m_bActive		 = false;
					pSeal->pev->framerate	 = 0;
					pSeal->pev->renderfx	 = 0;
				}
			}
		}
	}
#endif

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 0, 0, 0, 0 );
}


void WWForceBlastSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < FORCEBLAST_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( FEV_HOSTONLY | FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}

