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

#include "ww/ww_particle.h"
#include "ww/ww_particlemanger.h"

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
void HUD_GetLastOrg( float *org );


//------------------------------------------------------------------------------
// C Function Prototypes
//------------------------------------------------------------------------------
extern "C"
{
	// general events
	void WWEvent_Taunt				( struct event_args_s * args );
	void WWEvent_IceGibs			( struct event_args_s * args );
	void WWEvent_Burn				( struct event_args_s * args );
	void WWEvent_Spark				( struct event_args_s * args );
	void WWEvent_Poison				( struct event_args_s * args );

	void WWEvent_ThornBush			( struct event_args_s * args );
	void WWEvent_CockleBur			( struct event_args_s * args );
	void WWEvent_Spore				( struct event_args_s * args );

	// spell events
	void WWEvent_StaffHit			( struct event_args_s * args );
	void WWEvent_Shield				( struct event_args_s * args );
	void WWEvent_Missile1			( struct event_args_s * args );
	void WWEvent_Missile2			( struct event_args_s * args );
	void WWEvent_SealSound			( struct event_args_s * args );
	void WWEvent_Updraft1			( struct event_args_s * args );
	void WWEvent_Updraft2			( struct event_args_s * args );
	void WWEvent_Levitate			( struct event_args_s * args );
	void WWEvent_LightningBolt1		( struct event_args_s * args );
	void WWEvent_LightningBolt2		( struct event_args_s * args );
	void WWEvent_SpotBolt1			( struct event_args_s * args );
	void WWEvent_SpotBolt2			( struct event_args_s * args );
	void WWEvent_FlameLick1			( struct event_args_s * args );
	void WWEvent_FlameLick2			( struct event_args_s * args );
	void WWEvent_FireBall1			( struct event_args_s * args );
	void WWEvent_FireBall2			( struct event_args_s * args );
	void WWEvent_DeathRay1			( struct event_args_s * args );
	void WWEvent_DeathRay2			( struct event_args_s * args );
	void WWEvent_Skull1				( struct event_args_s * args );
	void WWEvent_Skull2				( struct event_args_s * args );
	void WWEvent_WhiteRay1			( struct event_args_s * args );
	void WWEvent_WhiteRay2			( struct event_args_s * args );
	void WWEvent_ForceBlast			( struct event_args_s * args );
	void WWEvent_PebbleBlast		( struct event_args_s * args );
	void WWEvent_RollingStone1		( struct event_args_s * args );
	void WWEvent_RollingStone2		( struct event_args_s * args );
	void WWEvent_Bird1				( struct event_args_s * args );
	void WWEvent_Bird2				( struct event_args_s * args );
	void WWEvent_BearBite			( struct event_args_s * args );
	void WWEvent_DragonBreath1		( struct event_args_s * args );
	void WWEvent_DragonBreath2		( struct event_args_s * args );
	void WWEvent_Wyvern1			( struct event_args_s * args );
	void WWEvent_Wyvern2			( struct event_args_s * args );
	void WWEvent_IcePoke1			( struct event_args_s * args );
	void WWEvent_IcePoke2			( struct event_args_s * args );
	void WWEvent_FreezeRay1			( struct event_args_s * args );
	void WWEvent_FreezeRay2			( struct event_args_s * args );
	void WWEvent_BeanStalk			( struct event_args_s * args );
	void WWEvent_ThornBlast			( struct event_args_s * args );

	// satchel events
	void WWEvent_WindSatchel		( struct event_args_s * args );
	void WWEvent_LightningSatchel	( struct event_args_s * args );
	void WWEvent_FireSatchel		( struct event_args_s * args );
	void WWEvent_DeathSatchel		( struct event_args_s * args );
	void WWEvent_LifeSatchel		( struct event_args_s * args );
	void WWEvent_EarthSatchel		( struct event_args_s * args );
	void WWEvent_DragonSatchel		( struct event_args_s * args );
	void WWEvent_IceSatchel			( struct event_args_s * args );
	void WWEvent_NatureSatchel		( struct event_args_s * args );
	void WWEvent_CockleBur			( struct event_args_s * args );

	// seal events
	void WWEvent_WindSeal			( struct event_args_s * args );
	void WWEvent_LightningSeal		( struct event_args_s * args );
	void WWEvent_FireSeal			( struct event_args_s * args );
	void WWEvent_DeathSeal			( struct event_args_s * args );
	void WWEvent_LifeSeal			( struct event_args_s * args );
	void WWEvent_EarthSeal			( struct event_args_s * args );
	void WWEvent_DragonSeal			( struct event_args_s * args );
	void WWEvent_IceSeal			( struct event_args_s * args );
	void WWEvent_NatureSeal			( struct event_args_s * args );

	// Dragon
	void WWEvent_DragonFlap			( struct event_args_s * args );
	void WWEvent_DragonFire			( struct event_args_s * args );
}

	
//------------------------------------------------------------------------------
// Event Hooks
//------------------------------------------------------------------------------
void WWHookEvents( void )
{
	// general events
	gEngfuncs.pfnHookEvent( "events/taunt.sc",					WWEvent_Taunt				);
	gEngfuncs.pfnHookEvent( "events/icegibs.sc",				WWEvent_IceGibs				);
	gEngfuncs.pfnHookEvent( "events/burn.sc",					WWEvent_Burn				);
	gEngfuncs.pfnHookEvent( "events/spark.sc",					WWEvent_Spark				);
	gEngfuncs.pfnHookEvent( "events/poison.sc",					WWEvent_Poison				);

	gEngfuncs.pfnHookEvent( "events/thornbush.sc",				WWEvent_ThornBush			);
	gEngfuncs.pfnHookEvent( "events/cocklebur.sc",				WWEvent_CockleBur			);
	gEngfuncs.pfnHookEvent( "events/spore.sc",					WWEvent_Spore				);

	// general spell events
	gEngfuncs.pfnHookEvent( "events/spells/staff.sc",			WWEvent_StaffHit			);
	gEngfuncs.pfnHookEvent( "events/spells/shield.sc",			WWEvent_Shield				);
	gEngfuncs.pfnHookEvent( "events/spells/missile1.sc",		WWEvent_Missile1			);
	gEngfuncs.pfnHookEvent( "events/spells/missile2.sc",		WWEvent_Missile2			);
	gEngfuncs.pfnHookEvent( "events/spells/seal.sc",			WWEvent_SealSound			);
	gEngfuncs.pfnHookEvent( "events/spells/updraft1.sc",		WWEvent_Updraft1			);
	gEngfuncs.pfnHookEvent( "events/spells/updraft2.sc",		WWEvent_Updraft2			);
	gEngfuncs.pfnHookEvent( "events/spells/levitate.sc",		WWEvent_Levitate			);
	gEngfuncs.pfnHookEvent( "events/spells/lightningbolt1.sc",	WWEvent_LightningBolt1		);
	gEngfuncs.pfnHookEvent( "events/spells/lightningbolt2.sc",	WWEvent_LightningBolt2		);
	gEngfuncs.pfnHookEvent( "events/spells/spotbolt1.sc",		WWEvent_SpotBolt1			);
	gEngfuncs.pfnHookEvent( "events/spells/spotbolt2.sc",		WWEvent_SpotBolt2			);
	gEngfuncs.pfnHookEvent( "events/spells/flamelick1.sc",		WWEvent_FlameLick1			);
	gEngfuncs.pfnHookEvent( "events/spells/flamelick2.sc",		WWEvent_FlameLick2			);
	gEngfuncs.pfnHookEvent( "events/spells/fireball1.sc",		WWEvent_FireBall1			);
	gEngfuncs.pfnHookEvent( "events/spells/fireball2.sc",		WWEvent_FireBall2			);
	gEngfuncs.pfnHookEvent( "events/spells/deathray1.sc",		WWEvent_DeathRay1			);
	gEngfuncs.pfnHookEvent( "events/spells/deathray2.sc",		WWEvent_DeathRay2			);
	gEngfuncs.pfnHookEvent( "events/spells/skull1.sc",			WWEvent_Skull1				);
	gEngfuncs.pfnHookEvent( "events/spells/skull2.sc",			WWEvent_Skull2				);
	gEngfuncs.pfnHookEvent( "events/spells/whiteray1.sc",		WWEvent_WhiteRay1			);
	gEngfuncs.pfnHookEvent( "events/spells/whiteray2.sc",		WWEvent_WhiteRay2			);
	gEngfuncs.pfnHookEvent( "events/spells/forceblast.sc",		WWEvent_ForceBlast			);
	gEngfuncs.pfnHookEvent( "events/spells/pebbleblast.sc",		WWEvent_PebbleBlast			);
	gEngfuncs.pfnHookEvent( "events/spells/rollingstone1.sc",	WWEvent_RollingStone1		);
	gEngfuncs.pfnHookEvent( "events/spells/rollingstone2.sc",	WWEvent_RollingStone2		);
	gEngfuncs.pfnHookEvent( "events/spells/bird1.sc",			WWEvent_Bird1				);
	gEngfuncs.pfnHookEvent( "events/spells/bird2.sc",			WWEvent_Bird2				);
	gEngfuncs.pfnHookEvent( "events/spells/bearbite.sc",		WWEvent_BearBite			);
	gEngfuncs.pfnHookEvent( "events/spells/dragonbreath1.sc",	WWEvent_DragonBreath1		);
	gEngfuncs.pfnHookEvent( "events/spells/dragonbreath2.sc",	WWEvent_DragonBreath2		);
	gEngfuncs.pfnHookEvent( "events/spells/wyvern1.sc",			WWEvent_Wyvern1				);
	gEngfuncs.pfnHookEvent( "events/spells/wyvern2.sc",			WWEvent_Wyvern2				);
	gEngfuncs.pfnHookEvent( "events/spells/icepoke1.sc",		WWEvent_IcePoke1			);
	gEngfuncs.pfnHookEvent( "events/spells/icepoke2.sc",		WWEvent_IcePoke2			);
	gEngfuncs.pfnHookEvent( "events/spells/freezeray1.sc",		WWEvent_FreezeRay1			);
	gEngfuncs.pfnHookEvent( "events/spells/freezeray2.sc",		WWEvent_FreezeRay2			);
	gEngfuncs.pfnHookEvent( "events/spells/beanstalk.sc",		WWEvent_BeanStalk			);
	gEngfuncs.pfnHookEvent( "events/spells/thornblast.sc",		WWEvent_ThornBlast			);

	// satchel events
	gEngfuncs.pfnHookEvent( "events/satchels/wind.sc",			WWEvent_WindSatchel			);
	gEngfuncs.pfnHookEvent( "events/satchels/lightning.sc",		WWEvent_LightningSatchel	);
	gEngfuncs.pfnHookEvent( "events/satchels/fire.sc",			WWEvent_FireSatchel			);
	gEngfuncs.pfnHookEvent( "events/satchels/death.sc",			WWEvent_DeathSatchel		);
	gEngfuncs.pfnHookEvent( "events/satchels/life.sc",			WWEvent_LifeSatchel			);
	gEngfuncs.pfnHookEvent( "events/satchels/earth.sc",			WWEvent_EarthSatchel		);
	gEngfuncs.pfnHookEvent( "events/satchels/dragon.sc",		WWEvent_DragonSatchel		);
	gEngfuncs.pfnHookEvent( "events/satchels/ice.sc",			WWEvent_IceSatchel			);
	gEngfuncs.pfnHookEvent( "events/satchels/nature.sc",		WWEvent_NatureSatchel		);

	// seal events
	gEngfuncs.pfnHookEvent( "events/seals/wind.sc",				WWEvent_WindSeal			);
	gEngfuncs.pfnHookEvent( "events/seals/lightning.sc",		WWEvent_LightningSeal		);
	gEngfuncs.pfnHookEvent( "events/seals/fire.sc",				WWEvent_FireSeal			);
	gEngfuncs.pfnHookEvent( "events/seals/death.sc",			WWEvent_DeathSeal			);
	gEngfuncs.pfnHookEvent( "events/seals/life.sc",				WWEvent_LifeSeal			);
	gEngfuncs.pfnHookEvent( "events/seals/earth.sc",			WWEvent_EarthSeal			);
	gEngfuncs.pfnHookEvent( "events/seals/dragon.sc",			WWEvent_DragonSeal			);
	gEngfuncs.pfnHookEvent( "events/seals/ice.sc",				WWEvent_IceSeal				);
	gEngfuncs.pfnHookEvent( "events/seals/nature.sc",			WWEvent_NatureSeal			);

	// Monster Dragon
	gEngfuncs.pfnHookEvent( "events/monsters/dragonfire.sc",		WWEvent_DragonFire			);
	gEngfuncs.pfnHookEvent( "events/monsters/dragonflap.sc",		WWEvent_DragonFlap			);
	
}


//------------------------------------------------------------------------------
// Plays a random taunt sound
//------------------------------------------------------------------------------
void WWEvent_Taunt( struct event_args_s * args )
{
	static const char * taunts[] =
	{
		"player/taunt1.wav",
		"player/taunt2.wav",
		"player/taunt3.wav",
		"player/taunt4.wav",
		"player/taunt5.wav",
	};

	if( args->iparam1 )
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_VOICE,
		"player/bear_taunt.wav", 1.5f, ATTN_NORM, 0, 100 );
	else
	{
		int iTaunt = gEngfuncs.pfnRandomLong( 0, 4 );
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_VOICE,
			taunts[iTaunt], 1.0f, ATTN_NORM, 0, 100 );
	}
}


//------------------------------------------------------------------------------
// Spawns Ice Gibs
//------------------------------------------------------------------------------
void WWEvent_IceGibs( event_args_t * args )
{
	int idx = args->entindex;

	int iCount = gEngfuncs.pfnRandomFloat( 4, 12 );
	int iModel = gEngfuncs.pEventAPI->EV_FindModelIndex( "models/icegibs.mdl" );

	gEngfuncs.pEventAPI->EV_PlaySound( -1, args->origin, CHAN_BODY,"debris/bustglass1.wav",
		1.0f, ATTN_NORM, 0, 100 );

	for( int i = 0; i < iCount; i++ )
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
		vOrigin.z += gEngfuncs.pfnRandomFloat( -16, 32 );

		Vector vVelocity;
		vVelocity.x = gEngfuncs.pfnRandomFloat( -300, 300 );
		vVelocity.y = gEngfuncs.pfnRandomFloat( -300, 300 );
		vVelocity.z = gEngfuncs.pfnRandomFloat(  200, 400 );

		float flLife = gEngfuncs.pfnRandomFloat( 8, 12 );

		TEMPENTITY * pTemp = NULL;
		pTemp = gEngfuncs.pEfxAPI->R_TempModel( vOrigin, vVelocity, vAngles, flLife, iModel, TE_BOUNCE_NULL );

		if( pTemp != NULL )
		{
			pTemp->entity.curstate.body = gEngfuncs.pfnRandomLong( 0, 4 );

			pTemp->flags |= FTENT_HITSOUND;
			pTemp->flags |= FTENT_GRAVITY;
			pTemp->flags |= FTENT_ROTATE;
			pTemp->flags |= FTENT_COLLIDEALL;
			pTemp->flags |= FTENT_FADEOUT;
			pTemp->hitSound = BOUNCE_GLASS;
		}
	}
}


//------------------------------------------------------------------------------
// Burn Effect Callback
//------------------------------------------------------------------------------
void BurnThink( TEMPENTITY * pEnt, float flFrameTime, float flTime )
{
	if( pEnt->die - flTime < 0.0f &&
		!( pEnt->flags & FTENT_FLICKER ) &&
		pEnt->entity.curstate.iuser4 > 0 )
	{
		pEnt->flags |= FTENT_FLICKER;

		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/firez.spr" );
		float flSize = gEngfuncs.pfnRandomFloat( 0.4f, 0.6f );

		Vector vOffset;
		vOffset.x = gEngfuncs.pfnRandomFloat( -1.0f, 1.0f );
		vOffset.y = gEngfuncs.pfnRandomFloat( -1.0f, 1.0f );
		vOffset.z = 0.0f;
		vOffset = vOffset.Normalize() * gEngfuncs.pfnRandomFloat( 8.0f, 16.0f );
		vOffset.z = gEngfuncs.pfnRandomFloat( -32.0f, 32.0f );

		TEMPENTITY * pTemp = NULL;
		pTemp = gEngfuncs.pEfxAPI->R_TempSprite( pEnt->entity.origin, vOffset, flSize, iSprite,
			kRenderTransAdd, kRenderFxNone, 1, 0.8f, 0 );

		if( pTemp != NULL )
		{
			pTemp->flags |= FTENT_SPRCYCLE;
			pTemp->flags |= FTENT_FADEOUT;
			pTemp->flags |= FTENT_PLYRATTACHMENT;
			pTemp->flags |= FTENT_CLIENTCUSTOM;
			pTemp->flags |= FTENT_FLICKER;
			pTemp->clientIndex	= pEnt->clientIndex;
			pTemp->tentOffset	= vOffset;
			pTemp->fadeSpeed	= 4;
			pTemp->callback		= BurnThink;
			pTemp->entity.curstate.iuser4 = pEnt->entity.curstate.iuser4 - 1;
		}
	}
}


//------------------------------------------------------------------------------
// Burn Event
//------------------------------------------------------------------------------
void WWEvent_Burn( event_args_t * args )
{
	int idx = args->entindex;

	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/firez.spr" );

	for( int i = 0; i < 15; i++ )
	{
		float flSize = gEngfuncs.pfnRandomFloat( 0.2f, 0.5f );

		Vector vOffset;
		vOffset.x = gEngfuncs.pfnRandomFloat( -1.0f, 1.0f );
		vOffset.y = gEngfuncs.pfnRandomFloat( -1.0f, 1.0f );
		vOffset.z = 0.0f;
		vOffset = vOffset.Normalize() * gEngfuncs.pfnRandomFloat( 8.0f, 16.0f );
		vOffset.z = gEngfuncs.pfnRandomFloat( -32.0f, 32.0f );

		TEMPENTITY * pTemp = NULL;
		pTemp = gEngfuncs.pEfxAPI->R_TempSprite( vOffset, vOffset, flSize, iSprite,
			kRenderTransAdd, kRenderFxNone, 1, 0.8f, 0 );

		if( pTemp != NULL )
		{
			pTemp->flags |= FTENT_SPRCYCLE;
			pTemp->flags |= FTENT_FADEOUT;
			pTemp->flags |= FTENT_PLYRATTACHMENT;
			pTemp->flags |= FTENT_CLIENTCUSTOM;
			pTemp->clientIndex	= idx;
			pTemp->tentOffset	= vOffset;
			pTemp->fadeSpeed	= 4;
			pTemp->callback		= BurnThink;
			pTemp->entity.curstate.iuser4 = 3;
		}
	}
}

//------------------------------------------------------------------------------
// Poison Effect Callback
//------------------------------------------------------------------------------
void PoisonThink( TEMPENTITY * pEnt, float flFrameTime, float flTime )
{
	if( pEnt->die - flTime < 0.0f &&
		!( pEnt->flags & FTENT_FLICKER ) &&
		pEnt->entity.curstate.iuser4 > 0 )
	{
		pEnt->flags |= FTENT_FLICKER;

		int red, green, blue = 0;
		if(pEnt->entity.curstate.iuser3)
		{
			red = 200;
			green = 200;
			blue = 128;
		}
		else
		{
			green = 125;
		}

		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/poison.spr" );
		float flSize = gEngfuncs.pfnRandomFloat( 0.2f, 0.4f );

		Vector vOffset;
		vOffset.x = gEngfuncs.pfnRandomFloat( -1.0f, 1.0f );
		vOffset.y = gEngfuncs.pfnRandomFloat( -1.0f, 1.0f );
		vOffset.z = 0.0f;
		vOffset = vOffset.Normalize() * gEngfuncs.pfnRandomFloat( 8.0f, 16.0f );
		vOffset.z = gEngfuncs.pfnRandomFloat( -32.0f, 32.0f );

		TEMPENTITY * pTemp = NULL;
		pTemp = gEngfuncs.pEfxAPI->R_TempSprite( pEnt->entity.origin, vOffset, flSize, iSprite,
			kRenderTransAdd, kRenderFxNone, 1, 0.8f, 0 );

		if( pTemp != NULL )
		{
			pTemp->flags |= FTENT_SPRCYCLE;
			pTemp->flags |= FTENT_FADEOUT;
			pTemp->flags |= FTENT_PLYRATTACHMENT;
			pTemp->flags |= FTENT_CLIENTCUSTOM;
			pTemp->flags |= FTENT_FLICKER;
			pTemp->clientIndex	= pEnt->clientIndex;
			pTemp->tentOffset	= vOffset;
			pTemp->fadeSpeed	= 4;
			pTemp->callback		= PoisonThink;
			pTemp->entity.curstate.iuser4 = pEnt->entity.curstate.iuser4 - 1;
			pTemp->entity.curstate.rendercolor.r = red;
			pTemp->entity.curstate.rendercolor.g = green;
			pTemp->entity.curstate.rendercolor.b = blue;
		}
	}
}


//------------------------------------------------------------------------------
// Poison Event
//------------------------------------------------------------------------------
void WWEvent_Poison( event_args_t * args )
{
	int idx = args->entindex;
	
	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/poison.spr" );
	int red, green, blue = 0;
	if(args->bparam1)
	{
		red = 200;
		green = 200;
		blue = 128;
	}
	else
	{
		green = 125;
	}

	for( int i = 0; i < 15; i++ )
	{
		float flSize = gEngfuncs.pfnRandomFloat( 0.2f, 0.4f );

		Vector vOffset;
		vOffset.x = gEngfuncs.pfnRandomFloat( -1.0f, 1.0f );
		vOffset.y = gEngfuncs.pfnRandomFloat( -1.0f, 1.0f );
		vOffset.z = 0.0f;
		vOffset = vOffset.Normalize() * gEngfuncs.pfnRandomFloat( 8.0f, 16.0f );
		vOffset.z = gEngfuncs.pfnRandomFloat( -32.0f, 32.0f );

		TEMPENTITY * pTemp = NULL;
		pTemp = gEngfuncs.pEfxAPI->R_TempSprite( vOffset, vOffset, flSize, iSprite,
			kRenderTransAdd, kRenderFxNone, 1, 0.8f, 0 );

		if( pTemp != NULL )
		{
			pTemp->flags |= FTENT_SPRCYCLE;
			pTemp->flags |= FTENT_FADEOUT;
			pTemp->flags |= FTENT_PLYRATTACHMENT;
			pTemp->flags |= FTENT_CLIENTCUSTOM;
			pTemp->clientIndex	= idx;
			pTemp->tentOffset	= vOffset;
			pTemp->fadeSpeed	= 4;
			pTemp->callback		= PoisonThink;
			pTemp->entity.curstate.iuser3 = args->bparam1;
			pTemp->entity.curstate.iuser4 = 3;
			pTemp->entity.curstate.rendercolor.r = red;
			pTemp->entity.curstate.rendercolor.g = green;
			pTemp->entity.curstate.rendercolor.b = blue;
		}
	}
}

//------------------------------------------------------------------------------
// Spark Think
//------------------------------------------------------------------------------
void SparkThink( TEMPENTITY * pEnt, float flFrameTime, float flTime )
{
	if( pEnt->entity.baseline.fuser1 > flTime )
		return;

	Vector vOrigin;
	Vector vSrc, vDst;

	if( EV_IsLocal( pEnt->clientIndex ) )
		HUD_GetLastOrg( vOrigin );
	else
		vOrigin = pEnt->entity.origin;

	vSrc = vDst = Vector( 0, 0, 0 );

	vSrc.x += gEngfuncs.pfnRandomFloat( -40, 40 );
	vSrc.y += gEngfuncs.pfnRandomFloat( -40, 40 );
	vSrc.z += gEngfuncs.pfnRandomFloat( -40, 40 );
	vSrc = vSrc.Normalize() * 32.0f;

	vDst = vSrc * -1.0f;

	vSrc = vSrc + vOrigin;
	vDst = vDst + vOrigin;

	if( EV_IsLocal( pEnt->clientIndex ) )
	{
		vSrc.z -= 20.0f;
		vDst.z -= 20.0f;
	}
	
	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/smoke.spr" );

	gEngfuncs.pEfxAPI->R_BeamPoints( vSrc, vDst, iSprite, 0.2f, 0.5f, 1, 1, 0, 0, 0, 1, 1, 0 );
	pEnt->entity.baseline.fuser1 = flTime + gEngfuncs.pfnRandomFloat( 0.0f, 0.2f );
}


//------------------------------------------------------------------------------
// Spark Event
//------------------------------------------------------------------------------
void WWEvent_Spark( event_args_t * args )
{
	int idx = args->entindex;

	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/smoke.spr" );

	for( int i = 0; i < 3; i++ )
	{
		Vector vZero = Vector( 0, 0, 0 );
		float flLife = gEngfuncs.pfnRandomFloat( 1.0f, 2.0f );

		TEMPENTITY * pTemp = NULL;
		pTemp = gEngfuncs.pEfxAPI->R_TempSprite( vZero, vZero, 1, iSprite, 0, 0, 0, flLife, 0 );

		if( pTemp != NULL )
		{
			pTemp->flags |= FTENT_PLYRATTACHMENT;
			pTemp->flags |= FTENT_CLIENTCUSTOM;
			pTemp->flags |= FTENT_NOMODEL;
			pTemp->clientIndex	= idx;
			pTemp->callback		= SparkThink;
		}
	}
}
