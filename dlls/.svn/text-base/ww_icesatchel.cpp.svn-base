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


class IceSatchel : public WWSatchel
{
public:
	virtual void	Spawn			( void );
	virtual void	Precache		( void );

	virtual void	SatchelExplode	( void );
};


LINK_ENTITY_TO_CLASS( ww_satchel_ice, IceSatchel );


void IceSatchel::Spawn( void )
{
	WWSatchel::Spawn();


	m_iExplodeCount = 1;
}


void IceSatchel::Precache( void )
{
	PRECACHE_MODEL( "models/satchels/ice.mdl"	);
	PRECACHE_MODEL( "sprites/xspark2.spr"		);
	PRECACHE_SOUND( "satchels/ice.wav"			);
	m_usEvent = PRECACHE_EVENT( 1, "events/satchels/ice.sc" );
	SET_MODEL( edict(), "models/satchels/ice.mdl" );
}


void IceSatchel::SatchelExplode( void )
{
	CBasePlayer * pOwner = (CBasePlayer *)CBaseEntity::Instance( pev->owner );

	if( !pOwner )
		return;

	CBaseEntity * pVictim = NULL;
	while( ( pVictim = UTIL_FindEntityInSphere( pVictim, pev->origin, 96.0f ) ) != NULL )
	{
		if( !pVictim->IsPlayer() )
			continue;

		if( pOwner->IRelationship( pVictim ) < R_NO )
			continue;

		CBasePlayer * pPlayer = (CBasePlayer *)pVictim;
		pPlayer->SetTimer( TIMER_FROZEN, 5.0f, true );
	}

	PLAYBACK_EVENT( 0, edict(), m_usEvent );
}