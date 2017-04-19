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
#include "ww_deathwizard.h"

#include "tf_globals.h"
#include "tf_globalinfo.h"
#include "tf_ent.h"


DeathWizard::DeathWizard( CBasePlayer * pPlayer ) : WWBaseWizard( pPlayer )
{
	m_flPlayerSpeed		= SPEED_MEDLOW;
	m_iMaxHealth		= 100;
	m_iMaxArmor			= 120;
}


void DeathWizard::Equip( void )
{
	WWBaseWizard::Equip();

	m_bMadeSoul = FALSE;
}


void DeathWizard::GiveSpells( void )
{
	m_pPlayer->GiveNamedItem( "ww_staff"			);
	m_pPlayer->GiveNamedItem( "ww_spell_shield"		);
	m_pPlayer->GiveNamedItem( "ww_spell_seal"		);
	m_pPlayer->GiveNamedItem( "ww_spell_missile"	);
	m_pPlayer->GiveNamedItem( "ww_spell_skull"		);
	m_pPlayer->GiveNamedItem( "ww_spell_deathray"	);
}


void DeathWizard::DoSpecial( int slot )
{
	if( m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] < DEATHWIZARD_SPECIAL_COST )
	{
		ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#LowOnMagic" );
		return;
	}

	CBaseEntity * pBody = UTIL_FindEntityGeneric( "bodyque", m_pPlayer->pev->origin, 100 );

	if( pBody && !( pBody->pev->effects & EF_NODRAW ) )
	{
		m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] -= DEATHWIZARD_SPECIAL_COST;

		pBody->pev->effects |= EF_NODRAW;

		ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#Death_DrawEnergy" );

		EMIT_SOUND( ENT(m_pPlayer->pev), CHAN_BODY, "spells/necro_grimfeast.wav", 1.0f, ATTN_NORM );

		m_pPlayer->pev->health		= m_iMaxHealth;
		m_pPlayer->m_bitsDamageType	= 0;
	}
	else
		ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#Death_NoBody" );
}


void DeathWizard::Think( void )
{
	if( m_pPlayer->pev->health <= 0 && !m_bMadeSoul )
	{
		m_bMadeSoul = TRUE;

//		CBaseEntity * pEnt = CBaseEntity::Create( "deathsoul", m_pPlayer->pev->origin, m_pPlayer->pev->angles, m_pPlayer->edict() );
	}

	WWBaseWizard::Think();
}