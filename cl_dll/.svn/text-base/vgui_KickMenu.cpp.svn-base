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

class CKickHandler:public ActionSignal{
public:
	CKickHandler(int iKick){
		m_iKick=iKick;
	}

	virtual void actionPerformed(Panel* panel){
		if(g_PlayerInfoList[m_iKick].name){
			char s[128];
			sprintf(s,"say votekick %s\n",g_PlayerInfoList[m_iKick].name);
			ClientCmd(s);
		}
		gViewPort->HideTopMenu();
	}
private:
	int m_iKick;
};

CKickMenu::CKickMenu(int iTrans, int iRemoveMe, int x,int y,int wide,int tall) : CMenuPanel(iTrans, iRemoveMe, x,y,wide,tall){
	m_X=x;
	m_Y=y;
	m_SizeX=wide;
	m_SizeY=tall;
	int r,g,b,a;

	for(int i=0;i<MAX_PLAYERS;i++){
		ActionSignal *as=new CKickHandler(i+1);

		m_pButtons[i]=new CommandButton(i,"",XRES(32+(i/8)*160),YRES(64*(i+1)-(i/8)*(64*8-92-10)),XRES(128),YRES(32), 0);
		m_pButtons[i]->setContentAlignment(vgui::Label::a_west);
		m_pButtons[i]->addActionSignal(as);
		m_pButtons[i]->addInputSignal(new CHandler_MenuButtonOver(this,-1));
		m_pButtons[i]->setParent(this);
	}

	// Get the scheme used for the Titles
	CSchemeManager *pSchemes = gViewPort->GetSchemeManager();
	SchemeHandle_t hTitleScheme = pSchemes->getSchemeHandle("Title Font");
	SchemeHandle_t hMOTDText = pSchemes->getSchemeHandle("Briefing Text");
	Font *pTitleFont = pSchemes->getFont( hTitleScheme );

	Label *pLabel = new Label("",0,0);
	pLabel->setParent(this);
	pLabel->setFont(pTitleFont);
	pSchemes->getFgColor( hTitleScheme, r, g, b, a );
	pLabel->setFgColor( r, g, b, a );
	pSchemes->getBgColor( hTitleScheme, r, g, b, a );
	pLabel->setBgColor( r, g, b, a );
	pLabel->setContentAlignment( vgui::Label::a_west );
	pLabel->setText(gHUD.m_TextMessage.BufferedLocaliseTextString("Choose player to vote out:"));
}

void CKickMenu::Open(void){
	Update();
	
	setVisible(true);
	
	CMenuPanel::Open();
}

void CKickMenu::Update(){
	for(int i=0;i<MAX_PLAYERS;i++){
		char sz[256];

		m_pButtons[i]->setVisible(TRUE);

		if(g_PlayerInfoList[i+1].name==NULL || g_PlayerInfoList[i+1].name[0]==0){
			m_pButtons[i]->setVisible(FALSE);
		}
		else{
			sprintf(sz,"%s",CHudTextMessage::BufferedLocaliseTextString(g_PlayerInfoList[i+1].name));
			m_pButtons[i]->setText(sz);
		}
	}
}
