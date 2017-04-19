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
extern int tracerCount[ 32 ];


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
void VectorAngles( const float *forward, float *angles );
float EV_HLDM_PlayTextureSound( int idx, pmtrace_t * ptr, float * vecSrc, float * vecEnd, int iBulletType );


//------------------------------------------------------------------------------
// C Function Prototypes
//------------------------------------------------------------------------------
extern "C"
{
	void WWEvent_PebbleBlast	( struct event_args_s * args );
	void WWEvent_RollingStone1	( struct event_args_s * args );
	void WWEvent_RollingStone2	( struct event_args_s * args );
	void WWEvent_Bird1			( struct event_args_s * args );
	void WWEvent_Bird2			( struct event_args_s * args );
	void WWEvent_BearBite		( struct event_args_s * args );
}


//------------------------------------------------------------------------------
// PebbleBlast Event
//------------------------------------------------------------------------------
void WWEvent_PebbleBlast( event_args_t * args )
{
	int idx = args->entindex;

	if( args->iparam1 )
	{
		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_EARTH_PEBBLEBLAST_IDLE, 0 );
		return;
	}

	if( EV_IsLocal( idx ) )
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_EARTH_PEBBLEBLAST_CAST, 0 );

	gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
		"spells/pebbleblast.wav", gEngfuncs.pfnRandomFloat( 0.9f, 1.0f ), ATTN_NORM, 0,
		85 + gEngfuncs.pfnRandomLong( 0, 0x1F ) );

	Vector vecSrc;
	EV_GetGunPosition( args, vecSrc, args->origin );

	Vector vForward, vRight, vUp;
	AngleVectors( args->angles, vForward, vRight, vUp );

	vecSrc = vecSrc - vRight * 6.0f;

	EV_HLDM_FireBullets( idx, vForward, vRight, vUp, PEBBLEBLAST_COUNT, vecSrc, vForward, 2048,
		BULLET_PLAYER_BUCKSHOT, -1, &tracerCount[ idx - 1 ], 0.1f, 0.1f );
}


//------------------------------------------------------------------------------
// RollingStone Cast Event
//------------------------------------------------------------------------------
void WWEvent_RollingStone1( event_args_t * args )
{
	int idx = args->entindex;

	if( args->iparam1 )
	{
		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_EARTH_ROLLINGSTONE_IDLE, 0 );
		return;
	}

	// play casting sound
	gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON, "spells/stone_cast.wav",
		1.0f, ATTN_NORM, 0, PITCH_NORM );

	if( !EV_IsLocal( idx ) )
		return;

	gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_EARTH_ROLLINGSTONE_CAST, 0 );
}

//------------------------------------------------------------------------------
// RollingStone Entity Event
//------------------------------------------------------------------------------
void WWEvent_RollingStone2( event_args_t * args )
{
	int i = 0;
	switch( args->iparam1 )
	{
	// create
	case 0:
		break;

	// hit ground
	case 1:
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
			"weapons/ric5.wav", 1.0f, ATTN_NORM, 0, 100 );

		// create a few particles for dust
		for( i = 0; i < 3; i++ )
		{
			WWParticle * pPart = g_pParticleManager->Allocate( PARTICLE_PRIORITY_NONE );

			if( !pPart )
				break;

			pPart->SetSprite( "sprites/stmbal1.spr" );
			pPart->m_bDraw				= true;
			pPart->m_vOrigin			= args->origin;
			pPart->m_flLife				= 1.0f;
			pPart->m_flBirth			= gEngfuncs.GetClientTime() + 0.01f * (float)i;
			pPart->m_flScale			= 0.5f;
			pPart->m_flScaleDecay		= -1.0f;
			pPart->m_iRenderMode		= kRenderTransAdd;
			pPart->m_flFrameRate		= 30.0f;
			pPart->m_flRenderColor[0]	= 0.5f;
			pPart->m_flRenderColor[1]	= 0.4f;
			pPart->m_flRenderColor[2]	= 0.1f;
			pPart->m_flRenderColor[3]	= 0.2f;
			pPart->m_vVelocity[0]		= gEngfuncs.pfnRandomFloat( -5.0f, 5.0f );
			pPart->m_vVelocity[1]		= gEngfuncs.pfnRandomFloat( -5.0f, 5.0f );
			pPart->m_vVelocity[2]		= gEngfuncs.pfnRandomFloat( -5.0f, 5.0f );
			pPart->m_iFlags				|= WW_PFLAG_DIE_ON_LAST_FRAME;
		}

		break;

	// hit target
	case 2:
		break;

	// die
	case 3:
		break;
	}
}


//------------------------------------------------------------------------------
// Bird Cast Event
//------------------------------------------------------------------------------
void WWEvent_Bird1( event_args_t * args )
{
	int idx = args->entindex;

	if( !EV_IsLocal( idx ) )
		return;

	if( args->iparam1 )
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_EARTH_BIRD_IDLE, 0 );
	else
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_EARTH_BIRD_CAST, 0 );
}


//------------------------------------------------------------------------------
// Bird Explosion Event
//------------------------------------------------------------------------------
void WWEvent_Bird2( event_args_t * args )
{
	switch( args->iparam1 )
	{
	// create
	case 0:
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
			"npc/bird.wav", 1.0f, ATTN_NORM, 0, 100 );
		break;

	// target aquired
	case 1:
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
			"npc/bird.wav", 1.0f, ATTN_NORM, 0, 100 );
		break;

	// killed
	case 2:
		gEngfuncs.pEventAPI->EV_StopSound( args->entindex, CHAN_WEAPON, "npc/bird.wav" );
		break;
	}
}


//------------------------------------------------------------------------------
// BearBite Event
//------------------------------------------------------------------------------
void WWEvent_BearBite( event_args_t * args )
{
	int idx			 = args->entindex;
	int iHit		 = args->bparam1;

	static const char * sounds[] =
	{
		"spells/bearbite_hitbod1.wav",
		"spells/bearbite_hitbod2.wav",
		"spells/bearbite_hitbod3.wav",
		"spells/bearbite_hitwall.wav",
		"spells/bearbite_miss.wav",
	};

	// idle animation, host only
	if( EV_IsLocal( idx ) && args->bparam2 )
	{
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_EARTH_BEARBITE_IDLE, 0 );
		return;
	}

	if( EV_IsLocal( idx ) )
	{
		Vector vSrc, vDst, vAngles, vForward;

		vAngles = gHUD.m_vecAngles;
		AngleVectors( vAngles, vForward, NULL, NULL );

		EV_GetGunPosition( args, vSrc, args->origin );
		vDst = vSrc + vForward * BEARBITE_RANGE;

		pmtrace_t tr;
		gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction( false, true );	
		gEngfuncs.pEventAPI->EV_PushPMStates();
		gEngfuncs.pEventAPI->EV_SetSolidPlayers( idx - 1 );	
		gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
		gEngfuncs.pEventAPI->EV_PlayerTrace( vSrc, vDst, PM_STUDIO_BOX, -1, &tr );
		gEngfuncs.pEventAPI->EV_PopPMStates();

		if( tr.fraction != 1.0f )
			iHit = 1;

		if( EV_IsPlayer( tr.ent ) )
			iHit = 2;

		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_EARTH_BEARBITE_HIT1, 0 );
	}

	if( iHit )
	{
		if( iHit == 2 )
			gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
				sounds[gEngfuncs.pfnRandomLong( 0, 2 )], 1.0f, ATTN_NORM, 0, 100 );
		else
			gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON, sounds[3],
				1.0f, ATTN_NORM, 0, 100 );
	}
	else
		gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON, sounds[4],
			1.0f, ATTN_NORM, 0, 100 );
}
