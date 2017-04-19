#include "../hud.h"
#include "../cl_util.h"
#include "cl_entity.h"
#include "r_efx.h"
#include "triangleapi.h"
#include "pm_shared.h"
#include "pm_defs.h"
#include "pmtrace.h"
#include "entity_types.h"
#include "com_model.h"
#include "../studio_util.h"
#include "event_api.h"
#include "event_args.h"

#include "ww_emitter.h"
#include "ww_emittermanger.h"
#include "ww_particle.h"
#include "ww_particlemanger.h"

// int WWEmitter::Count( void );
unsigned int WWEmitter::_count = 0;


WWEmitter::WWEmitter()
{
	_count++;

	m_iPriority			= PARTICLE_PRIORITY_NONE;

	m_pPrev				= NULL;
	m_pNext				= NULL;

	memset( m_szSprite, 0, sizeof(m_szSprite) );
	sprintf( m_szSprite, "sprites/flare1.spr" );

	m_vOrigin			= Vector( 0, 0, 0 );
	m_vAngles			= Vector( 0, 0, 0 );

	m_flThink			= 0.1f;
	m_flNextThink		= 0.0f;

	m_iFlags			= WW_EFLAG_NONE;
	m_iOrient			= WW_ORIENT_ALL;

	m_flLife			= 0.0f;
	m_flDelay			= 0.0f;
	m_iCount			= 0;
	
	m_flSpread			= 0.0f;
	m_vSpreadMin		= Vector( 0, 0, 0 );
	m_vSpreadMax		= Vector( 0, 0, 0 );

	m_flScale			= 1.0f;
	m_flScaleDecay		= 0.0f;

	m_flGravity			= 0.0f;
	m_flGravityDecay	= 0.0f;

	m_flSpeed			= 0.0f;
	m_flSpeedDecay		= 0.0f;

	m_flWindScale		= 0.0f;
	
	m_iRenderMode		= kRenderNormal;
	m_flRenderColor[0]	= 1.0f;
	m_flRenderColor[1]	= 1.0f;
	m_flRenderColor[2]	= 1.0f;
	m_flRenderColor[3]	= 1.0f;
	m_flRenderDecay		= 0.0f;
}


WWEmitter::~WWEmitter()
{
	_count--;

	if( g_pEmitterManager != NULL )
	{
		if( g_pEmitterManager->Head() == this )
			g_pEmitterManager->SetHead( m_pNext );

		if( g_pEmitterManager->Tail() == this )
			g_pEmitterManager->SetTail( m_pPrev );
	}

	if( m_pPrev != NULL )
		m_pPrev->SetNext( m_pNext );

	if( m_pNext != NULL )
		m_pNext->SetPrev( m_pPrev );
}


WWEmitter * WWEmitter::Prev( void )
{
	return m_pPrev;
}


void WWEmitter::SetPrev( WWEmitter * pPart )
{
	m_pPrev = pPart;
}


WWEmitter * WWEmitter::Next( void )
{
	return m_pNext;
}


void WWEmitter::SetNext( WWEmitter * pPart )
{
	m_pNext = pPart;
}

extern vec3_t v_origin;

int WWEmitter::Think( float flTime )
{
	if( !g_pParticleManager )
		return 0;

	if( flTime < m_flNextThink )
		return 0;

	if( effects->value == 0.0f )
		return 0;

	if( strstr( m_szSprite, "snow" ) != NULL || strstr( m_szSprite, "rain" ) != NULL )
		return 0;

	m_flNextThink = flTime + m_flThink;

	float flRad;
	Vector vForward, vRight, vUp, vSpread;

	flRad = M_PI / 180.0f;

	AngleVectors( m_vAngles, vForward, vRight, vUp );

	for( ;; )
	{
		if( m_iFlags & WW_EFLAG_NO_BLOCKED )
			break;

		pmtrace_t tr;
		gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );

		gEngfuncs.pEventAPI->EV_PlayerTrace( v_origin, m_vOrigin, PM_GLASS_IGNORE, -1, &tr );
		if( tr.fraction == 1.0f && !( tr.startsolid || tr.allsolid ) )
			break;

		gEngfuncs.pEventAPI->EV_PlayerTrace( v_origin, m_vOrigin + Vector( 0, 0, 8 ), PM_GLASS_IGNORE, -1, &tr );
		if( tr.fraction == 1.0f && !( tr.startsolid || tr.allsolid ) )
			break;

		gEngfuncs.pEventAPI->EV_PlayerTrace( v_origin, m_vOrigin - Vector( 0, 0, 8 ), PM_GLASS_IGNORE, -1, &tr );
		if( tr.fraction == 1.0f && !( tr.startsolid || tr.allsolid ) )
			break;

		gEngfuncs.pEventAPI->EV_PlayerTrace( v_origin, m_vOrigin + Vector( 0, 8, 0 ), PM_GLASS_IGNORE, -1, &tr );
		if( tr.fraction == 1.0f && !( tr.startsolid || tr.allsolid ) )
			break;

		gEngfuncs.pEventAPI->EV_PlayerTrace( v_origin, m_vOrigin - Vector( 0, 8, 0 ), PM_GLASS_IGNORE, -1, &tr );
		if( tr.fraction == 1.0f && !( tr.startsolid || tr.allsolid ) )
			break;

		gEngfuncs.pEventAPI->EV_PlayerTrace( v_origin, m_vOrigin + Vector( 8, 0, 0 ), PM_GLASS_IGNORE, -1, &tr );
		if( tr.fraction == 1.0f && !( tr.startsolid || tr.allsolid ) )
			break;

		gEngfuncs.pEventAPI->EV_PlayerTrace( v_origin, m_vOrigin - Vector( 8, 0, 0 ), PM_GLASS_IGNORE, -1, &tr );
		if( tr.fraction == 1.0f && !( tr.startsolid || tr.allsolid ) )
			break;

		return 0;
	}

	float flDist = ( v_origin - m_vOrigin ).Length();

	int iPriority = m_iPriority;

	if( m_iPriority == PARTICLE_PRIORITY_NONE )
	{
		float flMaxDist = CVAR_GET_FLOAT( "emit_dist" );

		if( flDist < flMaxDist * 0.125f )
			iPriority = PARTICLE_PRIORITY_NONE;
		else if( flDist < flMaxDist * 0.25f)
			iPriority = PARTICLE_PRIORITY_HIGH;
		else if( flDist < flMaxDist * 0.5f )
			iPriority = PARTICLE_PRIORITY_MEDIUM;
		else
			iPriority = PARTICLE_PRIORITY_LOW;
	}


	for( int i = 0; i < m_iCount; i++ )
	{
		WWParticle * pPart = g_pParticleManager->Allocate( iPriority );

		if( !pPart )
			break;

		pPart->m_iOrient = WW_ORIENT_NONE;

		if( !( m_iFlags & WW_EFLAG_NO_ORIENT_PITCH ) )
			pPart->m_iOrient |= WW_ORIENT_PITCH;

		if( !( m_iFlags & WW_EFLAG_NO_ORIENT_YAW ) )
			pPart->m_iOrient |= WW_ORIENT_YAW;
		
		if( !( m_iFlags & WW_EFLAG_NO_ORIENT_ROLL ) )
			pPart->m_iOrient |= WW_ORIENT_ROLL;

		if( !( m_iFlags & WW_EFLAG_NO_PITCH_SPREAD ) )
			vSpread.x = flRad * gEngfuncs.pfnRandomFloat( -m_flSpread, m_flSpread );

		if( !( m_iFlags & WW_EFLAG_NO_PITCH_SPREAD ) )
			vSpread.y = flRad * gEngfuncs.pfnRandomFloat( -m_flSpread, m_flSpread );

		if( !( m_iFlags & WW_EFLAG_NO_PITCH_SPREAD ) )
			vSpread.z = flRad * gEngfuncs.pfnRandomFloat( -m_flSpread, m_flSpread );

		pPart->SetSprite( m_szSprite );

		pPart->m_iFlags				= m_iFlags;
		pPart->m_bDraw				= true;

		pPart->m_vOrigin			= m_vOrigin;
		pPart->m_vVelocity			= ( vForward + vSpread ) * m_flSpeed;

		pPart->m_flLife				= m_flLife;
		pPart->m_flDelay			= m_flDelay * (float)i;

		pPart->m_vOrigin.x			+= gEngfuncs.pfnRandomFloat( m_vSpreadMin.x, m_vSpreadMax.x );
		pPart->m_vOrigin.y			+= gEngfuncs.pfnRandomFloat( m_vSpreadMin.y, m_vSpreadMax.y );
		pPart->m_vOrigin.z			+= gEngfuncs.pfnRandomFloat( m_vSpreadMin.z, m_vSpreadMax.z );

		pPart->m_flScale			= m_flScale;
		pPart->m_flScaleDecay		= m_flScaleDecay;

		pPart->m_flGravity			= m_flGravity;
		pPart->m_flGravityDecay		= m_flGravityDecay;

		pPart->m_flSpeedDecay		= m_flSpeedDecay;

		pPart->m_flWindScale		= m_flWindScale;

		pPart->m_iRenderMode		= m_iRenderMode;
		pPart->m_flRenderColor[0]	= m_flRenderColor[0];
		pPart->m_flRenderColor[1]	= m_flRenderColor[1];
		pPart->m_flRenderColor[2]	= m_flRenderColor[2];
		pPart->m_flRenderColor[3]	= m_flRenderColor[3];
		pPart->m_flRenderDecay		= m_flRenderDecay;

	}

	return 1;
}
