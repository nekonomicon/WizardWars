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


LINK_ENTITY_TO_TF( item_tfgoal, TFItemGoal, TFEntity );
LINK_ENTITY_TO_TF( item_wwgoal, TFItemGoal, TFEntity );


void TFItemGoal::DropItems( CBasePlayer * pPlayer, BOOL bForced )
{
	if( !pPlayer )
		return;

	// cycle through all ents looking for goal ents
//	for( int i = 1; i <= gpGlobals->maxEntities; i++ )
	for( TFEntity * pTF = TFEntity::m_pHead; pTF != NULL; pTF = pTF->m_pNext )
	{
		// HACK HACK HACK
		if( (int)pTF < 255 )
			break;

		if( pTF->m_pPrev == NULL && pTF != TFEntity::m_pHead )
			continue;

		if( pTF->Classify() != TF_ENT_ITEM_GOAL )
			continue;

		if( pTF->GetPlayer() == pPlayer )
		{
			if( !( pTF->m_iGoalActivation & TF_GOAL_ACTIVATION_IS_DROPPABLE ) && !bForced )
				continue;

			pTF->GoalDrop( pPlayer );

			Vector vAngles = pPlayer->pev->angles;
			vAngles.x = 0;
			UTIL_MakeVectors( vAngles );

			pTF->tfpev->velocity = pTF->tfpev->velocity + gpGlobals->v_forward * 256.0f;
		}
	}
}


TFItemGoal::TFItemGoal()
{
	m_hPlayer			= NULL;
	m_hActivator		= NULL;
	m_flLastActivate	= 0.0f;
	m_flActivateTime	= 0.0f;
	m_flInActiveTime	= 0.0f;
	m_flReturnTime		= 0.0f;
	m_flDropReturnTime	= 0.0f;
	m_flNoTouch			= 0.0f;
	m_flNextTouch		= 0.0f;
	m_bMoved			= false;
	m_iHudItems			= 0;
}


//------------------------------------------------------------------------------
// Spawn:
//
//------------------------------------------------------------------------------
void TFItemGoal::Spawn( void )
{
	Precache();

	m_iState = m_iInitialState;

	m_vOrigin = pev->origin;
	m_vAngles = pev->angles;

	if( m_iModel != 0 )
		SET_MODEL( ENT( pev ), STRING( m_iModel ) );

	if( m_iGoalActivation & TF_GOAL_ACTIVATION_DROP_MODEL_ON_SPAWN )
	{
		pev->movetype = MOVETYPE_TOSS;
		pev->solid    = SOLID_BBOX;

		SET_SIZE( edict(), Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );
	
		SetThink(&TFItemGoal:: FallThink );
	}
	else
	{
		pev->movetype = MOVETYPE_NONE;

		if( m_iGoalActivation & TF_GOAL_ACTIVATION_IS_SOLID )
			pev->solid = SOLID_BBOX;
		else
			pev->solid = SOLID_TRIGGER;

		SetThink(&TFItemGoal:: GoalThink );
	}

	pev->sequence	= LookupSequence( "not_carried" );
	ResetSequenceInfo();

	pev->origin		= m_vOrigin;
	pev->angles		= m_vAngles;

	SET_ORIGIN( edict(), pev->origin );

	m_hPlayer = NULL;

	SetTouch(&TFItemGoal:: GoalTouch );

	if( m_iState != TF_GOALSTATE_REMOVED )
		pev->effects &= ~EF_NODRAW;
	else
		pev->effects |= EF_NODRAW;


	if( m_iModel != 0 && m_iGoalActivation & TF_GOAL_ACTIVATION_GLOWS_WHEN_NOT_CARRIED )
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
	
	m_flNextTouch  = gpGlobals->time + 0.1f;
	pev->nextthink = gpGlobals->time + 0.1f;
}


//------------------------------------------------------------------------------
// Precache:
//
//------------------------------------------------------------------------------
void TFItemGoal::Precache()
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
void TFItemGoal::KeyValue( KeyValueData * pkvd )
{	
	if( ParseSettings( pkvd->szKeyName, pkvd->szValue) )
		pkvd->fHandled = TRUE;
	else
	{
		ALERT( at_console, "TFItemGoal(%s)::KeyValue( \"%s\", \"%s\" ) not handled.\n",
			STRING( pev->netname ), pkvd->szKeyName, pkvd->szValue );

		CBaseEntity::KeyValue( pkvd );
	}
};


//------------------------------------------------------------------------------
// ParseSettings:
//	Parsing settings
//------------------------------------------------------------------------------
BOOL TFItemGoal::ParseSettings( const char * key, const char * value )
{
	if( TFStrEq( 1, key, "items" ) )
	{
		m_iHudItems = atoi( value );

		return TRUE;
	}

	return TFEntity::ParseSettings( key, value );
}

//------------------------------------------------------------------------------
// PlaceItem:
//	Places item in its original location
//------------------------------------------------------------------------------
void TFItemGoal::PlaceItem( void )
{
	if( m_iGoalActivation & TF_GOAL_ACTIVATION_DROP_MODEL_ON_SPAWN )
	{
		pev->movetype = MOVETYPE_TOSS;
		pev->solid    = SOLID_BBOX;

		SET_SIZE( edict(), Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );
	
		SetThink(&TFItemGoal:: FallThink );
	}
	else
	{
		pev->movetype = MOVETYPE_NONE;

		if( m_iGoalActivation & TF_GOAL_ACTIVATION_IS_SOLID )
			pev->solid = SOLID_BBOX;
		else
			pev->solid = SOLID_TRIGGER;

		SetThink(&TFItemGoal:: GoalThink );
	}

	pev->effects &= ~EF_NODRAW;

	m_bMoved	= false;

	pev->origin = m_vOrigin;
	pev->angles = m_vAngles;
	pev->aiment = NULL;
	pev->owner  = NULL;

	SET_ORIGIN( edict(), pev->origin );

	m_hPlayer = NULL;
	m_iState  = TF_GOALSTATE_INACTIVE;

	if( m_iModel )
		ResetSequenceInfo();

	SetTouch(&TFItemGoal:: GoalTouch );

	pev->nextthink = gpGlobals->time + 0.1f;
}


//------------------------------------------------------------------------------
// Use:
//	Called when another entity triggers us
//------------------------------------------------------------------------------
void TFItemGoal::Use( CBaseEntity * pActivate, CBaseEntity * pCaller, USE_TYPE useType, float value )
{
	Debug( "Item(%s) trying to be used.", GoalName() );

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
void TFItemGoal::GoalThink( void )
{
	if( m_iModel )
		StudioFrameAdvance();

	if( m_hPlayer != NULL && m_iState == TF_GOALSTATE_ACTIVE )
	{
		if( !( m_iGoalActivation & TF_GOAL_ACTIVATION_KEEP_ITEM_WHEN_PLAYER_DIES ) )
		{
			CBasePlayer * pPlayer = GetPlayer();

			// if our carrier is dead or an observer
			if( !pPlayer->IsAlive() || pPlayer->IsObserver() )
			{
				if( m_iGoalActivation & TF_GOAL_ACTIVATION_DROP_ON_DEATH )
					GoalDrop( pPlayer );
			}
		}
	}

	if( m_flActivateTime != 0.0f && gpGlobals->time >= m_flActivateTime )
	{
		ProcessGoals( m_hActivator );
		m_flActivateTime = 0.0f;
//		m_flInActiveTime = gpGlobals->time + m_flStayActive;
	}

	if( m_flInActiveTime != 0.0f && gpGlobals->time >= m_flInActiveTime )
	{
		GoalInActivate( NULL );
		m_flInActiveTime = 0.0f;
	}

	if( m_flReturnTime != 0.0f && gpGlobals->time >= m_flReturnTime )
	{
		GoalReturn( NULL );
		m_flReturnTime = 0.0f;
	}

	if( m_flDropReturnTime != 0.0f && gpGlobals->time >= m_flDropReturnTime )
	{
		if( m_iGoalActivation & TF_GOAL_ACTIVATION_REMOVE_AFTER_PAUSE )
		{
			GoalRemove( NULL );

			if( m_iGoalActivation & TF_GOAL_ACTIVATION_RETURN_AFTER_PAUSE_REMOVE )
				GoalReturn( NULL );
		}
		else
			GoalReturn( NULL );

		m_flDropReturnTime = 0.0f;
	}

	pev->nextthink = gpGlobals->time + 0.1f;
}


//------------------------------------------------------------------------------
// FallThink:
//	
//------------------------------------------------------------------------------
void TFItemGoal::FallThink( void )
{
	pev->movetype	= MOVETYPE_TOSS;
	pev->solid		= SOLID_SLIDEBOX;

	if ( pev->flags & FL_ONGROUND )
	{
		pev->angles.x = 0;

		pev->movetype = MOVETYPE_NONE;

		if( m_iGoalActivation & TF_GOAL_ACTIVATION_IS_SOLID )
			pev->solid = SOLID_SLIDEBOX;
		else
			pev->solid = SOLID_TRIGGER;

		SET_MODEL( edict(), STRING( pev->model ) );
		SET_ORIGIN( edict(), pev->origin );// link into world.

		SetThink(&TFItemGoal:: GoalThink );
	}

	pev->nextthink = gpGlobals->time + 0.1f;
}



//------------------------------------------------------------------------------
// GoalTouch:
//	Process entity when touched
//------------------------------------------------------------------------------
void TFItemGoal::GoalTouch( CBaseEntity * pEntity )
{
	Debug( "Item(%s) was touched by entity(%s).", GoalName(), STRING( pEntity->pev->netname ) );

	if( m_iState != TF_GOALSTATE_INACTIVE )
	{
		Debug( "Touch failed. Goal is not inactive." );
		return;
	}

	if( m_flNoTouch != 0.0f && gpGlobals->time <= m_flNoTouch )
	{
		Debug( "Touch failed. Can not touch goal yet." );
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
// Attach:
//	Attaches the item onto the player so they can carry it around
//------------------------------------------------------------------------------
void TFItemGoal::Attach( CBasePlayer * pPlayer )
{
	if( !pPlayer )
		return;

	pev->origin		= pPlayer->pev->origin;

	pev->movetype	= MOVETYPE_FOLLOW;
	pev->aiment		= pPlayer->edict();
	pev->owner		= pPlayer->edict();

	m_bMoved		= true;

	pev->sequence	= LookupSequence( "carried" );
	ResetSequenceInfo();

	SetThink(&TFItemGoal:: GoalThink );
	SetTouch( NULL );
}


//------------------------------------------------------------------------------
// Detach:
//	Detaches the item from the player
//------------------------------------------------------------------------------
void TFItemGoal::Detach( void )
{
	pev->movetype	= MOVETYPE_TOSS;
	pev->aiment		= NULL;
	pev->owner		= NULL;

	pev->sequence	= LookupSequence( "not_carried" );
	ResetSequenceInfo();

	pev->flags &= ~FL_ONGROUND;

	SET_ORIGIN( edict(), pev->origin );
	SET_SIZE( edict(), Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );

	pev->velocity.x	= RANDOM_FLOAT(  -50,  50 );
	pev->velocity.y	= RANDOM_FLOAT(  -50,  50 );
	pev->velocity.z	= RANDOM_FLOAT(  100, 200 );

	if( m_hPlayer != NULL )
	{
		CBasePlayer * pPlayer = GetPlayer();

		pPlayer->SetHudItems( m_iHudItems, FALSE );

		if( !( m_iGoalActivation & TF_GOAL_ACTIVATION_PLAYER_KEEPS_EFFECTS ) )
		{
			if( m_flGodTime )
				pPlayer->SetTimer( TIMER_INVULNERABILITY, 0.0f );

			if( m_flQuadTime )
				pPlayer->SetTimer( TIMER_SUPERDAMAGE, 0.0f );

			if( m_flInvisTime )
				pPlayer->SetTimer( TIMER_INVISIBILITY, 0.0f );

			if( m_flRadTime )
				pPlayer->SetTimer( TIMER_PROTECTION, 0.0f );

			if( m_iGoalResults & TF_GOAL_RESULT_PREVENT_SPY_DISGUISE )
				pPlayer->AllowSpy( TRUE );

			if( m_iGoalActivation & TF_GOAL_ACTIVATION_PLAYER_GLOWS )
				pPlayer->pev->effects &= ~EF_BRIGHTLIGHT;

			if( m_iGoalActivation & TF_GOAL_ACTIVATION_REDUCE_SPEED )
				pPlayer->SetSpeed( SPEED_HALFSPEED, false );

			if( m_iGoalResults & TF_GOAL_RESULT_PREVENT_SPY_DISGUISE )
				pPlayer->AllowSpy( TRUE );
		}

		m_hPlayer = NULL;
	}

	
	SetThink(&TFItemGoal:: FallThink );
	SetTouch(&TFItemGoal:: GoalTouch );
}


//------------------------------------------------------------------------------
// Activate:
//	Does the activation event
//------------------------------------------------------------------------------
void TFItemGoal::GoalActivate( CBaseEntity * pEntity )
{
	Debug( "Item(%s) trying to activate.", GoalName() );

	if( m_iState == TF_GOALSTATE_ACTIVE )
	{
		Debug( "Activate failed. Goal is already active." );
		return;
	}

	// wait between activations
	if( gpGlobals->time < m_flActiveTime - m_flLastActivate )
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

	m_iState			= TF_GOALSTATE_ACTIVE;
	m_hActivator		= pEntity;
	m_flLastActivate	= gpGlobals->time;

	if( m_flDelayTime != 0.0f )
		m_flActivateTime = gpGlobals->time + m_flDelayTime;
	else
		ProcessGoals( m_hActivator );

	Debug( "Activation sucessful." );
}


//------------------------------------------------------------------------------
// InActivate:
//	Does the inactivation event
//------------------------------------------------------------------------------
void TFItemGoal::GoalInActivate( CBaseEntity * pEntity )
{
	Debug( "Item(%s) trying to inactivate.", GoalName() );

	if( m_iState != TF_GOALSTATE_INACTIVE )
	{
		Debug( "Inactivation failed. Goal is already inactive." );
		return;
	}

	m_iState = TF_GOALSTATE_INACTIVE;
	pev->effects &= ~EF_NODRAW;

	if( pev->movetype == MOVETYPE_FOLLOW || m_hPlayer != NULL )
		Detach();

	Debug( "Inactivation sucessful." );
}


//------------------------------------------------------------------------------
// Remove:
//	Does the remove event
//------------------------------------------------------------------------------
void TFItemGoal::GoalRemove( CBaseEntity * pEntity )
{
	Debug( "Item(%s) trying to remove.", GoalName() );

	if( m_iState == TF_GOALSTATE_REMOVED )
	{
		Debug( "Remove failed. Goal is already removed." );
		return;
	}

	if( pev->movetype == MOVETYPE_FOLLOW || m_hPlayer != NULL )
		Detach();

	m_iState = TF_GOALSTATE_REMOVED;
	pev->effects |= EF_NODRAW;

	if( m_iGoalActivation & TF_GOAL_ACTIVATION_RETURN_IF_REMOVE_BY_GOAL )
		m_flReturnTime = gpGlobals->time;

	Debug( "Remove sucessful." );
}


//------------------------------------------------------------------------------
// Restore:
//	Does the restore event
//------------------------------------------------------------------------------
void TFItemGoal::GoalRestore( CBaseEntity * pEntity )
{
	Debug( "Item(%s) trying to restore.", GoalName() );

	if( m_iState != TF_GOALSTATE_REMOVED )
	{
		Debug( "Restore failed. Goal is not removed." );
		return;
	}

	if( pev->movetype == MOVETYPE_FOLLOW )
		GoalDrop( NULL );

	m_hPlayer = NULL;
	m_iState = TF_GOALSTATE_INACTIVE;
	pev->effects &= ~EF_NODRAW;

	Debug( "Restore sucessful." );
}


//------------------------------------------------------------------------------
// Grab:
//	Handles when a player grabs this goal
//------------------------------------------------------------------------------
void TFItemGoal::GoalGrab( CBaseEntity * pEntity )
{
	if( !pEntity || !pEntity->IsPlayer() || !pEntity->IsAlive() )
		return;

	if( m_iState != TF_GOALSTATE_ACTIVE )
	{
		ProcessEffects( pEntity );
		m_iState = TF_GOALSTATE_ACTIVE;
	}

	m_hPlayer = pEntity;

	Attach( GetPlayer() );

	m_flReturnTime = m_flDropReturnTime = 0.0f;

	pev->effects &= ~EF_NODRAW;

	if( m_iGoalActivation & TF_GOAL_ACTIVATION_PLAYER_GLOWS )
		pEntity->pev->effects |= EF_BRIGHTLIGHT;

	if( m_iGoalActivation & TF_GOAL_ACTIVATION_REDUCE_SPEED )
		((CBasePlayer *)pEntity)->SetSpeed( SPEED_HALFSPEED, true );

	// all items glow when carried
	if( m_iModel != 0 )
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
}

//------------------------------------------------------------------------------
// Drop:
//	Handles when the goal has been dropped
//------------------------------------------------------------------------------
void TFItemGoal::GoalDrop( CBaseEntity * pEntity )
{
	if( pev->movetype != MOVETYPE_FOLLOW )
		return;

	if( m_iModel != 0 && m_iGoalActivation & TF_GOAL_ACTIVATION_GLOWS_WHEN_NOT_CARRIED )
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

	Detach();

	m_iState			= TF_GOALSTATE_INACTIVE;
	m_flDropReturnTime	= gpGlobals->time + m_flPauseTime;
	m_flNoTouch			= gpGlobals->time + 1.5f;

	// only show messages if dropped by a player
	int i;
	for( i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBaseEntity * pPlayer = NULL;

		pPlayer = UTIL_PlayerByIndex( i );

		if( !pPlayer || pPlayer->IsObserver() )
			continue;

		int iMsg = 0;

		if( m_iMessages[ TF_MSG_DROP_AP ] != 0 && pPlayer == pEntity )
			iMsg = m_iMessages[ TF_MSG_DROP_AP ];
		
		else if( m_iMessages[ TF_MSG_DROP_AP_TEAM ] != 0 && pPlayer->pev->team == pEntity->pev->team )
			iMsg = m_iMessages[ TF_MSG_DROP_AP_TEAM ];
		
		else if( m_iMessages[ TF_MSG_DROP_OWNER ] != 0 && pPlayer->pev->team == m_iOwner )
			iMsg = m_iMessages[ TF_MSG_DROP_OWNER ];
		
		else if( m_iMessages[ TF_MSG_DROP_AP_NONTEAM ] != 0 && pPlayer->pev->team != pEntity->pev->team )
			iMsg = m_iMessages[ TF_MSG_DROP_AP_NONTEAM ];

		else if( m_iMessages[ TF_MSG_DROP_NONOWNER ] != 0 && pPlayer->pev->team != m_iOwner )
			iMsg = m_iMessages[ TF_MSG_DROP_NONOWNER ];

		if( iMsg != 0 )
		{
			MESSAGE_BEGIN( MSG_ONE, gmsgHudText, NULL, pPlayer->edict() );
				WRITE_STRING( STRING( iMsg ) );
				if( pEntity != NULL && pEntity->IsPlayer() )
					WRITE_STRING( STRING( pEntity->pev->netname ) );
			MESSAGE_END();
		}
	}

	if( m_iGoalActivation & TF_GOAL_ACTIVATION_RETURN_ON_DEATH_DROP )
		m_flDropReturnTime = gpGlobals->time;
}


//------------------------------------------------------------------------------
// Return:
//	Handles when the goal has been dropped
//------------------------------------------------------------------------------
void TFItemGoal::GoalReturn( CBaseEntity * pEntity )
{
	Debug( "Item(%s) trying to return.", GoalName() );

	if( pev->movetype == MOVETYPE_FOLLOW || m_hPlayer != NULL )
		Detach();

	PlaceItem();

	int i;
	for( i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBaseEntity * pPlayer = NULL;

		pPlayer = UTIL_PlayerByIndex( i );

		if( !pPlayer || pPlayer->IsObserver() )
			continue;

		int iMsg = 0;

		if( m_iMessages[ TF_MSG_RETURN_AP ] != 0 && pPlayer == pEntity )
			iMsg = m_iMessages[ TF_MSG_RETURN_AP ];
		
		else if( m_iMessages[ TF_MSG_RETURN_AP_TEAM ] != 0 && pPlayer->pev->team == m_iReqTeam )
			iMsg = m_iMessages[ TF_MSG_RETURN_AP_TEAM ];
		
		else if( m_iMessages[ TF_MSG_RETURN_OWNER ] != 0 && pPlayer->pev->team == m_iOwner )
			iMsg = m_iMessages[ TF_MSG_RETURN_OWNER ];
		
		else if( m_iMessages[ TF_MSG_RETURN_AP_NONTEAM ] != 0 && pPlayer->pev->team != m_iReqTeam )
			iMsg = m_iMessages[ TF_MSG_RETURN_AP_NONTEAM ];

		else if( m_iMessages[ TF_MSG_RETURN_NONOWNER ] != 0 && pPlayer->pev->team != m_iOwner )
			iMsg = m_iMessages[ TF_MSG_RETURN_NONOWNER ];

		if( iMsg != 0 )
		{
			MESSAGE_BEGIN( MSG_ONE, gmsgHudText, NULL, pPlayer->edict() );
				WRITE_STRING( STRING( iMsg ) );
				if( pEntity != NULL && pEntity->IsPlayer() )
					WRITE_STRING( STRING( pEntity->pev->netname ) );
			MESSAGE_END();
		}
	}

	if( m_iGoalOnReturn )
	{
		// cycle through all ents looking for goal ents
		// for( i = 1; i <= gpGlobals->maxEntities; i++ )
		for( TFEntity * pTF = m_pHead; pTF != NULL; pTF = pTF->m_pNext )
		{
			if( pTF->m_iId == m_iGoalOnReturn )
			{
				if( pTF->m_iState == TF_GOALSTATE_REMOVED )
					pTF->GoalRestore( pEntity );

				pTF->GoalActivate( pEntity );
			}
		}
	}

	Debug( "Return sucessful." );
}
