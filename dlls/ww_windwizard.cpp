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
#include "ww_windwizard.h"
#include "tf_globals.h"
#include "tf_globalinfo.h"
#include "tf_ent.h"


WindWizard::WindWizard( CBasePlayer * pPlayer ) : WWBaseWizard( pPlayer )
{
	m_flPlayerSpeed		= SPEED_HIGH;
	m_iMaxHealth		= 80;
	m_iMaxArmor			= 80;
}


void WindWizard::Equip( void )
{
	WWBaseWizard::Equip();

	g_engfuncs.pfnSetPhysicsKeyValue( m_pPlayer->edict(), "windwizard", "1" );

	m_flNextCloneTime = 0.0f;

}


void WindWizard::GiveSpells( void )
{
	m_pPlayer->GiveNamedItem( "ww_staff"			);
	m_pPlayer->GiveNamedItem( "ww_spell_shield"		);
	m_pPlayer->GiveNamedItem( "ww_spell_seal"		);
	m_pPlayer->GiveNamedItem( "ww_spell_missile"	);
	m_pPlayer->GiveNamedItem( "ww_spell_levitate"	);
	m_pPlayer->GiveNamedItem( "ww_spell_updraft"	);
}


void WindWizard::DoSpecial( int slot )
{
	if( !m_pPlayer )
		return;
/*
	if( gpGlobals->time < m_flNextCloneTime )
		return;

	if( m_pPlayer->m_rgAmmo[ m_pPlayer->GetAmmoIndex( "mana" ) ] < WINDWIZARD_SPECIAL_COST )
	{
		ClientPrint(m_pPlayer->pev,HUD_PRINTCENTER,"#LowOnMagic");
		return;
	}

	if( !FBitSet( m_pPlayer->pev->flags, FL_ONGROUND ) )
	{
		ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#Wind_AirClone" );
		return;
	}

	Vector	vDir;
	float	flSpeed;

	vDir = UTIL_VecToAngles( m_pPlayer->pev->velocity );
	flSpeed = m_pPlayer->pev->velocity.Length();

	if( flSpeed < 200 )
	{
		ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#Wind_SlowClone" );
		return;
	}

	m_flNextCloneTime = gpGlobals->time + WINDWIZARD_SPECIAL_DELAY;

	m_pPlayer->m_rgAmmo[ m_pPlayer->GetAmmoIndex( "mana" ) ] -= WINDWIZARD_SPECIAL_COST;

	int x = 30;
	if( RANDOM_FLOAT( -1.0f, 1.0f ) < 0.0f )
		x *= -1;
	
	m_pPlayer->pev->v_angle.x	= 0;
	m_pPlayer->pev->v_angle.y	= m_pPlayer->pev->angles.y + x;

	m_pPlayer->pev->angles		= m_pPlayer->pev->v_angle;
	m_pPlayer->pev->fixangle	= TRUE;

	UTIL_MakeVectors( m_pPlayer->pev->angles );

	m_pPlayer->pev->velocity	= gpGlobals->v_forward * flSpeed;
	m_pPlayer->pev->velocity.z	= 0;
	
	vDir.y = vDir.y - x;

	CBaseEntity::Create( "monster_wizardclone", m_pPlayer->pev->origin, vDir, m_pPlayer->edict() );
*/
}


void WindWizard::Think()
{
	WWBaseWizard::Think();
}


void WindWizard::Remove()
{
	WWBaseWizard::Remove();

	g_engfuncs.pfnSetPhysicsKeyValue( m_pPlayer->edict(), "windwizard", "0" );
}