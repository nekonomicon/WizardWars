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
void	HUD_GetLastOrg				( float *org );
void	TempEntityRegister			( cl_entity_t * pEntity );
void	VectorAngles				( const float * forward, float * angles );
float	EV_HLDM_PlayTextureSound	( int idx, pmtrace_t * ptr, float * vecSrc, float * vecEnd, int iBulletType );


//------------------------------------------------------------------------------
// C Function Prototypes
//------------------------------------------------------------------------------
extern "C"
{
	void WWEvent_StaffHit	( struct event_args_s * args );
	void WWEvent_Shield		( struct event_args_s * args );
	void WWEvent_Missile1	( struct event_args_s * args );
	void WWEvent_Missile2	( struct event_args_s * args );
	void WWEvent_SealSound	( struct event_args_s * args );
}


//------------------------------------------------------------------------------
// Staff Event
//------------------------------------------------------------------------------
void WWEvent_StaffHit( event_args_t * args )
{
/*
	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/smoke.spr" );

	gEngfuncs.pEfxAPI->R_BeamKill( args->entindex | 0x3000 );
	gEngfuncs.pEfxAPI->R_BeamFollow( args->entindex | 0x3000, iSprite, 0.2f, 10.0f, 1, 1, 1, 0.5f );
*/

	int idx			 = args->entindex;
	int iPlayerClass = args->iparam1;
	int iSwingCount  = args->iparam2;
	int iHit		 = args->bparam1;

	static const char * sounds[] =
	{
		"spells/staff_hitbod1.wav",
		"spells/staff_hitbod2.wav",
		"spells/staff_hitbod3.wav",
		"spells/staff_miss1.wav",
	};

	if( args->bparam2 )
	{
		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_IDLE, 0 );
		return;
	}

	Vector vSrc, vDst, vAngles, vForward;

	if( EV_IsLocal( idx ) )
		vAngles = gHUD.m_vecAngles;
	else
		vAngles = args->angles;

	AngleVectors( vAngles, vForward, NULL, NULL );
	EV_GetGunPosition( args, vSrc, args->origin );
	vDst = vSrc + vForward * STAFF_RANGE;

	pmtrace_t tr;
	gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction( false, true );	
	gEngfuncs.pEventAPI->EV_PushPMStates();
	gEngfuncs.pEventAPI->EV_SetSolidPlayers( idx - 1 );	
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
	gEngfuncs.pEventAPI->EV_PlayerTrace( vSrc, vDst, PM_STUDIO_BOX, -1, &tr );
	gEngfuncs.pEventAPI->EV_PopPMStates();

	if( EV_IsLocal( idx ) )
	{
		if( tr.fraction != 1.0f )
			iHit = 1;

		switch( iSwingCount % 3 )
		{
		case 0: gEngfuncs.pEventAPI->EV_WeaponAnimation( ( iHit ? HANDS_HIT1 : HANDS_MISS1 ), 0 ); break;
		case 1: gEngfuncs.pEventAPI->EV_WeaponAnimation( ( iHit ? HANDS_HIT2 : HANDS_MISS2 ), 0 ); break;
		case 2: gEngfuncs.pEventAPI->EV_WeaponAnimation( ( iHit ? HANDS_HIT3 : HANDS_MISS3 ), 0 ); break;
		}
	}

	if( iHit )
	{
		if( EV_IsPlayer( tr.ent ) )
			gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON, sounds[iSwingCount % 3],
				1.0f, ATTN_NORM, 0, 100 );
		else
			EV_HLDM_PlayTextureSound( idx, &tr, vSrc, vDst, BULLET_PLAYER_CROWBAR );
	}
/*
	if( iHit && EV_IsPlayer( tr.ent ) )
		gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON, sounds[iSwingCount % 3], 1.0f, ATTN_NORM, 0, 100 );
*/
	else
		gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON, sounds[3], 1.0f, ATTN_NORM, 0, 100 );
}



//------------------------------------------------------------------------------
// Shield Spell
//------------------------------------------------------------------------------
int PARTICLEAPI ShieldSpellThink( WWParticle * pPart, const float & flFrametime )
{
	pdata_t * pData = pPart->m_pPrivate;

	float r = gEngfuncs.GetClientTime() * 10.0f + pData->i1 * 2.5f;

	pData->f1 += 25.0f * flFrametime;
	pData->f2 += 200.0f * flFrametime;
	pData->f3 -= sinf( r ) * pData->f2 * flFrametime;
	pData->f4 -= cosf( r ) * pData->f2 * flFrametime;

	pData->v2 = Vector( pData->f3, pData->f4, pData->f1 );

	cl_entity_t * player = NULL;

	if( pData->p1 != NULL )
		player = (cl_entity_t *)pData->p1;

	if( player != NULL )
	{
		if( EV_IsLocal( player->index ) )
		{
			HUD_GetLastOrg( pPart->m_vOrigin );
			pPart->m_vOrigin = pPart->m_vOrigin + pData->v2;
			pPart->m_vOrigin.z -= 24.0f;
		}
		else
			pPart->m_vOrigin = player->origin + pData->v2;
	}
	else
		pPart->m_vOrigin = pData->v1 + pData->v2;

	return 1;
}


//------------------------------------------------------------------------------
// Shield Spell Cast Event
//------------------------------------------------------------------------------
void WWEvent_Shield( event_args_t * args )
{
	int idx = args->entindex;

	if( args->iparam1 )
	{
		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_SHIELD_IDLE, 0 );
		return;
	}

	if( EV_IsLocal( idx ) )
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_SHIELD_CAST, 0 );
	
	gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON,
		"spells/armorspell.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );

	if( !g_pParticleManager )
		return;

	for( int i = 0; i < 10; i++ )
	{
		WWParticle * pPart = g_pParticleManager->Allocate( PARTICLE_PRIORITY_NONE );

		if( !pPart )
			break;

		pdata_t * pData = (pdata_t *)malloc( sizeof(pdata_t) );
		memset( pData, 0, sizeof(pdata_t) );

		pData->i1 = i;
		pData->v1 = args->origin;
		pData->p1 = gEngfuncs.GetEntityByIndex( args->entindex );

		pPart->SetSprite( "sprites/effects/armorspell.spr" );
		pPart->SetPrivate( pData, true );
		pPart->SetThink( ShieldSpellThink );

		pPart->m_bDraw				= true;
		pPart->m_iFlags				= 0;
		pPart->m_vOrigin			= args->origin;
		pPart->m_flLife				= 1.0f;
		pPart->m_flBirth			= gEngfuncs.GetClientTime() + 0.05f * (float)i;
		pPart->m_flScale			= 0.25f;
		pPart->m_iRenderMode		= kRenderTransAdd;
		pPart->m_flRenderColor[0]	= 1.0f;
		pPart->m_flRenderColor[1]	= 1.0f;
		pPart->m_flRenderColor[2]	= 1.0f;
		pPart->m_flRenderColor[3]	= 1.0f;
	}
}


//------------------------------------------------------------------------------
// Missile HitCallBack
//------------------------------------------------------------------------------
void MissileThink( TEMPENTITY * pEnt, float flFrametime, float flTime )
{
	pEnt->entity.curstate.frame += 255.0f * flFrametime;
	if( pEnt->entity.curstate.frame >= 255.0f )
		pEnt->entity.curstate.frame = pEnt->entity.curstate.frame - (int)pEnt->entity.curstate.frame;
}


//------------------------------------------------------------------------------
// Missile HitCallBack
//------------------------------------------------------------------------------
void MissileTouch( TEMPENTITY * pEntity, pmtrace_t * pTrace )
{
	Vector vOrigin = pTrace->endpos;

	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/xspark2.spr" );
	TEMPENTITY * pTemp = gEngfuncs.pEfxAPI->R_TempSprite( vOrigin, Vector( 0, 0, 0 ), 1.0f, iSprite,
		kRenderGlow, kRenderFxNoDissipation, 1.0f, 1.0f, FTENT_SPRANIMATE );

	if( pTemp != NULL )
	{
		pTemp->entity.curstate.framerate		= 15;
		pTemp->entity.curstate.rendercolor.r	= 255;
		pTemp->entity.curstate.rendercolor.g	= 255;
		pTemp->entity.curstate.rendercolor.b	= 255;
	}

	gEngfuncs.pEventAPI->EV_PlaySound( -1, vOrigin, CHAN_WEAPON,
		"weapons/xbow_hitbod1.wav", 1.0f, ATTN_NORM, 0, 100 );

	gEngfuncs.pEfxAPI->R_BeamKill( pEntity->entity.index );

	switch( gEngfuncs.pfnRandomLong( 0, 0 ) )
	{
	case 0: EV_HLDM_DecalTrace( pTrace, "{bproof1" ); break;
	}
}


//------------------------------------------------------------------------------
// Missile Cast Event
//------------------------------------------------------------------------------
void WWEvent_Missile1( event_args_t * args )
{
	int idx = args->entindex;

	if( args->iparam1 )
	{
		if( EV_IsLocal( idx ) )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_MISSILE_IDLE, 0 );
		return;
	}

	// play casting sound
	gEngfuncs.pEventAPI->EV_PlaySound(
		idx, args->origin, CHAN_WEAPON, "spells/magicmissile.wav", 1.0f, ATTN_NORM, 0, PITCH_NORM );

	// only predict local missiles, everyone else has to use the servers
	if( !EV_IsLocal( idx ) )
		return;

	if( !args->bparam1 )
		gEngfuncs.pEventAPI->EV_WeaponAnimation( HANDS_MISSILE_CAST, 0 );

	Vector vAngles = gHUD.m_vecAngles;
	Vector vForward, vRight, vUp;
	AngleVectors( vAngles, vForward, vRight, vUp );

	vAngles[PITCH] = -vAngles[PITCH];

	Vector vOrigin;
	EV_GetGunPosition( args, (float *)&vOrigin, args->origin );
	vOrigin.z -= 8.0f;
	vOrigin = vOrigin + vForward * 8.0f;

	if( args->bparam1 )
		vOrigin = vOrigin + vRight * 8.0f;
	else
		vOrigin = vOrigin - vRight * 8.0f;

	int iModels = gEngfuncs.pEventAPI->EV_FindModelIndex( "models/missile.mdl" );
	int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/smoke.spr" );

	TEMPENTITY * pTemp = NULL;
	pTemp = gEngfuncs.pEfxAPI->R_TempModel( vOrigin, vForward,
		vAngles, 99.0f, iModels, 0 );

	if( pTemp != NULL )
	{
		TempEntityRegister( &pTemp->entity );

		pTemp->clientIndex	= idx;
		pTemp->flags = FTENT_COLLIDEALL | FTENT_COLLIDEKILL | FTENT_PERSIST | FTENT_CLIENTCUSTOM;
		pTemp->entity.curstate.rendermode		= kRenderTransAdd;
		pTemp->entity.curstate.rendercolor.r	= 255;
		pTemp->entity.curstate.rendercolor.g	= 255;
		pTemp->entity.curstate.rendercolor.b	= 255;
		pTemp->entity.curstate.renderamt		= 255;
		pTemp->entity.curstate.renderfx			= kRenderFxMana;
		pTemp->entity.baseline.origin			= vForward * MISSILE_SPEED;
		pTemp->callback							= MissileThink;
		pTemp->hitcallback						= MissileTouch;

		gEngfuncs.pEfxAPI->R_BeamFollow( pTemp->entity.index, iSprite, 0.3f, 2.0f, 0.5f, 0.6f, 1.0f, 1.0f );
		gEngfuncs.pEfxAPI->R_BeamFollow( pTemp->entity.index, iSprite, 0.2f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f );
	}
}


//------------------------------------------------------------------------------
// Missile Event
//------------------------------------------------------------------------------
void WWEvent_Missile2( event_args_t * args )
{
	if( args->iparam1 )
	{
		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/smoke.spr" );

		gEngfuncs.pEfxAPI->R_BeamFollow( args->entindex, iSprite, 0.3f, 2.0f, 0.5f, 0.6f, 1.0f, 1.0f );
		gEngfuncs.pEfxAPI->R_BeamFollow( args->entindex, iSprite, 0.2f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f );
	}
	else
	{
		dlight_t * dl = gEngfuncs.pEfxAPI->CL_AllocDlight(0);
		dl->origin	= args->origin;
		dl->radius	= 196;
		dl->color.r	= 120;
		dl->color.g	= 120;
		dl->color.b	= 255;
		dl->decay	= 100;
		dl->die		= gEngfuncs.GetClientTime() + 0.02f;

		int iSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/xspark2.spr" );
		TEMPENTITY * pTemp = gEngfuncs.pEfxAPI->R_TempSprite( args->origin, Vector( 0, 0, 0 ), 1.0f,
			iSprite, kRenderGlow, kRenderFxNoDissipation, 1.0f, 1.0f, FTENT_SPRANIMATE );

		if( pTemp != NULL )
		{
			pTemp->entity.curstate.framerate		= 15;
			pTemp->entity.curstate.rendercolor.r	= 255;
			pTemp->entity.curstate.rendercolor.g	= 255;
			pTemp->entity.curstate.rendercolor.b	= 255;
		}

		gEngfuncs.pEventAPI->EV_PlaySound( -1, args->origin, CHAN_WEAPON,
			"weapons/xbow_hitbod1.wav", 1.0f, ATTN_NORM, 0, 100 );

		gEngfuncs.pEfxAPI->R_BeamKill( args->entindex );

		Vector vSrc, vDst, vOrigin;
		vOrigin = args->origin;
		vSrc.x = gEngfuncs.pfnRandomFloat( -1, 1 );
		vSrc.y = gEngfuncs.pfnRandomFloat( -1, 1 );
		vSrc.z = gEngfuncs.pfnRandomFloat( -1, 1 );
		vSrc = vSrc.Normalize() * 8.0f;
		vDst = vOrigin - vSrc;
		vSrc = vOrigin + vSrc;

		pmtrace_t tr;
		gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
		gEngfuncs.pEventAPI->EV_PlayerTrace( vSrc, vDst, PM_WORLD_ONLY, -1, &tr );

		switch( gEngfuncs.pfnRandomLong( 0, 0 ) )
		{
		case 0: EV_HLDM_DecalTrace( &tr, "{bproof1" ); break;
		}
	}
}


//------------------------------------------------------------------------------
// Seal Sound Event
//------------------------------------------------------------------------------
void WWEvent_SealSound( event_args_t * args )
{
	int	idx		= args->entindex;
	int	sound	= args->iparam1;
	int	stop	= args->iparam2;

	static const char * sounds[] =
	{
		"spells/seal_start.wav",
		"spells/seal_loop.wav",
		"spells/seal_end.wav",
	};

	gEngfuncs.pEventAPI->EV_StopSound( idx, CHAN_WEAPON, sounds[0] );
	gEngfuncs.pEventAPI->EV_StopSound( idx, CHAN_WEAPON, sounds[1] );
	gEngfuncs.pEventAPI->EV_StopSound( idx, CHAN_WEAPON, sounds[2] );

	if( !stop )
		gEngfuncs.pEventAPI->EV_PlaySound( idx, args->origin, CHAN_WEAPON, sounds[sound], 1.0f, ATTN_NORM, 0, 100 );
}


