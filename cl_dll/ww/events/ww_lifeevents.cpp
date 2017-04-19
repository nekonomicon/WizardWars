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
void TempEntityRegister( cl_entity_t * pEntity );



//------------------------------------------------------------------------------
// C Function Prototypes
//------------------------------------------------------------------------------
extern "C"
{
	void WWEvent_WhiteRay1		( struct event_args_s * args );
	void WWEvent_WhiteRay2		( struct event_args_s * args );
	void WWEvent_ForceBlast		( struct event_args_s * args );
}


//------------------------------------------------------------------------------
// WhiteRay, Start
//------------------------------------------------------------------------------
void WWEvent_WhiteRay1( event_args_t * args )
{
	int idx = args->entindex;

	switch( args->iparam1 )
	{
	case 0:
		gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
			"spells/whiteray_loop.wav", 1.0f, ATTN_NORM, 0, 100 );

		if( EV_IsLocal( idx ) )
		{
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_LIFE_WHITERAY_CAST, 0 );

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

			iBeamType = 3;
			flBeamDist = WHITERAY_RANGE;

			// startEnt, end, modelIndex, life, width
			// amplitude, brightness, speed, startFrame, framerate, r, g, b );
			pBeam = gEngfuncs.pEfxAPI->R_BeamEntPoint( idx | 0x1000, args->origin, iSprite, 99999, 0.5f,
				0.0f, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f );

			pBeam2 = gEngfuncs.pEfxAPI->R_BeamEntPoint( idx | 0x1000, args->origin, iSprite, 99999, 0.2f,
				0.0f, 1.0f, 5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f );

			UpdateBeams();
		}
		break;

	case 1:
		gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
			"spells/whiteray_loop.wav", 1.0f, ATTN_NORM, 0, 100 );
		break;

	case 2:
		if( EV_IsLocal( idx ) )
		{
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_LIFE_WHITERAY_IDLE, 0 );

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
		break;
	}

}


//------------------------------------------------------------------------------
// WhiteRay2, End
//------------------------------------------------------------------------------
void WWEvent_WhiteRay2( event_args_t * args )
{
	int idx = args->entindex;

	gEngfuncs.pEventAPI->EV_StopSound( idx, CHAN_WEAPON, "spells/whiteray_loop.wav" );

	if( !EV_IsLocal( idx ) )
		return;

	gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_LIFE_WHITERAY_END, 0 );

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
// ForceBlast
//------------------------------------------------------------------------------
void WWEvent_ForceBlast( event_args_t * args )
{
	int idx = args->entindex;

	if( args->iparam1 )
	{
		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_LIFE_FORCEBLAST_IDLE, 0 );
		return;
	}

	if( EV_IsLocal( idx ) )
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_LIFE_FORCEBLAST_CAST, 0 );

	// play casting sound
	gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON, "spells/forceblast_cast.wav",
		1.0f, ATTN_NORM, 0, PITCH_NORM );

	Vector vOrigin = args->origin;

	TEMPENTITY * pTemp = NULL;
	Vector vVelocity = Vector( 0, 0, 0 );
	int ent1 = 0, ent2 = 0;

	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/shockwave.spr" );

	vVelocity.x = vVelocity.y = -FORCEBLAST_RADIUS * 2.0f;
	pTemp = gEngfuncs.pEfxAPI->R_TempModel( vOrigin, vVelocity, Vector( 0, 0, 0 ), 0.25f, iSprite, 0 );

	if( pTemp != NULL )
	{
		TempEntityRegister( &pTemp->entity );

		ent1 = pTemp->entity.index;

		pTemp->flags						= FTENT_PERSIST | FTENT_NOMODEL;
		pTemp->entity.curstate.rendermode	= kRenderTransAdd;
		pTemp->entity.curstate.renderamt	= 0;
	}

	vVelocity.x = vVelocity.y = FORCEBLAST_RADIUS * 2.0f;
	pTemp = gEngfuncs.pEfxAPI->R_TempModel( vOrigin, vVelocity, Vector( 0, 0, 0 ), 0.25f, iSprite, 0 );

	if( pTemp != NULL )
	{
		TempEntityRegister( &pTemp->entity );

		ent2 = pTemp->entity.index;

		pTemp->flags						= FTENT_PERSIST | FTENT_NOMODEL;
		pTemp->entity.curstate.rendermode	= kRenderTransAdd;
		pTemp->entity.curstate.renderamt	= 0;
	}

	// startEnt, endEnt, modelIndex, life, width, amplitude, brightness, speed
	// startFrame, framerate, r, g, b
	gEngfuncs.pEfxAPI->R_BeamRing( ent1, ent2, iSprite, 0.2f, 20.0f, 0.0f, 0.5f, 1.0f, 0, 0, 0.0f, 0.0f, 1.0f );
}
