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
// Global Variables
//------------------------------------------------------------------------------
extern BEAM * pBeam;
extern BEAM * pBeam2;
extern int iBeamType;
extern float flBeamDist;


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
void UpdateBeams( void );
void TempEntityRegister( cl_entity_t * pEntity );


//------------------------------------------------------------------------------
// C Function Prototypes
//------------------------------------------------------------------------------
extern "C"
{
	void WWEvent_IcePoke1		( struct event_args_s * args );
	void WWEvent_IcePoke2		( struct event_args_s * args );
	void WWEvent_FreezeRay1		( struct event_args_s * args );
	void WWEvent_FreezeRay2		( struct event_args_s * args );
}


//------------------------------------------------------------------------------
// IcePoke Think
//------------------------------------------------------------------------------
void IcePokeThink( TEMPENTITY * pEnt, float flFrametime, float flTime )
{
	pEnt->entity.curstate.frame += 255.0f * flFrametime;
	if( pEnt->entity.curstate.frame >= 255.0f )
		pEnt->entity.curstate.frame = pEnt->entity.curstate.frame - (int)pEnt->entity.curstate.frame;
}


//------------------------------------------------------------------------------
// IcePoke Touch
//------------------------------------------------------------------------------
void IcePokeTouch( TEMPENTITY * pEntity, pmtrace_t * pTrace )
{
	Vector vOrigin = pTrace->endpos;

	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/xspark2.spr" );

	TEMPENTITY * pTemp = gEngfuncs.pEfxAPI->R_TempSprite( vOrigin, Vector( 0, 0, 0 ), 1.0f, iSprite,
		kRenderGlow, kRenderFxNoDissipation, 1.0f, 1.0f, FTENT_SPRANIMATE );

	if( pTemp != NULL )
	{
		pTemp->entity.curstate.framerate		= 15;
		pTemp->entity.curstate.rendercolor.r	= 255;
		pTemp->entity.curstate.rendercolor.g	= 255;
		pTemp->entity.curstate.rendercolor.b	= 255;
	}

	switch( gEngfuncs.pfnRandomLong( 0, 1 ) )
	{
	case 0: EV_HLDM_DecalTrace( pTrace, "{break1" ); break;
	case 1: EV_HLDM_DecalTrace( pTrace, "{break2" ); break;
	}

	gEngfuncs.pEventAPI->EV_PlaySound( -1, vOrigin, CHAN_WEAPON,
		"weapons/xbow_hitbod1.wav", 1.0f, ATTN_NORM, 0, 100 );

	gEngfuncs.pEfxAPI->R_BeamKill( pEntity->entity.index );
}



//------------------------------------------------------------------------------
// IcePoke Cast Event
//------------------------------------------------------------------------------
void WWEvent_IcePoke1( event_args_t * args )
{
	int idx = args->entindex;

	if( !EV_IsLocal( idx ) )
		return;

	if( args->iparam1 == 1 )
	{
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_ICE_ICEPOKE_IDLE, 0 );
		return;
	}

	if( args->iparam1 == 2 )
	{
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_ICE_ICEPOKE_CAST, 0 );
		return;
	}

	Vector vAngles = gHUD.m_vecAngles;
	Vector vForward, vRight, vUp;
	AngleVectors( vAngles, vForward, vRight, vUp );

	vAngles[PITCH] = -vAngles[PITCH];

	Vector vOrigin;
	EV_GetGunPosition( args, (float *)&vOrigin, args->origin );
	vOrigin.z -= 8.0f;

	vOrigin = vOrigin + vForward               * 16.0f;
	vOrigin = vOrigin + vRight * args->fparam1 * 16.0f;
	vOrigin = vOrigin - vUp    * args->fparam2 * 16.0f;

	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_WEAPON,
		"spells/icepoke.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );

	int iModels = gEngfuncs.pEventAPI->EV_FindModelIndex( "models/iceshard.mdl" );
	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/smoke.spr" );

	TEMPENTITY * pTemp = NULL;
	pTemp = gEngfuncs.pEfxAPI->R_TempModel( vOrigin, vForward, vAngles, 99.0f, iModels, 0 );

	if( pTemp != NULL )
	{
		TempEntityRegister( &pTemp->entity );

		pTemp->clientIndex	= idx;
		pTemp->flags = FTENT_COLLIDEALL | FTENT_COLLIDEKILL | FTENT_PERSIST | FTENT_CLIENTCUSTOM;
		pTemp->entity.curstate.rendermode		= kRenderTransAdd;
		pTemp->entity.curstate.rendercolor.r	= 255;
		pTemp->entity.curstate.rendercolor.g	= 255;
		pTemp->entity.curstate.rendercolor.b	= 255;
		pTemp->entity.curstate.renderamt		= 255;
		pTemp->entity.baseline.origin			= vForward * ICEPOKE_SPEED;
		pTemp->callback							= IcePokeThink;
		pTemp->hitcallback						= IcePokeTouch;

		gEngfuncs.pEfxAPI->R_BeamFollow( pTemp->entity.index, iSprite, 0.3f, 2.0f, 0.0f, 0.6f, 1.0f, 1.0f );
		gEngfuncs.pEfxAPI->R_BeamFollow( pTemp->entity.index, iSprite, 0.1f, 0.5f, 0.1f, 0.1f, 1.0f, 1.0f );
	}
}


//------------------------------------------------------------------------------
// IcePoke Entity Event
//------------------------------------------------------------------------------
void WWEvent_IcePoke2( event_args_t * args )
{
	if( args->iparam1 )
	{
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
			"spells/icepoke.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );

		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/smoke.spr" );

		gEngfuncs.pEfxAPI->R_BeamFollow( args->entindex, iSprite, 0.3f, 2.0f, 0.0f, 0.6f, 1.0f, 1.0f );
		gEngfuncs.pEfxAPI->R_BeamFollow( args->entindex, iSprite, 0.1f, 0.5f, 0.1f, 0.1f, 1.0f, 1.0f );
	}
	else
	{
		Vector vOrigin = args->origin;

		dlight_t * dl = gEngfuncs.pEfxAPI->CL_AllocDlight(0);
		dl->origin	= vOrigin;
		dl->radius	= 196;
		dl->color.r	= 0;
		dl->color.g	= 150;
		dl->color.b	= 255;
		dl->decay	= 100;
		dl->die		= gEngfuncs.GetClientTime() + 0.02f;

		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/xspark2.spr" );
		TEMPENTITY * pTemp = gEngfuncs.pEfxAPI->R_TempSprite( vOrigin, Vector( 0, 0, 0 ), 1.0f,
			iSprite, kRenderGlow, kRenderFxNoDissipation, 1.0f, 1.0f, FTENT_SPRANIMATE );

		if( pTemp != NULL )
		{
			pTemp->entity.curstate.framerate		= 15;
			pTemp->entity.curstate.rendercolor.r	= 255;
			pTemp->entity.curstate.rendercolor.g	= 255;
			pTemp->entity.curstate.rendercolor.b	= 255;
		}

		gEngfuncs.pEventAPI->EV_PlaySound( -1, vOrigin, CHAN_WEAPON,
			"weapons/xbow_hitbod1.wav", 1.0f, ATTN_NORM, 0, 100 );

		gEngfuncs.pEfxAPI->R_BeamKill( args->entindex );

		Vector vSrc, vDst;
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
		case 0: EV_HLDM_DecalTrace( &tr, "{break1" ); break;
		case 1: EV_HLDM_DecalTrace( &tr, "{break2" ); break;
		}
	}
}


//------------------------------------------------------------------------------
// FreezeRay, Start
//------------------------------------------------------------------------------
void WWEvent_FreezeRay1( event_args_t * args )
{
	int idx = args->entindex;

	switch( args->iparam1 )
	{
	case 0:
		gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
			"spells/freezeray_loop.wav", 1.0f, ATTN_NORM, 0, 100 );

		if( EV_IsLocal( idx ) )
		{
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_ICE_FREEZERAY_CAST, 0 );

			int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/smoke.spr" );

			if( pBeam != NULL )
			{
				pBeam->die = 0.0f;
				pBeam = NULL;
			}

			if( pBeam2 != NULL )
			{
				pBeam2->die = 0.0f;
				pBeam2 = NULL;
			}

			iBeamType = 1;
			flBeamDist = FREEZERAY_RANGE;

			// startEnt, end, modelIndex, life, width
			// amplitude, brightness, speed, startFrame, framerate, r, g, b );
			pBeam = gEngfuncs.pEfxAPI->R_BeamEntPoint( idx | 0x1000, args->origin, iSprite, 99999, 1.0f,
				0.05f, 1.0f, 5.0f, 0.0f, 0.0f, 0.5f, 0.5f, 1.0f );

			pBeam2 = gEngfuncs.pEfxAPI->R_BeamEntPoint( idx | 0x1000, args->origin, iSprite, 99999, 0.2f,
				0.0f, 1.0f, 5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f );

			UpdateBeams();
		}
		break;

	case 1:
		gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
			"spells/freezeray_loop.wav", 1.0f, ATTN_NORM, 0, 100 );
		break;

	case 2:
		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_ICE_FREEZERAY_IDLE, 0 );
		break;
	}

}


//------------------------------------------------------------------------------
// FreezeRay2, End
//------------------------------------------------------------------------------
void WWEvent_FreezeRay2( event_args_t * args )
{
	int idx = args->entindex;

	gEngfuncs.pEventAPI->EV_StopSound( idx, CHAN_WEAPON, "spells/freezeray_loop.wav" );

	if( !EV_IsLocal( idx ) )
		return;

	gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_ICE_FREEZERAY_END, 0 );

	if( pBeam != NULL )
	{
		pBeam->die = 0.0f;
		pBeam = NULL;
	}

	if( pBeam2 != NULL )
	{
		pBeam2->die = 0.0f;
		pBeam2 = NULL;
	}
}
