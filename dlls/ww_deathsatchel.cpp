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


class DeathSatchel : public WWSatchel
{
public:
	virtual void	Spawn			( void );
	virtual void	Precache		( void );

	virtual void	SatchelExplode	( void );
};


LINK_ENTITY_TO_CLASS( ww_satchel_death, DeathSatchel );


void DeathSatchel::Spawn( void )
{
	WWSatchel::Spawn();


	pev->renderfx		= kRenderFxPoison;
	m_iExplodeCount		= 1;
}


void DeathSatchel::Precache( void )
{
	PRECACHE_MODEL( "models/satchels/death.mdl" );
	PRECACHE_MODEL( "sprites/stmbal1.spr"		);
	PRECACHE_SOUND( "satchels/death.wav"		);
	m_usEvent = PRECACHE_EVENT( 1, "events/satchels/death.sc" );
	SET_MODEL( edict(), "models/satchels/death.mdl" );
}


void DeathSatchel::SatchelExplode( void )
{
	::RadiusDamage( pev->origin, pev, VARS(pev->owner), 50.0f, 126.0f, 0, DMG_POISON | DMG_POISONCONTAG );
	PLAYBACK_EVENT( 0, edict(), m_usEvent );
}