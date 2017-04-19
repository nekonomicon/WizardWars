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


LINK_ENTITY_TO_CLASS( info_tf_teamcheck, TFTeamCheck );
LINK_ENTITY_TO_CLASS( info_ww_teamcheck, TFTeamCheck );


//------------------------------------------------------------------------------
// Spawn:
//
//------------------------------------------------------------------------------
void TFTeamCheck::Spawn( void )
{
}


//------------------------------------------------------------------------------
// KeyValue:
//
//------------------------------------------------------------------------------
void TFTeamCheck::KeyValue( KeyValueData * pkvd )
{
	if( TFStrEq( 1, pkvd->szKeyName, "team_no" ) )
	{
		m_iTeam = atoi( pkvd->szValue );

		pkvd->fHandled = TRUE;
	}
	else
		CPointEntity::KeyValue( pkvd );
}
