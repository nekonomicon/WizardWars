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
void TempEntityRegister( cl_entity_t * pEntity );
void VectorAngles( const float *forward, float *angles );
void WWEvent_FireBall_CallBack( TEMPENTITY * pEnt, float flFrametime, float flTime );
void WWEvent_FireBall_HitCallBack( TEMPENTITY * pEntity, pmtrace_t * pTrace );


//------------------------------------------------------------------------------
// C Function Prototypes
//------------------------------------------------------------------------------
extern "C"
{
	void WWEvent_FlameLick1		( struct event_args_s * args );
	void WWEvent_FlameLick2		( struct event_args_s * args );
	void WWEvent_FireBall1		( struct event_args_s * args );
	void WWEvent_FireBall2		( struct event_args_s * args );
}


//------------------------------------------------------------------------------
// FlameLick Particle Callback
//------------------------------------------------------------------------------
int PARTICLEAPI FlameLickThink( WWParticle * pPart, const float & flFrametime )
{
	float flScale = 1.0f;
	
	if( pPart->m_flLife != 0.0f )
		flScale = 1.0f / pPart->m_flLife;

	// if the flame is still pretty much blue
	if( pPart->m_flRenderColor[2] >= 0.5f )
	{
		pPart->m_flRenderColor[0] += 2.0f * flFrametime * flScale;
		pPart->m_flRenderColor[1] += 1.0f * flFrametime * flScale;
		pPart->m_flRenderColor[2] -= 3.0f * flFrametime * flScale;
		pPart->m_flRenderColor[3] += 6.0f * flFrametime * flScale;
	}

	// if the flame is pretty much red
	else
	{
		if( pPart->m_flRenderColor[0] <= 1.0f )
		{
			pPart->m_flRenderColor[0] += 4.0f * flFrametime * flScale;
			pPart->m_flRenderColor[1] += 1.0f * flFrametime * flScale;
		}

		pPart->m_flRenderColor[2] -= 0.5f * flFrametime * flScale;
		pPart->m_flScale += 2.0f * flFrametime * flScale;
	}

	pdata_t * pData = pPart->m_pPrivate;

	float r = gEngfuncs.GetClientTime() * 10.0f + pData->i1 * 5.0f;
	float s = sinf( r ) * 200.0f;
	float c = cosf( r ) * 200.0f;

	pPart->m_vOrigin = pPart->m_vOrigin + pData->v1 * 50.0f * flScale * flFrametime;
	pPart->m_vOrigin = pPart->m_vOrigin + pData->v2 * s * flFrametime;
	pPart->m_vOrigin = pPart->m_vOrigin + pData->v3 * c * flFrametime;

	return pPart->Think( flFrametime );
}


//------------------------------------------------------------------------------
// Creates a FlameLick Explosion
//------------------------------------------------------------------------------
void CreateFlameLick( Vector vOrigin, Vector vAngles, float flLife )
{
	// play sound at endpos of trace
	gEngfuncs.pEventAPI->EV_PlaySound( -1, vOrigin, CHAN_STATIC, "spells/flamelick_hit.wav",
		1.0f, ATTN_NORM, 0, PITCH_NORM );

	dlight_t * dl = gEngfuncs.pEfxAPI->CL_AllocDlight( 0 );
	dl->origin  = vOrigin;
	dl->radius	= 128;
	dl->color.r	= 255;
	dl->color.g	= 100;
	dl->color.b	= 50;
	dl->die		= gHUD.m_flTime + 0.5f;

	// no fire in water
	if( gEngfuncs.PM_PointContents( vOrigin, NULL ) == CONTENTS_WATER )
	{
		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/bubble.spr" ); 

		Vector vMins = vOrigin;
		Vector vMaxs = vOrigin;

		vMins.x = vOrigin.x - 16.0f;
		vMaxs.x = vOrigin.x + 16.0f;
		vMins.y = vOrigin.y - 16.0f;
		vMaxs.y = vOrigin.y + 16.0f;

		gEngfuncs.pEfxAPI->R_Bubbles( vMins, vMaxs, 32.0f, iSprite, 16, 32.0f );
	}

	// particle effect
	else if( effects->value != 0.0f )
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

			pPart->SetSprite( "sprites/stmbal1.spr" );
			pPart->SetPrivate( pData, true );
			pPart->SetThink( FlameLickThink );

			pPart->m_bDraw				= true;
			pPart->m_vOrigin			= vOrigin;
			pPart->m_flLife				= flLife;
			pPart->m_flBirth			= gEngfuncs.GetClientTime() + 0.01f * (float)i;
			pPart->m_flScale			= 0.5f;
			pPart->m_iRenderMode		= kRenderTransAdd;
			pPart->m_flFrameRate		= 30.0f;
			pPart->m_flRenderColor[0]	= 0.2f;
			pPart->m_flRenderColor[1]	= 0.2f;
			pPart->m_flRenderColor[2]	= 1.0f;
			pPart->m_flRenderColor[3]	= 0.1f;

			if( flLife == 1.0f )
				pPart->m_iFlags = WW_PFLAG_DIE_ON_LAST_FRAME;
		}
	}

	// retro effect
	else
	{
		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/zerogxplode.spr" );

		// create temporary entity
		TEMPENTITY * pTemp = NULL;
		pTemp = gEngfuncs.pEfxAPI->R_TempSprite( vOrigin, Vector( 0, 0, 0 ), 1.0f, iSprite,
			kRenderGlow, kRenderFxNoDissipation, 1, flLife, FTENT_SPRANIMATE );

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
// FlameLick Cast Event
//	Local player predicts his own explosion
//------------------------------------------------------------------------------
void WWEvent_FlameLick1( event_args_t * args )
{
	int idx = args->entindex;

	if( args->iparam1 && EV_IsLocal( idx ) )
	{
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_FIRE_FLAMELICK_IDLE, 0 );
		return;
	}

	// play casting sound
	gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON, "spells/flamelick_cast.wav",
		1.0f, ATTN_NORM, 0, PITCH_NORM );

	if( !EV_IsLocal( idx ) )
		return;

	gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_FIRE_FLAMELICK_CAST, 0 );

	Vector vSrc, vDst;
	EV_GetGunPosition( args, (float *)&vSrc, args->origin );

	Vector vAngles = gHUD.m_vecAngles;

	Vector vForward, vRight, vUp;
	AngleVectors( vAngles, vForward, vRight, vUp );

	vDst = vSrc + vForward * 8192.0f;

	pmtrace_t tr;
	gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction( false, true );
	gEngfuncs.pEventAPI->EV_PushPMStates();
	gEngfuncs.pEventAPI->EV_SetSolidPlayers( idx - 1 );	
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );

	gEngfuncs.pEventAPI->EV_PlayerTrace( vSrc, vDst, PM_STUDIO_BOX, -1 , &tr );

	if( tr.startsolid )
		gEngfuncs.pEventAPI->EV_PlayerTrace( vDst, vSrc, PM_STUDIO_BOX, -1 , &tr );

	gEngfuncs.pEventAPI->EV_PopPMStates();

	VectorAngles( tr.plane.normal, vAngles );
	vAngles[PITCH] = -vAngles[PITCH];

	if( tr.ent )
		vAngles = Vector( 0, 0, 1 );

	CreateFlameLick( tr.endpos, vAngles, 1.0f );
}


//------------------------------------------------------------------------------
// FlameLick Explosion Event
//	Local player doesn't recieve this, he predicts the explosion
//------------------------------------------------------------------------------
void WWEvent_FlameLick2( event_args_t * args )
{
	int idx = args->entindex;

	if( gEngfuncs.PM_PointContents( args->origin, NULL ) == CONTENTS_WATER )
		return;

	Vector vOrigin, vAngles;
	Vector vForward, vRight, vUp;

	vOrigin = args->origin;
	VectorAngles( args->angles, vAngles );
	AngleVectors( vAngles, vForward, vRight, vUp );

	Vector vSrc = vOrigin - vForward * 16.0f;
	Vector vDst = vOrigin + vForward * 16.0f;

	pmtrace_t tr;
	gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction( true, false );
	gEngfuncs.pEventAPI->EV_PushPMStates();
	gEngfuncs.pEventAPI->EV_SetSolidPlayers( 0 );	
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );

	gEngfuncs.pEventAPI->EV_PlayerTrace( vSrc, vDst, PM_STUDIO_BOX, -1 , &tr );

	if( tr.startsolid )
		gEngfuncs.pEventAPI->EV_PlayerTrace( vDst, vSrc, PM_STUDIO_BOX, -1 , &tr );

	gEngfuncs.pEventAPI->EV_PopPMStates();

	vOrigin = tr.endpos;

	VectorAngles( tr.plane.normal, vAngles );
	vAngles[PITCH] = -vAngles[PITCH];

	if( tr.ent )
		vAngles = Vector( 0, 0, 1 );

	CreateFlameLick( tr.endpos, vAngles, 1.0f );
}


//------------------------------------------------------------------------------
// FireBall HitCallBack
//------------------------------------------------------------------------------
void FireBallThink( TEMPENTITY * pEnt, float flFrametime, float flTime )
{
	dlight_t * dl = gEngfuncs.pEfxAPI->CL_AllocDlight( 0 );
	dl->origin	= pEnt->entity.origin;
	dl->radius	= 128;
	dl->color.r	= 255;
	dl->color.g	= 150;
	dl->color.b	= 50;
	dl->die		= flTime + 0.01f;

	pEnt->entity.curstate.frame += 255.0f * flFrametime;
	if( pEnt->entity.curstate.frame >= 255.0f )
		pEnt->entity.curstate.frame = pEnt->entity.curstate.frame - (int)pEnt->entity.curstate.frame;
}


//------------------------------------------------------------------------------
// FireBall HitCallBack
//------------------------------------------------------------------------------
void FireBallTouch( TEMPENTITY * pEntity, pmtrace_t * pTrace )
{
	gEngfuncs.pEfxAPI->R_Explosion( pEntity->entity.origin, 0, 1.0f, 15, 0 );
	gEngfuncs.pEfxAPI->R_BeamKill( pEntity->entity.index );

	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/zerogxplode.spr" );
	TEMPENTITY * pTemp = gEngfuncs.pEfxAPI->R_TempSprite( pTrace->endpos, Vector( 0, 0, 0 ), 1.0f,
		iSprite, kRenderGlow, kRenderFxNoDissipation, 1.0f, 1.0f, FTENT_SPRANIMATE );

	if( pTemp != NULL )
	{
		pTemp->entity.curstate.framerate		= 15;
		pTemp->entity.curstate.rendercolor.r	= 255;
		pTemp->entity.curstate.rendercolor.g	= 255;
		pTemp->entity.curstate.rendercolor.b	= 255;
	}

	switch( gEngfuncs.pfnRandomLong( 0, 1 ) )
	{
	case 0: EV_HLDM_DecalTrace( pTrace, "{scorch1" ); break;
	case 1: EV_HLDM_DecalTrace( pTrace, "{scorch2" ); break;
	}
	// 1.2.4 sound loop fix
	gEngfuncs.pEventAPI->EV_StopSound( pEntity->clientIndex, CHAN_BODY, "spells/fireball_ambience.wav" );
}


//------------------------------------------------------------------------------
// FireBall Trail
//------------------------------------------------------------------------------
void CreateFireBallTrail( int idx )
{
	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/smoke.spr" );

	gEngfuncs.pEfxAPI->R_BeamFollow( idx, iSprite, 0.5f, 15.0f, 1.0f, 1.0f, 0.0f, 1.0f );
	gEngfuncs.pEfxAPI->R_BeamFollow( idx, iSprite, 1.0f, 10.0f, 1.0f, 0.6f, 0.1f, 1.0f );
	gEngfuncs.pEfxAPI->R_BeamFollow( idx, iSprite, 1.5f,  5.0f, 0.6f, 0.3f, 0.3f, 1.0f );
	gEngfuncs.pEfxAPI->R_BeamFollow( idx, iSprite, 1.0f,  2.5f, 0.3f, 0.3f, 0.9f, 1.0f );
}


//------------------------------------------------------------------------------
// FireBall Cast Event
//------------------------------------------------------------------------------
void WWEvent_FireBall1( event_args_t * args )
{
	int idx = args->entindex;

	if( args->iparam1 )
	{
		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_FIRE_FIREBALL_IDLE, 0 );
		return;
	}

	// play casting sound
	gEngfuncs.pEventAPI->EV_PlaySound(
		idx, args->origin, CHAN_WEAPON, "spells/fireball_cast.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );

	// only predict local missiles, everyone else has to use the servers
	if( !EV_IsLocal( idx ) )
		return;

	if( !args->bparam1 )
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_FIRE_FIREBALL_CAST, 0 );

	Vector vOrigin, vAngles;

	vAngles = gHUD.m_vecAngles;
	
	Vector vForward, vRight, vUp;
	AngleVectors( vAngles, vForward, vRight, vUp );

	vAngles[PITCH] = -vAngles[PITCH];

	EV_GetGunPosition( args, (float *)&vOrigin, args->origin );
	
	vOrigin = vOrigin + vForward * 8.0f;
	vOrigin.z -= 8.0f;

	int iModel = gEngfuncs.pEventAPI->EV_FindModelIndex( "models/rollingstone.mdl" );

	TEMPENTITY * pTemp = NULL;

	pTemp = gEngfuncs.pEfxAPI->R_TempModel( vOrigin, vForward * FIREBALL_SPEED, vAngles, 99.0f, iModel, 0 );

	if( pTemp != NULL )
	{
		TempEntityRegister( &pTemp->entity );

		pTemp->clientIndex	= idx;
		pTemp->flags = FTENT_COLLIDEALL | FTENT_COLLIDEKILL | FTENT_PERSIST | FTENT_CLIENTCUSTOM;
		pTemp->entity.curstate.rendermode		= kRenderNormal;
		pTemp->entity.curstate.rendercolor.r	= 255;
		pTemp->entity.curstate.rendercolor.g	= 0;
		pTemp->entity.curstate.rendercolor.b	= 0;
		pTemp->entity.curstate.renderamt		= 6;
		pTemp->entity.curstate.renderfx			= kRenderFxGlowShell;
		pTemp->callback							= FireBallThink;
		pTemp->hitcallback						= FireBallTouch;

//		gEngfuncs.pEventAPI->EV_PlaySound( pTemp->entity.index, args->origin, CHAN_BODY,
//			"spells/fireball_ambience.wav", 1.0f, ATTN_NORM, 0, 100 );

		CreateFireBallTrail( pTemp->entity.index );
	}

	iModel = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/animglow01.spr" );

	pTemp = gEngfuncs.pEfxAPI->R_TempSprite( vOrigin, vForward * FIREBALL_SPEED, 1.0f, iModel, kRenderGlow, kRenderFxNoDissipation, 1.0f, 999.0f, 0 );

	if( pTemp != NULL )
	{
		pTemp->clientIndex	= idx;
		pTemp->flags = FTENT_SPRANIMATE | FTENT_SPRANIMATELOOP | FTENT_COLLIDEALL | FTENT_COLLIDEKILL | FTENT_PERSIST;
		pTemp->entity.curstate.framerate = 30.0f;
	}
}


//------------------------------------------------------------------------------
// FireBall Event
//	The server calls this on the fireball's birth and death
//------------------------------------------------------------------------------
void WWEvent_FireBall2( event_args_t * args )
{
	if( args->iparam1 )
	{
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
			"spells/fireball_ambience.wav", 1.0f, ATTN_NORM, 0, 100 );

		CreateFireBallTrail( args->entindex );
	}
	else
	{
		gEngfuncs.pEventAPI->EV_StopSound( args->entindex, CHAN_BODY, "spells/fireball_ambience.wav" );

		gEngfuncs.pEfxAPI->R_Explosion( args->origin, 0, 1.0f, 15, 0 );
		gEngfuncs.pEfxAPI->R_BeamKill( args->entindex );

		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/zerogxplode.spr" );
		TEMPENTITY * pTemp = gEngfuncs.pEfxAPI->R_TempSprite( args->origin, Vector( 0, 0, 0 ), 1.0f,
			iSprite, kRenderGlow, kRenderFxNoDissipation, 1.0f, 1.0f, FTENT_SPRANIMATE );

		if( pTemp != NULL )
		{
			pTemp->entity.curstate.framerate		= 15;
			pTemp->entity.curstate.rendercolor.r	= 255;
			pTemp->entity.curstate.rendercolor.g	= 255;
			pTemp->entity.curstate.rendercolor.b	= 255;
		}

		Vector vSrc, vDst, vOrigin;
		vOrigin = args->origin;
		vSrc.x = gEngfuncs.pfnRandomFloat( -1, 1 );
		vSrc.y = gEngfuncs.pfnRandomFloat( -1, 1 );
		vSrc.z = gEngfuncs.pfnRandomFloat( -1, 1 );
		vSrc = vSrc.Normalize() * 8.0f;
		vDst = vOrigin - vSrc;
		vSrc = vOrigin + vSrc;

		pmtrace_t tr;
		gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
		gEngfuncs.pEventAPI->EV_PlayerTrace( vSrc, vDst, PM_WORLD_ONLY, -1, &tr );

		switch( gEngfuncs.pfnRandomLong( 0, 1 ) )
		{
		case 0: EV_HLDM_DecalTrace( &tr, "{scorch1" ); break;
		case 1: EV_HLDM_DecalTrace( &tr, "{scorch2" ); break;
		}
	}
}

