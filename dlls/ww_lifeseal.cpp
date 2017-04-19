#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "animation.h"
#include "saverestore.h"
#include "monsters.h"
#include "player.h"
#include "shake.h"

#include "ww_seals.h"


extern int gmsgEFShield;


class LifeSeal : public WWSeal
{
public:
	virtual void	Spawn		( void );
	virtual void	Precache	( void );

	virtual void	Process		( CBaseEntity * pOther );

private:
	unsigned short	m_usEvent;
};


LINK_ENTITY_TO_CLASS( ww_seal_life, LifeSeal );


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void LifeSeal::Spawn( void )
{
	Precache();
	WWSeal::Spawn();

	SET_MODEL( ENT(pev), "models/seals/life.mdl" );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void LifeSeal::Precache( void )
{
	PRECACHE_MODEL( "models/seals/life.mdl" );
	PRECACHE_SOUND( "seals/life.wav"		);
	m_usEvent = PRECACHE_EVENT( 1, "events/seals/life.sc" );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void LifeSeal::Process( CBaseEntity * pOther )
{
	if( !pOther || !pOther->IsPlayer() || !pOther->IsAlive() )
		return;

	CBasePlayer * pPlayer = ((CBasePlayer *)pOther);

	if( pPlayer->IRelationship( this ) < R_NO )
	{
		if( m_flMana < 10.0f )
			return;

		if( pPlayer->pev->health >= pPlayer->pev->max_health && pPlayer->m_bitsDamageType == 0 )
			return;

		pPlayer->TakeHealth( 20, DMG_GENERIC );
		pPlayer->m_bitsDamageType = 0;

		m_flMana -= 10.0f;
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

		pPlayer->TakeDamage( pev, pPlayer->pev, 10.0f, DMG_SONIC );

		m_flMana -= 10.0f;
		m_flShowTime = gpGlobals->time + 5.0f;
		m_flNextProcess = gpGlobals->time + 0.5f;

		PLAYBACK_EVENT_SHORT( FEV_HOSTONLY | FEV_GLOBAL, pev->owner, m_usEvent,
			m_flMana, 0, 0, pPlayer->entindex(), 2, 0 );
		PLAYBACK_EVENT_SHORT( 0, edict(), m_usEvent, 0, 0, 2, pPlayer->entindex(), 0, 0 );
	}
}

