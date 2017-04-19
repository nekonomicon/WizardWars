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
#include "ww_lifewizard.h"

#include "tf_globals.h"
#include "tf_globalinfo.h"
#include "tf_ent.h"


LifeWizard::LifeWizard( CBasePlayer * pPlayer ) : WWBaseWizard( pPlayer )
{
	m_flPlayerSpeed		= SPEED_HIGH;
	m_iMaxHealth		= 75;
	m_iMaxArmor			= 80;

	m_flNextHealthGain	= 0.0f;
	m_flSacrafise		= 0.0f;
	m_flNextSacrafise	= 0.0f;
}


void LifeWizard::Equip( void )
{
	m_bMadeSoul			= FALSE;
	m_flNextHealthGain	= 0.0f;
	m_flSacrafise		= 0.0f;
	m_flNextSacrafise	= 0.0f;

	WWBaseWizard::Equip();
}


void LifeWizard::GiveSpells( void )
{
	m_pPlayer->GiveNamedItem( "ww_staff"				);
	m_pPlayer->GiveNamedItem( "ww_spell_shield"			);
	m_pPlayer->GiveNamedItem( "ww_spell_seal"			);
	m_pPlayer->GiveNamedItem( "ww_spell_doublemissile"	);
	m_pPlayer->GiveNamedItem( "ww_spell_whiteray"		);
	m_pPlayer->GiveNamedItem( "ww_spell_forceblast"		);
}


void LifeWizard::Think( void )
{
	if( m_pPlayer->IsAlive() && m_flSacrafise != 0.0f && gpGlobals->time > m_flSacrafise )
		Sacrafise();

	if( m_pPlayer->IsAlive() && gpGlobals->time >= m_flNextHealthGain )
	{
		m_pPlayer->TakeHealth( 4, DMG_GENERIC );
		m_pPlayer->m_bitsDamageType = 0;
		m_flNextHealthGain = gpGlobals->time + 5.0f;
	}

	if( m_pPlayer->pev->health <= 0 && !m_bMadeSoul )
	{
		m_bMadeSoul = TRUE;
//		CBaseEntity * pEnt = CBaseEntity::Create( "lifesoul", m_pPlayer->pev->origin, m_pPlayer->pev->angles, m_pPlayer->edict() );
	}


	WWBaseWizard::Think();
}


void LifeWizard::DoSpecial( int slot )
{
	if( !m_pPlayer )
		return;
	if(m_flNextSacrafise > gpGlobals->time)
		return;

	if( m_flSacrafise != 0.0f )
		return;

	// we can't sacrafise ourselves if we are already dead
	// CAN THIS EVEN HAPPEN?!!! IF SO, FIX IT
	if( !m_pPlayer->IsAlive() )
		return;

	// sacrafise requires a confirmation
	if( slot != 1 )
		return;

	if( m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] < LIFEWIZARD_SPECIAL_COST )
	{
		ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#LowOnMagic" );
		return;
	}

	EMIT_SOUND( m_pPlayer->edict(), CHAN_BODY, "spells/healer_sacrifice.wav", 1.0f, ATTN_NORM );

	m_flSacrafise = gpGlobals->time + 4.0f;
}

void LifeWizard::Sacrafise( void )
{
	m_flNextSacrafise = gpGlobals->time + 5.0f;

	m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] -= LIFEWIZARD_SPECIAL_COST;

	m_pPlayer->TakeDamage( m_pPlayer->pev, m_pPlayer->pev, 1000.0f, DMG_SACRAFICE );

	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, m_pPlayer->pev->origin );
		WRITE_BYTE ( TE_BEAMTORUS );
		WRITE_COORD( m_pPlayer->pev->origin.x );
		WRITE_COORD( m_pPlayer->pev->origin.y );
		WRITE_COORD( m_pPlayer->pev->origin.z );
		WRITE_COORD( m_pPlayer->pev->origin.x );
		WRITE_COORD( m_pPlayer->pev->origin.y );
		WRITE_COORD( m_pPlayer->pev->origin.z + 300 );
	 	WRITE_SHORT( g_sModelIndexSmoke );
		WRITE_BYTE ( 0   );		// startframe
		WRITE_BYTE ( 0   );		// framerate
		WRITE_BYTE ( 8   );		// life
		WRITE_BYTE ( 20  );		// width
		WRITE_BYTE ( 20  );		// noise
		WRITE_BYTE ( 0   );		// r
		WRITE_BYTE ( 0   );		// g
		WRITE_BYTE ( 255 );		// b
		WRITE_BYTE ( 255 );		// a
		WRITE_BYTE ( 1   );		// speed
	MESSAGE_END();

	CBaseEntity * pEntity = NULL;
	
	while( ( pEntity = UTIL_FindEntityInSphere( pEntity, m_pPlayer->pev->origin, 250 ) ) != NULL )
	{
		if( pEntity == m_pPlayer )
			continue;

		if( !pEntity->IsPlayer() )
			continue;

		if(  m_pPlayer->IRelationship( pEntity ) < R_NO )
		{
			pEntity->TakeHealth( 100, DMG_SACRAFICE );//1.2.1 was 300
			pEntity->pev->armorvalue = pEntity->pev->armortype;

			if( ((CBasePlayer*)pEntity)->m_bitsDamageType & DMG_POISON )
			{
				m_pPlayer->pev->frags++;

				if(((CBasePlayer*)pEntity)->m_bitsDamageType & DMG_POISONCONTAG)
					pEntity->SetTouch(NULL); // not contagious

				((CBasePlayer*)pEntity)->m_bitsDamageType = 0;
			}
		}
		else
		{
			UTIL_ScreenFade( pEntity, Vector( 255, 255, 255 ), 2.0f,2.0f, 255.0f, FFADE_IN );

			// 1.2.1 more damage!
//			pEntity->pev->health		= (int)( pEntity->pev->health		/ 2.0f );
//			pEntity->pev->armorvalue	= (int)( pEntity->pev->armorvalue	/ 2.0f );
			((CBasePlayer*)pEntity)->TakeDamage( m_pPlayer->pev, m_pPlayer->pev, 200.0f, DMG_SACRAFICE );
		}
	}

	m_pPlayer->AddPoints( 1, FALSE );
}
