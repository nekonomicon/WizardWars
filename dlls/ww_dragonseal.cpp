#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "animation.h"
#include "saverestore.h"
#include "monsters.h"
#include "player.h"
#include "weapons.h"

#include "ww_seals.h"


void RadiusDamage( Vector, entvars_t *, entvars_t *, float, float, int, int );


class DragonSeal : public WWSeal
{
public:
	virtual void	Spawn		( void );
	virtual void	Precache	( void );

	virtual void	Process		( CBaseEntity * pOther );

private:
	unsigned short	m_usEvent;
};


LINK_ENTITY_TO_CLASS( ww_seal_dragon, DragonSeal );


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DragonSeal::Spawn( void )
{
	Precache();
	WWSeal::Spawn();

	SET_MODEL( ENT(pev), "models/seals/dragon.mdl" );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DragonSeal::Precache( void )
{
	PRECACHE_MODEL( "models/seals/dragon.mdl"	);
	PRECACHE_SOUND( "seals/dragon.wav"			);
	m_usEvent = PRECACHE_EVENT( 1, "events/seals/dragon.sc" );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DragonSeal::Process( CBaseEntity * pOther )
{
	if( !pOther || !pOther->IsPlayer() || !pOther->IsAlive() )
		return;

	CBasePlayer * pPlayer = ((CBasePlayer *)pOther);

	if( pPlayer->IRelationship( this ) < R_NO )
	{
		if( m_flMana < 10.0f )
			return;

		if( pPlayer->IsTimerActive( TIMER_PROTECTION ) )
			return;

		pPlayer->SetTimer( TIMER_PROTECTION, 30.0f );

		m_flMana -= 10.0f;
		m_flShowTime = gpGlobals->time + 5.0f;
		m_flNextProcess = gpGlobals->time + 1.0f;

		PLAYBACK_EVENT_SHORT( FEV_HOSTONLY | FEV_GLOBAL, pev->owner, m_usEvent,
			m_flMana, 0, 0, pPlayer->entindex(), 1, 0 );
		PLAYBACK_EVENT_SHORT( 0, edict(), m_usEvent, 0, 0, 1, pPlayer->entindex(), 0, 0 );
	}
	else
	{
		if( m_flMana < 20.0f )
			return;

		RadiusDamage( pev->origin + Vector( 0, 0, 4 ), pev, VARS( pev->owner ), 75, 200.0f, 0, DMG_BURN );

		MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pev->origin );
			WRITE_BYTE ( TE_EXPLOSION			);
			WRITE_COORD( pev->origin.x			);
			WRITE_COORD( pev->origin.y			);
			WRITE_COORD( pev->origin.z + 4		);
			WRITE_SHORT( g_sModelIndexFireball	);
			WRITE_BYTE ( 35						);
			WRITE_BYTE ( 12						);
			WRITE_BYTE ( 0						);
		MESSAGE_END();

		m_flMana -= 20.0f;
		m_flShowTime = gpGlobals->time + 5.0f;
		m_flNextProcess = gpGlobals->time + 0.5f;

		PLAYBACK_EVENT_SHORT( FEV_HOSTONLY | FEV_GLOBAL, pev->owner, m_usEvent,
			m_flMana, 0, 0, pPlayer->entindex(), 2, 0 );
		PLAYBACK_EVENT_SHORT( 0, edict(), m_usEvent, 0, 0, 2, pPlayer->entindex(), 0, 0 );
	}
}

