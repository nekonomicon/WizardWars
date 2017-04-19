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
//
// status_icons.cpp
//
#include "hud.h"
#include "cl_util.h"
#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include <string.h>
#include <stdio.h>
#include "parsemsg.h"
#include "event_api.h"

#include "ww_shared/ww_defs.h"


DECLARE_MESSAGE( m_StatusIcons, StatusIcon );
DECLARE_MESSAGE( m_StatusIcons, HudItems );

int CHudStatusIcons::Init( void )
{
	HOOK_MESSAGE( StatusIcon );
	HOOK_MESSAGE( HudItems );

	gHUD.AddHudElem( this );

	Reset();

	return 1;
}

int CHudStatusIcons::VidInit( void )
{

	return 1;
}

void CHudStatusIcons::Reset( void )
{
	memset( m_IconList, 0, sizeof m_IconList );
	m_iFlags &= ~HUD_ACTIVE;
}

// Draw status icons along the left-hand side of the screen
int CHudStatusIcons::Draw( float flTime )
{
	if (gEngfuncs.IsSpectateOnly())
		return 1;
	// find starting position to draw from, along right-hand side of screen
	int x = 5;
	int y = ScreenHeight / 2;
	
	// loop through icon list, and draw any valid icons drawing up from the middle of screen
	for ( int i = 0; i < MAX_ICONSPRITES; i++ )
	{
		if ( m_IconList[i].spr )
		{
			y -= ( m_IconList[i].rc.bottom - m_IconList[i].rc.top ) + 5;
			
			SPR_Set( m_IconList[i].spr, m_IconList[i].r, m_IconList[i].g, m_IconList[i].b );
			SPR_DrawAdditive( 0, x, y, &m_IconList[i].rc );
		}
	}
	
	return 1;
}

// Message handler for StatusIcon message
// accepts five values:
//		byte   : TRUE = ENABLE icon, FALSE = DISABLE icon
//		string : the sprite name to display
//		byte   : red
//		byte   : green
//		byte   : blue
int CHudStatusIcons::MsgFunc_StatusIcon( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	int ShouldEnable = READ_BYTE();
	char *pszIconName = READ_STRING();
	if ( ShouldEnable )
	{
		int r = READ_BYTE();
		int g = READ_BYTE();
		int b = READ_BYTE();
		EnableIcon( pszIconName, r, g, b );
		m_iFlags |= HUD_ACTIVE;
	}
	else
	{
		DisableIcon( pszIconName );
	}

	return 1;
}

// XYPHN - 040228
//	Added to use less network traffic
typedef struct hud_items_s
{
	unsigned int	iId;
	const char *	pszIcon;
	int				r, g, b;
} hud_items_t;

int CHudStatusIcons::MsgFunc_HudItems( const char * pszName, int iSize, void * pbuf )
{
	BEGIN_READ( pbuf, iSize );

	int iItems = READ_LONG();
	int bAdd = READ_BYTE();

	static hud_items_t items[] =
	{
		{ HUD_GROUNDED,			"status_grounded",	255, 255, 255,	},
		{ HUD_ROOTED,			"status_rooted",	255, 255, 255,	},
		{ HUD_FROZEN,			"status_frozen",	255, 255, 255,	},
		{ HUD_FROSTED,			"status_frosted",	255, 255, 255,	},
		{ HUD_COCKLEBUR,		"status_cocklebur",	255, 255, 255,	},
		{ HUD_BEARMORPH,		"status_bear",		255, 255, 255,	},
		{ HUD_IMMUNITY,			"status_immune",	255, 255, 255,	},
		{ HUD_YELLOWITEM,		"item_yellow",		255, 255, 255,	},
		{ HUD_GREENITEM,		"item_green",		255, 255, 255,	},
		{ HUD_BLUEITEM,			"item_blue",		255, 255, 255,	},
		{ HUD_REDITEM,			"item_red",			255, 255, 255,	},
		{ HUD_INVISIBILITY,		"status_invis",		255, 255, 255,	},
		{ HUD_INVULNERABILITY,	"status_invul",		255, 255, 255,	},
		{ HUD_PROTECTION,		"status_suit",		255, 255, 255,	},
		{ HUD_SUPERDAMAGE,		"status_quad",		255, 255, 255,	},
		{ HUD_YELLOWITEM2,		"item_yellow",		255, 255, 255,	},
		{ HUD_GREENITEM2,		"item_green",		255, 255, 255,	},
	};

	for( int i = 0; i < ( sizeof(items) / sizeof(items[0])); i++ )
	{
		if( iItems & items[i].iId )
		{
			if( bAdd )
			{
				EnableIcon( (char *)items[i].pszIcon, items[i].r, items[i].g, items[i].b );
				m_iFlags |= HUD_ACTIVE;
			}
			else
				DisableIcon( (char *)items[i].pszIcon );
		}
	}

	return 1;
}


// add the icon to the icon list, and set it's drawing color
void CHudStatusIcons::EnableIcon( char *pszIconName, unsigned char red, unsigned char green, unsigned char blue )
{
	// check to see if the sprite is in the current list
	for ( int i = 0; i < MAX_ICONSPRITES; i++ )
	{
		if ( !stricmp( m_IconList[i].szSpriteName, pszIconName ) )
			break;
	}

	if ( i == MAX_ICONSPRITES )
	{
		// icon not in list, so find an empty slot to add to
		for ( i = 0; i < MAX_ICONSPRITES; i++ )
		{
			if ( !m_IconList[i].spr )
				break;
		}
	}

	// if we've run out of space in the list, overwrite the first icon
	if ( i == MAX_ICONSPRITES )
	{
		i = 0;
	}

	// Load the sprite and add it to the list
	// the sprite must be listed in hud.txt
	int spr_index = gHUD.GetSpriteIndex( pszIconName );
	m_IconList[i].spr = gHUD.GetSprite( spr_index );
	m_IconList[i].rc = gHUD.GetSpriteRect( spr_index );
	m_IconList[i].r = red;
	m_IconList[i].g = green;
	m_IconList[i].b = blue;
	strcpy( m_IconList[i].szSpriteName, pszIconName );
}

void CHudStatusIcons::DisableIcon( char *pszIconName )
{
	// find the sprite is in the current list
	for ( int i = 0; i < MAX_ICONSPRITES; i++ )
	{
		if ( !stricmp( m_IconList[i].szSpriteName, pszIconName ) )
		{
			// clear the item from the list
			memset( &m_IconList[i], 0, sizeof icon_sprite_t );
			return;
		}
	}
}
