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
// Function Prototypes
//------------------------------------------------------------------------------
void HUD_GetLastOrg( float *org );


//------------------------------------------------------------------------------
// C Function Prototypes
//------------------------------------------------------------------------------
extern "C"
{
	void WWEvent_DragonBreath1	( struct event_args_s * args );
	void WWEvent_DragonBreath2	( struct event_args_s * args );
	void WWEvent_Wyvern1		( struct event_args_s * args );
	void WWEvent_Wyvern2		( struct event_args_s * args );
	void WWEvent_DragonFlap		( struct event_args_s * args );
	void WWEvent_DragonFire		( struct event_args_s * args );
}


//------------------------------------------------------------------------------
// DragonBreath Particle Function
//------------------------------------------------------------------------------
int PARTICLEAPI DragonBreathThink( WWParticle * pPart, const float & flFrametime  )
{
	pdata_t * pData = pPart->m_pPrivate;

	// if the flame is still pretty much blue
	if( pPart->m_flRenderColor[2] >= 0.5f )
	{
		pPart->m_flRenderColor[0] += 2.0f * flFrametime;
		pPart->m_flRenderColor[1] += 1.0f * flFrametime;
		pPart->m_flRenderColor[2] -= 8.0f * flFrametime;
		pPart->m_flRenderColor[3] += 8.0f * flFrametime;
	}

	// if the flame is pretty much red
	else
	{
		if( pPart->m_flRenderColor[0] < 1.0f )
		{
			pPart->m_flRenderColor[0] += 4.0f * flFrametime;
			pPart->m_flRenderColor[1] += 1.5f * flFrametime;
			pPart->m_flRenderColor[2] -= 0.5f * flFrametime;
			pPart->m_flRenderColor[3] -= 0.5f * flFrametime;
		}

		pPart->m_flScale += 2.0f * flFrametime;
	}

	
	cl_entity_t * player = (cl_entity_t *)pData->p1;

	if( !player )
		return pPart->Think( flFrametime );

	Vector vOrigin = player->origin + pData->v1;
	Vector vAngles = player->angles;
	Vector vEndPos;

	Vector vForward, vRight, vUp;
	AngleVectors( vAngles, vForward, vRight, vUp );

	if( EV_IsLocal( player->index ) )
	{
		Vector vAngles = gHUD.m_vecAngles;
		AngleVectors( vAngles, vForward, vRight, vUp );

		HUD_GetLastOrg( vOrigin );
		vOrigin = vOrigin + pData->v1;
	}

	float fPercent = ( gEngfuncs.GetClientTime() - pPart->m_flBirth );


	if( pData->i1 == 0 )
	{
		pData->v2 = Vector( 0, 0, 0 );
		pData->v2 = pData->v2 + vRight * gEngfuncs.pfnRandomFloat( -32.0f, 32.0f );
		pData->v2 = pData->v2 + vUp    * gEngfuncs.pfnRandomFloat( -32.0f, 32.0f );

		pPart->m_bDraw = true;
		pPart->m_vOrigin = vOrigin;

		pData->i1 = 1;
	}

	vEndPos = vOrigin + vForward * DRAGONBREATH_RANGE * fPercent + pData->v2 * fPercent;

	if( fPercent > 0.5f && pData->i2 == 0 )
	{
		Vector vSrc = vOrigin;
		Vector vDst = vOrigin + vForward * DRAGONBREATH_RANGE;
	
		pmtrace_t tr;
		gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
		gEngfuncs.pEventAPI->EV_PlayerTrace( vSrc, vDst, PM_WORLD_ONLY, -1, &tr );

		switch( gEngfuncs.pfnRandomLong( 0, 1 ) )
		{
		case 0: EV_HLDM_DecalTrace( &tr, "{scorch1" ); break;
		case 1: EV_HLDM_DecalTrace( &tr, "{scorch2" ); break;
		}

		pData->i2 = 1;
	}

	pPart->m_vVelocity = ( vEndPos - pPart->m_vOrigin ).Normalize() * ( 100.0f + 300.0f * ( 1.0f - fPercent ) );

	return pPart->Think( flFrametime );
}


//------------------------------------------------------------------------------
// Retro DragonBreath Touch
//------------------------------------------------------------------------------
void DragonBreathTouch( TEMPENTITY * pEntity, pmtrace_t * pTrace )
{
	switch( gEngfuncs.pfnRandomLong( 0, 1 ) )
	{
	case 0: EV_HLDM_DecalTrace( pTrace, "{scorch1" ); break;
	case 1: EV_HLDM_DecalTrace( pTrace, "{scorch2" ); break;
	}

	pEntity->flags		 &= ~FTENT_CLIENTCUSTOM;
	pEntity->hitcallback  = NULL;
}


//------------------------------------------------------------------------------
// DragonBreath Spell
//------------------------------------------------------------------------------
void WWEvent_DragonBreath1( event_args_t * args )
{
	int idx = args->entindex;

	if( args->iparam1 )
	{
		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_DRAGON_DRAGONBREATH_CAST, 0 );
		return;
	}

	Vector vAngles, vOrigin, vCastOrigin;

	if( EV_IsLocal( idx ) )
	{
		vAngles = gHUD.m_vecAngles;
		HUD_GetLastOrg( vOrigin );
	}
	else
	{
		vAngles = args->angles;
		// 1.2.7 #10 Draco Breath to low
		vOrigin = args->origin;
		EV_GetGunPosition( args, vOrigin, args->origin );
	}


	Vector vForward, vRight, vUp;
	AngleVectors( vAngles, vForward, vRight, vUp );


	vCastOrigin = Vector( 0, 0, 0 );
	vCastOrigin = vCastOrigin + vForward * 16.0f;
	vCastOrigin = vCastOrigin - vRight   * 6.0f;

	// 1.2.7 #10 Draco Breath to low
	if( !EV_IsLocal( idx ) )
	{
		vCastOrigin = vCastOrigin + vUp * 16.0f;
	}
	else
	{
		vCastOrigin.z -= 6.0f;
	}

	// play cast sound
	gEngfuncs.pEventAPI->EV_PlaySound( idx, vOrigin, CHAN_WEAPON,
		"spells/dragonbreath_loop.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );

	// particle effect
	if( effects->value != 0.0f )
	{
		cl_entity_t * pPlayer = gEngfuncs.GetEntityByIndex( idx );

		if( !pPlayer )
			return;

		for( int i = 0; i < 10; i++ )
		{
			WWParticle * pPart = g_pParticleManager->Allocate( PARTICLE_PRIORITY_NONE );

			if( !pPart )
				break;

			pdata_t * pData = (pdata_t *)malloc( sizeof(pdata_t) );
			memset( pData, 0, sizeof(pdata_t) );

			pPart->SetSprite( "sprites/stmbal1.spr" );

			pPart->SetPrivate( pData, true );
			pPart->SetThink( DragonBreathThink );

			pPart->m_bDraw				= false;
			pPart->m_vOrigin			= vOrigin + vCastOrigin;
			pPart->m_flLife				= 0.8f;
			pPart->m_flBirth			= gEngfuncs.GetClientTime() + 0.04f * (float)i;
			pPart->m_flScale			= 0.2f;
			pPart->m_flScaleDecay		= -1.0f;
			pPart->m_iFlags				= WW_PFLAG_COLLIDE;
			pPart->m_iRenderMode		= kRenderTransAdd;
			pPart->m_flFrameRate		= 20.0f;
			pPart->m_flRenderColor[0]	= 0.2f;
			pPart->m_flRenderColor[1]	= 0.2f;
			pPart->m_flRenderColor[2]	= 1.0f;
			pPart->m_flRenderColor[3]	= 0.1f;

			pData->p1					= pPlayer;
			pData->f1					= 0.5f;	// travel time is 0.5 seconds
			pData->v1					= vCastOrigin;
			pData->i2					= i; // so only the first makes a decal
		}
	}

	// retro effect
	else
	{
		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/fire.spr" );

		for( int i = 0; i < 5; i++ )
		{
			TEMPENTITY * pTemp = NULL;

			Vector vForward;
			AngleVectors( vAngles, vForward, NULL, NULL );

			pTemp = gEngfuncs.pEfxAPI->R_TempSprite(
				vOrigin + vCastOrigin, vForward * ( 1000.0f - i * 100.0f ),
				gEngfuncs.pfnRandomFloat( 1.0f, 1.5f ),
				iSprite, kRenderTransAdd, kRenderFxNone, 
				gEngfuncs.pfnRandomFloat( 0.5f, 1.0f ),
				0.3f + i * 0.02f, 0 );

			if( pTemp != NULL )
			{
				pTemp->flags	   |= FTENT_SPRCYCLE;
				pTemp->flags	   |= FTENT_COLLIDEALL;
				pTemp->flags	   |= FTENT_NOREFLECT;
				pTemp->clientIndex	= idx;

				if( i == 0 )
				{
					pTemp->flags	   |= FTENT_CLIENTCUSTOM;
					pTemp->hitcallback	= DragonBreathTouch;
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
// DragonBreath 2
//------------------------------------------------------------------------------
void WWEvent_DragonBreath2( event_args_t * args )
{
	if( !EV_IsLocal( args->entindex ) )
		return;

	gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_DEATH_DEATHRAY_IDLE, 0 );
}


//------------------------------------------------------------------------------
// Wyvern Cast Event
//------------------------------------------------------------------------------
void WWEvent_Wyvern1( event_args_t * args )
{
	int idx = args->entindex;

	if( !EV_IsLocal( idx ) )
		return;

	if( args->iparam1 )
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_DRAGON_WYVERN_IDLE, 0 );
	else
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_DRAGON_WYVERN_CAST, 0 );
}


//------------------------------------------------------------------------------
// Wyvern Explosion Event
//------------------------------------------------------------------------------
void WWEvent_Wyvern2( event_args_t * args )
{
	switch( args->iparam1 )
	{
	// create
	case 0:
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
			"npc/wyvern.wav", 1.0f, ATTN_NORM, 0, 100 );
		break;

	// target aquired
	case 1:
		gEngfuncs.pEventAPI->EV_PlaySound( args->entindex, args->origin, CHAN_BODY,
			"npc/wyvern.wav", 1.0f, ATTN_NORM, 0, 100 );
		break;

	// killed
	case 2:
		Vector vSrc = args->origin;
		Vector vDst = args->origin;
		vDst.z += 24.0f;

		pmtrace_t tr;
		gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
		gEngfuncs.pEventAPI->EV_PlayerTrace( vSrc, vDst, PM_WORLD_ONLY, -1, &tr );

		gEngfuncs.pEventAPI->EV_StopSound( args->entindex, CHAN_WEAPON, "npc/wyvern.wav" );
		gEngfuncs.pEfxAPI->R_Explosion( tr.endpos, 0, 1.0f, 15, 0 );

		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/zerogxplode.spr" );
		TEMPENTITY * pTemp = gEngfuncs.pEfxAPI->R_TempSprite( tr.endpos, Vector( 0, 0, 0 ), 1.0f,
			iSprite, kRenderGlow, kRenderFxNoDissipation, 1.0f, 1.0f, FTENT_SPRANIMATE );
		
		break;
	}
}


//------------------------------------------------------------------------------
// Monster Dragon
//------------------------------------------------------------------------------

void WWEvent_DragonFireTrail(tempent_s *ent, float frametime, float currenttime)
{
	TEMPENTITY *te;
	vec3_t velocity;

	if((gEngfuncs.pfnRandomLong(0,10))%10==0)
	{
		int iTrail=gEngfuncs.pEventAPI->EV_FindModelIndex("sprites/gargeye1.spr");
		int flags=FTENT_SPRCYCLE|FTENT_FADEOUT|FTENT_COLLIDEALL|FTENT_GRAVITY;
		float life=gEngfuncs.pfnRandomFloat(.2,.8);
		float size=gEngfuncs.pfnRandomFloat(.2,.5);

		velocity[0]+=gEngfuncs.pfnRandomFloat(-80,80);
		velocity[1]+=gEngfuncs.pfnRandomFloat(-80,80);
		velocity[2]+=gEngfuncs.pfnRandomFloat(-80,80);

		te=gEngfuncs.pEfxAPI->R_TempSprite(ent->entity.origin,velocity,size,iTrail,kRenderGlow,kRenderFxNoDissipation,1,life,flags);
	}
}

void WWEvent_DragonFireHit(tempent_s *ent,pmtrace_s *pTrace)
{
	int iExplode=gEngfuncs.pEventAPI->EV_FindModelIndex("sprites/fexplo1.spr");

	VectorMA(ent->entity.origin,40,pTrace->plane.normal,ent->entity.origin);

	gEngfuncs.pEfxAPI->R_Explosion(ent->entity.origin,iExplode,1.5,40,TE_EXPLFLAG_NONE);
}

void WWEvent_DragonFire(struct event_args_s *args)
{
	int idx=args->entindex;
	vec3_t origin;
	vec3_t angles;
	vec3_t forward,right,up;
	int iDragonFlame;
	TEMPENTITY *te;
	float size;
	float alpha;
	float flLife=args->fparam1;
	
	if(flLife==0)
		flLife=10;

	iDragonFlame=gEngfuncs.pEventAPI->EV_FindModelIndex("sprites/xspark4.spr");

	VectorCopy( args->origin, origin );
	VectorCopy( args->angles, angles );

	angles[0]*=-1;
	AngleVectors(angles,forward,right,up);
	VectorScale(forward,1400,forward);

	size=gEngfuncs.pfnRandomFloat(1,1.5);
	alpha=gEngfuncs.pfnRandomFloat(.5,1);

	te=gEngfuncs.pEfxAPI->R_TempSprite(origin,forward,size,iDragonFlame,kRenderTransAdd,kRenderFxNone,alpha,flLife,0);

	if(te)
	{
		te->flags|=FTENT_SPRCYCLE;
		te->flags|=FTENT_COLLIDEALL;
		te->flags|=FTENT_COLLIDEKILL;
		te->flags|=FTENT_CLIENTCUSTOM;
		te->clientIndex=idx;
		te->callback=WWEvent_DragonFireTrail;
		te->hitcallback=WWEvent_DragonFireHit;
	}

	gEngfuncs.pEventAPI->EV_PlaySound(idx,origin,CHAN_WEAPON,"monster/dragon/dragon_fire.wav",1,ATTN_NORM,0,gEngfuncs.pfnRandomFloat(100,150));
}

void WWEvent_DragonFlap(struct event_args_s *args)
{
	int idx=args->entindex;
	vec3_t origin;
	vec3_t angles;

	VectorCopy(args->origin,origin);

	gEngfuncs.pEventAPI->EV_PlaySound(idx,origin,CHAN_VOICE,"monster/dragon/dragon_fly1.wav",1,ATTN_NORM,0,100);
}
