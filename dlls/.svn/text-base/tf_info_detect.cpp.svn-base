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
#include "tf_info_detect.h"


LINK_ENTITY_TO_CLASS( info_tfdetect, TFInfoDetect );
LINK_ENTITY_TO_CLASS( info_wwdetect, TFInfoDetect );


TFInfoDetect::TFInfoDetect()
{
}


TFInfoDetect::~TFInfoDetect( void )
{
}


//------------------------------------------------------------------------------
// Spawn:
//
//------------------------------------------------------------------------------
void TFInfoDetect::Spawn( void )
{
	Precache();

	if( g_pTFGlobalInfo != NULL )
		g_pTFGlobalInfo->UpdateSettings( pev->impulse );
}


//------------------------------------------------------------------------------
// Precache:
//
//------------------------------------------------------------------------------
void TFInfoDetect::Precache()
{
}


//------------------------------------------------------------------------------
// KeyValue:
//	Forwards parsing onto g_pTFGlobalInfo or CPointEntity::KeyValue
//------------------------------------------------------------------------------
void TFInfoDetect::KeyValue( KeyValueData * pkvd )
{
	if( !g_pTFGlobalInfo )
		CPointEntity( pkvd );

	if( g_pTFGlobalInfo->ParseSettings( pkvd->szKeyName, pkvd->szValue ) )
		pkvd->fHandled = TRUE;
	else
	{
		ALERT( at_console, "TFInfoDetect::KeyValue( \"%s\", \"%s\" ) not handled.\n",
			pkvd->szKeyName, pkvd->szValue );

		CPointEntity::KeyValue( pkvd );
	}
};