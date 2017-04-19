#include "hud.h"
#include "cl_util.h"
#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "entity_types.h"
#include "usercmd.h"
#include "pm_defs.h"
#include "pm_materials.h"
#include "eventscripts.h"
#include "ev_hldm.h"
#include "r_efx.h"
#include "event_api.h"
#include "event_args.h"
#include "in_defs.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"

#include "ww/ww_particle.h"
#include "ww/ww_particlemanger.h"



//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
void VectorAngles( const float *forward, float *angles );


//------------------------------------------------------------------------------
// C Function Prototypes
//------------------------------------------------------------------------------
extern "C"
{
	void WWEvent_Updraft1		( struct event_args_s * args );
	void WWEvent_Updraft2		( struct event_args_s * args );
	void WWEvent_Levitate		( struct event_args_s * args );
}


//------------------------------------------------------------------------------
// Updraft Particle Function
//------------------------------------------------------------------------------
int PARTICLEAPI UpdraftThink( WWParticle * pPart, const float & flFrametime )
{
	pdata_t * pData = pPart->m_pPrivate;

	pData->f1 += 1000.0f * flFrametime;

	float r = gEngfuncs.GetClientTime() * 10.0f + pData->i1;
	float s = sinf( r ) * 200.0f;
	float c = cosf( r ) * 200.0f;

	pPart->m_vOrigin = pPart->m_vOrigin + pData->v1 * 150.0f * flFrametime;
	pPart->m_vOrigin = pPart->m_vOrigin + pData->v2 * s * flFrametime;
	pPart->m_vOrigin = pPart->m_vOrigin + pData->v3 * c * flFrametime;

	pPart->m_flRenderColor[3] -= 1.0f * flFrametime;
	pPart->m_flScale += 2.0f * flFrametime;

	return 1;
}


//------------------------------------------------------------------------------
// Creates an Updraft effect
//------------------------------------------------------------------------------
void CreateUpdraft( Vector vOrigin, Vector vNormal )
{
	Vector vAngles;
	VectorAngles( vNormal, vAngles );
	vAngles[PITCH] = -vAngles[PITCH];

	gEngfuncs.pEventAPI->EV_PlaySound( -1, vOrigin, CHAN_STATIC, "spells/updraft_hit.wav",
		1.0f, ATTN_NORM, 0, PITCH_NORM );

	// particle effect
	if( effects->value != 0.0f )
	{
		Vector vForward, vRight, vUp;
		AngleVectors( vAngles, vForward, vRight, vUp );

		// create a few particles for a twirling wind effect
		for( int i = 0; i < 10; i++ )
		{
			WWParticle * pPart = g_pParticleManager->Allocate( PARTICLE_PRIORITY_NONE );

			if( !pPart )
				break;

			pdata_t * pData = (pdata_t *)malloc( sizeof(pdata_t) );
			memset( pData, 0, sizeof(pdata_t) );

			pData->i1 = i;
			pData->v1 = vForward;
			pData->v2 = vRight;
			pData->v3 = vUp;

			pPart->SetSprite( "sprites/flare1.spr" );
			pPart->SetPrivate( pData, true );
			pPart->SetThink( UpdraftThink );

			pPart->m_bDraw				= true;
			pPart->m_iFlags				= 0;
			pPart->m_vOrigin			= vOrigin;
//			pPart->m_vOrigin			= pPart->m_vOrigin + vForward * gEngfuncs.pfnRandomFloat( -10, 10 );
//			pPart->m_vOrigin			= pPart->m_vOrigin + vUp * gEngfuncs.pfnRandomFloat( -10, 10 );

			pPart->m_flLife				= 1.0f;
			pPart->m_flBirth			= gEngfuncs.GetClientTime() + 0.02f * (float)i;

			pPart->m_flScale			= 0.5f;

			pPart->m_iRenderMode		= kRenderTransAdd;
			pPart->m_flRenderColor[0]	= 0.4f;
			pPart->m_flRenderColor[1]	= 0.5f;
			pPart->m_flRenderColor[2]	= 0.6f;
			pPart->m_flRenderColor[3]	= 0.5f;
		}
	}

	// retro effect
	else
	{
		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/effects/updraft.spr" ); 

		// create temporary entity
		TEMPENTITY * pTemp = NULL;
		pTemp = gEngfuncs.pEfxAPI->R_TempSprite( vOrigin, Vector( 0, 0, 64 ), 0.5f, iSprite,
			kRenderGlow, kRenderFxNoDissipation, 1, 1, FTENT_SPRANIMATE );

		if( pTemp )
		{
			pTemp->entity.curstate.framerate     = 20;
			pTemp->entity.curstate.rendercolor.r = 255;
			pTemp->entity.curstate.rendercolor.g = 255;
			pTemp->entity.curstate.rendercolor.b = 255;
		}
	}
}


//------------------------------------------------------------------------------
// Updraft Cast Event
//------------------------------------------------------------------------------
void WWEvent_Updraft1( event_args_t * args )
{
	int idx = args->entindex;

	if( args->iparam1 )
	{
		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_WIND_UPDRAFT_IDLE, 0 );
		return;
	}

	// play casting sound
	gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON, "spells/updraft_cast.wav",
		1.0f, ATTN_NORM, 0, PITCH_NORM );

	if( !EV_IsLocal( idx ) )
		return;

	gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_WIND_UPDRAFT_CAST, 0 );

	Vector vSrc, vDst;
	EV_GetGunPosition( args, (float *)&vSrc, args->origin );

	Vector vAngles = gHUD.m_vecAngles;

	Vector vForward, vRight, vUp;
	AngleVectors( vAngles, vForward, vRight, vUp );

	vDst = vSrc + vForward * 8192.0f;

	pmtrace_t tr;
	gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction( true, false );
	gEngfuncs.pEventAPI->EV_PushPMStates();
	gEngfuncs.pEventAPI->EV_SetSolidPlayers( idx - 1 );	
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );

	gEngfuncs.pEventAPI->EV_PlayerTrace( vSrc, vDst, PM_STUDIO_BOX, -1 , &tr );

	if( tr.startsolid )
		gEngfuncs.pEventAPI->EV_PlayerTrace( vDst, vSrc, PM_STUDIO_BOX, -1 , &tr );

	gEngfuncs.pEventAPI->EV_PopPMStates();

	if( tr.ent )
		vAngles = Vector( 0, 0, 1 );

	CreateUpdraft( tr.endpos, tr.plane.normal );
}


//------------------------------------------------------------------------------
// Updraft Effect Event
//------------------------------------------------------------------------------
void WWEvent_Updraft2( event_args_t * args )
{
	int idx = args->entindex;

	Vector vOrigin, vAngles;
	Vector vForward, vRight, vUp;

	vOrigin = args->origin;
	VectorAngles( args->angles, vAngles );
	AngleVectors( vAngles, vForward, vRight, vUp );

	Vector vSrc = vOrigin + vForward * 16.0f;
	Vector vDst = vOrigin - vForward * 16.0f;

	pmtrace_t tr;
	gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction( true, false );
	gEngfuncs.pEventAPI->EV_PushPMStates();
	gEngfuncs.pEventAPI->EV_SetSolidPlayers( 0 );	
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );

	gEngfuncs.pEventAPI->EV_PlayerTrace( vSrc, vDst, PM_STUDIO_BOX, -1 , &tr );

	if( tr.startsolid )
		gEngfuncs.pEventAPI->EV_PlayerTrace( vDst, vSrc, PM_STUDIO_BOX, -1 , &tr );

	gEngfuncs.pEventAPI->EV_PopPMStates();

	if( tr.ent )
		vAngles = Vector( 0, 0, 1 );

	CreateUpdraft( tr.endpos, tr.plane.normal );
}


//------------------------------------------------------------------------------
// Levitate Event
//------------------------------------------------------------------------------
void WWEvent_Levitate( event_args_t * args )
{
	int idx = args->entindex;

	if( args->iparam1 )
	{
		gEngfuncs.pEventAPI->EV_StopSound( idx, CHAN_WEAPON, "spells/levitate.wav" );
		return;
	}

	gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON, "spells/levitate.wav",
		1.0f, ATTN_NORM, 0, PITCH_NORM );
}