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


class DragonSatchel : public WWSatchel
{
public:
	virtual void	Spawn			( void );
	virtual void	Precache		( void );

	virtual void	SatchelTick		( void );
	virtual void	SatchelExplode	( void );

private:
	bool			m_bExplode;
};


LINK_ENTITY_TO_CLASS( ww_satchel_dragon, DragonSatchel );


void DragonSatchel::Spawn( void )
{
	WWSatchel::Spawn();


	m_iExplodeCount = 5;
	m_flMultiDelay	= 1.0f;
	m_bExplode		= false;
}


void DragonSatchel::Precache( void )
{
	PRECACHE_MODEL( "models/satchels/dragon.mdl"	);
	PRECACHE_MODEL( "models/dragontooth.mdl"		);
	PRECACHE_SOUND( "satchels/dragon.wav"			);
	m_usEvent = PRECACHE_EVENT( 1, "events/satchels/dragon.sc" );
	SET_MODEL( edict(), "models/satchels/dragon.mdl" );
}


void DragonSatchel::SatchelTick( void )
{
	if( pev->flags & FL_ONGROUND )
	{
		pev->angles		= Vector(   0, 0, 0 );
		pev->avelocity	= Vector( 999, 0, 0 );
		pev->velocity	= Vector(   0, 0, 0 );
		pev->velocity.z = 400.0f;
		pev->flags		&= ~FL_ONGROUND;

		m_bExplode = true;
	}

	// dragon satchel behaves a little different
	if( m_bExplode && pev->velocity.z < 0 )
	{
		m_bExplode = false;
		m_iExplodeCount--;
		::RadiusDamage( pev->origin, pev, VARS(pev->owner), 150.0f, 100.0f, 0, DMG_BULLET );
		PLAYBACK_EVENT( 0, edict(), m_usEvent );
	}
}


void DragonSatchel::SatchelExplode( void )
{
	m_iExplodeCount++;
}