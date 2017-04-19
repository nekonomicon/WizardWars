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
void CreateFlameLick( Vector vOrigin, Vector vAngles, float flLife );
void VectorAngles( const float *forward, float *angles );


//------------------------------------------------------------------------------
// C Function Prototypes
//------------------------------------------------------------------------------
extern "C"
{
	void WWEvent_WindSatchel		( struct event_args_s * args );
	void WWEvent_LightningSatchel	( struct event_args_s * args );
	void WWEvent_FireSatchel		( struct event_args_s * args );
	void WWEvent_DeathSatchel		( struct event_args_s * args );
	void WWEvent_LifeSatchel		( struct event_args_s * args );
	void WWEvent_EarthSatchel		( struct event_args_s * args );
	void WWEvent_DragonSatchel		( struct event_args_s * args );
	void WWEvent_IceSatchel			( struct event_args_s * args );
	void WWEvent_NatureSatchel		( struct event_args_s * args );
}


//------------------------------------------------------------------------------
// Wind Particle Think
//------------------------------------------------------------------------------
int PARTICLEAPI WindSatchelThink( WWParticle * pPart, const float & flFrametime )
{
	pdata_t * pData = pPart->m_pPrivate;

	float r = gEngfuncs.GetClientTime() * 10.0f + pData->i1;
	float s = sinf( r ) * 200.0f;
	float c = cosf( r ) * 200.0f;

	pPart->m_vOrigin = pPart->m_vOrigin + pData->v1 * 200.0f * flFrametime;
	pPart->m_vOrigin = pPart->m_vOrigin + pData->v2 * s * flFrametime;
	pPart->m_vOrigin = pPart->m_vOrigin + pData->v3 * c * flFrametime;
	pPart->m_flRenderColor[3] -= 0.2f * flFrametime;
	pPart->m_flScale += 2.0f * flFrametime;

	return 1;
}


//------------------------------------------------------------------------------
// Wind Satchel Event
//------------------------------------------------------------------------------
void WWEvent_WindSatchel( event_args_t * args )
{
	switch( args->iparam1 )
	{
	// spawn
	case 0:
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
			"ambience/rwind1.wav", 1.0f, ATTN_NORM, 0, 100 );
		break;

	// explode
	case 1:
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
			"satchels/wind.wav", 1.0f, ATTN_NORM, 0, 100 );
		break;
	}

	if( args->iparam1 )
	{
		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/xfire2.spr" ); 

		// create temporary entity
		TEMPENTITY * pTemp = NULL;
		pTemp = gEngfuncs.pEfxAPI->R_TempSprite( args->origin, Vector( 0, 0, 0 ), 2.0f, iSprite,
			kRenderGlow, kRenderFxNoDissipation, 1, 3.0f, FTENT_SPRANIMATE );

		if( pTemp )
			pTemp->entity.curstate.framerate = 20;

		return;
	}

	// particle effect
	if( effects->value != 0.0f )
	{
		// create a few particles for a twirling wind effect
		for( int i = 0; i < 20; i++ )
		{
			WWParticle * pPart = g_pParticleManager->Allocate( PARTICLE_PRIORITY_NONE );

			if( !pPart )
				break;

			pdata_t * pData = (pdata_t *)malloc( sizeof(pdata_t) );
			memset( pData, 0, sizeof(pdata_t) );

			pData->i1 = i;
			pData->v1 = Vector( 0, 0, 1 );
			pData->v2 = Vector( 0, 1, 0 );
			pData->v3 = Vector( 1, 0, 0 );

			pPart->SetSprite( "sprites/flare1.spr" );
			pPart->SetPrivate( pData, true );
			pPart->SetThink( WindSatchelThink );

			pPart->m_bDraw				= true;
			pPart->m_iFlags				= 0;
			pPart->m_vOrigin			= args->origin;
			pPart->m_flLife				= 2.0f;
			pPart->m_flBirth			= gEngfuncs.GetClientTime() + 0.1f * (float)i;
			pPart->m_flScale			= 0.5f;
			pPart->m_iRenderMode		= kRenderTransAdd;
			pPart->m_flRenderColor[0]	= 0.4f;
			pPart->m_flRenderColor[1]	= 0.5f;
			pPart->m_flRenderColor[2]	= 0.6f;
			pPart->m_flRenderColor[3]	= 0.5f;
		}
	}

	// retro effect
	else
	{
		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/effects/updraft.spr" ); 

		// create temporary entity
		TEMPENTITY * pTemp = NULL;
		pTemp = gEngfuncs.pEfxAPI->R_TempSprite( args->origin, Vector( 0, 0, 64 ), 0.5f, iSprite,
			kRenderGlow, kRenderFxNoDissipation, 1, 3.0f, FTENT_SPRANIMATE );

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
// Lightning Satchel Event
//------------------------------------------------------------------------------
void WWEvent_LightningSatchel( event_args_t * args )
{
	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"satchels/lightning.wav", 1.0f, ATTN_NORM, 0, 100 );

	Vector vTop = args->origin; vTop.z += 2048;
	Vector vBot = args->origin; vBot.z -= 2048;

	pmtrace_t tr;
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );

	gEngfuncs.pEventAPI->EV_PlayerTrace( args->origin, vTop, PM_GLASS_IGNORE, -1, &tr );
	vTop = tr.endpos;

	gEngfuncs.pEventAPI->EV_PlayerTrace( args->origin, vBot, PM_GLASS_IGNORE, -1, &tr );
	vBot = tr.endpos;

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
}


//------------------------------------------------------------------------------
// Fire Satchel Event
//------------------------------------------------------------------------------
void WWEvent_FireSatchel( event_args_t * args )
{
	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"satchels/fire.wav", 1.0f, ATTN_NORM, 0, 100 );

	CreateFlameLick( args->origin, Vector( -90, 0, 0 ), 2.0f );
}


//------------------------------------------------------------------------------
// Death Satchel Event
//------------------------------------------------------------------------------
void WWEvent_DeathSatchel( event_args_t * args )
{
	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"satchels/death.wav", 1.0f, ATTN_NORM, 0, 100 );

	// particle effect
	if( effects->value != 0.0f )
	{
		for( int i = 0; i < 10; i++ )
		{
			WWParticle * pPart = g_pParticleManager->Allocate( PARTICLE_PRIORITY_NONE );

			if( !pPart )
				break;

			pPart->SetSprite( "sprites/flare1.spr" );
			pPart->m_bDraw = true;
			pPart->m_vOrigin = args->origin;
			pPart->m_vVelocity.x = gEngfuncs.pfnRandomFloat( -128, 128 );
			pPart->m_vVelocity.y = gEngfuncs.pfnRandomFloat( -128, 128 );
			pPart->m_vVelocity.z = gEngfuncs.pfnRandomFloat(   64, 128 );
			pPart->m_flRenderColor[0] = gEngfuncs.pfnRandomFloat( 0.2f, 0.4f );
			pPart->m_flRenderColor[1] = gEngfuncs.pfnRandomFloat( 0.8f, 1.0f );
			pPart->m_flRenderColor[2] = gEngfuncs.pfnRandomFloat( 0.0f, 0.2f );
			pPart->m_flRenderColor[3] = 0.1f;
			pPart->m_flRenderDecay = 0.1f;
			pPart->m_iRenderMode = kRenderTransAdd;
			pPart->m_flBirth = gEngfuncs.GetClientTime() + 0.01f;
			pPart->m_flLife  = 0.5f;
			pPart->m_flScale = 0.5f;
			pPart->m_flScaleDecay = -1.0f;
		}
	}
	else
	{
		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/stmbal1.spr" ); 

		// create temporary entity
		TEMPENTITY * pTemp = NULL;
		pTemp = gEngfuncs.pEfxAPI->R_TempSprite( args->origin, Vector( 0, 0, 64 ), 1.5f, iSprite,
			kRenderGlow, kRenderFxNoDissipation, 1, 3.0f, FTENT_SPRANIMATE );

		if( pTemp )
		{
			pTemp->entity.curstate.framerate     = 20;
			pTemp->entity.curstate.rendercolor.r = 128;
			pTemp->entity.curstate.rendercolor.g = 255;
			pTemp->entity.curstate.rendercolor.b = 64;
		}
	}
}


//------------------------------------------------------------------------------
// Life Satchel Event
//------------------------------------------------------------------------------
void WWEvent_LifeSatchel( event_args_t * args )
{
	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"satchels/life.wav", 1.0f, ATTN_NORM, 0, 100 );

	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/b-tele1.spr" ); 

	// create temporary entity
	TEMPENTITY * pTemp = NULL;
	pTemp = gEngfuncs.pEfxAPI->R_TempSprite( args->origin, Vector( 0, 0, 0 ), 1.5f, iSprite,
		kRenderGlow, kRenderFxNoDissipation, 1, 3.0f, FTENT_SPRANIMATE );

	if( pTemp )
	{
		pTemp->entity.curstate.framerate     = 20;
		pTemp->entity.curstate.rendercolor.r = 250;
		pTemp->entity.curstate.rendercolor.g = 250;
		pTemp->entity.curstate.rendercolor.b = 255;
	}
}


//------------------------------------------------------------------------------
// Earth Satchel Event
//------------------------------------------------------------------------------
void WWEvent_EarthSatchel( event_args_t * args )
{
	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"satchels/earth.wav", 1.0f, ATTN_NORM, 0, 100 );

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
// Dragon Satchel Event
//------------------------------------------------------------------------------
void WWEvent_DragonSatchel( event_args_t * args )
{
	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"satchels/dragon.wav", 1.0f, ATTN_NORM, 0, 100 );

	int iCount = gEngfuncs.pfnRandomLong( 5, 20 );
	int iModel = gEngfuncs.pEventAPI->EV_FindModelIndex( "models/dragontooth.mdl" );
	
	for( int i = 0; i < iCount; i++ )
	{
		Vector vOrigin = args->origin;

		Vector vVelocity;
		vVelocity.x = gEngfuncs.pfnRandomFloat( -1.0f, 1.0f );
		vVelocity.y = gEngfuncs.pfnRandomFloat( -1.0f, 1.0f );
		vVelocity.z = gEngfuncs.pfnRandomFloat( -1.0f, 1.0f );
		vVelocity = vVelocity.Normalize() * 600.0f;
		
		Vector vAngles;
		VectorAngles( vVelocity, vAngles );

		TEMPENTITY * pTemp = NULL;
		pTemp = gEngfuncs.pEfxAPI->R_TempModel( vOrigin, vVelocity, vAngles, 2.0f, iModel, 0 );

		if( pTemp != NULL )
		{
			pTemp->flags &= ~FTENT_GRAVITY;
			pTemp->flags |= FTENT_COLLIDEALL;
			pTemp->flags |= FTENT_COLLIDEKILL;
		}
	}
}


//------------------------------------------------------------------------------
// Ice Satchel Event
//------------------------------------------------------------------------------
void WWEvent_IceSatchel( event_args_t * args )
{
	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"satchels/ice.wav", 1.0f, ATTN_NORM, 0, 100 );

	gEngfuncs.pEfxAPI->R_TeleportSplash( args->origin );
}


//------------------------------------------------------------------------------
// Nature Satchel Event
//------------------------------------------------------------------------------
void WWEvent_NatureSatchel( event_args_t * args )
{
	gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
		"satchels/nature.wav", 1.0f, ATTN_NORM, 0, 100 );
}



