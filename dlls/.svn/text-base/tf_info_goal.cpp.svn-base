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



LINK_ENTITY_TO_TF( info_tfgoal, TFInfoGoal, TFEntity );
LINK_ENTITY_TO_TF( info_wwgoal, TFInfoGoal, TFEntity );
LINK_ENTITY_TO_TF( i_t_g, TFInfoGoal, TFEntity );
LINK_ENTITY_TO_TF( i_w_g, TFInfoGoal, TFEntity );


TFInfoGoal::TFInfoGoal()
{
	m_flLastActivate	= 0.0f;
	m_flActivateTime	= 0.0f;
	m_flInActiveTime	= 0.0f;
	m_flNextTouch		= 0.0f;
	m_hActivator		= NULL;
}


//------------------------------------------------------------------------------
// Spawn:
//
//------------------------------------------------------------------------------
void TFInfoGoal::Spawn( void )
{
	Precache();

	m_iState = m_iInitialState;

	pev->solid = SOLID_TRIGGER;

	if( m_iModel != 0 )
	{
		SET_MODEL( ENT( pev ), STRING( m_iModel ) );

		if( m_iGoalActivation & TF_GOAL_ACTIVATION_DROP_MODEL_ON_SPAWN )
			pev->movetype = MOVETYPE_TOSS;
		else
			pev->movetype = MOVETYPE_NONE;

		if( m_iState != TF_GOALSTATE_REMOVED )
			pev->effects &= ~EF_NODRAW;
		else
			pev->effects |= EF_NODRAW;
	}
	else
	{
		pev->movetype = MOVETYPE_NONE;

		SET_MODEL( ENT( pev ), STRING( pev->model ) );

		pev->effects |= EF_NODRAW;
	}


	// I guess info goals that drop also glow?!
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


	SetThink(&TFInfoGoal:: GoalThink );
	SetTouch(&TFInfoGoal:: GoalTouch );

	if( m_iModel )
		ResetSequenceInfo();

	m_flNextTouch  = gpGlobals->time + 0.1f;
	pev->nextthink = gpGlobals->time + 0.1f;
}


//------------------------------------------------------------------------------
// Precache:
//
//------------------------------------------------------------------------------
void TFInfoGoal::Precache()
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
void TFInfoGoal::KeyValue( KeyValueData * pkvd )
{	
	if( ParseSettings( pkvd->szKeyName, pkvd->szValue) )
		pkvd->fHandled = TRUE;
	else
	{
		ALERT( at_console, "TFInfoGoal(%s)::KeyValue( \"%s\", \"%s\" ) not handled.\n",
			STRING( pev->netname ), pkvd->szKeyName, pkvd->szValue );

		CBaseEntity::KeyValue( pkvd );
	}
};


//------------------------------------------------------------------------------
// ParseSettings:
//	Parsing settings
//------------------------------------------------------------------------------
BOOL TFInfoGoal::ParseSettings( const char * key, const char * value )
{
	return TFEntity::ParseSettings( key, value );
}


//------------------------------------------------------------------------------
// Use:
//	Called when another entity triggers us
//------------------------------------------------------------------------------
void TFInfoGoal::Use( CBaseEntity * pActivate, CBaseEntity * pCaller, USE_TYPE useType, float value )
{
	Debug( "Goal(%s) trying to be used.", GoalName() );

	if( m_iMaster != 0 && !UTIL_IsMasterTriggered( m_iMaster, pActivate ) )
	{
		Debug( "Use failed. Master was not triggered." );
		return;
	}

	GoalActivate( pActivate );

	Debug( "Use sucessful." );
}


//------------------------------------------------------------------------------
// GoalThink:
//	Does the thinking for the entity
//------------------------------------------------------------------------------
void TFInfoGoal::GoalThink( void )
{
	if( m_iModel )
		StudioFrameAdvance();

	if( m_flActivateTime != 0.0f && gpGlobals->time >= m_flActivateTime )
	{
		m_flActivateTime = 0.0f;
		m_flInActiveTime = gpGlobals->time + m_flStayActive;

		ProcessGoals( m_hActivator );

		if( m_iModel != 0 )
			pev->effects |= EF_NODRAW;

		SetTouch( NULL );
	}

	if( m_flInActiveTime != 0.0f && gpGlobals->time >= m_flInActiveTime )
	{
		GoalInActivate( NULL );
		m_flInActiveTime = 0.0f;
	}

	pev->nextthink = gpGlobals->time + 0.1f;
}


//------------------------------------------------------------------------------
// GoalTouch:
//	Process entity when touched
//------------------------------------------------------------------------------
void TFInfoGoal::GoalTouch( CBaseEntity * pEntity )
{
	Debug( "Goal(%s) was touched by entity(%s).", GoalName(), STRING( pEntity->pev->netname ) );

	if( m_iState != TF_GOALSTATE_INACTIVE )
	{
		Debug( "Touch failed. Goal is removed." );
		return;
	}

	if( !( m_iGoalActivation & TF_GOAL_ACTIVATION_PLAYER_TOUCH ) )
	{
		Debug( "Touch failed. Not accepting touch calls." );
		return;
	}

	if( !pEntity || !pEntity->IsPlayer() || !pEntity->IsAlive() || pEntity->IsObserver() )
	{
		Debug( "Touch failed. Not a valid player." );
		return;
	}

	if( m_flNextTouch >= gpGlobals->time )
	{
		Debug( "Touch failed. Touched to soon." );
		return;
	}

	GoalActivate( pEntity );

	m_flNextTouch = gpGlobals->time + 0.1f;

	Debug( "Touch sucessful." );
}


//------------------------------------------------------------------------------
// Activate:
//	Does the activation event
//------------------------------------------------------------------------------
void TFInfoGoal::GoalActivate( CBaseEntity * pEntity )
{
	Debug( "Goal(%s) trying to activate.", GoalName() );

	if( m_iState == TF_GOALSTATE_ACTIVE )
	{
		Debug( "Activation failed. Goal is already active." );
		return;
	}

	// wait between activations
	if( gpGlobals->time < m_flLastActivate + m_flActiveTime )
	{
		Debug( "Activation failed. Not enough time since last activation." );
		return;
	}

	// if we still have an activation waiting
	if( m_flActivateTime != 0.0f )
	{
		Debug( "Activation failed. Still have an activation pending." );
		return;
	}

	if( !CheckActivation( pEntity ) )
	{
		Debug( "Activation failed. Failed standard activation check." );

		if( m_iElseGoal != 0 )
		{
			TFEntity * pTF = UTIL_FindEntityByTFId( NULL, m_iElseGoal );

			if( pTF != NULL )
				pTF->GoalActivate( pEntity );
		}

		return;
	}
	
	m_iState		 = TF_GOALSTATE_ACTIVE;
	m_hActivator	 = pEntity;
	m_flLastActivate = gpGlobals->time;

	if( m_flDelayTime != 0.0f )
		m_flActivateTime = gpGlobals->time + m_flDelayTime;
	else
	{
		m_flActivateTime = 0.0f;
		m_flInActiveTime = gpGlobals->time + m_flStayActive;

		ProcessGoals( m_hActivator );

		if( m_iModel != 0 )
			pev->effects |= EF_NODRAW;

		SetTouch( NULL );
	}

	Debug( "Activation sucessful." );
}


//------------------------------------------------------------------------------
// GoalInActivate:
//	Does the inactivation event
//------------------------------------------------------------------------------
void TFInfoGoal::GoalInActivate( CBaseEntity * pEntity )
{
	Debug( "Goal(%s) trying to inactivate.", GoalName() );
/*
	if( m_iState == TF_GOALSTATE_INACTIVE )
	{
		Debug( "Inactivation failed. Goal is already inactive." );
		return;
	}
*/
	m_iState = TF_GOALSTATE_INACTIVE;
	m_flInActiveTime = 0.0f;

	if( m_iModel != 0 )
		pev->effects &= ~EF_NODRAW;

	SetTouch(&TFInfoGoal:: GoalTouch );

	Debug( "Inactivation sucessful." );
}


//------------------------------------------------------------------------------
// GoalRemove:
//	Does the remove event
//------------------------------------------------------------------------------
void TFInfoGoal::GoalRemove( CBaseEntity * pEntity )
{
	Debug( "Goal(%s) trying to remove.", GoalName() );
/*
	if( m_iState == TF_GOALSTATE_REMOVED )
	{
		Debug( "Remove failed. Goal is already removed." );
		return;
	}
*/
	m_iState = TF_GOALSTATE_REMOVED;
	m_flActivateTime = 0.0f;
	m_flInActiveTime = 0.0f;

	if( m_iModel != 0 )
		pev->effects |= EF_NODRAW;

	SetTouch( NULL );

	Debug( "Remove sucessful." );
}

//------------------------------------------------------------------------------
// GoalRestore:
//	Does the restore event
//------------------------------------------------------------------------------
void TFInfoGoal::GoalRestore( CBaseEntity * pEntity )
{
	Debug( "Goal(%s) trying to restore.", GoalName() );
/*
	if( m_iState != TF_GOALSTATE_REMOVED )
	{
		Debug( "Restore failed. Goal is not removed." );
		return;
	}
*/
	m_iState = TF_GOALSTATE_INACTIVE;
	m_flInActiveTime = 0.0f;

	if( m_iModel != 0 )
		pev->effects &= ~EF_NODRAW;

	SetTouch(&TFInfoGoal:: GoalTouch );

	Debug( "Restore sucessful." );
}
