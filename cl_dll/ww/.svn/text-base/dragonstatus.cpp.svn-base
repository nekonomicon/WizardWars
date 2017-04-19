/***
*
*	Copyright (c) 1999, Valve LLC. All rights reserved.
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
//
// DragonStatus.cpp
//
// implementation of CHudDragonStatus class
//

#include "STDIO.H"
#include "STDLIB.H"
#include "MATH.H"

#include "hud.h"
#include "cl_util.h"
#include "const.h"
#include "entity_types.h"
#include "studio_event.h" // def. of mstudioevent_t
#include "r_efx.h"
#include "event_api.h"
#include "pm_defs.h"
#include "pmtrace.h"	
#include "parsemsg.h"
#include <string.h>
#include "com_weapons.h"

DECLARE_MESSAGE(m_DragonStatus,Dragon)

extern struct local_state_s *g_finalstate;

bool g_isMounted;

int CHudDragonStatus::Init(void){
	HOOK_MESSAGE(Dragon);
	m_iDragonHealth = 100;
	m_iDragonFlame = 100;

	gHUD.AddHudElem(this);
	return 1;
}

void CHudDragonStatus::Reset( void ){
}

int CHudDragonStatus::VidInit(void){
	m_hSprite = gHUD.GetSpriteIndex("dragon");

	return 1;
}

extern void HUD_SendWeaponAnim(int iAnim,int body,int force);

int CHudDragonStatus::MsgFunc_Dragon(const char *pszName,  int iSize, void *pbuf ){
	BEGIN_READ( pbuf, iSize );
	int x = READ_BYTE();
	int y = READ_BYTE();

	m_iFlags |= HUD_ACTIVE;

	g_isMounted=true;

	if(m_iDragonHealth==0){
		HUD_SendWeaponAnim(0,0,1);
		m_flNextAnimation=gHUD.m_flTime+1;

		g_isMounted=false;
	}

	// Only update the fade if we've changed health
	m_iDragonHealth=x;
	m_iDragonFlame=y;

	return 1;
}

// Returns back a color from the
// Green <-> Yellow <-> Red ramp
void CHudDragonStatus::GetFlameColor(int val,int &r,int &g,int &b){
	r=((float)(100-val))*2.5;
	g=0;
	b=((float)(val))*2.5;
}

void CHudDragonStatus::GetHealthColor(int val,int &r,int &g,int &b){
	r=((float)(100-val))*2.5;
	g=((float)(val))*2.5;
	b=0;
}

int CHudDragonStatus::Draw(float flTime){
	int r,g,b;
	int x=5,y=0;
	int w=0;

	if(m_iDragonHealth<=0)
		return 1;

	if(gHUD.m_iHideHUDDisplay&HIDEHUD_HEALTH)
		return 1;

	// Only draw health if we have the suit.
	if(gHUD.m_iWeaponBits&(1<<(WEAPON_SUIT))){
		y=ScreenHeight-gHUD.m_iFontHeight*2.5;
		x=ScreenWidth/2.5;
		w=gHUD.GetSpriteRect(m_hSprite).right-gHUD.GetSpriteRect(m_hSprite).left;

		UnpackRGB(r,g,b,RGB_YELLOWISH);
		ScaleColors(r,g,b,128);
		SPR_Set(gHUD.GetSprite(m_hSprite),r,g,b);
		SPR_DrawAdditive(0,x,y,&gHUD.GetSpriteRect(m_hSprite));

		x=x+w;

		GetHealthColor(m_iDragonHealth,r,g,b);
		FillRGBA(x,y+40,m_iDragonHealth,10,r,g,b,255);

		GetFlameColor(m_iDragonFlame,r,g,b);
		FillRGBA(x,y+10,m_iDragonFlame,10,r,g,b,255);
	}

	if(m_flNextAnimation<gHUD.m_flTime){
		int i=gEngfuncs.pfnRandomLong(0,1);

		if(g_finalstate && g_finalstate->playerstate.velocity[2]>1)
			i=1;
		
		if(i==1){
			m_flNextAnimation=gHUD.m_flTime+1;
			HUD_SendWeaponAnim(0,0,1);

			PlaySound("monster/dragon/dragon_fly1.wav",1);
		}
		else{
			m_flNextAnimation=gHUD.m_flTime+4;
			HUD_SendWeaponAnim(1,0,1);
		}
	}

	return 1;
}