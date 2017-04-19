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
extern float flBeamDist;
extern int iBeamType;
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
	void WWEvent_DeathRay1		( struct event_args_s * args );
	void WWEvent_DeathRay2		( struct event_args_s * args );
	void WWEvent_Skull1			( struct event_args_s * args );
	void WWEvent_Skull2			( struct event_args_s * args );
}


//------------------------------------------------------------------------------
// DeathRay, Start
//------------------------------------------------------------------------------
void WWEvent_DeathRay1( event_args_t * args )
{
	int idx = args->entindex;

	switch( args->iparam1 )
	{
	case 0:
		gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
			"spells/deathray_loop.wav", 1.0f, ATTN_NORM, 0, 100 );

		if( EV_IsLocal( idx ) )
		{
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_DEATH_DEATHRAY_CAST, 0 );

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

			iBeamType = 2;
			flBeamDist = DEATHRAY_RANGE;

			// startEnt, end, modelIndex, life, width
			// amplitude, brightness, speed, startFrame, framerate, r, g, b );
			pBeam = gEngfuncs.pEfxAPI->R_BeamEntPoint( idx | 0x1000, args->origin, iSprite, 99999, 0.5f,
				0.0f, 1.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f );

			pBeam2 = gEngfuncs.pEfxAPI->R_BeamEntPoint( idx | 0x1000, args->origin, iSprite, 99999, 0.2f,
				0.0f, 1.0f, 5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f );

			UpdateBeams();
		}
		break;

	case 1:
		gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
			"spells/deathray_loop.wav", 1.0f, ATTN_NORM, 0, 100 );
		break;

	case 2:
		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_DEATH_DEATHRAY_IDLE, 0 );
		break;
	}

}


//------------------------------------------------------------------------------
// DeathRay2, End
//------------------------------------------------------------------------------
void WWEvent_DeathRay2( event_args_t * args )
{
	int idx = args->entindex;

	gEngfuncs.pEventAPI->EV_StopSound( idx, CHAN_WEAPON, "spells/deathray_loop.wav" );

	if( !EV_IsLocal( idx ) )
		return;

	gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_DEATH_DEATHRAY_END, 0 );

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
// Skull Cast Event
//------------------------------------------------------------------------------
void WWEvent_Skull1( event_args_t * args )
{
	int idx = args->entindex;

	if( !EV_IsLocal( idx ) )
		return;

	if( args->iparam1 )
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_DEATH_SKULL_IDLE, 0 );
	else
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_DEATH_SKULL_CAST, 0 );
}


//------------------------------------------------------------------------------
// Skull Explosion Event
//------------------------------------------------------------------------------
void WWEvent_Skull2( event_args_t * args )
{
	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/smoke.spr" );

	switch( args->iparam1 )
	{
	// create
	case 0:
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
			"npc/skull.wav", 1.0f, ATTN_NORM, 0, 100 );
		gEngfuncs.pEfxAPI->R_BeamFollow( args->entindex, iSprite, 0.9f, 8.0f, 1.0f, 0.0f, 0.0f, 1.0f );
		gEngfuncs.pEfxAPI->R_BeamFollow( args->entindex, iSprite, 0.6f, 4.0f, 1.0f, 0.5f, 0.0f, 1.0f );
		gEngfuncs.pEfxAPI->R_BeamFollow( args->entindex, iSprite, 0.3f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f );
		break;

	// target aquired
	case 1:
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
			"npc/skull.wav", 1.0f, ATTN_NORM, 0, 100 );
		break;

	// killed
	case 2:
		gEngfuncs.pEventAPI->EV_StopSound( args->entindex, CHAN_WEAPON, "npc/skull.wav" );
		gEngfuncs.pEfxAPI->R_BeamKill( args->entindex );

		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
			"npc/skull_poof.wav", 1.0f, ATTN_NORM, 0, 100 );

		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/stmbal1.spr" );
		TEMPENTITY * pTemp = gEngfuncs.pEfxAPI->R_TempSprite( args->origin, Vector( 0, 0, 0 ), 1.0f, iSprite,
			kRenderGlow, kRenderFxNoDissipation, 1.0f, 1.0f, FTENT_SPRANIMATE );

		if( pTemp != NULL )
		{
			pTemp->entity.curstate.scale			= 2.0f;
			pTemp->entity.curstate.framerate		= 15;
			pTemp->entity.curstate.rendercolor.r	= 255;
			pTemp->entity.curstate.rendercolor.g	= 32;
			pTemp->entity.curstate.rendercolor.b	= 0;
		}

		break;
	}
}