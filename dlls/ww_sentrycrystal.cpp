#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "effects.h"

#include "ww_sentrycrystal.h"

#include "tf_globals.h"
#include "tf_globalinfo.h"
#include "tf_teamcheck.h"


LINK_ENTITY_TO_CLASS( ww_sentrycrystal,		WWSentryCrystal );
LINK_ENTITY_TO_CLASS( monster_miniturret,	WWSentryCrystal );
LINK_ENTITY_TO_CLASS( monster_turret,		WWSentryCrystal );


enum
{
	SCSKIN_RED = 0,
	SCSKIN_BLUE,
	SCSKIN_GREEN,
	SCSKIN_YELLOW,
};


WWSentryCrystal::WWSentryCrystal() : CBaseMonster()
{
	m_iszTeamcheck = 0;
	m_iOwner = 0;
	m_iOrient = 0;
}


void WWSentryCrystal::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "teamcheck" ) )
	{
		m_iszTeamcheck = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}

	else if( FStrEq( pkvd->szKeyName, "team_no" ) )
	{
		m_iOwner = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}

	else if( FStrEq( pkvd->szKeyName, "orientation" ) )
	{
		m_iOrient = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}

	// ignore the old values
	else if( FStrEq( pkvd->szKeyName, "maxsleep" )	||
		FStrEq( pkvd->szKeyName, "searchspeed" )	||
		FStrEq( pkvd->szKeyName, "turnrate" )		||
		FStrEq( pkvd->szKeyName, "style" )			||
		FStrEq( pkvd->szKeyName, "height" )			||
		FStrEq( pkvd->szKeyName, "value1" )			||
		FStrEq( pkvd->szKeyName, "value2" )			||
		FStrEq( pkvd->szKeyName, "value3" )			)
		pkvd->fHandled = TRUE;
	else
		CBaseMonster::KeyValue( pkvd );
}


void WWSentryCrystal::Precache( )
{
	PRECACHE_SOUND( "spells/icepoke.wav" );
	PRECACHE_MODEL( "models/sentrycrystal.mdl");
}


void WWSentryCrystal::Spawn()
{ 
	Precache();

	SET_MODEL( ENT(pev), "models/sentrycrystal.mdl" );
	
	pev->health			= 100;
	pev->movetype		= MOVETYPE_FLY;
	pev->solid			= SOLID_SLIDEBOX;
	pev->takedamage		= DAMAGE_NO;
	pev->rendermode		= kRenderTransAdd;
	pev->rendercolor	= Vector( 255.0f, 255.0f, 255.0f );
	pev->renderamt		= 255;
	pev->flags		   |= EF_DIMLIGHT;

	m_flFieldOfView		= VIEW_FIELD_FULL;

	if( m_iOrient )
		pev->angles = Vector( -180, 0, 0 );

	UTIL_SetSize( pev, Vector( -8, -8, -8 ), Vector( 8, 8, 8 ) );

	SetThink(&WWSentryCrystal:: SearchAndDestroy );	
	ResetSequenceInfo();

	m_flNextFire = 0.0f;
	m_flLastFire = 0.0f;

	pev->nextthink = gpGlobals->time + 0.1; 
}


void WWSentryCrystal::SearchAndDestroy( void )
{
//	CBaseEntity * pEnemy = NULL;

	pev->team = m_iOwner;

	if( m_iszTeamcheck != 0 )
	{
		TFTeamCheck * pTeamCheck = (TFTeamCheck *)UTIL_FindEntityByTargetname( NULL, STRING( m_iszTeamcheck ) );

		if( pTeamCheck != NULL )
			pev->team = pTeamCheck->GetTeam();
	}

	Look( 1024 );
	Fire( BestVisibleEnemy() );

	StudioFrameAdvance( 0.1f );

	if( gpGlobals->time - m_flLastFire > 5.0f || m_flLastFire == 0.0f )
	{
		pev->skin = SCSKIN_GREEN;
		pev->sequence = 0;
	}
	
	else if( gpGlobals->time - m_flLastFire > 1.0f )
	{
		pev->skin = SCSKIN_YELLOW;
		pev->sequence = 2;
	}

	else
	{
		pev->skin = SCSKIN_RED;
		pev->sequence = 1;
	}

	pev->nextthink = gpGlobals->time + 0.1f; 
}


int WWSentryCrystal::IRelationship( CBaseEntity * pTarget )
{
	// some reason the teamcheck and other stuff is reversed?
	return -CBaseMonster::IRelationship( pTarget );
}


void WWSentryCrystal::Fire( CBaseEntity * pEnemy )
{
	if( !pEnemy )
		return;

	if( gpGlobals->time < m_flNextFire )
		return;

	m_flLastFire = gpGlobals->time;

	EMIT_SOUND( ENT(pev), CHAN_AUTO, "spells/icepoke.wav", 1.0f, ATTN_NORM );

	pEnemy->TakeDamage( VARS(INDEXENT(0)), VARS(INDEXENT(0)), 1000.0f, DMG_GENERIC );

	Vector vOrigin, vAngles;

	CBaseMonster::GetBonePosition( 0, vOrigin, vAngles );

	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
		WRITE_BYTE ( TE_BEAMENTPOINT );
		WRITE_SHORT( pEnemy->entindex() );
		WRITE_COORD( vOrigin.x );
		WRITE_COORD( vOrigin.y );
		WRITE_COORD( vOrigin.z );
		WRITE_SHORT( g_sModelIndexSmoke );
		WRITE_BYTE ( 0   );		// framestart
		WRITE_BYTE ( 0   );		// framerate
		WRITE_BYTE ( 1   );		// life
		WRITE_BYTE ( 2   );		// width
		WRITE_BYTE ( 0   );		// noise
		WRITE_BYTE ( 255 );		// r
		WRITE_BYTE ( 0   );		// g
		WRITE_BYTE ( 0   );		// b
		WRITE_BYTE ( 255 );		// brightness
		WRITE_BYTE ( 0   );		// speed
	MESSAGE_END();

	m_flNextFire = gpGlobals->time + 0.5f;
}


void WWSentryCrystal::TraceAttack( entvars_t * pevAttacker, float flDamage, Vector vecDir, TraceResult * ptr, int bitsDamageType )
{
	return;
}


int WWSentryCrystal::TakeDamage( entvars_t * pevInflictor, entvars_t * pevAttacker, float flDamage, int bitsDamageType )
{
	return 0;
}

