#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "effects.h"
#include "saverestore.h"
#include "weapons.h"
#include "nodes.h"
#include "doors.h"
#include "game.h"
#include "gamerules.h"
#include "items.h"
#include "player.h"

#include "tf_globals.h"
#include "tf_globalinfo.h"
#include "tf_teamcheck.h"
#include "tf_ent.h"
#include "tf_info_goal.h"
#include "tf_item_goal.h"
#include "tf_info_timer.h"
#include "tf_info_teamspawn.h"


LINK_ENTITY_TO_TF( info_tfgoal_timer, TFInfoTimer, TFEntity );
LINK_ENTITY_TO_TF( info_wwgoal_timer, TFInfoTimer, TFEntity );
LINK_ENTITY_TO_TF( i_t_t, TFInfoTimer, TFEntity );
LINK_ENTITY_TO_TF( i_w_t, TFInfoTimer, TFEntity );


TFInfoTimer::TFInfoTimer()
{
	m_flSearchTime = 0.0f;
	m_flInActiveTime = 0.0f;
}


//------------------------------------------------------------------------------
// Spawn:
//
//------------------------------------------------------------------------------
void TFInfoTimer::Spawn( void )
{
	Precache();

	m_iState = m_iInitialState;

	pev->movetype = MOVETYPE_NONE;
	pev->solid	  = SOLID_TRIGGER;

	SetThink(&TFInfoTimer:: GoalThink );
	SetTouch( NULL );

	if( m_iModel != 0 )
	{
		SET_MODEL( edict(), STRING( m_iModel ) );

		if( m_iState == TF_GOALSTATE_ACTIVE )
			pev->effects &= ~EF_NODRAW;
		else
			pev->effects |= EF_NODRAW;
	}
	else
		pev->effects |= EF_NODRAW;


	// I guess info timers that drop also glow?!
	if( m_iModel != 0 && m_iGoalActivation & TF_GOAL_ACTIVATION_DROP_MODEL_ON_SPAWN )
	{
		pev->renderfx = kRenderFxGlowShell;

		switch( m_iOwner )
		{
		default:
		case TF_TEAM_NONE:   pev->rendercolor = Vector( 200, 200, 200 ); break;
		case TF_TEAM_BLUE:   pev->rendercolor = Vector(   0,   0, 200 ); break;
		case TF_TEAM_RED:    pev->rendercolor = Vector( 200,   0,   0 ); break;
		case TF_TEAM_YELLOW: pev->rendercolor = Vector( 200, 200,   0 ); break;
		case TF_TEAM_GREEN:  pev->rendercolor = Vector(   0, 200,   0 ); break;
		}

		pev->renderamt = 10;
	}
	else
	{
		pev->renderfx		= kRenderFxNone;
		pev->rendercolor	= Vector( 200, 200, 200 );
		pev->renderamt		= 255;
	}

	if( m_iState != TF_GOALSTATE_REMOVED )
		m_flNextCheck = gpGlobals->time + m_flSearchTime;
	else
		m_flNextCheck = 0.0f;

	pev->nextthink = gpGlobals->time + 0.1f;
}


void TFInfoTimer::Precache( void )
{
	if( m_iModel != 0 )
		PRECACHE_MODEL( (char *)STRING( m_iModel ) );

	if( m_iMessages[ TF_MSG_SOUND ] != 0 )
		PRECACHE_SOUND( (char *)STRING( m_iMessages[ TF_MSG_SOUND ] ) );
}


//------------------------------------------------------------------------------
// KeyValue:
//	Parsing settings
//------------------------------------------------------------------------------
void TFInfoTimer::KeyValue( KeyValueData * pkvd )
{	
	if( ParseSettings( pkvd->szKeyName, pkvd->szValue) )
		pkvd->fHandled = TRUE;
	else
	{
		ALERT( at_console, "TFInfoTimer(%s)::KeyValue( \"%s\", \"%s\" ) not handled.\n",
			STRING( pev->netname ), pkvd->szKeyName, pkvd->szValue );

		CBaseEntity::KeyValue( pkvd );
	}
};


//------------------------------------------------------------------------------
// ParseSettings:
//	Parsing settings
//------------------------------------------------------------------------------
BOOL TFInfoTimer::ParseSettings( const char * key, const char * value )
{
	if( TFStrEq( 1, key, "search_time" ) )
	{
		m_flSearchTime = atoi( value );

		return TRUE;
	}

	return TFEntity::ParseSettings( key, value );
}


//------------------------------------------------------------------------------
// Use:
//	Called when another entity triggers us
//------------------------------------------------------------------------------
void TFInfoTimer::Use( CBaseEntity * pActivate, CBaseEntity * pCaller, USE_TYPE useType, float value )
{
	if( m_iMaster != 0 && !UTIL_IsMasterTriggered( m_iMaster, pActivate ) )
		return;

	GoalActivate( pActivate );
}


//------------------------------------------------------------------------------
// GoalThink:
//	Does the thinking for the entity
//------------------------------------------------------------------------------
void TFInfoTimer::GoalThink( void )
{
	if( m_iModel != 0 )
	{
		if( m_iState != TF_GOALSTATE_REMOVED )
			pev->effects &= ~EF_NODRAW;
		else
			pev->effects |= EF_NODRAW;
	}

	if( m_flNextCheck != 0.0 && gpGlobals->time >= m_flNextCheck && m_iState != TF_GOALSTATE_REMOVED )
	{
		m_flNextCheck = 0.0f;
		GoalActivate( NULL );
	}

	if( m_flInActiveTime != 0.0 && gpGlobals->time >= m_flInActiveTime )
	{
		m_flInActiveTime = 0.0f;
		GoalInActivate( NULL );
	}

	pev->nextthink = gpGlobals->time + 0.1f; //m_flSearchTime;
}


//------------------------------------------------------------------------------
// Activate:
//	Does the activation event
//------------------------------------------------------------------------------
void TFInfoTimer::GoalActivate( CBaseEntity * pEntity )
{
	Debug( "Timer(%s) trying to activate.", Name() );

	if( m_iState != TF_GOALSTATE_INACTIVE )
	{
		Debug( "Activate failed. Goal was not inactive." );
		return;
	}

	if( !CheckActivation( pEntity ) )
	{
		Debug( "Activate failed. Going inactive." );
		GoalInActivate( pEntity );	
		return;
	}

	ProcessGoals( pEntity );

	if( m_iModel != 0 )
		pev->effects &= ~EF_NODRAW;

	m_flNextCheck		= gpGlobals->time + m_flSearchTime;
	m_flInActiveTime	= gpGlobals->time + m_flStayActive;

	if( m_iGoalResults & TF_GOAL_RESULT_REMOVE_IMMEDIATELY )
		GoalRemove( pEntity );

	Debug( "Activation sucessful." );
}


//------------------------------------------------------------------------------
// GoalInActivate:
//	Does the inactivation event
//------------------------------------------------------------------------------
void TFInfoTimer::GoalInActivate( CBaseEntity * pEntity )
{
	Debug( "Timer(%s) trying to inactivate.", Name() );
/*
	if( m_iState == TF_GOALSTATE_INACTIVE )
	{
		Debug( "Inactivation failed. Goal is already inactive." );
		return;
	}
*/
	m_iState = TF_GOALSTATE_INACTIVE;
	m_flNextCheck = gpGlobals->time + m_flSearchTime;

	Debug( "Inactivation sucessful." );
}


//------------------------------------------------------------------------------
// GoalRemove:
//	Does the remove event
//------------------------------------------------------------------------------
void TFInfoTimer::GoalRemove( CBaseEntity * pEntity )
{
	Debug( "Timer(%s) trying to remove.", Name() );
/*
	if( m_iState == TF_GOALSTATE_REMOVED )
	{
		Debug( "Remove failed. Already removed." );
		return;
	}
*/
	m_iState = TF_GOALSTATE_REMOVED;

	m_flNextCheck		= 0.0f;
	m_flInActiveTime	= 0.0f;

	pev->effects |= EF_NODRAW;

	Debug( "Remove sucessful." );
}

//------------------------------------------------------------------------------
// GoalRestore:
//	Does the restore event
//------------------------------------------------------------------------------
void TFInfoTimer::GoalRestore( CBaseEntity * pEntity )
{
	Debug( "Timer(%s) trying to restore.", Name() );
/*
	if( m_iState != TF_GOALSTATE_REMOVED )
	{
		Debug( "Restore failed. Timer is not removed." );
		return;
	}
*/

	m_iState = TF_GOALSTATE_INACTIVE;
	m_flNextCheck = gpGlobals->time + m_flSearchTime;

	Debug( "Restore sucessful." );
}
