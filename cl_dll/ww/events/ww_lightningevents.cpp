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
extern TEMPENTITY * pCrosshair;


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
void UpdateBeams( void );


//------------------------------------------------------------------------------
// C Function Prototypes
//------------------------------------------------------------------------------
extern "C"
{
	void WWEvent_LightningBolt1	( struct event_args_s * args );
	void WWEvent_LightningBolt2	( struct event_args_s * args );
	void WWEvent_SpotBolt1		( struct event_args_s * args );
	void WWEvent_SpotBolt2		( struct event_args_s * args );
}


//------------------------------------------------------------------------------
// LightningBolt1, Start
//------------------------------------------------------------------------------
void WWEvent_LightningBolt1( event_args_t * args )
{
	int idx = args->entindex;

	switch( args->iparam1 )
	{
	case 0:
		gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
			"spells/bolt_loop.wav", 1.0f, ATTN_NORM, 0, 100 );

		if( EV_IsLocal( idx ) )
		{
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_LIGHTNING_LIGHTNINGBOLT_CAST, 0 );

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

			iBeamType = 4;
			flBeamDist = LIGHTNINGBOLT_RANGE;

			// startEnt, end, modelIndex, life, width
			// amplitude, brightness, speed, startFrame, framerate, r, g, b );
			pBeam = gEngfuncs.pEfxAPI->R_BeamEntPoint( idx | 0x1000, args->origin, iSprite, 99999, 0.2f,
				0.1f, 1.0f, 5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f );

			pBeam2 = gEngfuncs.pEfxAPI->R_BeamEntPoint( idx | 0x1000, args->origin, iSprite, 99999, 0.2f,
				0.1f, 1.0f, 10.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.25f );

			UpdateBeams();
		}
		break;

	case 1:
		gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
			"spells/bolt_loop.wav", 1.0f, ATTN_NORM, 0, 100 );

		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_LIGHTNING_LIGHTNINGBOLT_CAST, 0 );
		break;

	case 2:
		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_LIGHTNING_LIGHTNINGBOLT_IDLE, 0 );
		break;
	}

}


//------------------------------------------------------------------------------
// LightningBolt2, End
//------------------------------------------------------------------------------
void WWEvent_LightningBolt2( event_args_t * args )
{
	int idx = args->entindex;

	gEngfuncs.pEventAPI->EV_StopSound( idx, CHAN_WEAPON, "spells/bolt_loop.wav" );

	if( !EV_IsLocal( idx ) )
		return;

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



//------------------------------------------------------------------------------
// SpotBolt1
//------------------------------------------------------------------------------
void WWEvent_SpotBolt1( event_args_t * args )
{
	int idx = args->entindex;
	int iSprite;

	if( args->iparam2 )
	{
		gEngfuncs.pEventAPI->EV_StopSound( idx, CHAN_WEAPON, "spells/bolt_loop.wav" );

		if( EV_IsLocal( idx ) )
		{
			if( pCrosshair != NULL )
			{
				pCrosshair->die = 0.0f;
				pCrosshair = NULL;
			}
		}
	}

	switch( args->iparam1 )
	{
	case 1:
		gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
			"spells/bolt_start.wav", 1.0f, ATTN_NORM, 0, 100 );

		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_LIGHTNING_SPOTBOLT_START, 0 );
		break;

	case 2:
		gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
			"spells/bolt_loop.wav", 1.0f, ATTN_NORM, 0, 100 );
		
		if( EV_IsLocal( idx ) )
		{
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_LIGHTNING_SPOTBOLT_CHARGE, 0 );

			if( pCrosshair != NULL )
			{
				pCrosshair->die = 0.0f;
				pCrosshair = NULL;
			}

			flBeamDist = 8192.0f;

			iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/dot.spr" );
			pCrosshair = gEngfuncs.pEfxAPI->R_TempSprite( args->origin, Vector( 0, 0, 0 ), 1.0f,
				iSprite, kRenderGlow, kRenderFxNoDissipation, 1.0f, 1.0f, 0 );

			if( pCrosshair != NULL )
			{
				pCrosshair->entity.curstate.rendercolor.r = 250;
				pCrosshair->entity.curstate.rendercolor.g = 150;
				pCrosshair->entity.curstate.rendercolor.b = 0;
				pCrosshair->entity.curstate.renderamt = 255;
			}
		}
		break;

	case 3:
		if( EV_IsLocal( idx ) )
		{
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_LIGHTNING_SPOTBOLT_FULL, 0 );

			if( pCrosshair != NULL )
			{
				pCrosshair->entity.curstate.rendercolor.r = 250;
				pCrosshair->entity.curstate.rendercolor.g = 250;
				pCrosshair->entity.curstate.rendercolor.b = 0;
				pCrosshair->entity.curstate.renderamt = 255;
			}
		}

		break;

	case 4:
		gEngfuncs.pEventAPI->EV_StopSound( idx, CHAN_WEAPON, "spells/bolt_loop.wav" );

		if( args->bparam1 )
		{
			gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
				"spells/bolt_end.wav", 1.0f, ATTN_NORM, 0, 100 );
		}

		if( EV_IsLocal( idx ) )
		{
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_LIGHTNING_SPOTBOLT_FIRE, 0 );

			if( pCrosshair != NULL )
			{
				pCrosshair->die = 0.0f;
				pCrosshair = NULL;
			}
		}
		break;

	case 5:
		if( EV_IsLocal( idx ) )
		{
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_LIGHTNING_SPOTBOLT_IDLE, 0 );

			if( pCrosshair != NULL )
			{
				pCrosshair->die = 0.0f;
				pCrosshair = NULL;
			}
		}
		break;

	case 6:
		gEngfuncs.pEventAPI->EV_StopSound( idx, CHAN_WEAPON, "spells/bolt_loop.wav" );

		if( EV_IsLocal( idx ) )
		{
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_LIGHTNING_SPOTBOLT_IDLE, 0 );

			if( pCrosshair != NULL )
			{
				pCrosshair->die = 0.0f;
				pCrosshair = NULL;
			}
		}
	}
}


//------------------------------------------------------------------------------
// SpotBolt2, Effect
//------------------------------------------------------------------------------
void WWEvent_SpotBolt2( event_args_t * args )
{
	Vector vTop = args->origin; vTop.z += 2048;
	Vector vBot = args->origin; vBot.z -= 2048;

	float flRandom1 = gEngfuncs.pfnRandomFloat( -2048.0f, 2048.0f );
	float flRandom2 = gEngfuncs.pfnRandomFloat( -2048.0f, 2048.0f );

	vTop.x += flRandom1;
	vBot.x -= flRandom1;
	vTop.y += flRandom2;
	vBot.y -= flRandom2;

	pmtrace_t tr;
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );

	gEngfuncs.pEventAPI->EV_PlayerTrace( args->origin, vTop, PM_GLASS_IGNORE, -1, &tr );
	if( !tr.allsolid )
		vTop = tr.endpos;
	else
		vTop = args->origin;

	gEngfuncs.pEventAPI->EV_PlayerTrace( args->origin, vBot, PM_GLASS_IGNORE, -1, &tr );
	if( !tr.allsolid )
		vBot = tr.endpos;
	else
		vBot = args->origin;

	EV_HLDM_DecalTrace( &tr, "{gargstomp" );
	gEngfuncs.pEfxAPI->R_SparkEffect( vBot, 32, -500, 500 );
	gEngfuncs.pEfxAPI->R_SparkShower( vBot );

	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/smoke.spr" );

	vTop.z += 512.0f;
	gEngfuncs.pEfxAPI->R_BeamPoints( vTop, vBot, iSprite, 0.1f, 2.0f + args->fparam1 + 1.0f, 1.0f, 2.0f,
		0.0f, 0, 0, 0.8, 0.6, 0.2f );

	dlight_t * dl = gEngfuncs.pEfxAPI->CL_AllocDlight( 0 );
	dl->origin  = args->origin;
	dl->radius	= 512 * args->fparam1;
	dl->color.r	= 255;
	dl->color.g	= 200;
	dl->color.b	= 10;
	dl->die		= gEngfuncs.GetClientTime() + 0.05f;

	gEngfuncs.pEventAPI->EV_PlaySound( -1, args->origin, CHAN_STATIC,
		"ambience/thunder.wav", 1.0f, ATTN_NORM, 0, 100 );

}
