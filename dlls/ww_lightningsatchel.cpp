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


class LightningSatchel : public WWSatchel
{
public:
	virtual void	Spawn			( void );
	virtual void	Precache		( void );

	virtual void	SatchelExplode	( void );
};


LINK_ENTITY_TO_CLASS( ww_satchel_lightning, LightningSatchel );


void LightningSatchel::Spawn( void )
{
	WWSatchel::Spawn();

// 1.2.8 #33 remove extra delay
//	m_flExplodeDelay	= 2.0f;
	m_iExplodeCount		= 3;
	m_flMultiDelay		= 0.4f;
}


void LightningSatchel::Precache( void )
{
	PRECACHE_MODEL( "models/satchels/lightning.mdl" );
	PRECACHE_SOUND( "satchels/lightning.wav"		);
	m_usEvent = PRECACHE_EVENT( 1, "events/satchels/lightning.sc" );
	SET_MODEL( edict(), "models/satchels/lightning.mdl" );
}


void LightningSatchel::SatchelExplode( void )
{
	::RadiusDamage( pev->origin, pev, VARS(pev->owner), 115.0f, 175.0f, 0, DMG_SHOCK );
	PLAYBACK_EVENT( 0, edict(), m_usEvent );
}