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
#include "ww_archmage.h"

#include "tf_globals.h"
#include "tf_globalinfo.h"
#include "tf_ent.h"


ArchMage::ArchMage( CBasePlayer * pPlayer ) : WWBaseWizard( pPlayer )
{
	m_flPlayerSpeed		= SPEED_LOW;
	m_iMaxHealth		= 60;
	m_iMaxArmor			= 40;
}


void ArchMage::Equip( void )
{
	WWBaseWizard::Equip();
}


void ArchMage::GiveSpells( void )
{
	m_pPlayer->GiveNamedItem( "ww_staff"			);
	m_pPlayer->GiveNamedItem( "ww_spell_shield"		);
	m_pPlayer->GiveNamedItem( "ww_spell_missile"	);
}
