//=========== (C) Copyright 1999 Valve, L.L.C. All rights reserved. ===========
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================

#include "VGUI_Font.h"
#include "VGUI_ScrollPanel.h"
#include "VGUI_TextImage.h"
#include "VGUI_ImagePanel.h"

#include<VGUI_StackLayout.h>

#include "hud.h"
#include "cl_util.h"
#include "camera.h"
#include "kbutton.h"
#include "const.h"

#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ServerBrowser.h"

#define WORLDBUTTON_X		XRES(70)
#define WORLDBUTTON_Y		YRES(25)

//--------------------`---------------------------------------------------------
// Purpose: Constructs a message panel
//-----------------------------------------------------------------------------
extern void COM_FileBase ( const char *in, char *out);
extern char g_szWorldName[64];
extern int g_iWorldCoordX;
extern int g_iWorldCoordY;
extern int g_iPlayerclass;

CWorldPanel::CWorldPanel(int iTrans, int iRemoveMe, int x,int y,int wide,int tall) : CMenuPanel(iTrans, iRemoveMe, x,y,wide,tall){
	m_X=x;
	m_Y=y;
	m_SizeX=wide;
	m_SizeY=tall;

	int r, g, b, a;

	// Get the scheme used for the Titles
	CSchemeManager *pSchemes = gViewPort->GetSchemeManager();

	SchemeHandle_t hTitleScheme = pSchemes->getSchemeHandle("Title Font");
	SchemeHandle_t hMOTDText = pSchemes->getSchemeHandle("Briefing Text");
	SchemeHandle_t hWindowScheme = pSchemes->getSchemeHandle( "Window" );

	pSchemes->getBorderColor( hWindowScheme, r, g, b, a );

	m_pPanel1=new CTransparentPanel(0,x,y,wide,tall);
	m_pPanel1->setParent(this);
	m_pPanel1->setBorder(new LineBorder(Color(r, g, b, a)));
	m_pPanel1->setVisible(true);
	m_pPanel2=new CTransparentPanel(0,x,y,wide,tall);
	m_pPanel2->setParent(this);
	m_pPanel2->setBorder(new LineBorder(Color(r, g, b, a)));
	m_pPanel2->setVisible(true);
	
	m_pWorldPanel=new ImagePanel(NULL);
	m_pWorldPanel->setParent(this);
	m_pWorldPanel->setVisible(true);

	m_pButton=new CommandButton(CHudTextMessage::BufferedLocaliseTextString(" "),0,0,WORLDBUTTON_X,WORLDBUTTON_Y);
	m_pButton->addActionSignal(new CMenuHandler_TextWindow(HIDE_TEXTWINDOW));
	m_pButton->setParent(this);
	m_pButton->setVisible(true);
	
	//Now here comes the little X that tells where this fight is taking place:
	m_pCrossPanel=new ImagePanel(NULL);
	m_pCrossPanel->setParent(this);
	m_pCrossPanel->setVisible(true);
}

void CWorldPanel::Open(void){
	if(CVAR_GET_FLOAT("cl_showmap")==0 || g_szWorldName==NULL || g_szWorldName[0]==NULL){
		gViewPort->HideTopMenu();
		return;
	}

	Update();
	
	setVisible(true);
	
	CMenuPanel::Open();
	SetActiveInfo(1);
}

void CWorldPanel::Update(){
	int x,y,wx,wy;
	FileInputStream *fis;
	BitmapTGA *pTGA;
	char sz[256];
	int i=0;
	int arrowX,arrowY;

	if(g_iPlayerClass!=0)
		setTransparency(0);

	if(ScreenWidth<800)
		i=320;
	else
		i=640;
	sprintf(sz,"%s/gfx/vgui/%d_%s.tga",gEngfuncs.pfnGetGameDirectory(),i,g_szWorldName);
	fis=new FileInputStream(sz,false);
	pTGA=new BitmapTGA(fis,true);
	fis->close();

	m_pWorldPanel->setImage(pTGA);
	m_pWorldPanel->getSize(x,y);
	m_X=(m_SizeX-x)/2;
	m_Y=(m_SizeY-YRES(32)-y)/2;
	m_pWorldPanel->setPos(m_X,m_Y);

	m_pPanel1->setPos(m_X-XRES(1),m_Y-YRES(1));
	m_pPanel1->setSize(x+XRES(4),y+YRES(4));
	m_pPanel2->setPos(m_X-XRES(8),m_Y-YRES(8));
	m_pPanel2->setSize(x+XRES(16),y+YRES(16));

	//Calculate the X,Y pos of where you are
	wx=((float)g_iWorldCoordX/1000.0)*(float)x;
	wy=((float)g_iWorldCoordY/1000.0)*(float)y;

	m_pButton->setPos(m_X-XRES(8),m_Y-YRES(8));
	m_pButton->setSize(x+XRES(16),y+YRES(16));

	if(ScreenWidth<800)
		i=320;
	else
		i=640;
	sprintf(sz,"%s/gfx/vgui/%d_cross.tga",gEngfuncs.pfnGetGameDirectory(),i);
	fis=new FileInputStream(sz,false);
	pTGA=new BitmapTGA(fis,true);
	fis->close();
	m_pCrossPanel->setImage(pTGA);
	m_pCrossPanel->getSize(arrowX,arrowY);
	m_pCrossPanel->setPos(wx+m_X-arrowX/2,wy+m_Y-arrowY/2);
}

void CWorldPanel::Think(){
	if(abs(gEngfuncs.GetClientTime()-m_flTime)>.4){
		m_flTime=gEngfuncs.GetClientTime();
	
		if(m_pCrossPanel->isVisible())
			m_pCrossPanel->setVisible(false);
		else
			m_pCrossPanel->setVisible(true);
	}
}