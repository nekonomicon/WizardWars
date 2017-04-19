#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "gamerules.h"
#include "effects.h"

#include "ww_satchels.h"


class LifeSatchel : public WWSatchel
{
public:
	virtual void	Spawn			( void );
	virtual void	Precache		( void );

	virtual void	SatchelExplode	( void );
};


LINK_ENTITY_TO_CLASS( ww_satchel_life, LifeSatchel );

#define LIFESATCHEL_DAMAGE	110//1.2.1 WAS 220

void LifeSatchel::Spawn( void )
{
	WWSatchel::Spawn();


	m_iExplodeCount		= 1;
}


void LifeSatchel::Precache( void )
{
	PRECACHE_MODEL( "models/satchels/life.mdl"	);
	PRECACHE_MODEL( "sprites/b-tele1.spr"		);
	PRECACHE_SOUND( "satchels/life.wav"			);
	m_usEvent = PRECACHE_EVENT( 1, "events/satchels/life.sc" );
	SET_MODEL( edict(), "models/satchels/life.mdl" );
}


void LifeSatchel::SatchelExplode( void )
{
	CBasePlayer * pPlayer = (CBasePlayer *)CBaseEntity::Instance( pev->owner );
	
	if( !pPlayer )
	{
		UTIL_Remove( this );
		return;
	}

	CBaseEntity * pVictim = NULL;
	while( ( pVictim = UTIL_FindEntityInSphere( pVictim, pev->origin, 175.0f ) ) != NULL )
	{
		if( !pVictim->IsPlayer() || !pVictim->IsAlive())
			continue;

		if( pPlayer->IRelationship( pVictim ) < R_NO )
		{
			pVictim->TakeHealth( LIFESATCHEL_DAMAGE, DMG_GENERIC );
			if( ((CBasePlayer*)pVictim)->m_bitsDamageType & DMG_POISON )
			{
				if( ((CBasePlayer*)pVictim)->m_bitsDamageType & DMG_POISONCONTAG)
				{
					pVictim->SetTouch(NULL); // not contagious
					pPlayer->pev->frags++;
				}

				((CBasePlayer*)pVictim)->m_bitsDamageType = 0;
			}
		}
		else
			pVictim->TakeDamage( pev, VARS(pev->owner), LIFESATCHEL_DAMAGE, DMG_GENERIC );
	}
	
	PLAYBACK_EVENT( 0, edict(), m_usEvent );
}