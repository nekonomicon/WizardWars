#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "pm_shared.h"

#include "tf_globals.h"
#include "tf_ent.h"
#include "tf_item_goal.h"


extern int gmsgCurWeapon;
extern int gmsgSetFOV;
extern int gmsgTeamInfo;
extern int gmsgSpectator;


// Player has become a spectator. Set it up.
// This was moved from player.cpp.
void CBasePlayer::StartObserver( Vector vecPosition, Vector vecViewAngle, BOOL bNoInit )
{
	// clear any clientside entities attached to this player
	MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, pev->origin );
		WRITE_BYTE( TE_KILLPLAYERATTACHMENTS );
		WRITE_BYTE( (BYTE)entindex() );
	MESSAGE_END();

	// Holster weapon immediately, to allow it to cleanup
	if( m_pActiveItem )
		m_pActiveItem->Holster( );

	if( m_pClass != NULL )
	{
		m_pClass->Remove();
		delete m_pClass;
		m_pClass = NULL;
	}

	// Setup flags
	m_iHideHUD		= HIDEHUD_ALL;
	pev->effects	= EF_NODRAW;
	pev->view_ofs	= g_vecZero;
	pev->angles		= pev->v_angle = vecViewAngle;
	pev->fixangle	= TRUE;
	pev->solid		= SOLID_NOT;
	pev->takedamage	= DAMAGE_NO;
	pev->movetype	= MOVETYPE_NONE;
	pev->deadflag	= DEAD_RESPAWNABLE;
	pev->health		= 1;
	m_afPhysicsFlags |= PFLAG_OBSERVER;
	ClearBits( m_afPhysicsFlags, PFLAG_DUCKING );
	ClearBits( pev->flags, FL_DUCKING );

	if( !bNoInit )
	{
		// Clear out the status bar
		m_fInitHUD = TRUE;

		// Update Team Status
		pev->team = 0;
		pev->playerclass = 0;

		MESSAGE_BEGIN( MSG_ALL, gmsgTeamInfo );
			WRITE_BYTE( entindex() );
			WRITE_BYTE( 0 );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
			WRITE_BYTE ( entindex() );
			WRITE_SHORT( 0 );
			WRITE_SHORT( 0 );
			WRITE_BYTE ( 0 );
			WRITE_BYTE ( 0 );
		MESSAGE_END();
	}

	// Remove all the player's stuff
	RemoveAllItems( FALSE, TRUE );

	TFItemGoal::DropItems( this, TRUE );

	// Move them to the new position
	UTIL_SetOrigin( pev, vecPosition );

	// Find a player to watch
	m_flNextObserverInput = 0;
	Observer_SetMode(OBS_CHASE_LOCKED);

	// Tell all clients this player is now a spectator
	MESSAGE_BEGIN( MSG_ALL, gmsgSpectator );  
		WRITE_BYTE( entindex() );
		WRITE_BYTE( 1 );
	MESSAGE_END();
}


// Leave observer mode
void CBasePlayer::StopObserver( void )
{
	// Turn off spectator
	if ( pev->iuser1 || pev->iuser2 )
	{
		// Tell all clients this player is not a spectator anymore
		MESSAGE_BEGIN( MSG_ALL, gmsgSpectator );  
			WRITE_BYTE( entindex() );
			WRITE_BYTE( 0 );
		MESSAGE_END();

		pev->iuser1 = pev->iuser2 = 0; 
		m_hObserverTarget = NULL;
	}
}


// Find the next client in the game for this player to spectate
void CBasePlayer::Observer_FindNextPlayer( bool bReverse )
{
	// MOD AUTHORS: Modify the logic of this function if you want to restrict the observer to watching
	//only a subset of the players. e.g. Make it check the target's team.

	int iStart;
	if ( m_hObserverTarget )
		iStart = ENTINDEX( m_hObserverTarget->edict() );
	else
		iStart = ENTINDEX( edict() );

	int iCurrent = iStart;
	m_hObserverTarget = NULL;
	int iDir = bReverse ? -1 : 1; 

	do
	{
		iCurrent += iDir;

		// Loop through the clients
		if( iCurrent > gpGlobals->maxClients )
			iCurrent = 1;
		if( iCurrent < 1 )
			iCurrent = gpGlobals->maxClients;

		CBaseEntity * pEnt = UTIL_PlayerByIndex( iCurrent );

		if( !pEnt )
			continue;

		if( pEnt == this )
			continue;

		// Don't spec observers or invisible players
		if( ((CBasePlayer*)pEnt)->IsObserver() || pEnt->pev->effects & EF_NODRAW )
			continue;

		// MOD AUTHORS: Add checks on target here.

		m_hObserverTarget = pEnt;
		break;

	} while( iCurrent != iStart );

	// Did we find a target?
	if( m_hObserverTarget )
	{
		// Store the target in pev so the physics DLL can get to it
		pev->iuser2 = ENTINDEX( m_hObserverTarget->edict() );

		// Move to the target
		UTIL_SetOrigin( pev, m_hObserverTarget->pev->origin );

		ALERT( at_console, "Now Tracking %s\n", STRING( m_hObserverTarget->pev->netname ) );
	}
	else
	{
		ALERT( at_console, "No observer targets.\n" );
	}
}


// Handle buttons in observer mode
void CBasePlayer::Observer_HandleButtons( void )
{
	// Slow down mouse clicks
	if( m_flNextObserverInput > gpGlobals->time )
		return;

	// Jump changes from modes: Chase to Roaming
	if( m_afButtonPressed & IN_JUMP )
	{
		if ( pev->iuser1 == OBS_CHASE_LOCKED )
			Observer_SetMode( OBS_CHASE_FREE );

		else if ( pev->iuser1 == OBS_CHASE_FREE )
			Observer_SetMode( OBS_ROAMING );

		else if ( pev->iuser1 == OBS_ROAMING )
			Observer_SetMode( OBS_IN_EYE );

		else if ( pev->iuser1 == OBS_IN_EYE )
			Observer_SetMode( OBS_MAP_FREE );

		else if ( pev->iuser1 == OBS_MAP_FREE )
			Observer_SetMode( OBS_MAP_CHASE );

		else
			Observer_SetMode( OBS_CHASE_LOCKED ); // don't use OBS_CHASE_LOCKED anymore

		m_flNextObserverInput = gpGlobals->time + 0.2;
	}

	// Attack moves to the next player
	if ( m_afButtonPressed & IN_ATTACK && pev->iuser1 != OBS_ROAMING )
	{
		Observer_FindNextPlayer( false );

		m_flNextObserverInput = gpGlobals->time + 0.2;
	}

	// Attack2 moves to the prev player
	if ( m_afButtonPressed & IN_ATTACK2 && pev->iuser1 != OBS_ROAMING )
	{
		Observer_FindNextPlayer( true );

		m_flNextObserverInput = gpGlobals->time + 0.2;
	}
}


// Attempt to change the observer mode
void CBasePlayer::Observer_SetMode( int iMode )
{
	// Just abort if we're changing to the mode we're already in
	if( iMode == pev->iuser1 )
		return;

	// is valid mode ?
	if( iMode < OBS_CHASE_LOCKED || iMode > OBS_MAP_CHASE )
		iMode = OBS_IN_EYE; // now it is

	// if we are not roaming, we need a valid target to track
	if( (iMode != OBS_ROAMING && iMode != OBS_MAP_FREE) && (m_hObserverTarget == NULL) )
	{
		Observer_FindNextPlayer( false );

		// if we didn't find a valid target switch to roaming
		if (m_hObserverTarget == NULL)
		{
			ClientPrint( pev, HUD_PRINTCENTER, "#Spec_NoTarget" );
			iMode = OBS_ROAMING;
		}
	}

	// set spectator mode
	pev->iuser1 = iMode;

	// set target if not roaming
	if( iMode == OBS_ROAMING || iMode == OBS_MAP_FREE )
		pev->iuser2 = 0;
	else
		pev->iuser2 = ENTINDEX( m_hObserverTarget->edict() );
}
