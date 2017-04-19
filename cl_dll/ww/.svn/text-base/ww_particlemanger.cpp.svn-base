#include "../hud.h"
#include "../cl_util.h"
#include "cl_entity.h"
#include "r_efx.h"
#include "triangleapi.h"
#include "pm_shared.h"
#include "entity_types.h"
#include "com_model.h"
#include "../studio_util.h"

#include "ww_particle.h"
#include "ww_particlemanger.h"

cvar_t * effects		= NULL;
cvar_t * part_speeds	= NULL;
cvar_t * part_low		= NULL;
cvar_t * part_med		= NULL;
cvar_t * part_high		= NULL;
cvar_t * part_max		= NULL;

WWParticleManager * g_pParticleManager = NULL;


WWParticleManager::WWParticleManager()
{
	m_pHead			= NULL;
	m_pTail			= NULL;

	for( int i = WW_ORIENT_NONE; i <= WW_ORIENT_ALL; i++ )
	{
		m_vForward[i]	= Vector( 0, 0, 0 );
		m_vRight[i]		= Vector( 0, 0, 0 );
		m_vUp[i]		= Vector( 0, 0, 0 );
	}

	if( !effects )
	{
		effects		= CVAR_CREATE( "effects", "1", FCVAR_ARCHIVE | FCVAR_CLIENTDLL );

		part_speeds = CVAR_CREATE( "part_speeds", "0",  FCVAR_CLIENTDLL );

		part_low	= CVAR_CREATE( "part_low",	"200",	FCVAR_ARCHIVE | FCVAR_CLIENTDLL );
		part_med	= CVAR_CREATE( "part_med",	"500",	FCVAR_ARCHIVE | FCVAR_CLIENTDLL );
		part_high	= CVAR_CREATE( "part_high",	"1000",	FCVAR_ARCHIVE | FCVAR_CLIENTDLL );
		part_max	= CVAR_CREATE( "part_max",	"2000",	FCVAR_ARCHIVE | FCVAR_CLIENTDLL );
	}

	m_flLastThink	= 0.0f;
}


WWParticleManager::~WWParticleManager()
{
	Clear();
}

void WWParticleManager::Clear( void )
{
	if( !m_pHead )
		return;

	WWParticle * pPart = m_pTail;
	WWParticle * pTemp = NULL;

	for(;;)
	{
		if( !pPart )
			break;

		// kill the custom death so it doesnt trigger
		pPart->m_pfnCustomDeath = NULL;

		pTemp = pPart->m_pPrev;
		delete pPart;
		pPart = pTemp;
	}

	m_pHead = NULL;
	m_pTail = NULL;

	if( WWParticle::Count() != 0 )
		gEngfuncs.Con_DPrintf( "Error, could not delete all particles. %i still in memory.\n", WWParticle::Count() );
}


WWParticle * WWParticleManager::Allocate( int iPriority )
{
	if( effects->value == 0.0f )
		return NULL;

	if( WWParticle::Count() >= part_max->value )
		return NULL;

	switch( iPriority )
	{
	case PARTICLE_PRIORITY_LOW:
		if( WWParticle::Count() >= part_low->value )
			return NULL;
		break;
	
	case PARTICLE_PRIORITY_MEDIUM:
		if( WWParticle::Count() >= part_med->value )
			return NULL;
		break;

	case PARTICLE_PRIORITY_HIGH:
		if( WWParticle::Count() >= part_high->value )
			return NULL;
		break;

	default:
	case PARTICLE_PRIORITY_NONE:
		break;
	}


	WWParticle * pReturn = NULL;

	pReturn = new WWParticle( iPriority );

	WWParticle * pPart = m_pHead;
	WWParticle * pLast = NULL;

	for( ;; )
	{
		if( !pPart )
			break;

		pLast = pPart;
		pPart = pPart->m_pNext;
	}

	if( !pLast )
		m_pHead = pReturn;
	else
	{
		pLast->m_pNext = pReturn;
		pReturn->m_pPrev = pLast;
	}

	m_pTail = pReturn;

	return pReturn;
}


void WWParticleManager::Process( float flTime )
{
	float flFrametime = flTime - m_flLastThink;
	m_flLastThink = flTime;

	WWParticle * pPart = m_pHead;
	WWParticle * pTemp = NULL;

	Vector vViewAngles, vAngles;

	extern vec3_t v_angles;
	vViewAngles = v_angles;
//	gEngfuncs.GetViewAngles( (float *)vViewAngles );

	for( int i = WW_ORIENT_NONE; i < WW_ORIENT_TOTAL; i++ )
	{
		vAngles.x = vViewAngles.x * g_vOrients[i].x;
		vAngles.y = vViewAngles.y * g_vOrients[i].y;
		vAngles.z = vViewAngles.z * g_vOrients[i].z;
		AngleVectors( vAngles, m_vForward[i], m_vRight[i], m_vUp[i] );
	}

	unsigned int iTotal = 0, iThink = 0, iDraw = 0;
	float flScreen[2];

	for( ;; )
	{
		if( !pPart )
			break;

		iTotal++;

		// if the particle is dead or we have _too_ many particles
		if( flTime >= pPart->m_flBirth + pPart->m_flDelay + pPart->m_flLife ||
			iTotal > part_max->value )
		{
			pTemp = pPart->m_pNext;
			delete pPart;
			pPart = pTemp;

			continue;
		}

		// only think + draw if the particle is active
		if( flTime >= pPart->m_flBirth + pPart->m_flDelay )
		{
			if( pPart->m_pfnCustomThink != NULL )
				iThink += pPart->m_pfnCustomThink( pPart, flFrametime );
			else
				iThink += pPart->Think( flFrametime );

			if( pPart->m_bDraw != false && !gEngfuncs.pTriAPI->WorldToScreen( pPart->m_vOrigin, flScreen ) )
				iDraw += pPart->Draw();
		}
		
		pPart = pPart->m_pNext;
	}

	if( part_speeds->value == 1.0f )
		gEngfuncs.Con_Printf( "p.speeds: total=%04u processed=%04u, drawn=%04u, thought=%04u\n", WWParticle::Count(), iTotal, iDraw, iThink );
}


Vector WWParticleManager::CalcPoint( WWParticle * pPart, float flWidth, float flHeight, float flScale, int iOrient )
{
	if( !pPart )
		return Vector( 0, 0, 0 );

	Vector vReturn = pPart->m_vOrigin;

	vReturn = vReturn + ( m_vRight[iOrient] * flWidth  * flScale );
	vReturn = vReturn + ( m_vUp[iOrient]    * flHeight * flScale );

	return vReturn;
}