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
// C Function Prototypes
//------------------------------------------------------------------------------
extern "C"
{
	void WWEvent_BeanStalk		( struct event_args_s * args );
	void WWEvent_ThornBlast		( struct event_args_s * args );
}



//------------------------------------------------------------------------------
// PebbleBlast Event
//------------------------------------------------------------------------------
void WWEvent_BeanStalk( event_args_t * args )
{
	int idx = args->entindex;

	if( args->iparam1 )
	{
		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_NATURE_BEANSTALK_IDLE, 0 );
		return;
	}

	gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
		"spells/beanstalk.wav", gEngfuncs.pfnRandomFloat( 0.9f, 1.0f ),
		ATTN_NORM, 0, 85 + gEngfuncs.pfnRandomLong( 0, 0x1F ) );
}


//------------------------------------------------------------------------------
// PebbleBlast Event
//------------------------------------------------------------------------------
void WWEvent_ThornBlast( event_args_t * args )
{
	int idx = args->entindex;

	if( args->iparam1 )
	{
		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_NATURE_THORNBLAST_IDLE, 0 );
		return;
	}

	if( EV_IsLocal( idx ) )
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_NATURE_THORNBLAST_CAST, 0 );

	gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
		"spells/thornblast.wav", gEngfuncs.pfnRandomFloat( 0.9f, 1.0f ), ATTN_NORM, 0,
		85 + gEngfuncs.pfnRandomLong( 0, 0x1F ) );

	Vector vecSrc;
	EV_GetGunPosition( args, vecSrc, args->origin );

	Vector vForward, vRight, vUp;
	AngleVectors( args->angles, vForward, vRight, vUp );

	vecSrc = vecSrc - vRight * 6.0f;

	EV_HLDM_FireBullets( idx, vForward, vRight, vUp, THORNBLAST_COUNT, vecSrc, vForward, 2048,
		BULLET_PLAYER_BUCKSHOT, -1, &tracerCount[ idx - 1 ], 0.08f, 0.08f );
}