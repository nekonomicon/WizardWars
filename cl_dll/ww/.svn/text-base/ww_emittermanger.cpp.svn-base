#include "../hud.h"
#include "../cl_util.h"
#include "cl_entity.h"
#include "r_efx.h"
#include "triangleapi.h"
#include "pm_shared.h"
#include "entity_types.h"
#include "com_model.h"
#include "../studio_util.h"

#include "ww_emitter.h"
#include "ww_emittermanger.h"
#include "ww_particle.h"
#include "ww_particlemanger.h"


WWEmitterManager *	g_pEmitterManager = NULL;
extern char g_szCurrentLevel[256];


WWEmitterManager::WWEmitterManager()
{
	m_pHead		= NULL;
	m_pTail		= NULL;

	CVAR_CREATE( "emit_speeds", "0", FCVAR_CLIENTDLL );
	CVAR_CREATE( "emit_dist", "2048", FCVAR_ARCHIVE | FCVAR_CLIENTDLL );
	CVAR_CREATE( "emit_max", "200", FCVAR_ARCHIVE | FCVAR_CLIENTDLL );

	// reset the current level so emitters are reloaded
	memset( g_szCurrentLevel, 0, sizeof(g_szCurrentLevel) );
}


WWEmitterManager::~WWEmitterManager()
{
	Clear();
}


WWEmitter * WWEmitterManager::Head( void )
{
	return m_pHead;
}


void WWEmitterManager::SetHead( WWEmitter * pPart )
{
	m_pHead = pPart;
}


WWEmitter * WWEmitterManager::Tail( void )
{
	return m_pTail;
}


void WWEmitterManager::SetTail( WWEmitter * pPart )
{
	m_pTail = pPart;
}


void WWEmitterManager::Clear( void )
{
	if( !m_pHead )
		return;

	WWEmitter * pPart = m_pTail;
	WWEmitter * pTemp = NULL;

	for(;;)
	{
		if( !pPart )
			break;

		pTemp = pPart->Prev();
		delete pPart;
		pPart = pTemp;
	}

	m_pHead = NULL;
	m_pTail = NULL;

	if( WWEmitter::Count() != 0 )
		gEngfuncs.Con_DPrintf( "Error, could not delete all emitters. %i still in memory.\n", WWEmitter::Count() );
}


WWEmitter * WWEmitterManager::Allocate( void )
{
	if( WWEmitter::Count() >= CVAR_GET_FLOAT( "emit_max" ) )
		return NULL;

	WWEmitter * pReturn = NULL;

	pReturn = new WWEmitter();

	WWEmitter * pPart = m_pHead;
	WWEmitter * pLast = NULL;

	for( ;; )
	{
		if( !pPart )
			break;

		pLast = pPart;
		pPart = pPart->Next();
	}

	if( !pLast )
		m_pHead = pReturn;
	else
	{
		pLast->SetNext( pReturn );
		pReturn->SetPrev( pLast );
	}

	m_pTail = pReturn;

	return pReturn;
}

cl_entity_t * g_pPlayer = NULL;

#define __SORTED_EMITTERS__
#if defined( __SORTED_EMITTERS__ )

int __cdecl SortEmitter( const void * p1, const void * p2 )
{
	if( !g_pPlayer || !p1 || !p2 )
		return 0;

	WWEmitter * pE1 = NULL;
	WWEmitter * pE2 = NULL;

	pE1 = (WWEmitter *)((void **)p1)[0];
	pE2 = (WWEmitter *)((void **)p2)[0];

	if( !pE1 || !pE2 )
		return 0;

	Vector vOrigin = g_pPlayer->origin;

	float flDist1 = ( pE1->m_vOrigin - vOrigin ).Length();
	float flDist2 = ( pE2->m_vOrigin - vOrigin ).Length();

	if( flDist1 > flDist2 )
		return 1;

	if( flDist1 < flDist2 )
		return -1;

	return 0;
}

#endif

void WWEmitterManager::Process( float flTime )
{
	WWEmitter * pEmit = NULL;
	float flScreen[2];

	unsigned int iArray = 0, iCount = 0, iTotal = 0;

	g_pPlayer = gEngfuncs.GetLocalPlayer();

	if( !g_pPlayer )
		return;

#if defined( __SORTED_EMITTERS__ )

	// XYPHN - 040201
	//	Put emitters into a temporary array. Sort the array by distance, this
	//	will to help ensures that the emitters furthest away have the lowest
	//	priority.

	WWEmitter ** pArray = (WWEmitter **)malloc( sizeof(WWEmitter *) * WWEmitter::Count() );
	memset( pArray, 0, sizeof(WWEmitter *) * WWEmitter::Count() );

	int i = 0;
	for( pEmit = m_pHead;; pEmit = pEmit->Next() )
	{
		if( !pEmit )
			break;

		pArray[i] = pEmit;
		i++;
	}

	qsort( pArray, i, sizeof(WWEmitter *), SortEmitter );

	for( int j = 0; j < i; j++ )
	{
		if( !pArray[j] )
			continue;

		iTotal++;

		if( pArray[j]->m_iFlags & WW_EFLAG_2D_LENGTH )
		{
			if( ( g_pPlayer->origin - pArray[j]->m_vOrigin ).Length2D() > CVAR_GET_FLOAT( "emit_dist" ) )
				continue;
		}
		else
		{
			if( ( g_pPlayer->origin - pArray[j]->m_vOrigin ).Length() > CVAR_GET_FLOAT( "emit_dist" ) )
				continue;
		}

		if( gEngfuncs.pTriAPI->WorldToScreen( pArray[j]->m_vOrigin, flScreen ) )
			continue;
		
		iCount += pArray[j]->Think( flTime );
	}

	free( pArray );

#else

	for( pEmit = m_pHead;; pEmit = pEmit->Next() )
	{
		if( !pEmit )
			break;

		iTotal++;

		if( pEmit->m_iFlags & WW_EFLAG_2D_LENGTH )
		{
			if( ( g_pPlayer->origin - pEmit->m_vOrigin ).Length2D() > CVAR_GET_FLOAT( "emit_dist" ) )
				continue;
		}
		else
		{
			if( ( g_pPlayer->origin - pEmit->m_vOrigin ).Length() > CVAR_GET_FLOAT( "emit_dist" ) )
				continue;
		}

		if( gEngfuncs.pTriAPI->WorldToScreen( pEmit->m_vOrigin, flScreen ) )
			continue;
		
		iCount += pEmit->Think( flTime );
	}

#endif

	if( CVAR_GET_FLOAT( "emit_speeds" ) == 1.0f )
		gEngfuncs.Con_Printf( "e.speeds: total=%u processed=%u\n", iTotal, iCount );
}
