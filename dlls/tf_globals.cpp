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


//------------------------------------------------------------------------------
// UTIL_FindEntityByTFId
//	Find a tf entity by it's id
//------------------------------------------------------------------------------
TFEntity * UTIL_FindEntityByTFId( TFEntity * pStart, int iId )
{
	if( pStart == NULL )
		pStart = TFEntity::m_pHead;

	for( TFEntity * pTF = pStart; pTF != NULL; pTF = pTF->m_pNext )
	{
		// 1.2.5 was =?
		if( pTF->m_iId == iId )
			return pTF;
	}

	return NULL;
}


//------------------------------------------------------------------------------
// UTIL_FindEntityByTFGroup
//	Find a tf entity by it's group id
//------------------------------------------------------------------------------
TFEntity * UTIL_FindEntityByTFGroup( TFEntity * pStart, int iGroup )
{
	if( pStart == NULL )
		pStart = TFEntity::m_pHead;

	for( TFEntity * pTF = TFEntity::m_pHead; pTF != NULL; pTF = pTF->m_pNext )
	{
		if( pTF->m_pPrev == NULL && pTF != TFEntity::m_pHead )
			continue;

		// 1.2.5 was =?
		if( pTF->m_iGroupId == iGroup )
			return pTF;
	}

	return NULL;
}


//------------------------------------------------------------------------------
// FMultiStrEq:
//	Checks if szText is equal to one of the arguments.
//------------------------------------------------------------------------------
BOOL TFStrEq( int total, const char * szText, ... )
{
	int count = 0;
	va_list		args;
	char *		pszNext = NULL;

	va_start( args, szText );

	do
	{
		count++;

		if( count > total )
			break;

		pszNext = NULL;
		pszNext = (char *)va_arg( args, char * );

		if( !pszNext )
			break;

		if( !stricmp( szText, pszNext ) )
		{
			va_end( args );
			return TRUE;
		}

	} while( pszNext != NULL );

	va_end( args );

	return FALSE;
}
