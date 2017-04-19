/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#include "../hud.h"
#include "../cl_util.h"
#include "../demo.h"

#include "demo_api.h"
#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"

#include "pm_defs.h"
#include "event_api.h"
#include "entity_types.h"
#include "r_efx.h"

BEAM * pBeam = NULL;
BEAM * pBeam2 = NULL;
float flBeamDist = 0.0f;
TEMPENTITY * pCrosshair;
void HUD_GetLastOrg( float *org );
void EV_HLDM_DecalTrace( pmtrace_t * trace, char * decalName );
int iBeamType = 0;
Vector vLastDecal;
extern cvar_t * effects;


void UpdateBeams( void )
{
	vec3_t forward, vecSrc, vecEnd, origin, angles, right, up;
	vec3_t view_ofs;
	pmtrace_t tr;
	cl_entity_t *pthisplayer = gEngfuncs.GetLocalPlayer();
	int idx = pthisplayer->index;

	if( flBeamDist < 100.0f )
		flBeamDist = 100.0f;

	// Get our exact viewangles from engine
	gEngfuncs.GetViewAngles( (float *)angles );

	// Determine our last predicted origin
	HUD_GetLastOrg( (float *)&origin );

	AngleVectors( angles, forward, right, up );

	VectorCopy( origin, vecSrc );
	
	VectorMA( vecSrc, flBeamDist, forward, vecEnd );

	gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction( false, true );	
						
	// Store off the old count
	gEngfuncs.pEventAPI->EV_PushPMStates();
					
	// Now add in all of the players.
	gEngfuncs.pEventAPI->EV_SetSolidPlayers ( idx - 1 );	

	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
	gEngfuncs.pEventAPI->EV_PlayerTrace( vecSrc, vecEnd, PM_STUDIO_BOX, -1, &tr );

	gEngfuncs.pEventAPI->EV_PopPMStates();

	if ( pBeam )
	{
		pBeam->target = tr.endpos;
		pBeam->die	  = gEngfuncs.GetClientTime() + 0.1; // We keep it alive just a little bit forward in the future, just in case.
	}
		
	if ( pBeam2 )
	{
		pBeam2->target = tr.endpos;
		pBeam2->die	   = gEngfuncs.GetClientTime() + 0.1; // We keep it alive just a little bit forward in the future, just in case.
	}

	if( pCrosshair )
	{
		pCrosshair->entity.origin = tr.endpos;
		pCrosshair->die = gEngfuncs.GetClientTime() + 0.1;
	}

	if( tr.fraction != 1.0f && ( pBeam || pBeam2 ) && effects->value == 1 )
	{
		switch( iBeamType )
		{
		// freeze ray
		case 1:
			if( ( tr.endpos - vLastDecal ).Length() < 8.0f )
				return;

			switch( gEngfuncs.pfnRandomLong( 0, 1 ) )
			{
			case 0: EV_HLDM_DecalTrace( &tr, "{break1" ); break;
			case 1: EV_HLDM_DecalTrace( &tr, "{break2" ); break;
			}

			break;

		// death ray
		case 2:
			if( ( tr.endpos - vLastDecal ).Length() < 4.0f )
				break;

			gEngfuncs.pEfxAPI->R_BulletImpactParticles( tr.endpos );

			switch( gEngfuncs.pfnRandomLong( 0, 5 ) )
			{
			case 0: EV_HLDM_DecalTrace( &tr, "{shot1" ); break;
			case 1: EV_HLDM_DecalTrace( &tr, "{shot2" ); break;
			case 2: EV_HLDM_DecalTrace( &tr, "{shot3" ); break;
			case 3: EV_HLDM_DecalTrace( &tr, "{shot4" ); break;
			case 4: EV_HLDM_DecalTrace( &tr, "{shot5" ); break;
			}
			break;
		
		// white ray
		case 3:
			if( ( tr.endpos - vLastDecal ).Length() < 4.0f )
				break;
			gEngfuncs.pEfxAPI->R_FlickerParticles( tr.endpos );
			break;

		// lightning bolt
		case 4:
			gEngfuncs.pEfxAPI->R_SparkEffect( tr.endpos, 1, -300, 300 );
			break;
		}

		vLastDecal = tr.endpos;
	}
}

/*
=====================
Game_AddObjects

Add game specific, client-side objects here
=====================
*/
void Game_AddObjects( void )
{
	if( pBeam || pBeam2 || pCrosshair )
		UpdateBeams();
}

