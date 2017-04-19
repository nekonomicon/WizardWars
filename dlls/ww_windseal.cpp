#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "animation.h"
#include "saverestore.h"
#include "monsters.h"
#include "player.h"

#include "ww_seals.h"


class WindSeal : public WWSeal
{
public:
	virtual void	Spawn		( void );
	virtual void	Precache	( void );

	virtual void	Process		( CBaseEntity * pOther );

private:
	unsigned short	m_usEvent;
};


LINK_ENTITY_TO_CLASS( ww_seal_wind, WindSeal );


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindSeal::Spawn( void )
{
	Precache();
	WWSeal::Spawn();

	SET_MODEL( ENT(pev), "models/seals/wind.mdl" );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindSeal::Precache( void )
{
	PRECACHE_MODEL( "models/seals/wind.mdl" );
	PRECACHE_SOUND( "seals/wind.wav" );
	m_usEvent = PRECACHE_EVENT( 1, "events/seals/wind.sc" );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WindSeal::Process( CBaseEntity * pOther )
{
	if( !pOther || !pOther->IsPlayer() || !pOther->IsAlive() )
		return;

	CBasePlayer * pPlayer = ((CBasePlayer *)pOther);

	if( pPlayer->IRelationship( this ) < R_NO )
	{
		if( m_flMana < 5.0f )
			return;

		pPlayer->pev->velocity.z += 800.0f;

		m_flMana -= 5.0f;
		m_flShowTime = gpGlobals->time + 5.0f;
		m_flNextProcess = gpGlobals->time + 1.0f;

		PLAYBACK_EVENT_SHORT( FEV_HOSTONLY | FEV_GLOBAL, pev->owner, m_usEvent,
			m_flMana, 0, 0, pPlayer->entindex(), 1, 0 );
		PLAYBACK_EVENT_SHORT( 0, edict(), m_usEvent, 0, 0, 1, pPlayer->entindex(), 0, 0 );
	}
	else
	{
		if( m_flMana < 10.0f )
			return;

		Vector vDiff = pPlayer->pev->origin - pev->origin;
		vDiff.z *= 0.5f;
		vDiff = vDiff.Normalize();

		pPlayer->pev->velocity = pPlayer->pev->velocity + vDiff * 800.0f;

		m_flMana -= 10.0f;
		m_flShowTime = gpGlobals->time + 5.0f;
		m_flNextProcess = gpGlobals->time + 0.5f;

		PLAYBACK_EVENT_SHORT( FEV_HOSTONLY | FEV_GLOBAL, pev->owner, m_usEvent,
			m_flMana, 0, 0, pPlayer->entindex(), 2, 0 );
		PLAYBACK_EVENT_SHORT( 0, edict(), m_usEvent, 0, 0, 2, pPlayer->entindex(), 0, 0 );
	}
}

