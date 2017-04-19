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


class WindSatchel : public WWSatchel
{
public:
	virtual void	Spawn			( void );
	virtual void	Precache		( void );

	virtual void	SatchelTick		( void );
	virtual void	SatchelExplode	( void );

private:
	bool			m_bActive;
};


LINK_ENTITY_TO_CLASS( ww_satchel_wind, WindSatchel );


void WindSatchel::Spawn( void )
{
	WWSatchel::Spawn();


	m_flExplodeDelay	= 2.0f;
	m_iExplodeCount		= 1;

	m_bActive			= false;
}


void WindSatchel::Precache( void )
{
	PRECACHE_MODEL( "models/satchels/wind.mdl"		);
	PRECACHE_MODEL( "sprites/xfire2.spr"			);
	PRECACHE_MODEL( "sprites/effects/updraft.spr"	); 
	PRECACHE_SOUND( "satchels/wind.wav"				);
	PRECACHE_SOUND( "ambience/rwind1.wav"			);
	m_usEvent = PRECACHE_EVENT( 1, "events/satchels/wind.sc" );
	SET_MODEL( edict(), "models/satchels/wind.mdl" );
}


void WindSatchel::SatchelTick( void )
{
	CBasePlayer * pPlayer = (CBasePlayer *)CBaseEntity::Instance( pev->owner );
	
	if( !pPlayer )
	{
		UTIL_Remove( this );
		return;
	}

	if( !m_bActive )
	{
		PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f, (float *)&g_vecZero, (float *)&g_vecZero,
			0.0f, 0.0f, 0, 0, 0, 0 );
		m_bActive = true;
	}

	CBaseEntity * pVictim = NULL;
	while( ( pVictim = UTIL_FindEntityInSphere( pVictim, pev->origin, 300.0f ) ) != NULL )
	{
		if( !pVictim->IsAlive() ||
			 pVictim->IsBSPModel() ||
			!pVictim->pev->takedamage ||
			 pVictim->pev->movetype == MOVETYPE_NONE ||
			 FClassnameIs( pVictim->pev, "thornbush" ) //1.2.1 don't affect TP's
		  )
			continue;

		if( pPlayer->IRelationship( pVictim ) < R_NO )
			continue;

		Vector vDiff = pev->origin - pVictim->pev->origin;

		float flRatio = 1.0f - ( vDiff.Length() / 300.0f );

		if( flRatio < 0.0f )
			flRatio = 0.0f;

		if( flRatio > 1.0f )
			flRatio = 1.0f;

		pVictim->pev->velocity = vDiff.Normalize() * 1000.0f * flRatio;
		pVictim->pev->velocity.z = 1000.0f * flRatio;
	}
}


void WindSatchel::SatchelExplode( void )
{
	::RadiusDamage( pev->origin, pev, VARS(pev->owner), 150.0f,
		128.0f, 0, DMG_GENERIC );

	PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f, (float *)&g_vecZero, (float *)&g_vecZero,
		0.0f, 0.0f, 1, 0, 0, 0 );
}