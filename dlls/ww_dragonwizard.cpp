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
#include "ww_dragonwizard.h"

#include "tf_globals.h"
#include "tf_globalinfo.h"
#include "tf_ent.h"


DragonWizard::DragonWizard( CBasePlayer * pPlayer ) : WWBaseWizard( pPlayer )
{
	m_flPlayerSpeed		= SPEED_MEDLOW;
	m_iMaxHealth		= 100;
	m_iMaxArmor			= 135;
}


void DragonWizard::GiveSpells( void )
{
	m_pPlayer->GiveNamedItem( "ww_staff"				);
	m_pPlayer->GiveNamedItem( "ww_spell_shield"			);
	m_pPlayer->GiveNamedItem( "ww_spell_seal"			);
	m_pPlayer->GiveNamedItem( "ww_spell_doublemissile"	);
	m_pPlayer->GiveNamedItem( "ww_spell_dragonbreath"	);
	m_pPlayer->GiveNamedItem( "ww_spell_wyvern"			);
}


void DragonWizard::DoSpecial( int slot )
{
/*
	// requires confirmation
	if( slot != 1 )
		return;

	if( m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] < DRAGONWIZARD_SPECIAL_COST )
	{
		ClientPrint(m_pPlayer->pev,HUD_PRINTCENTER,"#LowOnMagic");
		return;
	}

	m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] -= DRAGONWIZARD_SPECIAL_COST;

	float flAmount = ( m_pPlayer->pev->max_health - m_pPlayer->pev->health ) / m_pPlayer->pev->max_health;

	MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, m_pPlayer->pev->origin );
		WRITE_BYTE ( TE_EXPLOSION );				// This makes a dynamic light and the explosion sprites/sound
		WRITE_COORD( m_pPlayer->pev->origin.x );	// Send to PAS because of the sound
		WRITE_COORD( m_pPlayer->pev->origin.y );
		WRITE_COORD( m_pPlayer->pev->origin.z );
		WRITE_SHORT( g_sModelIndexFireball );
		WRITE_BYTE ( 80 * flAmount );				// scale * 10
		WRITE_BYTE ( 15  );							// framerate
		WRITE_BYTE ( TE_EXPLFLAG_NONE );
	MESSAGE_END();

	EMIT_SOUND( m_pPlayer->edict(), CHAN_WEAPON, "spells/draco_explode.wav", 1.0f, ATTN_NORM );

	m_pPlayer->TakeDamage( m_pPlayer->pev, m_pPlayer->pev, 1000.0f, DMG_SACRAFICE );

	RadiusDamage( m_pPlayer->pev->origin, m_pPlayer->pev, m_pPlayer->pev, DRAGONWIZARD_SPECIAL_STRENGTH * flAmount, DRAGONWIZARD_SPECIAL_STRENGTH * flAmount, CLASS_NONE, DMG_BURN|DMG_BLAST );
*/
}
