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


// taken from dlls/ww/ents/ww_thornbush.h
enum
{
	THORNBUSH_EVENT_SPAWN,
	THORNBUSH_EVENT_FIRE,
	THORNBUSH_EVENT_FIRESPORE,
	THORNBUSH_EVENT_KILLED,
	THORNBUSH_EVENT_ENCHANT,
	THORNBUSH_EVENT_FERTILIZE,
	THORNBUSH_EVENT_HARVEST,
};


//------------------------------------------------------------------------------
// C Function Prototypes
//------------------------------------------------------------------------------
extern "C"
{
	void WWEvent_ThornBush			( struct event_args_s * args );
	void WWEvent_CockleBur			( struct event_args_s * args );
	void WWEvent_Spore				( struct event_args_s * args );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWEvent_ThornBush( event_args_t * args )
{
	int iTarget		= args->iparam1;
	int iLevel		= args->iparam2;
	int bEnchanted	= args->bparam1;
	int iModel		= 0;

	int i;

	const char * sound = NULL;

	switch( (int)args->fparam1 )
	{
	case THORNBUSH_EVENT_SPAWN:
// 1.2.6 #15 Play grow sound on spawn
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
			"npc/thornbush_upgrade1.wav", 1.0f, ATTN_NORM, 0, 100 );

		break;

	case THORNBUSH_EVENT_FIRE:
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
			"npc/thornbush_attack.wav", 1.0f, ATTN_NORM, 0, 100 );
		break;

	case THORNBUSH_EVENT_FIRESPORE:
		break;

	case THORNBUSH_EVENT_KILLED:
		iModel = gEngfuncs.pEventAPI->EV_FindModelIndex( "models/bushgibs.mdl" );

		if( !iModel )
			break;
		
		for( i = 0; i < 8; i++ )
		{
			Vector vOrigin = args->origin;
			Vector vAngles = args->angles;

			float flRandom1 = gEngfuncs.pfnRandomFloat( -1, 1 );
			float flRandom2 = gEngfuncs.pfnRandomFloat( -1, 1 );

			if( flRandom1 > -0.5f && flRandom1 < 0.5f )
				flRandom1 *= 2.0f;
			if( flRandom2 > -0.5f && flRandom2 < 0.5f )
				flRandom2 *= 2.0f;

			vOrigin.x += 48 * flRandom1;
			vOrigin.y += 48 * flRandom2;
			vOrigin.z += gEngfuncs.pfnRandomFloat( 16, 40 );

			Vector vVelocity;
			vVelocity.x = gEngfuncs.pfnRandomFloat( -300, 300 );
			vVelocity.y = gEngfuncs.pfnRandomFloat( -300, 300 );
			vVelocity.z = gEngfuncs.pfnRandomFloat(  200, 400 );

			float flLife = gEngfuncs.pfnRandomFloat( 8, 12 );

			TEMPENTITY * pTemp = NULL;
			pTemp = gEngfuncs.pEfxAPI->R_TempModel( vOrigin, vVelocity, vAngles, flLife, iModel, 0 );

			if( pTemp != NULL )
			{
				pTemp->entity.curstate.body = gEngfuncs.pfnRandomLong( 0, 4 );

				pTemp->flags |= FTENT_HITSOUND;
				pTemp->flags |= FTENT_GRAVITY;
				pTemp->flags |= FTENT_ROTATE;
				pTemp->flags |= FTENT_COLLIDEALL;
				pTemp->flags |= FTENT_FADEOUT;
			}
		}

		break;

	case THORNBUSH_EVENT_ENCHANT:
		break;

	case THORNBUSH_EVENT_FERTILIZE:
		switch( gEngfuncs.pfnRandomLong( 0, 2 ) )
		{
		case 0: sound = "npc/thornbush_upgrade1.wav"; break;
		case 1: sound = "npc/thornbush_upgrade2.wav"; break;
		case 2: sound = "npc/thornbush_upgrade3.wav"; break;
		}

		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin,
			CHAN_BODY, sound, 1.0f, ATTN_NORM, 0, 110 );

		gEngfuncs.pEfxAPI->R_LargeFunnel( args->origin, 0 );

		break;

	case THORNBUSH_EVENT_HARVEST:
		switch( gEngfuncs.pfnRandomLong( 0, 2 ) )
		{
		case 0: sound = "npc/thornbush_upgrade1.wav"; break;
		case 1: sound = "npc/thornbush_upgrade2.wav"; break;
		case 2: sound = "npc/thornbush_upgrade3.wav"; break;
		}

		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin,
			CHAN_BODY, sound, 1.0f, ATTN_NORM, 0, 90 );

		gEngfuncs.pEfxAPI->R_LargeFunnel( args->origin, 1 );

		break;
	}
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWEvent_CockleBur( event_args_t * args )
{
	switch( args->iparam1 )
	{
	// touched
	case 1:
		break;

	// killed
	case 2:
		gEngfuncs.pEfxAPI->R_Blood( args->origin, Vector( 0, 0, 1 ), 190, 45 );
		break;

	default:
		return;
	}

	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"npc/cocklebur.wav", 1.0f, ATTN_NORM, 0, 100 );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWEvent_Spore( event_args_t * args )
{
	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"npc/spore.wav", 1.0f, ATTN_NORM, 0, 100 );
}
