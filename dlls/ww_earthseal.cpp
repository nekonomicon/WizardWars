#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "animation.h"
#include "saverestore.h"
#include "monsters.h"
#include "player.h"

#include "ww_seals.h"


extern int gmsgShake;
void RadiusDamage( Vector, entvars_t *, entvars_t *, float, float, int, int );


class EarthSeal : public WWSeal
{
public:
	virtual void	Spawn		( void );
	virtual void	Precache	( void );

	virtual void	Process		( CBaseEntity * pOther );

private:
	unsigned short	m_usEvent;
};


LINK_ENTITY_TO_CLASS( ww_seal_earth, EarthSeal );


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EarthSeal::Spawn( void )
{
	Precache();
	WWSeal::Spawn();

	SET_MODEL( ENT(pev), "models/seals/earth.mdl" );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EarthSeal::Precache( void )
{
	PRECACHE_MODEL( "models/seals/earth.mdl"	);
	PRECACHE_SOUND( "seals/earth.wav"			);
	m_usEvent = PRECACHE_EVENT( 1, "events/seals/earth.sc" );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EarthSeal::Process( CBaseEntity * pOther )
{
	if( !pOther || !pOther->IsPlayer() || !pOther->IsAlive() )
		return;

	CBasePlayer * pPlayer = ((CBasePlayer *)pOther);

	if( pPlayer->IRelationship( this ) < R_NO )
	{
		if( m_flMana < 10.0f )
			return;

		if( pPlayer->pev->armorvalue >= pPlayer->pev->armortype )
			return;

		pPlayer->pev->armorvalue += 50.0f;

		if( pPlayer->pev->armorvalue > pPlayer->pev->armortype )
			pPlayer->pev->armorvalue = pPlayer->pev->armortype;

		m_flMana -= 10.0f;
		m_flShowTime = gpGlobals->time + 5.0f;
		m_flNextProcess = gpGlobals->time + 1.0;

		PLAYBACK_EVENT_SHORT( FEV_HOSTONLY | FEV_GLOBAL, pev->owner, m_usEvent,
			m_flMana, 0, 0, pPlayer->entindex(), 1, 0 );
		PLAYBACK_EVENT_SHORT( 0, edict(), m_usEvent, 0, 0, 1, pPlayer->entindex(), 0, 0 );
	}
	else
	{
		if( m_flMana < 20.0f )
			return;

		if( pPlayer->IsTimerActive( TIMER_GROUNDED ) )
			return;

		pPlayer->SetTimer( TIMER_GROUNDED, 5.0f );

		pPlayer->pev->origin.z = pev->origin.z;
		SET_ORIGIN( pPlayer->edict(), pPlayer->pev->origin );

		m_flMana -= 20.0f;
		m_flShowTime = gpGlobals->time + 5.0f;
		m_flNextProcess = gpGlobals->time + 0.5f;

		PLAYBACK_EVENT_SHORT( FEV_HOSTONLY | FEV_GLOBAL, pev->owner, m_usEvent,
			m_flMana, 0, 0, pPlayer->entindex(), 2, 0 );
		PLAYBACK_EVENT_SHORT( 0, edict(), m_usEvent, 0, 0, 2, pPlayer->entindex(), 0, 0 );
	}
}

