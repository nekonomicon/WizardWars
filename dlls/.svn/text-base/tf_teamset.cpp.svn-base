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
#include "tf_teamset.h"


LINK_ENTITY_TO_CLASS( info_tf_teamset, TFTeamSet );
LINK_ENTITY_TO_CLASS( info_ww_teamset, TFTeamSet );


//------------------------------------------------------------------------------
// Spawn:
//
//------------------------------------------------------------------------------
void TFTeamSet::Spawn( void )
{
}


//------------------------------------------------------------------------------
// KeyValue:
//
//------------------------------------------------------------------------------
void TFTeamSet::KeyValue( KeyValueData * pkvd )
{
	if( TFStrEq( 1, pkvd->szKeyName, "team_no" ) )
	{
		m_iTeam = atoi( pkvd->szValue );

		pkvd->fHandled = TRUE;
	}
	else
		CPointEntity::KeyValue( pkvd );
}


//------------------------------------------------------------------------------
// Use:
//
//------------------------------------------------------------------------------
void TFTeamSet::Use( CBaseEntity * pActivator, CBaseEntity * pCaller, USE_TYPE useType, float value )
{
	if( !pev->target )
		return;

	TFTeamCheck * pTeamCheck = (TFTeamCheck *)UTIL_FindEntityByTargetname( NULL, STRING( pev->target ) );

	if( !pTeamCheck )
		return;

	switch( m_iTeam )
	{
	case 0:
		if( pTeamCheck->GetTeam() == 1 )
			pTeamCheck->SetTeam( 2 );
		else
			pTeamCheck->SetTeam( 1 );
		break;

	case 1:
		pTeamCheck->SetTeam( 1 );
		break;

	case 2:
		pTeamCheck->SetTeam( 2 );
		break;

	case 3:
		pTeamCheck->SetTeam( 3 );
		break;

	case 4:
		pTeamCheck->SetTeam( 4 );
		break;
	}
}