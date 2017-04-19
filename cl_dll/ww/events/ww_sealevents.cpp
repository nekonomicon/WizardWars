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
#include "screenfade.h"
#include "shake.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"

#include "ww/ww_particle.h"
#include "ww/ww_particlemanger.h"


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
void TempEntityRegister( cl_entity_t * pEntity );


//------------------------------------------------------------------------------
// C Function Prototypes
//------------------------------------------------------------------------------
extern "C"
{
	void WWEvent_WindSeal		( struct event_args_s * args );
	void WWEvent_LightningSeal	( struct event_args_s * args );
	void WWEvent_FireSeal		( struct event_args_s * args );
	void WWEvent_DeathSeal		( struct event_args_s * args );
	void WWEvent_LifeSeal		( struct event_args_s * args );
	void WWEvent_EarthSeal		( struct event_args_s * args );
	void WWEvent_DragonSeal		( struct event_args_s * args );
	void WWEvent_IceSeal		( struct event_args_s * args );
	void WWEvent_NatureSeal		( struct event_args_s * args );
}


//------------------------------------------------------------------------------
// Wind Seal Event
//------------------------------------------------------------------------------
void WWEvent_WindSeal( event_args_t * args )
{
	switch( args->iparam1 )
	{
	// host usage report
	case 0:
		break;

	// friendly trigger
	case 1:
		if( EV_IsLocal( args->iparam2 ) )
			gHUD.m_Message.MessageAdd( TextMessageGet( "WindSeal" ) );
		break;

	// enemy trigger
	case 2:
		break;
	}

	if( !args->iparam1 )
		return;

	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"seals/wind.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );

	if( effects->value == 0.0f )
		return;

	for( int i = 0; i < 20; i++ )
	{
		WWParticle * pPart = g_pParticleManager->Allocate( PARTICLE_PRIORITY_NONE );

		if( !pPart )
			break;

		pPart->SetSprite( "sprites/flare1.spr" );

		pPart->m_bDraw				= true;
		pPart->m_iFlags				= WW_PFLAG_FEATHER;

		pPart->m_vOrigin			= args->origin;
		pPart->m_vOrigin.x		   += gEngfuncs.pfnRandomFloat( -32, 32 );
		pPart->m_vOrigin.y		   += gEngfuncs.pfnRandomFloat( -32, 32 );
		pPart->m_vVelocity.z		= gEngfuncs.pfnRandomFloat( 100, 300 );
		pPart->m_flLife				= gEngfuncs.pfnRandomFloat( 1.0f, 1.5f );
		pPart->m_flBirth			= gEngfuncs.GetClientTime() + 0.01f * (float)i;
		pPart->m_flScale			= 0.5f;
		pPart->m_iRenderMode		= kRenderTransAdd;
		pPart->m_flRenderColor[0]	= 0.4f;
		pPart->m_flRenderColor[1]	= 0.5f;
		pPart->m_flRenderColor[2]	= 0.6f;
		pPart->m_flRenderColor[3]	= 0.5f;
	}
}


//------------------------------------------------------------------------------
// Lightning Seal Event
//------------------------------------------------------------------------------
void WWEvent_LightningSeal( event_args_t * args )
{
	switch( args->iparam1 )
	{
	// host usage report
	case 0:
		break;

	// friendly trigger
	case 1:
		if( EV_IsLocal( args->iparam2 ) )
			gHUD.m_Message.MessageAdd( TextMessageGet( "LightningSeal" ) );
		break;

	// enemy trigger
	case 2:
		break;
	}

	if( !args->iparam1 )
		return;

	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"seals/lightning.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );

	if( effects->value == 0.0f )
		return;

	Vector vTop = args->origin; vTop.z += 2048;
	Vector vBot = args->origin; vBot.z -= 2048;

	if( !args->bparam2 )
	{
		vTop.x = vBot.x = vBot.x + gEngfuncs.pfnRandomFloat( -32, 32 );
		vTop.y = vBot.y = vBot.y + gEngfuncs.pfnRandomFloat( -32, 32 );
	}

	pmtrace_t tr;
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );

	gEngfuncs.pEventAPI->EV_PlayerTrace( args->origin, vTop, PM_GLASS_IGNORE, -1, &tr );
	vTop = tr.endpos;

	gEngfuncs.pEventAPI->EV_PlayerTrace( args->origin, vBot, PM_GLASS_IGNORE, -1, &tr );
	vBot = tr.endpos;

	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/smoke.spr" );

	vTop.z += 512.0f;
	gEngfuncs.pEfxAPI->R_BeamPoints( vTop, vBot, iSprite, 0.1f, 2.0f, 1.0f, 2.0f, 0.0f, 0, 0, 0.8, 0.6, 0.2f );
}


//------------------------------------------------------------------------------
// Fire Seal Event
//------------------------------------------------------------------------------
void WWEvent_FireSeal( event_args_t * args )
{
	switch( args->iparam1 )
	{
	// host usage report
	case 0:
		break;

	// friendly trigger
	case 1:
		if( EV_IsLocal( args->iparam2 ) )
			gHUD.m_Message.MessageAdd( TextMessageGet( "FireSeal" ) );
		break;

	// enemy trigger
	case 2:
		break;
	}

	if( !args->iparam1 )
		return;

	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"seals/fire.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );

	if( effects->value == 0.0f )
		return;

	for( int i = 0; i < 20; i++ )
	{
		WWParticle * pPart = g_pParticleManager->Allocate( PARTICLE_PRIORITY_NONE );

		if( !pPart )
			break;

		pPart->SetSprite( "sprites/fire.spr" );

		pPart->m_bDraw				= true;
		pPart->m_vOrigin			= args->origin;
		pPart->m_vVelocity.x		= gEngfuncs.pfnRandomFloat( -64, 64 );
		pPart->m_vVelocity.y		= gEngfuncs.pfnRandomFloat( -64, 64 );
		pPart->m_flLife				= gEngfuncs.pfnRandomFloat( 1.0f, 2.0f );
		pPart->m_flBirth			= gEngfuncs.GetClientTime() + 0.01f * (float)i;
		pPart->m_flScale			= 1.0f;
		pPart->m_flScaleDecay		= -0.1f;
		pPart->m_flSpeedDecay		= 0.1f;
		pPart->m_iRenderMode		= kRenderTransAdd;
		pPart->m_flRenderColor[0]	= 1.0f;
		pPart->m_flRenderColor[1]	= 0.8f;
		pPart->m_flRenderColor[2]	= 0.5f;
		pPart->m_flRenderColor[3]	= 1.0f;
		pPart->m_flRenderDecay		= 1.0f;
	}
}

//------------------------------------------------------------------------------
// Death Seal Event
//------------------------------------------------------------------------------
void WWEvent_DeathSeal( event_args_t * args )
{
	switch( args->iparam1 )
	{
	// host usage report
	case 0:
		break;

	// friendly trigger
	case 1:
		if( EV_IsLocal( args->iparam2 ) )
			gHUD.m_Message.MessageAdd( TextMessageGet( "DeathSeal" ) );
		break;

	// enemy trigger
	case 2:
		break;
	}

	if( !args->iparam1 )
		return;

	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"seals/death.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );

	if( effects->value == 0.0f )
		return;

	for( int i = 0; i < 10; i++ )
	{
		WWParticle * pPart = g_pParticleManager->Allocate( PARTICLE_PRIORITY_NONE );

		if( !pPart )
			break;

		pPart->SetSprite( "sprites/flare1.spr" );

		pPart->m_bDraw				= true;
		pPart->m_vOrigin			= args->origin;
		pPart->m_vVelocity.x		= gEngfuncs.pfnRandomFloat( -55, 55 );
		pPart->m_vVelocity.y		= gEngfuncs.pfnRandomFloat( -55, 55 );
		pPart->m_vVelocity.z		= gEngfuncs.pfnRandomFloat(  25, 55 );
		pPart->m_flLife				= gEngfuncs.pfnRandomFloat( 1.0f, 2.0f );
		pPart->m_flBirth			= gEngfuncs.GetClientTime() + 0.01f * (float)i;
		pPart->m_flScale			= 1.0f;
		pPart->m_flScaleDecay		= -0.1f;
		pPart->m_flSpeedDecay		= 0.1f;
		pPart->m_iRenderMode		= kRenderTransAdd;
		pPart->m_flRenderColor[0]	= gEngfuncs.pfnRandomFloat( 0.2f, 0.4f );
		pPart->m_flRenderColor[1]	= gEngfuncs.pfnRandomFloat( 0.8f, 1.0f );
		pPart->m_flRenderColor[2]	= gEngfuncs.pfnRandomFloat( 0.0f, 0.2f );
		pPart->m_flRenderColor[3]	= 0.5f;
		pPart->m_flRenderDecay		= 1.0f;
	}
}


//------------------------------------------------------------------------------
// Life Seal Event
//------------------------------------------------------------------------------
void WWEvent_LifeSeal( event_args_t * args )
{
	switch( args->iparam1 )
	{
	// host usage report
	case 0:
		break;

	// friendly trigger
	case 1:
		if( EV_IsLocal( args->iparam2 ) )
			gHUD.m_Message.MessageAdd( TextMessageGet( "LifeSeal" ) );
		break;

	// enemy trigger
	case 2:
		if( EV_IsLocal( args->iparam2 ) )
		{
			screenfade_t sf;
			sf.fader		= 255;
			sf.fadeg		= 255;
			sf.fadeb		= 255;
			sf.fadealpha	= 255;
			sf.fadeEnd		= gEngfuncs.GetClientTime() + 4.0f;
			sf.fadeTotalEnd	= 4.0f;
			sf.fadeReset	= gEngfuncs.GetClientTime() + 2.0f;
			sf.fadeSpeed	= 127.5f;
			sf.fadeFlags	= FFADE_IN;
			gEngfuncs.pfnSetScreenFade( &sf );
		}
		break;
	}

	if( !args->iparam1 )
		return;

	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"seals/life.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );
}


//------------------------------------------------------------------------------
// Earth Seal Event
//------------------------------------------------------------------------------
void WWEvent_EarthSeal( event_args_t * args )
{
	switch( args->iparam1 )
	{
	// host usage report
	case 0:
		break;

	// friendly trigger
	case 1:
		if( EV_IsLocal( args->iparam2 ) )
			gHUD.m_Message.MessageAdd( TextMessageGet( "EarthSeal" ) );
		break;

	// enemy trigger
	case 2:
		break;
	}

	if( !args->iparam1 )
		return;

	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"seals/earth.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );

	float flRadius = 200.0f;

	Vector vOrigin = args->origin;

	TEMPENTITY * pTemp = NULL;
	Vector vVelocity = Vector( 0, 0, 0 );
	int ent1 = 0, ent2 = 0;

	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/shockwave.spr" );

	vVelocity.x = vVelocity.y = -flRadius * 2.0f;
	pTemp = gEngfuncs.pEfxAPI->R_TempModel( vOrigin, vVelocity, Vector( 0, 0, 0 ), 0.25f, iSprite, 0 );

	if( pTemp != NULL )
	{
		TempEntityRegister( &pTemp->entity );

		ent1 = pTemp->entity.index;

		pTemp->flags						= FTENT_PERSIST | FTENT_NOMODEL;
		pTemp->entity.curstate.rendermode	= kRenderTransAdd;
		pTemp->entity.curstate.renderamt	= 0;
	}

	vVelocity.x = vVelocity.y = flRadius * 2.0f;
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
	// startFrame, framerate, r, g, b`
	gEngfuncs.pEfxAPI->R_BeamRing( ent1, ent2, iSprite, 0.2f, 20.0f, 0.0f, 0.5f, 1.0f, 0, 0, 1.0f, 0.6f, 0.2f );
}


//------------------------------------------------------------------------------
// Dragon Seal Event
//------------------------------------------------------------------------------
void WWEvent_DragonSeal( event_args_t * args )
{
	switch( args->iparam1 )
	{
	// host usage report
	case 0:
		break;

	// friendly trigger
	case 1:
		if( EV_IsLocal( args->iparam2 ) )
			gHUD.m_Message.MessageAdd( TextMessageGet( "DragonSeal" ) );
		break;

	// enemy trigger
	case 2:
		break;
	}

	if( !args->iparam1 )
		return;

	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"seals/dragon.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );

	float flRadius = 300.0f;

	Vector vOrigin = args->origin;

	TEMPENTITY * pTemp = NULL;
	Vector vVelocity = Vector( 0, 0, 0 );
	int ent1 = 0, ent2 = 0;

	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/shockwave.spr" );

	vVelocity.x = vVelocity.y = -flRadius * 2.0f;
	pTemp = gEngfuncs.pEfxAPI->R_TempModel( vOrigin, vVelocity, Vector( 0, 0, 0 ), 0.25f, iSprite, 0 );

	if( pTemp != NULL )
	{
		TempEntityRegister( &pTemp->entity );

		ent1 = pTemp->entity.index;

		pTemp->flags						= FTENT_PERSIST | FTENT_NOMODEL;
		pTemp->entity.curstate.rendermode	= kRenderTransAdd;
		pTemp->entity.curstate.renderamt	= 0;
	}

	vVelocity.x = vVelocity.y = flRadius * 2.0f;
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
	gEngfuncs.pEfxAPI->R_BeamRing( ent1, ent2, iSprite, 0.2f, 40.0f, 0.0f, 0.5f, 1.0f, 0, 0, 0.2f, 0.0f, 0.8f );
}


//------------------------------------------------------------------------------
// Ice Seal Event
//------------------------------------------------------------------------------
void WWEvent_IceSeal( event_args_t * args )
{
	switch( args->iparam1 )
	{
	// host usage report
	case 0:
		break;

	// friendly trigger
	case 1:
		if( EV_IsLocal( args->iparam2 ) )
			gHUD.m_Message.MessageAdd( TextMessageGet( "IceSeal" ) );
		break;

	// enemy trigger
	case 2:
		break;
	}

	if( !args->iparam1 )
		return;

	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"seals/ice.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );

	if( effects->value == 0.0f )
		return;

	for( int i = 0; i < 20; i++ )
	{
		WWParticle * pPart = g_pParticleManager->Allocate( PARTICLE_PRIORITY_NONE );

		if( !pPart )
			break;

		pPart->SetSprite( "sprites/flare1.spr" );

		pPart->m_bDraw				= true;
		pPart->m_iFlags				= WW_PFLAG_FEATHER;
		pPart->m_vOrigin			= args->origin;
		pPart->m_vVelocity.z		= gEngfuncs.pfnRandomFloat( 300, 600 );
		pPart->m_flLife				= gEngfuncs.pfnRandomFloat( 0.5f, 1.5f );
		pPart->m_flScale			= 0.1f;
		pPart->m_flScaleDecay		= -1.0f;
		pPart->m_flGravity			= 600.0f;
		pPart->m_iRenderMode		= kRenderTransAdd;
		pPart->m_flRenderColor[0]	= 0.3f;
		pPart->m_flRenderColor[1]	= 0.4f;
		pPart->m_flRenderColor[2]	= 0.6f;
		pPart->m_flRenderColor[3]	= 0.5f;
	}
}


//------------------------------------------------------------------------------
// Nature Vines Think
//------------------------------------------------------------------------------
void VinesThink( TEMPENTITY * pEnt, float flFrametime, float flTime )
{
	pEnt->entity.curstate.frame += 255.0f * flFrametime;

	if( pEnt->entity.curstate.frame > 255.0f )
	{
		pEnt->entity.curstate.sequence++;
		pEnt->entity.curstate.frame= 0;
	}
}


//------------------------------------------------------------------------------
// Nature Seal Event
//------------------------------------------------------------------------------
void WWEvent_NatureSeal( event_args_t * args )
{
	switch( args->iparam1 )
	{
	// host usage report
	case 0:
		break;

	// friendly trigger
	case 1:
		if( EV_IsLocal( args->iparam2 ) )
			gHUD.m_Message.MessageAdd( TextMessageGet( "NatureSeal" ) );
		break;

	// enemy trigger
	case 2:
		break;
	}

	if( !args->iparam1 )
		return;

	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"seals/nature.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );

	int iModel = gEngfuncs.pEventAPI->EV_FindModelIndex( "models/thornbush1.mdl" );

	Vector vOrigin = args->origin;
	Vector vAngles = args->angles;

	TEMPENTITY * pTemp = NULL;
	pTemp = gEngfuncs.pEfxAPI->R_TempModel( vOrigin, Vector( 0, 0, 0 ), vAngles, 2.0f, iModel, 0 );

	if( pTemp != NULL )
	{
		pTemp->flags &= ~FTENT_GRAVITY;
		pTemp->flags |= FTENT_PERSIST;
		pTemp->flags |= FTENT_CLIENTCUSTOM;
		pTemp->callback = VinesThink;
	}
}


