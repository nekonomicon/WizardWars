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
#include "ww_firewizard.h"

#include "tf_globals.h"
#include "tf_globalinfo.h"
#include "tf_ent.h"
#include "tf_info_goal.h"


FireWizard::FireWizard( CBasePlayer * pPlayer ) : WWBaseWizard( pPlayer )
{
	m_flPlayerSpeed		= SPEED_LOW;
	m_iMaxHealth		= 100;
	m_iMaxArmor			= 150;
}


void FireWizard::GiveSpells( void )
{
	m_pPlayer->GiveNamedItem( "ww_staff"			);
	m_pPlayer->GiveNamedItem( "ww_spell_shield"		);
	m_pPlayer->GiveNamedItem( "ww_spell_seal"		);
	m_pPlayer->GiveNamedItem( "ww_spell_missile"	);
	m_pPlayer->GiveNamedItem( "ww_spell_fireball"	);
	m_pPlayer->GiveNamedItem( "ww_spell_flamelick"	);
}


void FireWizard::DoSpecial( int slot )
{
	if( !m_pPlayer )
		return;

	// fissure requires a confirmation
	if( slot != 1 )
		return;

	// 1.2.6 #1 8 seconds between fissures
	if( gpGlobals->time < m_flNextSpecialTime)
		return;

	m_flNextSpecialTime = gpGlobals->time + 8.0f;

	if( m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] < FIREWIZARD_SPECIAL_COST )
	{
		ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#LowOnMagic" );
		return;
	}

	m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] -= FIREWIZARD_SPECIAL_COST;

	Vector vOrigin, vAngles;

	vAngles = m_pPlayer->pev->v_angle;
	vAngles.x = 0;

	UTIL_MakeVectors( vAngles );

	vOrigin = m_pPlayer->pev->origin + gpGlobals->v_forward * 52.0f;
	vOrigin.z += 8;

	CBaseEntity * pEnt = CBaseEntity::Create( "proj_fissure", vOrigin, vAngles, m_pPlayer->edict() );

	EMIT_SOUND( m_pPlayer->edict(), CHAN_WEAPON, "spells/fissure_cast.wav", 1.0f, ATTN_NORM );

	StopForTime( 3.0f );

	return;
}


class WWFissure : public CBaseMonster
{
public:
	void Spawn( void )
	{
		Precache();

		SET_MODEL  ( edict(), "models/fissure.mdl" );
		SET_SIZE   ( edict(), Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );
		SET_ORIGIN ( edict(), pev->origin );

		pev->movetype	= MOVETYPE_FLY;
		pev->solid		= SOLID_NOT;
		pev->rendermode	= kRenderTransColor;
		pev->renderamt	= 0;

		pev->sequence = 1;
		ResetSequenceInfo();

		SetThink(&WWFissure :: LengthenThink );
		pev->nextthink = gpGlobals->time + 0.5f;
	};

	void Precache( void )
	{
		PRECACHE_SOUND( "spells/fissure_boom.wav" );
		PRECACHE_MODEL( "models/fissure.mdl" );
	};

	void EXPORT LengthenThink( void )
	{
		pev->rendermode	= kRenderNormal;
		pev->renderamt	= 255;

		pev->sequence = 1;
		pev->frame = 0;
		ResetSequenceInfo();

		SetThink(&WWFissure :: WiddenThink );
		pev->nextthink = gpGlobals->time + 1.0f;
	};

	void EXPORT WiddenThink( void )
	{
		pev->sequence = 2;
		pev->frame = 0;
		ResetSequenceInfo();

		SetThink(&WWFissure :: CrackThink );
		pev->nextthink = gpGlobals->time + 1.0f;
	};


	void EXPORT CrackThink( void )
	{
		CBasePlayer * pPlayer = NULL;

		if( pev->owner != NULL )
			pPlayer = (CBasePlayer *)CBaseEntity::Instance( pev->owner );

		CBaseEntity * pVictim = NULL;

		while( ( pVictim = UTIL_FindEntityInSphere( pVictim, pev->origin, FIREWIZARD_FISSURE_RADIUS ) ) != NULL )
		{
			if( pVictim == pPlayer )
				continue;

			TraceResult tr;
			UTIL_TraceLine( pev->origin, pVictim->pev->origin, ignore_monsters, edict(), &tr );

			if( tr.flFraction != 1.0f )
				continue;

			if( pVictim->pev->takedamage )
				pVictim->TakeDamage( pev, VARS(pev->owner), FIREWIZARD_FISSURE_DAMAGE, DMG_BLAST );

			if( pVictim->Classify() == TF_ENT_INFO_GOAL )
			{
				TFEntity * pTF = (TFInfoGoal *)pVictim;

				if( pTF != NULL && pTF->m_iGoalActivation & TF_GOAL_ACTIVATION_DETPACK_EXPLOSION )
					pTF->GoalActivate( pPlayer );
			}
		}

		EMIT_SOUND( edict(), CHAN_BODY, "spells/fissure_boom.wav", 1.0f, ATTN_NORM );
		UTIL_Remove( this );
	};
};

LINK_ENTITY_TO_CLASS( proj_fissure, WWFissure );
