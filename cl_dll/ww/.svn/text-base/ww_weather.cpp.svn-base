#include "../hud.h"
#include "../cl_util.h"
#include "cl_entity.h"
#include "r_efx.h"
#include "triangleapi.h"
#include "pm_shared.h"
#include "entity_types.h"
#include "com_model.h"
#include "../studio_util.h"
#include "pm_defs.h"
#include "pm_materials.h"
#include "eventscripts.h"
#include "event_api.h"
#include "event_args.h"
#include "screenfade.h"
#include "shake.h"

#include "ww_particle.h"
#include "ww_particlemanger.h"
#include "ww_weather.h"


//------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------
extern vec3_t v_origin;
extern vec3_t v_angles;

Vector	g_vOrigin;
Vector	g_vForward;
Vector	g_vRight;
float	g_flLeftRotate[3][4];
float	g_flRightRotate[3][4];

cvar_t * weather = NULL;
WeatherManager * g_pWeatherManager = NULL;


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
int PARTICLEAPI WeatherThink( WWParticle * pPart, const float & flFrametime );
void PARTICLEAPI WeatherTouch( WWParticle * pPart, pmtrace_t * pTrace );
void PARTICLEAPI WeatherDeath( WWParticle * pPart );


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool FindSky( Vector & vOrigin )
{
	Vector vSrc = vOrigin;
	Vector vDst = vOrigin;
	vDst.z += 2048.0f;

	pmtrace_t tr;
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
	gEngfuncs.pEventAPI->EV_PlayerTrace( vSrc, vDst, PM_GLASS_IGNORE, -1, &tr );

//	if( tr.startsolid || tr.allsolid )
//		return false;

	int iContents = gEngfuncs.PM_PointContents( tr.endpos, NULL );

	if( tr.inwater && iContents == CONTENTS_WATER )
		return false;

	if( iContents == CONTENTS_SKY )
	{
		vOrigin.z = ( vOrigin.z + tr.endpos[2] ) * 0.5f;
		return true;
	}

	return false;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int PARTICLEAPI WeatherThink( WWParticle * pPart, const float & flFrametime )
{
	Vector vDiff = ( pPart->m_vOrigin - v_origin );
	vDiff.z = 0.0f;

	float flDotProd1 = DotProduct( vDiff.Normalize(), g_vForward );
	float flDist = vDiff.Length2D();

	// if the particle left our view, spin it around.
	// this makes it seem like we are drawing particles all around us.
	if( flDotProd1 < 0.0 )
	{
		Vector vOut;
		float flDotProd2 = DotProduct( vDiff.Normalize(), g_vRight   );

		if( flDotProd2 > 0.0f )
		{
			vOut.x = DotProduct( vDiff, g_flLeftRotate[0] );
			vOut.y = DotProduct( vDiff, g_flLeftRotate[1] );
			vOut.z = DotProduct( vDiff, g_flLeftRotate[2] );
		}
		else
		{
			vOut.x = DotProduct( vDiff, g_flRightRotate[0] );
			vOut.y = DotProduct( vDiff, g_flRightRotate[1] );
			vOut.z = DotProduct( vDiff, g_flRightRotate[2] );
		}

		Vector vOrigin = g_vOrigin + vOut;
		pPart->m_vOrigin = vOrigin;
		pPart->m_bDraw = FindSky( vOrigin );
	}

//	if( flDotProd1 < 0 )
//		WeatherTouch( pPart, NULL );

	// if the particle was hidden, try moving it around
	if( !pPart->m_bDraw )
		WeatherTouch( pPart, NULL );
	else
		pPart->m_flLife = ( gEngfuncs.GetClientTime() - pPart->m_flBirth ) + 0.1f;

	return pPart->Think( flFrametime );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PARTICLEAPI WeatherTouch( WWParticle * pPart, pmtrace_t * pTrace )
{
	if( pTrace )
		gEngfuncs.pEfxAPI->R_SparkEffect( pTrace->endpos, 8, -200, 200 );

	if( effects->value == 0.0f || weather->value == 0.0f )
	{
		pPart->m_bDraw = false;
		pPart->m_flLife = 0.0f;
		return;
	}


	pPart->m_vOrigin = v_origin;
	pPart->m_vOrigin = pPart->m_vOrigin + g_vForward * gEngfuncs.pfnRandomFloat(   -64.0f, 1024.0f );
	pPart->m_vOrigin = pPart->m_vOrigin + g_vRight   * gEngfuncs.pfnRandomFloat( -1024.0f, 1024.0f );
	pPart->m_vOrigin.z = v_origin[2] + gEngfuncs.pfnRandomFloat( 0.0f, 512.0f );

	pPart->m_flNextThink = 0.0f;

	float flDistance = ( pPart->m_vOrigin - v_origin ).Length2D();

	if( flDistance < 256.0f )
		pPart->m_iPriority = PARTICLE_PRIORITY_HIGH;
	else if( flDistance < 512.0f)
		pPart->m_iPriority = PARTICLE_PRIORITY_MEDIUM;
	else
		pPart->m_iPriority = PARTICLE_PRIORITY_LOW;

	pPart->m_bDraw = FindSky( pPart->m_vOrigin );
	pPart->m_flLife = 0.0f;

	if( pPart->m_bDraw )
	{
		pPart->m_flBirth = gEngfuncs.GetClientTime() + 0.1f;
		pPart->m_flLife = 5.0f;
	}
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PARTICLEAPI WeatherDeath( WWParticle * pPart )
{
	if( g_pWeatherManager != NULL )
	{
		g_pWeatherManager->Killed( pPart ); 
		g_pWeatherManager->Create();
	}
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
WeatherManager::WeatherManager()
{
	weather = CVAR_CREATE( "weather", "0", FCVAR_ARCHIVE | FCVAR_CLIENTDLL );

	m_bInit			= false;
	m_iTotal		= 0;
	m_flNextCheck	= 0.0f;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
WeatherManager::~WeatherManager()
{
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int WeatherManager::Process( float flTime )
{
	weather->value = CVAR_GET_FLOAT( "weather" );

	if( effects->value == 0.0f || weather->value == 0.0f )
		return 0;

	Vector vAngles = v_angles;
	vAngles[PITCH] = 0;

	AngleVectors( vAngles, g_vForward, g_vRight, NULL );

	g_vOrigin = v_origin - g_vForward * 256.0f;

	AngleMatrix( Vector( 0, 140, 0 ), g_flLeftRotate );
	AngleMatrix( Vector( 0, -140, 0 ), g_flRightRotate );

	if( weather->value == 1 && gEngfuncs.pfnRandomFloat( 0, 1 ) > 0.98f )
	{
		Vector vOrigin = v_origin +
			g_vForward * gEngfuncs.pfnRandomFloat( -1024, 1024 ) +
			g_vRight   * gEngfuncs.pfnRandomFloat( -1024, 1024 );

		pmtrace_t tr;
		gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );

		Vector vTop = vOrigin; vTop.z += 2048.0f;
		Vector vBot = vOrigin; vBot.z -= 2048.0f;

		gEngfuncs.pEventAPI->EV_PlayerTrace( vOrigin, vTop, PM_GLASS_IGNORE, -1, &tr );
		vTop = tr.endpos;

		gEngfuncs.pEventAPI->EV_PlayerTrace( vOrigin, vBot, PM_GLASS_IGNORE, -1, &tr );
		vBot = tr.endpos;

		if( gEngfuncs.PM_PointContents( vTop, NULL ) == CONTENTS_SKY )
		{
			int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/smoke.spr" );

			vTop.z += 512.0f;
			gEngfuncs.pEfxAPI->R_BeamPoints( vTop, vBot, iSprite, 0.1f, 2.0f, 1.0f, 2.0f,
				0.0f, 0, 0, 0.8, 0.6, 1.0f );

			dlight_t * dl = gEngfuncs.pEfxAPI->CL_AllocDlight( 0 );
			dl->origin  = vOrigin;
			dl->radius	= 4096;
			dl->color.r	= 255;
			dl->color.g	= 255;
			dl->color.b	= 255;
			dl->minlight= 255;
			dl->die		= gHUD.m_flTime + 0.05f;

			gEngfuncs.pEventAPI->EV_PlaySound( -1, v_origin, CHAN_STATIC, "ambience/thunder.wav",
				gEngfuncs.pfnRandomFloat( 0.6f, 1.0f ), ATTN_NORM, 0, 100 );
		}
	}
	
	if( m_flNextCheck > flTime )
		return 1;

	m_flNextCheck = flTime + 0.1f;

	if( m_iTotal < MAX_WEATHER_PARTICLES )
	{
		for( int i = m_iTotal; i < MAX_WEATHER_PARTICLES; i++ )
			Create();
	}

	return 1;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WeatherManager::Create( void )
{
	if( weather->value == 0.0 )
		return;

	if( m_iTotal >= MAX_WEATHER_PARTICLES )
		return;

	WWParticle * pPart = g_pParticleManager->Allocate( PARTICLE_PRIORITY_LOW );

	if( !pPart )
		return;

	pPart->SetThink( WeatherThink );
	pPart->SetTouch( WeatherTouch );
	pPart->SetDeath( WeatherDeath );

	pPart->m_vOrigin			= v_origin;
	pPart->m_vOrigin			= pPart->m_vOrigin + g_vForward * gEngfuncs.pfnRandomFloat(   -64.0f, 1024.0f );
	pPart->m_vOrigin			= pPart->m_vOrigin + g_vRight   * gEngfuncs.pfnRandomFloat( -1024.0f, 1024.0f );
	pPart->m_vOrigin.z			= v_origin[2] + gEngfuncs.pfnRandomFloat( 0.0f, 512.0f );
	pPart->m_vVelocity			= Vector( 0, 0, 0 );

	pPart->m_flThink			= 0.1f;
	pPart->m_flNextThink		= 0.0f;
	pPart->m_flLife				= 5.0f;
	pPart->m_flScale			= 1.0f;

	pPart->m_iRenderMode		= kRenderTransAdd;
	pPart->m_flRenderColor[0]	= 1.0f;
	pPart->m_flRenderColor[1]	= 0.0f;
	pPart->m_flRenderColor[2]	= 0.0f;
	pPart->m_flRenderColor[3]	= 0.8f;

	pPart->m_iFlags				= WW_PFLAG_COLLIDE;

	if( weather->value == 1 )
	{
		pPart->SetSprite( "sprites/weather/rain1.spr" );
		pPart->m_iOrient			= WW_ORIENT_YAW;
		pPart->m_vVelocity.z		= -800;
	}
	else
	{
		pPart->SetSprite( "sprites/flare1.spr" );
		pPart->m_flScale			= 0.5f;
		pPart->m_iOrient			= WW_ORIENT_ALL;
		pPart->m_iFlags				= WW_PFLAG_COLLIDE | WW_PFLAG_FEATHER;
		pPart->m_vVelocity.z		= -300;
	}

	float flDistance = ( pPart->m_vOrigin - v_origin ).Length2D();

	if( flDistance < 256.0f )
		pPart->m_iPriority = PARTICLE_PRIORITY_HIGH;
	else if( flDistance < 512.0f)
		pPart->m_iPriority = PARTICLE_PRIORITY_MEDIUM;
	else
		pPart->m_iPriority = PARTICLE_PRIORITY_LOW;

	m_iTotal++;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WeatherManager::Killed( WWParticle * pPart )
{
	m_iTotal--;
}