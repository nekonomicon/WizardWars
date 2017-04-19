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
#include "tf_ent.h"
#include "tf_info_teamspawn.h"


TFGlobalInfo * g_pTFGlobalInfo = NULL;
extern int gmsgTeamScore;

cvar_t tf_debug = { "tf_debug", "0" };


TFGlobalInfo::TFGlobalInfo()
{
	// we can only have one global info at a time
	// delete old one if it exists
	if( g_pTFGlobalInfo != NULL )
		delete g_pTFGlobalInfo;

	int i;
	for( i = 0; i < TF_TEAM_MAXTEAMS; i++ )
		m_pLastSpawnEntity[i] = NULL;

	memset( m_iScore,			0, sizeof( m_iScore				) );
	memset( m_iLives,			0, sizeof( m_iLives				) );
	memset( m_iMaxPlayers,		0, sizeof( m_iMaxPlayers		) );
	memset( m_iAllies,			0, sizeof( m_iAllies			) );
	memset( m_iClassLimits,		0, sizeof( m_iClassLimits		) );
	memset( m_iTeamName,		0, sizeof( m_iTeamName			) );
	memset( m_iFlagInfoItem,	0, sizeof( m_iFlagInfoItem		) );
	memset( m_iszFlagMsg,		0, sizeof( m_iszFlagMsg			) );

	m_iszVersion			= ALLOC_STRING( TF_VERSION );
	m_iszLocalCmd			= 0;

	m_bNoHook				= FALSE;
	m_bMultiSkin			= FALSE;
	m_bClassPersistence		= TRUE;
	m_bCheatChecking		= TRUE;
	m_bFortressMap			= TRUE;
	m_bRespawnDelay1		= FALSE;
	m_bRespawnDelay2		= FALSE;
	m_bAutoTeam				= FALSE;
	m_bTeamFrags			= FALSE;

	m_iNumTeams				= 0;
	m_iTeams				= TF_TEAM_NONE;
	m_iDefaultTeams			= TF_TEAM_NONE;
	m_iGlobalTeams			= TF_TEAM_NONE;
	m_iGlobalClasses		= 0;

	m_iTeamName[ TF_TEAM_BLUE   ] = ALLOC_STRING( "Blue"   );
	m_iTeamName[ TF_TEAM_RED    ] = ALLOC_STRING( "Red"    );
	m_iTeamName[ TF_TEAM_YELLOW ] = ALLOC_STRING( "Yellow" );
	m_iTeamName[ TF_TEAM_GREEN  ] = ALLOC_STRING( "Green"  );

	CVAR_REGISTER( &tf_debug );

	g_pTFGlobalInfo = this;
}

TFGlobalInfo::~TFGlobalInfo()
{
	g_pTFGlobalInfo = NULL;
}


int TFGlobalInfo::NumTeams( void )
{
	if( m_iNumTeams != 0 )
		return m_iNumTeams;

	// global team matching if info wasn't added ( damn lazy mappers :P )
	int teams = 0;

	// XYPHN - 040202
	//	Added m_iTeams to the check.

	if( ( m_iTeams | m_iGlobalTeams ) & 1 )
		teams++;
	
	if( ( m_iTeams | m_iGlobalTeams ) & 2 )
		teams++;

	if( ( m_iTeams | m_iGlobalTeams ) & 4 )
		teams++;

	if( ( m_iTeams | m_iGlobalTeams ) & 8 )
		teams++;

	return teams;
}


//------------------------------------------------------------------------------
// IsValidTeam:
//	Checks to see if iTeam is a valid team.
//------------------------------------------------------------------------------
BOOL TFGlobalInfo::IsValidTeam( int iTeam )
{
	if( g_pGameRules->IsTeamplay() )
	{
		// if it is an invalid team
		if( iTeam <= TF_TEAM_NONE || iTeam >= TF_TEAM_MAXTEAMS )
			return FALSE;

		// if this team is valid
		if( m_iTeams & ( 1 << ( iTeam - 1 ) ) )
			return TRUE;

		if( m_iTeams == 0 && m_iNumTeams != 0 && m_iDefaultTeams & ( 1 << ( iTeam - 1 ) ) )
			return TRUE;

		// global team matching if info wasn't added ( damn lazy mappers :P )
		if( m_iTeams == 0 && m_iNumTeams == 0 && m_iGlobalTeams & ( 1 << ( iTeam - 1 ) ) )
			return TRUE;
	}
	else
	{
		// if this team is 0
		if( iTeam == TF_TEAM_NONE )
			return TRUE;
	}

	// if we get here, its not valid
	return FALSE;
}


//------------------------------------------------------------------------------
// IsValidClass:
//	Checks to see if iClass is a valid class for team iTeam.
//------------------------------------------------------------------------------
BOOL TFGlobalInfo::IsValidClass( int iTeam, int iClass )
{
//	int iBits = 0;

	if( iClass == -1 )
		iClass = TF_CLASS_CIVILIAN;

	// if it is an invalid team
	if( !IsValidTeam( iTeam ) )
		return FALSE;

	// if this is an invalid class
	if( iClass <= TF_CLASS_NONE || iClass >= TF_CLASS_LASTCLASS )
		return FALSE;

	if( g_pGameRules->IsTeamplay() )
	{
		// if the team is civilian only and we arent a civilian
		if( m_iClassLimits[ iTeam ] == -1 && iClass != TF_CLASS_CIVILIAN )
			return FALSE;

		// if we are a civilian, and civilians arent set
		if( m_iClassLimits[ iTeam ] != -1 && iClass == TF_CLASS_CIVILIAN )
			return FALSE;

		// if the class matches a classlimit
		if( m_iClassLimits[ iTeam ] != -1 && m_iClassLimits[ iTeam ] & g_iTFClassBits[ iClass ] )
			return FALSE;

		// global class matching if info wasn't added ( damn lazy mappers :P )
		if( m_iNumTeams == 0 && !( m_iGlobalClasses & g_iTFClassBits[ iClass ] ) )
			return FALSE;

		return TRUE;
	}
	else
	{
		// no civilians in deathmatch
		if( iClass == TF_CLASS_CIVILIAN )
			return FALSE;

		// if the class matches a classlimit
		if( m_iClassLimits[ 0 ] & g_iTFClassBits[ iClass ] )
			return FALSE;
// Codiac 1.2 B17 - Death Match maps fail here!
		// global class matching if info wasn't added ( damn lazy mappers :P )
//		if( m_iNumTeams == 0 && !( m_iGlobalClasses & g_iTFClassBits[ iClass ] ) )
//			return FALSE;

		return TRUE;
	}

	// how did we get here?!
	return FALSE;
}


//------------------------------------------------------------------------------
// TeamScore:
//	Returns the current score for iTeam.
//------------------------------------------------------------------------------
int TFGlobalInfo::TeamScore( int iTeam )
{
	// if this isnt a valid team
	if( !IsValidTeam( iTeam ) )
		return 0;

	UTIL_LogPrintf( "TFGlobalInfo::TeamScore: score for %d is %d\n",
		iTeam, m_iScore[ iTeam ] );

	return m_iScore[ iTeam ];
}


//------------------------------------------------------------------------------
// SetTeamScore:
//	Sets the current score for team iTeam equal to iScore.
//------------------------------------------------------------------------------
void TFGlobalInfo::SetTeamScore( int iTeam, int iScore )
{
	// if this isnt a valid team
	if( !IsValidTeam( iTeam ) )
		return;

	UTIL_LogPrintf( "TFGlobalInfo::SetTeamScore: Changing team score from %d to %d\n",
		m_iScore[ iTeam ], iScore );

	// update TeamFrags
	UpdateFrags();
	m_iScore[ iTeam ] = iScore;

	// Codiac 1.2 B20 TeamScore Fix
	MESSAGE_BEGIN( MSG_BROADCAST, gmsgTeamScore, NULL );
		WRITE_SHORT( iTeam );
		WRITE_SHORT( TeamScore( 1 ) );
	MESSAGE_END();
}


//------------------------------------------------------------------------------
// AddTeamScore:
//	Adds iScore to the current score for team iTeam.
//------------------------------------------------------------------------------
void TFGlobalInfo::AddTeamScore( int iTeam, int iScore )
{
	// if this isnt a valid team
	if( !iScore || !IsValidTeam( iTeam ) )
		return;

	UTIL_LogPrintf( "\nTFGlobalInfo::AddTeamScore: Changing team score for %d from %d to %d\n",
		iTeam, m_iScore[ iTeam ], m_iScore[ iTeam ] + iScore );

	// update TeamFrags
	UpdateFrags();
	m_iScore[ iTeam ] += iScore;

	// Codiac 1.2 B20 TeamScore Fix
	MESSAGE_BEGIN( MSG_BROADCAST, gmsgTeamScore, NULL );
		WRITE_SHORT( iTeam );
		WRITE_SHORT( TeamScore( iTeam ) );
	MESSAGE_END();
}


//------------------------------------------------------------------------------
// TeamLives:
//	Returns the current life count for team iTeam.
//	-1 == Infinite.
//------------------------------------------------------------------------------
int TFGlobalInfo::TeamLives( int iTeam )
{
	// if this isnt a valid team
	if( !IsValidTeam( iTeam ) )
		return 0;

	return ( m_iLives[ iTeam ] == 0 ? -1 : m_iLives[ iTeam ] );
}


//------------------------------------------------------------------------------
// SetTeamLives:
//	Sets the current life count for team iTeam equal to iLives.
//	-1 == Infinite.
//------------------------------------------------------------------------------
void TFGlobalInfo::SetTeamLives( int iTeam, int iLives )
{
	// if this isnt a valid team
	if( !IsValidTeam( iTeam ) )
		return;

	m_iLives[ iTeam ] = iLives;
}


//------------------------------------------------------------------------------
// SetTeamLives:
//	Adds iLives to the life count for team iTeam.
//------------------------------------------------------------------------------
void TFGlobalInfo::AddTeamLives( int iTeam, int iLives )
{
	// if this isnt a valid team
	if( !IsValidTeam( iTeam ) )
		return;

	// if we have infinite lives, you cant add or subtract
	if( m_iLives[ iTeam ] == -1 )
		return;

	// we are already out of lives
	if( iLives < 0 && m_iLives[ iTeam ] == 0 )
		return;

	m_iLives[ iTeam ] += iLives;
}


//------------------------------------------------------------------------------
// IsFriendTeam:
//	Checks to see if iTeam2 is a friend of iTeam1.
//------------------------------------------------------------------------------
BOOL TFGlobalInfo::IsFriendTeam( int iTeam1, int iTeam2 )
{
	// we have no friends in deathmatch
	if( !g_pGameRules->IsTeamplay() )
		return FALSE;

	// the teams should be set if this is teamplay
	if( iTeam1 == TF_TEAM_NONE || iTeam2 == TF_TEAM_NONE )
		return FALSE;

	// if team1 isnt a valid team
	if( !IsValidTeam( iTeam1 ) )
		return FALSE;

	// if team2 isnt a valid team
	if( !IsValidTeam( iTeam2 ) )
		return FALSE;

	// if they are on the same team
	if( iTeam1 == iTeam2 )
		return TRUE;

	// if they match the allies flag for team1
	if( m_iAllies[ iTeam1 ] & g_iTFTeamBits[ iTeam2 ] )
		return TRUE;

	return FALSE;
}


//------------------------------------------------------------------------------
// GetSpawnDelay:
//	Returns the current respawn delay based on map settings
//------------------------------------------------------------------------------
float TFGlobalInfo::GetSpawnDelay( void )
{
	// if both are set
	if( m_bRespawnDelay1 && m_bRespawnDelay2 )
		return 20.0f;

	// if number 2 is set
	if( m_bRespawnDelay2 )
		return 10.0f;

	// if number 1 is set
	if( m_bRespawnDelay1 )
		return 5.0f;

	// if none are set
	return 0.0f;
}


//------------------------------------------------------------------------------
// CanJoinTeam:
//	Returns wheither or not player pPlayer can join team iTeam.
//------------------------------------------------------------------------------
BOOL TFGlobalInfo::CanJoinTeam( CBasePlayer * pPlayer, int iTeam )
{
	// if this is a bad ent
	if( !pPlayer || FNullEnt( pPlayer ) )
		return FALSE;

	// if iTeam is an invalid team
	if( !IsValidTeam( iTeam ) )
		return FALSE;

	int i, iCount = 0;

	// gather player count for team iTeam
	for( i = 1; i < gpGlobals->maxClients; i++ )
	{
		CBaseEntity * pEntity = UTIL_PlayerByIndex( i );

		// ignore bad ents
		if( !pEntity || FNullEnt( pEntity ) )
			continue;

		// ignore players not on the team we are checking
		if( pEntity->pev->team != iTeam )
			continue;

		// ignore the player we are checking
		if( pEntity->entindex() == pPlayer->entindex() )
			continue;

		iCount++;
	}

	// if there are already enough players on team iTeam
	if( iCount >= m_iMaxPlayers[ iTeam ] )
		return FALSE;

	// everything seems fine, go for it
	return TRUE;
}


//------------------------------------------------------------------------------
// CanChangeClass:
//	Returns wheither or not player pPlayer can change to class iClass on team
//  iTeam.
//------------------------------------------------------------------------------
BOOL TFGlobalInfo::CanChangeClass( CBasePlayer * pPlayer, int iClass )
{
	// if this is a bad ent
	if( !pPlayer || FNullEnt( pPlayer ) )
		return FALSE;

	// if pPlayer is not on a valid team
	if( !IsValidTeam( pPlayer->pev->team ) )
		return FALSE;

	// if the class pPlayer wants is not a valid class for pPlayer's team
	if( !IsValidClass( pPlayer->pev->team, iClass ) )
		return FALSE;

	// if class persistance is on
	if( m_bClassPersistence )
	{
		// if pPlayer hasn't selected a class yet
		if( pPlayer->pev->playerclass != TF_CLASS_NONE )
			return TRUE;

		// if pPlayer is just switching to the same class
		if( pPlayer->pev->playerclass == iClass )
			return TRUE;

		return FALSE;
	}

	// everything looks fine
	return TRUE;
}


//------------------------------------------------------------------------------
// CanChangeClass:
//	Returns wheither or not player pPlayer can change to class iClass on team
//  iTeam.
//------------------------------------------------------------------------------
BOOL TFGlobalInfo::CanRespawn( CBasePlayer * pPlayer )
{
	// if this is a bad ent
	if( !pPlayer || FNullEnt( pPlayer ) )
		return FALSE;

	// if they arent on a valid team
	if( !IsValidTeam( pPlayer->pev->team ) )
		return FALSE;

	// if the team has infinite lives
	if( TeamLives( pPlayer->pev->team ) == -1 )
		return TRUE;

	// if the team has lives left
	if( TeamLives( pPlayer->pev->team ) > 0 )
		return TRUE;

	return FALSE;
}


//------------------------------------------------------------------------------
// DamageRatio:
//	Returns the ratio of physical damage pAttacker can do to pPlayer.
//------------------------------------------------------------------------------
float TFGlobalInfo::DamageRatio( CBasePlayer * pPlayer, CBaseEntity * pAttacker )
{
	// if this is a bad ent
	if( !pPlayer || FNullEnt( pPlayer ) )
		return 0.0f;

	// if this is a bad ent
	if( !pAttacker || FNullEnt( pAttacker ) )
		return 0.0f;

	if( IsFriendTeam( pPlayer->pev->team, pAttacker->pev->team ) )
	{
		// In normal TFC, here we would check if friendly fire is on
		// and return the correct ratio.
		return 0.0f;
	}

	return 1.0f;
}


//------------------------------------------------------------------------------
// BlastDamageRatio:
//	Returns the ratio of blast damage pAttacker can do to pPlayer.
//------------------------------------------------------------------------------
float TFGlobalInfo::BlastDamageRatio( CBasePlayer * pPlayer, CBaseEntity * pAttacker )
{
	// if this is a bad ent
	if( !pPlayer || FNullEnt( pPlayer ) )
		return 0.0f;

	// if this is a bad ent
	if( !pAttacker || FNullEnt( pAttacker ) )
		return 0.0f;

	if( IsFriendTeam( pPlayer->pev->team, pAttacker->pev->team ) )
	{
		// In normal TFC, here we would check if friendly fire is on
		// and return the correct ratio.
		return 0.0f;
	}

	return 1.0f;
}


//------------------------------------------------------------------------------
// ArmorDamageRatio:
//	Returns the ratio of armor damage pAttacker can do to pPlayer.
//------------------------------------------------------------------------------
float TFGlobalInfo::ArmorDamageRatio( CBasePlayer * pPlayer, CBaseEntity * pAttacker )
{
	// if this is a bad ent
	if( !pPlayer || FNullEnt( pPlayer ) )
		return 0.0f;

	// if this is a bad ent
	if( !pAttacker || FNullEnt( pAttacker ) )
		return 0.0f;

	if( IsFriendTeam( pPlayer->pev->team, pAttacker->pev->team ) )
	{
		// In normal TFC, here we would check if friendly fire is on
		// and return the correct ratio.
		return 0.0f;
	}

	return 0.5f;
}


//------------------------------------------------------------------------------
// GivePlayerPoints:
//	If TeamFrags is ON, gives team all points, return 0, else returns iPoints
//------------------------------------------------------------------------------
int TFGlobalInfo::GivePlayerPoints( CBasePlayer * pPlayer, int iPoints )
{
	// if this is a bad ent
	if( !pPlayer || FNullEnt( pPlayer ) )
		return 0;

	// if pPlayer isnt on a valid team
	if( !IsValidTeam( pPlayer->pev->team ) )
		return 0;

	// if pPlayer isnt a valid class for his team
	if( !IsValidClass( pPlayer->pev->team, pPlayer->pev->playerclass ) )
		return 0;

	// if TeamFrags is ON
	if( m_bTeamFrags )
	{
		// don't hurt the team if this guy lost points
		if( iPoints > 0 )
		{
			AddTeamScore( pPlayer->pev->team, iPoints );
			return 0;
		}
	}

	// Normal TFC does not do this, to the best of my knowledge.
	// I feel it better represents a teams worth, both in strategic combat as
	// well as tactical combat.
	else
	{
		// don't hurt the team if this guy lost points
		if( iPoints > 0 )
		{
			AddTeamScore( pPlayer->pev->team, iPoints );
			return iPoints;
		}
	}

	return iPoints;
}


//------------------------------------------------------------------------------
// UpdateFrags:
//	Updates all player's frags if TeamFrags is ON
//------------------------------------------------------------------------------
void TFGlobalInfo::UpdateFrags( void )
{
	if( !m_bTeamFrags )
		return;

	// cycle all players and update
	int i;
	for( i = 1; i < gpGlobals->maxClients; i++ )
	{
		CBaseEntity * pEntity = UTIL_PlayerByIndex( i );

		// ignore bad ents
		if( !pEntity || FNullEnt( pEntity ) )
			continue;

		// if they aren't on a valid team
		if( !IsValidTeam( pEntity->pev->team ) )
			continue;

		// if they aren't a valid class
		if( !IsValidClass( pEntity->pev->team, pEntity->pev->playerclass ) )
			continue;

		UTIL_LogPrintf( "TFGlobalInfo::UpdateFrags changing player score from %d to %d\n",
			pEntity->pev->frags, m_iScore[ pEntity->pev->team ] );

		pEntity->pev->frags = m_iScore[ pEntity->pev->team ];
	}
}


//------------------------------------------------------------------------------
// ParseSettings:
//	Send entity KeyValue values here to change global settings...
//	Should only be info_tfdetect, or any cust ent you have made
//------------------------------------------------------------------------------
BOOL TFGlobalInfo::ParseSettings( const char * pszKey, const char * pszValue )
{
	if( !pszKey || !pszValue )
		return FALSE;

	if( TFStrEq( 1, pszKey, "broadcast" ) )
	{
		m_iszVersion = ALLOC_STRING( pszValue );

		if( FStrEq( pszValue, TF_VERSION ) )
			m_bFortressMap = TRUE;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "hook_out" ) )
	{
		if( atoi( pszValue ) != 0 )
			m_bNoHook = TRUE;

		return TRUE;
	}

	else if( TFStrEq( 1, pszKey, "number_of_teams" ) )
	{
		m_iNumTeams = atoi( pszValue );

		if( m_iNumTeams >= 1 )
			m_iDefaultTeams |= 1;
		
		if( m_iNumTeams >= 2 )
			m_iDefaultTeams |= 2;
		
		if( m_iNumTeams >= 3 )
			m_iDefaultTeams |= 4;

		if( m_iNumTeams >= 4 )
			m_iDefaultTeams |= 8;


		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "team1_name" ) )
	{
		m_iTeamName[ TF_TEAM_BLUE ] = ALLOC_STRING( pszValue );
		m_iTeams |= 1;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "team2_name" ) )
	{
		m_iTeamName[ TF_TEAM_RED ] = ALLOC_STRING( pszValue );
		m_iTeams |= 2;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "team3_name" ) )
	{
		m_iTeamName[ TF_TEAM_YELLOW ] = ALLOC_STRING( pszValue );
		m_iTeams |= 4;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "team4_name" ) )
	{
		m_iTeamName[ TF_TEAM_GREEN ] = ALLOC_STRING( pszValue );
		m_iTeams |= 8;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "message" ) )
	{
		m_iszLocalCmd = ALLOC_STRING( pszValue );

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "ammo_shells" ) )
	{
		m_iLives[ TF_TEAM_BLUE ] = atoi( pszValue );
		m_iTeams |= 1;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "ammo_nails" ) )
	{
		m_iLives[ TF_TEAM_RED ] = atoi( pszValue );
		m_iTeams |= 2;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "ammo_rockets" ) )
	{
		m_iLives[ TF_TEAM_YELLOW ] = atoi( pszValue );
		m_iTeams |= 4;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "ammo_cells" ) )
	{
		m_iLives[ TF_TEAM_GREEN ] = atoi( pszValue );
		m_iTeams |= 8;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "maxammo_shells" ) )
	{
		m_iClassLimits[ TF_TEAM_BLUE ] = atoi( pszValue );
		m_iTeams |= 1;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "maxammo_nails" ) )
	{
		m_iClassLimits[ TF_TEAM_RED ] = atoi( pszValue );
		m_iTeams |= 2;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "maxammo_rockets" ) )
	{
		m_iClassLimits[ TF_TEAM_YELLOW ] = atoi( pszValue );
		m_iTeams |= 4;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "maxammo_cells" ) )
	{
		m_iClassLimits[ TF_TEAM_GREEN ] = atoi( pszValue );
		m_iTeams |= 8;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "ammo_medikit" ) )
	{
		m_iMaxPlayers[ TF_TEAM_BLUE ] = atoi( pszValue );
		m_iTeams |= 1;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "ammo_detpack" ) )
	{
		m_iMaxPlayers[ TF_TEAM_RED ] = atoi( pszValue );
		m_iTeams |= 2;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "maxammo_medikit" ) )
	{
		m_iMaxPlayers[ TF_TEAM_YELLOW ] = atoi( pszValue );
		m_iTeams |= 4;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "maxammo_detpack" ) )
	{
		m_iMaxPlayers[ TF_TEAM_GREEN ] = atoi( pszValue );
		m_iTeams |= 8;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "team1_allies" ) )
	{
		m_iAllies[ TF_TEAM_BLUE ] = atoi( pszValue );
		m_iTeams |= 1;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "team2_allies" ) )
	{
		m_iAllies[ TF_TEAM_RED ] = atoi( pszValue );
		m_iTeams |= 2;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "team3_allies" ) )
	{
		m_iAllies[ TF_TEAM_YELLOW ] = atoi( pszValue );
		m_iTeams |= 4;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "team4_allies" ) )
	{
		m_iAllies[ TF_TEAM_GREEN ] = atoi( pszValue );
		m_iTeams |= 8;

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "display_item_status1" ) )
	{
		m_iFlagInfoItem[ TF_GFLAG_ITEM1 ] = atoi( pszValue );

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "display_item_status2" ) )
	{
		m_iFlagInfoItem[ TF_GFLAG_ITEM2 ] = atoi( pszValue );

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "display_item_status3" ) )
	{
		m_iFlagInfoItem[ TF_GFLAG_ITEM3 ] = atoi( pszValue );

		return TRUE;
	}
	
	else if( TFStrEq( 1, pszKey, "display_item_status4" ) )
	{
		m_iFlagInfoItem[ TF_GFLAG_ITEM4 ] = atoi( pszValue );

		return TRUE;
	}
	
	else if( TFStrEq( 2, pszKey, "team_str_home", "t_s_h" ) )
	{
		m_iszFlagMsg[ TF_GMSG_ITEM_HOME_OWNER ] = ALLOC_STRING( pszValue );

		return TRUE;
	}
	
	else if( TFStrEq( 2, pszKey, "team_str_moved", "t_s_m" ) )
	{
		m_iszFlagMsg[ TF_GMSG_ITEM_DROPPED_OWNER ] = ALLOC_STRING( pszValue );

		return TRUE;
	}
	
	else if( TFStrEq( 2, pszKey, "team_str_carried", "t_s_c" ) )
	{
		m_iszFlagMsg[ TF_GMSG_ITEM_CARRIED_OWNER ] = ALLOC_STRING( pszValue );

		return TRUE;
	}
	
	else if( TFStrEq( 2, pszKey, "non_team_str_home", "n_s_h" ) )
	{
		m_iszFlagMsg[ TF_GMSG_ITEM_HOME_NON_OWNER ] = ALLOC_STRING( pszValue );

		return TRUE;
	}
	
	else if( TFStrEq( 2, pszKey, "non_team_str_moved", "n_s_m" ) )
	{
		m_iszFlagMsg[ TF_GMSG_ITEM_DROPPED_NON_OWNER ] = ALLOC_STRING( pszValue );

		return TRUE;
	}
	
	else if( TFStrEq( 2, pszKey, "non_team_str_carried", "n_s_c" ) )
	{
		m_iszFlagMsg[ TF_GMSG_ITEM_CARRIED_NON_OWNER ] = ALLOC_STRING( pszValue );

		return TRUE;
	}

	return FALSE;
}


//------------------------------------------------------------------------------
// ParseSettings:
//	Called after all settings have been parsed, sets bools based on iImpulse
//------------------------------------------------------------------------------
void TFGlobalInfo::UpdateSettings( int iImpulse  )
{
	if( iImpulse & TF_GAME_MULTISKIN )
		m_bMultiSkin = TRUE;

	if( iImpulse & TF_GAME_CLASS_PERSISTENCE )
		m_bClassPersistence = TRUE;

	if( iImpulse & TF_GAME_CHEAT_CHECKING )
		m_bCheatChecking = TRUE;

	if( iImpulse & TF_GAME_FORTRESS_MAP )
		m_bFortressMap = TRUE;

	if( iImpulse & TF_GAME_RESPAWN_DELAY1 )
		m_bRespawnDelay1 = TRUE;

	if( iImpulse & TF_GAME_RESPAWN_DELAY2 )
		m_bRespawnDelay2 = TRUE;

	if( iImpulse & TF_GAME_AUTO_TEAM )
		m_bAutoTeam = TRUE;

	if( iImpulse & TF_GAME_TEAM_FRAGS )
		m_bTeamFrags = TRUE;
}


//------------------------------------------------------------------------------
// GetPlayerSpawn:
//	Finds a valid spawn spot for pPlayer
//------------------------------------------------------------------------------
CBaseEntity * TFGlobalInfo::GetPlayerSpawn( CBasePlayer * pPlayer )
{
	if( !pPlayer )
	{
		return NULL;
	}

	if( !IsValidTeam( pPlayer->pev->team ) )
	{
		return NULL;
	}

	CBaseEntity * pSpot, * pReturn;

	pSpot	= m_pLastSpawnEntity[ pPlayer->pev->team ];
	pReturn	= NULL;

	bool bSpot = false;

	// while( ( pSpot = UTIL_FindEntityByClassify( pSpot, TF_ENT_INFO_TEAMSPAWN ) ) != NULL )
	TFEntity *pTF =NULL;

	for( pTF = TFEntity::m_pHead; pTF != NULL; pTF = pTF->m_pNext )
	{
		if( pTF->m_pPrev == NULL && pTF != TFEntity::m_pHead )
		{
			continue;
		}

		if( pTF->Classify() != TF_ENT_INFO_TEAMSPAWN )
		{
			continue;
		}

		TFInfoTeamSpawn * pSpawn = (TFInfoTeamSpawn *)pTF;

		// this is a hack to make sure we don't spawn in the same place every time
		// and that we cycle properly
		if( !bSpot )
		{
			if( pSpot == (CBaseEntity *)pSpawn )
				bSpot = true;

			continue;
		}


		if( pSpawn && pSpawn->CheckSpawn( pPlayer ) )
		{
			pReturn = pSpawn;
			break;
		}
	}

	// if we didn't get a return, start again from the beginning
	if( !pReturn )
	{
		pSpot = NULL;

		// while( ( pSpot = UTIL_FindEntityByClassify( pSpot, TF_ENT_INFO_TEAMSPAWN ) ) != NULL )
		for( pTF = TFEntity::m_pHead; pTF != NULL; pTF = pTF->m_pNext )
		{
			if( pTF->Classify() != TF_ENT_INFO_TEAMSPAWN )
			{
				continue;
			}

			TFInfoTeamSpawn * pSpawn = (TFInfoTeamSpawn *)pTF;

			if( pSpawn && pSpawn->CheckSpawn( pPlayer ) )
			{
				pReturn = pSpawn;
				break;
			}
		}
	}

	return ( m_pLastSpawnEntity[ pPlayer->pev->team ] = pReturn );
}


//------------------------------------------------------------------------------
// PlayerSpawn:
//	Forwards PlayerSpawn to any ents that need it
//------------------------------------------------------------------------------
void TFGlobalInfo::PlayerSpawn( CBasePlayer * pPlayer, CBaseEntity * pSpawn )
{
	if( !pSpawn || FNullEnt( pSpawn ) )
		return;

	if( pSpawn->Classify() == TF_ENT_INFO_TEAMSPAWN )
		((TFInfoTeamSpawn *)pSpawn )->PlayerSpawn( pPlayer );
}


//------------------------------------------------------------------------------
// ClientCommand:
//	Adds a few tf specific commands
//------------------------------------------------------------------------------
BOOL TFGlobalInfo::ClientCommand( CBasePlayer * pPlayer, const char * cmd )
{
#ifdef BETA
	if ( FStrEq( cmd, "!!kuky_team!!" ) )
	{	
		pPlayer->pev->team = atoi( CMD_ARGV(1) );

		return TRUE;
	}

	if ( FStrEq( cmd, "!!kuky_class!!" ) )
	{
		pPlayer->pev->playerclass = atoi( CMD_ARGV(1) );

		return TRUE;
	}
#endif // BETA

	return FALSE;
}


void TFGlobalInfo::AddGlobalTeam( int iTeam )
{
	if( !iTeam )
		return;

	m_iGlobalTeams |= ( 1 << ( iTeam - 1 ) );
}


void TFGlobalInfo::AddGlobalClass( int iClass )
{
	if( !iClass )
	{
		m_iGlobalClasses = 1023;
		return;
	}

	m_iGlobalClasses |= g_iTFClassBits[ iClass ];
}


int TFGlobalInfo::ValidClasses( int iTeam )
{
	if( g_pGameRules->IsTeamplay() && iTeam != 0 )
		return m_iClassLimits[ iTeam ];

	// Codiac 1.2 B21 Class selection fix
	return m_iGlobalClasses; //1023 - m_iGlobalClasses;
};


int TFGlobalInfo::ValidTeams( void )
{
	if( m_iTeams != 0 )
		return m_iTeams;

	if( m_iDefaultTeams != 0 )
		return m_iDefaultTeams;

	if( m_iNumTeams == 0 )
		return m_iGlobalTeams;

	return 0;
};
