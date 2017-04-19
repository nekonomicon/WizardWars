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


class EarthSatchel : public WWSatchel
{
public:
	virtual void	Spawn			( void );
	virtual void	Precache		( void );

	virtual void	SatchelExplode	( void );
};


LINK_ENTITY_TO_CLASS( ww_satchel_earth, EarthSatchel );


void EarthSatchel::Spawn( void )
{
	WWSatchel::Spawn();


	m_iExplodeCount = 3;
	m_flMultiDelay = 0.5f;
}


void EarthSatchel::Precache( void )
{
	PRECACHE_MODEL( "models/satchels/earth.mdl" );
	PRECACHE_SOUND( "satchels/earth.wav"		);
	m_usEvent = PRECACHE_EVENT( 1, "events/satchels/earth.sc" );
	SET_MODEL( edict(), "models/satchels/earth.mdl" );
}


void EarthSatchel::SatchelExplode( void )
{
	::RadiusDamage( pev->origin, pev, VARS(pev->owner), 75.0f, 90.0f, 0, DMG_CRUSH | DMG_FALL );
	PLAYBACK_EVENT( 0, edict(), m_usEvent );
}