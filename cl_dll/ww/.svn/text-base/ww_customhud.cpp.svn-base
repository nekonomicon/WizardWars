#include "hud.h"
#include "cl_util.h"
#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_SpectatorPanel.h"
#include "VGUI_Panel.h"
#include "VGUI_TextGrid.h"
#include "vgui_loadtga.h"

#include "ww_shared/ww_build.h"
#include "ww_customhud.h"


#include "ammohistory.h"
extern WeaponsResource gWR;
extern int g_weaponselect;

extern float g_lastFOV;


CustomHUD::CustomHUD()
: Panel( 0, 0, XRES(640), YRES(480) )
{
	m_iHealth		= 0;
	m_iTargetHealth	= 0;
	m_iMana			= 0;
	m_iTargetMana	= 0;
	m_iArmor		= 0;
	m_iTargetArmor	= 0;
	m_iSatchel		= 0;

	m_pHealth		= NULL;
	m_pHealthBulb	= NULL;
	m_pMana			= NULL;
	m_pManaBulb		= NULL;
	m_pLeftHand		= NULL;
	m_pRightHand	= NULL;
	m_pArmor		= NULL;
	m_pArmorBG		= NULL;
	m_pSatchel1		= NULL;
	m_pSatchel2		= NULL;
	m_pSatchel3		= NULL;
	m_pSatchel4		= NULL;

	m_pChatGrid		= NULL;

	m_bVisible		= true;
}


CustomHUD::~CustomHUD( void )
{
	Destroy();
}


void CustomHUD::Init( void )
{
	m_pShowHud = CVAR_CREATE( "show_hud", "1", FCVAR_ARCHIVE );

	Destroy();

	m_pHealthBulb = new CustomHUD_Image( "gfx/vgui/hud/bulb.tga" );
	m_pHealthBulb->setParent( this );

	m_pHealth = new CustomHUD_Image( "gfx/vgui/hud/health.tga" );
	m_pHealth->setParent( this );

	m_pManaBulb = new CustomHUD_Image( "gfx/vgui/hud/bulb.tga" );
	m_pManaBulb->setParent( this );

	m_pMana = new CustomHUD_Image( "gfx/vgui/hud/mana.tga" );
	m_pMana->setParent( this );

	m_pLeftHand = new CustomHUD_Image( "gfx/vgui/hud/lefthand.tga" );
	m_pLeftHand->setParent( this );
	
	m_pRightHand = new CustomHUD_Image( "gfx/vgui/hud/righthand.tga" );
	m_pRightHand->setParent( this );

	m_pArmorBG = new CustomHUD_Image( "gfx/vgui/hud/armorbg.tga" );
	m_pArmorBG->setParent( this );

	m_pArmor = new CustomHUD_Image( "gfx/vgui/hud/armor.tga" );
	m_pArmor->setParent( this );

	m_pSatchel1 = new CustomHUD_Image( "gfx/vgui/hud/satchel.tga" );
	m_pSatchel1->setParent( this );

	m_pSatchel2 = new CustomHUD_Image( "gfx/vgui/hud/satchel.tga" );
	m_pSatchel2->setParent( this );

	m_pSatchel3 = new CustomHUD_Image( "gfx/vgui/hud/satchel.tga" );
	m_pSatchel3->setParent( this );

	m_pSatchel4 = new CustomHUD_Image( "gfx/vgui/hud/satchel.tga" );
	m_pSatchel4->setParent( this );

	Resize();
	Show();
}

void CustomHUD::Destroy( void )
{
	SAFE_DELETE( m_pHealth );
	SAFE_DELETE( m_pHealthBulb );
	SAFE_DELETE( m_pMana );
	SAFE_DELETE( m_pManaBulb );
	SAFE_DELETE( m_pLeftHand );
	SAFE_DELETE( m_pRightHand );
	SAFE_DELETE( m_pArmor );
	SAFE_DELETE( m_pArmorBG );
	SAFE_DELETE( m_pSatchel1 );
	SAFE_DELETE( m_pSatchel2 );
	SAFE_DELETE( m_pSatchel3 );
	SAFE_DELETE( m_pSatchel4 );

	m_Weapons.removeAll();
}


void CustomHUD::Resize( void )
{
	m_pHealthBulb->setPos( 5, ScreenHeight - 110 );
	m_pLeftHand->setPos( 8, ScreenHeight - 35 );

	m_pManaBulb->setPos( ScreenWidth - 105, ScreenHeight - 110 );
	m_pRightHand->setPos( ScreenWidth - 58, ScreenHeight - 35 );

	m_pArmorBG->setPos( ScreenWidth - 180/*( ScreenWidth * 0.5f + 0.5f ) - 37*/, ScreenHeight - 110 );

	m_pSatchel1->setPos( 105, ScreenHeight - 60 );
	m_pSatchel2->setPos( 145/*ScreenWidth - 150*/, ScreenHeight - 60 );
	m_pSatchel3->setPos( 185/*145*/, ScreenHeight - 60 );
	m_pSatchel4->setPos( 225/*ScreenWidth - 190*/, ScreenHeight - 60 );
}


void CustomHUD::ResizePanel( CustomHUD_Image * pPanel, int x, int y, float fPercent )
{
	if( !pPanel )
		return;

	int w, h;

	fPercent = ( fPercent < 0.0f ? 0 : ( fPercent > 1.0f ? 1.0f : fPercent ) );

	pPanel->getImageSize( w, h );

	pPanel->setPos( x, y + h - h * fPercent );
	pPanel->setSize( w, h * fPercent );
	pPanel->setOffset( 0, h * fPercent - h );
}

void CustomHUD::Update( void )
{
	if( m_pShowHud->value == 0.0f )
		Hide();
	else if( gViewPort && gViewPort->m_pSpectatorPanel && gViewPort->m_pSpectatorPanel->isVisible() )
		Hide();
	else
		Show();
}

void CustomHUD::Hide( void )
{
	m_bVisible = false;
	setVisible( false );

	g_lastFOV = 90;
}

void CustomHUD::Show( void )
{
	m_bVisible = true;
	setVisible( true );
}


void CustomHUD::paintBackground( void )
{
#ifndef VGUI_HUD
	return;
#endif

	if( !m_bVisible )
		return;

	m_iTargetHealth	= gHUD.m_Health.m_iHealth;
	m_iTargetMana	= gWR.CountAmmo( 1 );
	m_iSatchel		= gWR.CountAmmo( 2 );

	Resize();

	// process health
	{

		if( m_iTargetHealth > m_iHealth )
			m_iHealth++;
		else if( m_iTargetHealth < m_iHealth )
			m_iHealth--;
		else
			m_iHealth = m_iTargetHealth;
	}

	// process mana
	{

		if( m_iTargetMana > m_iMana )
			m_iMana++;
		else if( m_iTargetMana < m_iMana )
			m_iMana--;
		else
			m_iMana = m_iTargetMana;
	}

	// process armor
	{
		m_iTargetArmor = gHUD.m_Battery.m_iBat;

		if( m_iTargetArmor > m_iArmor )
			m_iArmor++;
		else if( m_iTargetArmor < m_iArmor )
			m_iArmor--;
		else
			m_iArmor = m_iTargetArmor;
	}

	ResizePanel( m_pHealth,	5, ScreenHeight - 110, m_iHealth * 0.01f );
	ResizePanel( m_pMana, ScreenWidth - 105, ScreenHeight - 110, m_iMana * 0.005f );
	ResizePanel( m_pArmor, ScreenWidth - 180/*( ScreenWidth * 0.5f + 0.5f ) - 37*/, ScreenHeight - 110, m_iArmor * 0.007f );

	m_pSatchel1->setVisible( m_iSatchel >= 1 ? true : false );
	m_pSatchel2->setVisible( m_iSatchel >= 2 ? true : false );
	m_pSatchel3->setVisible( m_iSatchel >= 3 ? true : false );
	m_pSatchel4->setVisible( m_iSatchel >= 4 ? true : false );

	solveTraverse();
}


CustomHUD_Image::CustomHUD_Image( const char * pszFile )
: Panel( 0, 0, 0, 0 )
{
	int size[2];

	size[0] = size[1] = 0;
	_offset[0] = _offset[1] = 0;

	_image = vgui_LoadTGA( pszFile );

	if( _image != NULL )
		_image->getSize( size[0], size[1] );

	setSize( size[0], size[1] );
}


CustomHUD_Image::~CustomHUD_Image()
{
	SAFE_DELETE( _image );
}


void CustomHUD_Image::paint( void )
{
}


void CustomHUD_Image::paintBackground( void )
{
#ifndef VGUI_HUD
	return;
#endif

	if( !_image )
		return;

	_image->doPaint( this );
}


void CustomHUD_Image::setOffset( int x, int y )
{
	_offset[0] = x;
	_offset[1] = y;

	if( _image )
		_image->setPos( x, y );
}


void CustomHUD_Image::getOffset( int & x, int & y )
{
	x = _offset[0];
	y = _offset[1];
}


void CustomHUD_Image::getImageSize( int & w, int & h )
{
	w = h = 0;

	if( _image )
		_image->getSize( w, h );
}