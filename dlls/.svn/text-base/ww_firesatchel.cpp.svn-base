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


class FireSatchel : public WWSatchel
{
public:
	virtual void	Spawn			( void );
	virtual void	Precache		( void );

	virtual void	SatchelExplode	( void );

private:
	bool			m_bActive;
};


LINK_ENTITY_TO_CLASS( ww_satchel_fire, FireSatchel );


void FireSatchel::Spawn( void )
{
	WWSatchel::Spawn();


	m_iExplodeCount		= 10;
	m_flMultiDelay		= 0.2f;

	m_bActive			= false;
}


void FireSatchel::Precache( void )
{
	PRECACHE_MODEL( "models/satchels/fire.mdl"	);
	PRECACHE_SOUND( "satchels/fire.wav"			);
	m_usEvent = PRECACHE_EVENT( 1, "events/satchels/fire.sc" );
	SET_MODEL( edict(), "models/satchels/fire.mdl" );
}


void FireSatchel::SatchelExplode( void )
{
	::RadiusDamage( pev->origin, pev, VARS(pev->owner), 75.0f, 90.0f, 0, DMG_BURN | DMG_SLOWBURN );

	pev->velocity	= Vector( 0, 0, 0 );
	pev->rendermode	= kRenderTransColor;
	pev->renderamt	= 0;

	if( !m_bActive )
	{
		PLAYBACK_EVENT( 0, edict(), m_usEvent );
		m_bActive = true;
	}
}