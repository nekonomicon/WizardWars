#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "soundent.h"
#include "decals.h"
#include "gamerules.h"
#include "shake.h"
#include "enginecallback.h"

#include "ww_shared/ww_defs.h"
#include "ww_wizards.h"
#include "ww_lightningwizard.h"

#include "tf_globals.h"
#include "tf_globalinfo.h"
#include "tf_ent.h"


LightningWizard::LightningWizard( CBasePlayer *pPlayer ): WWBaseWizard( pPlayer )
{
	m_flPlayerSpeed		= SPEED_MED;
	m_iMaxHealth		= 90;
	m_iMaxArmor			= 90;
}


void LightningWizard::GiveSpells( void )
{
	m_pPlayer->GiveNamedItem( "ww_staff"				);
	m_pPlayer->GiveNamedItem( "ww_spell_shield"			);
	m_pPlayer->GiveNamedItem( "ww_spell_seal"			);
	m_pPlayer->GiveNamedItem( "ww_spell_lightningbolt"	);
	m_pPlayer->GiveNamedItem( "ww_spell_spotbolt"		);
}


void LightningWizard::DoSpecial( int slot )
{
	if( !m_pPlayer )
		return;
}