#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "animation.h"
#include "saverestore.h"
#include "monsters.h"
#include "player.h"

#include "ww_seals.h"


class NatureSeal : public WWSeal
{
public:
	virtual void	Spawn		( void );
	virtual void	Precache	( void );

	virtual void	Process		( CBaseEntity * pOther );
	virtual void	Tick		( void );
	virtual void	Activate	( void );

private:
	unsigned short	m_usEvent;
	float			m_flLastGathering;
};


LINK_ENTITY_TO_CLASS( ww_seal_nature, NatureSeal );


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void NatureSeal::Spawn( void )
{
	Precache();
	WWSeal::Spawn();

	SET_MODEL( ENT(pev), "models/seals/nature.mdl" );

	m_flLastGathering = -1.0f;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void NatureSeal::Precache( void )
{
	PRECACHE_MODEL( "models/seals/nature.mdl"	);
	PRECACHE_SOUND( "seals/nature.wav"			);
	m_usEvent = PRECACHE_EVENT( 1, "events/seals/nature.sc" );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void NatureSeal::Process( CBaseEntity * pOther )
{
	if( !pOther || !pOther->IsPlayer() || !pOther->IsAlive() )
		return;

	CBasePlayer * pPlayer = ((CBasePlayer *)pOther);

	if( pPlayer->IRelationship( this ) < R_NO )
	{
		if( m_flMana < 10.0f )
			return;

		if( pPlayer->GiveAmmo( 10, "mana", 200 ) < 0 )
			return;

		m_flMana -= 10.0f;
		m_flShowTime = gpGlobals->time + 5.0f;
		m_flNextProcess = gpGlobals->time + 1.0;

		PLAYBACK_EVENT_SHORT( FEV_HOSTONLY | FEV_GLOBAL, pev->owner, m_usEvent,
			m_flMana, 0, 0, pPlayer->entindex(), 1, 0 );
		PLAYBACK_EVENT_SHORT( 0, edict(), m_usEvent, 0, 0, 1, pPlayer->entindex(), 0, 0 );
	}
	else
	{
		if( m_flMana < 10.0f )
			return;

		if( pPlayer->IsTimerActive( TIMER_ROOTED ) )
			return;

		pPlayer->SetSpeed( SPEED_NOMOVE, true );
		pPlayer->SetTimer( TIMER_ROOTED, 5.0f );

		m_flMana -= 10.0f;
		m_flShowTime = gpGlobals->time + 5.0f;
		m_flNextProcess = gpGlobals->time + 1.0;

		PLAYBACK_EVENT_SHORT( FEV_HOSTONLY | FEV_GLOBAL, pev->owner, m_usEvent,
			m_flMana, 0, 0, pPlayer->entindex(), 2, 0 );
		PLAYBACK_EVENT_SHORT( 0, edict(), m_usEvent, 0, 0, 2, pPlayer->entindex(), 0, 0 );
	}

	m_flLastGathering = gpGlobals->time + 10.0f;
}

void NatureSeal::Tick( void )
{
	if( gpGlobals->time > m_flLastGathering && m_flLastGathering != -1.0f )
	{
		m_flMana = min( m_flMana + 10.0f, m_flMaxMana );
		m_flLastGathering = gpGlobals->time + 10.0;
	}
}

void NatureSeal::Activate( void )
{
	m_flLastGathering = gpGlobals->time;
}
