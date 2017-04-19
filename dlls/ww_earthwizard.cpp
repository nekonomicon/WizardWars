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
#include "ww_earthwizard.h"

#include "tf_globals.h"
#include "tf_globalinfo.h"
#include "tf_ent.h"


EarthWizard::EarthWizard( CBasePlayer * pPlayer ) : WWBaseWizard( pPlayer )
{
	m_flPlayerSpeed		= SPEED_MEDLOW;
	m_iMaxHealth		= 100;
	m_iMaxArmor			= 135;

	m_bIsBear			= FALSE;
}


const char * EarthWizard::GetWizardModel( void )
{
	if( m_bIsBear )
		return "bear";

	return WWBaseWizard::GetWizardModel();
}


void EarthWizard::DoSpecial( int slot )
{
	if( IsBear() )
		BecomeWizard();
	else
	{
		if( m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] < EARTHWIZARD_SPECIAL_COST )
		{
			ClientPrint(m_pPlayer->pev,HUD_PRINTCENTER,"#LowOnMagic");
			return;
		}

		m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] -= EARTHWIZARD_SPECIAL_COST;

		BecomeBear();
	}
}


//extern int gmsgStatusIcon;
void EarthWizard::BecomeBear( void )
{
	if( !m_pPlayer->IsAlive() )
		return;

	m_bIsBear		= TRUE;
	m_flNextAmmoUse	= 0.0f;

	m_pPlayer->SetPlayerModel();
	m_pPlayer->UpdateSpeed();
	m_pPlayer->RemoveAllItems( FALSE, FALSE );
	m_pPlayer->SetHudItems( HUD_BEARMORPH, TRUE );

	GiveSpells();

	g_engfuncs.pfnSetPhysicsKeyValue( m_pPlayer->edict(), "bear", "1" );

	EMIT_SOUND( m_pPlayer->edict(), CHAN_VOICE, "player/bear_morph.wav", 1.0f, ATTN_NORM );

	m_pPlayer->pev->health		= m_pPlayer->pev->health / m_pPlayer->pev->max_health;
	m_pPlayer->pev->max_health	= 200;
	m_pPlayer->pev->health		= m_pPlayer->pev->health * m_pPlayer->pev->max_health;

	m_pPlayer->pev->armorvalue	= m_pPlayer->pev->armorvalue / m_pPlayer->pev->armortype;
	m_pPlayer->pev->armortype	= 200;
	m_pPlayer->pev->armorvalue	= m_pPlayer->pev->armorvalue * m_pPlayer->pev->armortype;

}


void EarthWizard::BecomeWizard( void )
{
	m_bIsBear = FALSE;
	m_flNextAmmoUse	= 0.0f;

	m_pPlayer->SetPlayerModel();
	m_pPlayer->UpdateSpeed();
	m_pPlayer->RemoveAllItems( FALSE, FALSE );
	m_pPlayer->SetHudItems( HUD_BEARMORPH, FALSE );

	GiveSpells();

	g_engfuncs.pfnSetPhysicsKeyValue( m_pPlayer->edict(), "bear", "0" );


	if( !m_pPlayer->IsAlive() )
		return;

	m_pPlayer->pev->health		= m_pPlayer->pev->health / m_pPlayer->pev->max_health;
	m_pPlayer->pev->max_health	= m_iMaxHealth;
	m_pPlayer->pev->health		= m_pPlayer->pev->health * m_pPlayer->pev->max_health;

	m_pPlayer->pev->armorvalue	= m_pPlayer->pev->armorvalue / m_pPlayer->pev->armortype;
	m_pPlayer->pev->armortype	= m_iMaxArmor;
	m_pPlayer->pev->armorvalue	= m_pPlayer->pev->armorvalue * m_pPlayer->pev->armortype;
}


void EarthWizard::GiveSpells( void )
{
	if( !IsBear() )
	{
		m_pPlayer->GiveNamedItem( "ww_staff"				);
		m_pPlayer->GiveNamedItem( "ww_spell_shield"			);
		m_pPlayer->GiveNamedItem( "ww_spell_seal"			);
		m_pPlayer->GiveNamedItem( "ww_spell_pebbleblast"	);
		m_pPlayer->GiveNamedItem( "ww_spell_stone"			);
		m_pPlayer->GiveNamedItem( "ww_spell_bird"			);
	}
	else
		m_pPlayer->GiveNamedItem( "ww_bearbite"				);
}


void EarthWizard::Equip( void )
{
	if( IsBear() )
		BecomeWizard();

	m_bIsBear			= FALSE;
	m_flNextAmmoUse		= 0.0f;

	WWBaseWizard::Equip();
}

void EarthWizard::Remove( void )
{
	g_engfuncs.pfnSetPhysicsKeyValue( m_pPlayer->edict(), "bear", "0" );

	if( IsBear() )
		BecomeWizard();

	WWBaseWizard::Remove();
}


void EarthWizard::Think( void )
{
	if( IsBear() )
	{
		if( m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] < 5 )
			BecomeWizard();

		if( gpGlobals->time >= m_flNextAmmoUse )
		{
			m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] -= 5;
			m_flNextAmmoUse = gpGlobals->time + 2.5f;
		}

		if( !m_pPlayer->IsAlive() )
			BecomeWizard();
	}

/*
	// XYPHN - 040213
	//	Removed, was causing problems elsewhere.
	if( m_pPlayer->pev->renderamt < 255 && !IsBear() )
		m_pPlayer->pev->renderamt -= 5;

	if( m_pPlayer->pev->renderamt <= 0 && !IsBear() )
		BecomeBear();

	if( m_pPlayer->pev->renderamt < 255 && IsBear() )
		m_pPlayer->pev->renderamt += 5;

	if( m_pPlayer->pev->renderamt > 255 )
		m_pPlayer->pev->renderamt = 255;
*/

	WWBaseWizard::Think();
}


BOOL EarthWizard::CanHavePlayerItem( CBasePlayerItem * pWeapon )
{
	if( IsBear() )
	{
		if( !stricmp( STRING( pWeapon->pev->classname ), "ww_bearbite" ) )
			return TRUE;

		return FALSE;
	}

	return WWBaseWizard::CanHavePlayerItem( pWeapon );
}