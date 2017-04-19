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

#include "ww_particle.h"
#include "ww_particlemanger.h"


// int WWParticle::Count( void );
unsigned int WWParticle::_count = 0;


WWParticle::WWParticle( int iPriority )
{
	_count++;

	m_bDraw				= false;

	m_iPriority			= iPriority;

	m_pPrev				= NULL;
	m_pNext				= NULL;

	m_vOrigin			= Vector( 0, 0, 0 );
	m_vVelocity			= Vector( 0, 0, 0 );

	m_iFlags			= WW_PFLAG_NONE;
	m_iOrient			= WW_ORIENT_ALL;

	m_hSprite			= NULL;
	m_iSize[2]			= 0;
	m_iSize[2]			= 0;

	m_flFrame			= 0.0f;
	m_flFrameRate		= 10.0f;
	m_iFrameCount		= 0;

	m_iRenderMode		= kRenderNormal;
	m_flRenderColor[0]	= 1.0f;
	m_flRenderColor[1]	= 1.0f;
	m_flRenderColor[2]	= 1.0f;
	m_flRenderColor[3]	= 1.0f;
	m_flRenderDecay		= 0.0f;

	m_flNextThink		= 0.0f;

	m_flBirth			= gEngfuncs.GetClientTime();

	m_flLife			= 0.0f;
	m_flDelay			= 0.0f;
	
	m_flScale			= 1.0f;
	m_flScaleDecay		= 0.0f;

	m_flGravity			= 0.0f;
	m_flGravityDecay	= 0.0f;

	m_flSpeedDecay		= 0.0f;

	m_flWindScale		= 0.0f;

	m_pModel			= NULL;

	m_flTraceSize		= 0.0f;
	m_flRandomSeed		= gEngfuncs.pfnRandomFloat( 0.0f, 100000.0f );

	m_pfnCustomThink	= NULL;
	m_pfnCustomTouch	= NULL;
	m_pfnCustomDeath	= NULL;
	m_pPrivate			= NULL;
	m_bFreeData			= false;
}


WWParticle::~WWParticle()
{
	if( m_pfnCustomDeath != NULL )
		m_pfnCustomDeath( this );

	if( g_pParticleManager != NULL )
	{
		if( g_pParticleManager->m_pHead == this )
			g_pParticleManager->m_pHead = m_pNext;

		if( g_pParticleManager->m_pTail == this )
			g_pParticleManager->m_pTail = m_pPrev;
	}

	if( m_pPrev != NULL )
		m_pPrev->m_pNext = m_pNext;

	if( m_pNext != NULL )
		m_pNext->m_pPrev = m_pPrev;

	m_pPrev = NULL;
	m_pNext = NULL;

	if( m_bFreeData != NULL )
	{
		free( m_pPrivate );
		m_bFreeData = NULL;
	}

	_count--;
}


HSPRITE WWParticle::Sprite( void )
{
	return m_hSprite;
}


void WWParticle::SetSprite( const char * szFile )
{
	m_hSprite = SPR_Load( szFile );

	m_flFrame = 0.0f;
	m_iFrameCount = SPR_Frames( m_hSprite );

	m_iSize[0] = SPR_Width ( m_hSprite, 0 );
	m_iSize[1] = SPR_Height( m_hSprite, 0 );

	m_pModel = NULL;

	m_flTraceSize = ( m_iSize[0] > m_iSize[1] ? m_iSize[0] : m_iSize[1] ) * 0.5f;
}

	
int PARTICLEAPI WWParticle::Draw( void )
{
	if( !m_hSprite )
		return 0;

	if( m_flLife == 0.0f )
		return 0;

	if( m_pModel == NULL )
		m_pModel = (struct model_s *)gEngfuncs.GetSpritePointer( m_hSprite );

	float flWidth  = m_iSize[0] / 2;
	float flHeight = m_iSize[1] / 2;

	Vector vPointUL = g_pParticleManager->CalcPoint( this, -flWidth,  flHeight, m_flScale, m_iOrient );
	Vector vPointUR = g_pParticleManager->CalcPoint( this,  flWidth,  flHeight, m_flScale, m_iOrient );
	Vector vPointBL = g_pParticleManager->CalcPoint( this, -flWidth, -flHeight, m_flScale, m_iOrient );
	Vector vPointBR = g_pParticleManager->CalcPoint( this,  flWidth, -flHeight, m_flScale, m_iOrient );

	gEngfuncs.pTriAPI->RenderMode( m_iRenderMode );
	gEngfuncs.pTriAPI->Brightness( 1 );
	gEngfuncs.pTriAPI->Color4f( m_flRenderColor[0], m_flRenderColor[1], m_flRenderColor[2], m_flRenderColor[3] );
	gEngfuncs.pTriAPI->SpriteTexture( m_pModel, (int)( m_flFrame + 0.5f ) );
	gEngfuncs.pTriAPI->CullFace( TRI_NONE );
	gEngfuncs.pTriAPI->Begin( TRI_TRIANGLES );

	gEngfuncs.pTriAPI->TexCoord2f( 0, 0 );
	gEngfuncs.pTriAPI->Vertex3fv( (float *)&vPointUL );

	gEngfuncs.pTriAPI->TexCoord2f( 1, 0 );
	gEngfuncs.pTriAPI->Vertex3fv( (float *)&vPointUR );

	gEngfuncs.pTriAPI->TexCoord2f( 1, 1 );
	gEngfuncs.pTriAPI->Vertex3fv( (float *)&vPointBR );

	gEngfuncs.pTriAPI->TexCoord2f( 0, 0 );
	gEngfuncs.pTriAPI->Vertex3fv( (float *)&vPointUL );

	gEngfuncs.pTriAPI->TexCoord2f( 1, 1 );
	gEngfuncs.pTriAPI->Vertex3fv( (float *)&vPointBR );

	gEngfuncs.pTriAPI->TexCoord2f( 0, 1 );
	gEngfuncs.pTriAPI->Vertex3fv( (float *)&vPointBL );

	gEngfuncs.pTriAPI->End();

	return 1;
}


int PARTICLEAPI WWParticle::Think( const float & flFrametime )
{
	if( m_flScaleDecay != 0.0f )
	{
		m_flScale -= m_flScaleDecay * flFrametime;

		if( m_flScale <= 0.0 )
		{
			m_flScale = 0.0f;
			m_flLife = 0.0f;
		}
	}

	if( m_flRenderDecay != 0.0f )
	{
		m_flRenderColor[3] -= m_flRenderDecay * flFrametime;

		if( m_flRenderColor[3] <= 0.0 )
		{
			m_flRenderColor[3] = 0.0f;
			m_flLife = 0.0f;
		}
	}


	Vector vSimOrigin = m_vOrigin + m_vVelocity * flFrametime;

	// flutter the origin a bit
	// there is a better way to do this, I'm just lazy
	if( m_iFlags & WW_PFLAG_FEATHER )
	{
		float r = gEngfuncs.GetClientTime() * 10.0f + m_flRandomSeed;
		vSimOrigin.x += sin( r ) * 50.0f * flFrametime;
		vSimOrigin.y += cos( r ) * 50.0f * flFrametime;
	}

	// no touching if they arent drawn
	if( m_bDraw && m_iFlags & WW_PFLAG_COLLIDE )
	{
		Vector vOffset = Vector( 0, 0, 0 );

		if( m_vVelocity != vOffset )
			vOffset = m_vVelocity.Normalize() * m_flTraceSize * m_flScale;

		pmtrace_t tr;
		gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
		gEngfuncs.pEventAPI->EV_PlayerTrace( m_vOrigin, vSimOrigin + vOffset, PM_STUDIO_BOX, -1, &tr );

		if( tr.fraction != 1.0f )
		{
			if( m_pfnCustomTouch != NULL )
				m_pfnCustomTouch( this, &tr );

			if( m_iFlags & WW_PFLAG_DIE_ON_COLLIDE )
			{
				m_bDraw = false;
				m_flLife = 0.0f;
			}

			vSimOrigin = tr.endpos - vOffset;
		}
		else if( gEngfuncs.PM_PointContents( tr.endpos, NULL ) != CONTENTS_EMPTY )
		{
			if( m_pfnCustomTouch != NULL )
				m_pfnCustomTouch( this, &tr );

			if( m_iFlags & WW_PFLAG_DIE_ON_COLLIDE )
				m_flLife = 0.0f;

			vSimOrigin = m_vOrigin;
		}
	}

	m_vOrigin = vSimOrigin;

	// put off some calculation till later
	if( gEngfuncs.GetClientTime() < m_flNextThink )
		return 0;

	switch( m_iPriority )
	{
	case PARTICLE_PRIORITY_NONE:
		if( _count > part_max->value )
			m_flLife = 0.0f;
		break;

	case PARTICLE_PRIORITY_LOW:
		if( _count > part_low->value )
			m_flLife = 0.0f;
		break;

	case PARTICLE_PRIORITY_MEDIUM:
		if( _count > part_med->value )
			m_flLife = 0.0f;
		break;

	case PARTICLE_PRIORITY_HIGH:
		if( _count > part_high->value )
			m_flLife = 0.0f;
		break;
	}

	
	// apply gravity, lerped to create terminal velocity of gravity
	if( m_flGravity != 0.0f )
		m_vVelocity.z = ( m_vVelocity.z * 0.9f ) - ( m_flGravity * 0.1f );

	// lower/raise speed gradually
	if( m_flSpeedDecay != 0.0f )
	{
		float flDecay = 1.0f - m_flSpeedDecay;
		m_vVelocity.x *= flDecay;
		m_vVelocity.y *= flDecay;
		m_vVelocity.z *= flDecay;
	}

	// lower/raise gravity gradually
	if( m_flGravityDecay != 0.0f )
		m_flGravity -= m_flGravityDecay * 0.1f;

	if( m_iFrameCount > 1 )
	{
		// increment the frame
		m_flFrame += m_flFrameRate * 0.1f;
		if( (int)( m_flFrame + 0.5 ) >= m_iFrameCount )
		{
			if( m_iFlags & WW_PFLAG_DIE_ON_LAST_FRAME )
				m_flLife = 0.0f;

			m_flFrame = fmodf( m_flFrame, 1.0f );
		}
	}

	// next think
	m_flNextThink = gEngfuncs.GetClientTime() + 0.1f;

	return 1;
}


void WWParticle::SetPrivate( pdata_t * pPrivate, bool bFree )
{
	m_pPrivate	= pPrivate;
	m_bFreeData	= bFree;
}


void WWParticle::SetThink( ParticleThinkCall pfnCustom )
{
	m_pfnCustomThink = pfnCustom;
}

void WWParticle::SetTouch( ParticleTouchCall pfnCustom )
{
	m_pfnCustomTouch = pfnCustom;
}

void WWParticle::SetDeath( ParticleDeathCall pfnCustom )
{
	m_pfnCustomDeath = pfnCustom;
}