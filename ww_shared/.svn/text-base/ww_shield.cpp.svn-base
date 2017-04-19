#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_shield.h"


LINK_ENTITY_TO_CLASS( ww_spell_shield, WWShieldSpell );


void WWShieldSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_SHIELD;
	m_iDefaultAmmo	= 0;

	FallInit();
}


void WWShieldSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/armorspell.wav"				);
	PRECACHE_MODEL( "sprites/effects/armorspell.spr"	);
	PRECACHE_MODEL( "models/spellbook.mdl"				);

	m_usEvent = PRECACHE_EVENT( 1, "events/spells/shield.sc" );
}


int WWShieldSpell::iItemSlot( void )
{
	return SLOT_SHIELD + 1;
}


int WWShieldSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_SHIELD;
	pInfo->iSlot		= SLOT_SHIELD;
	pInfo->iPosition	= POS_SHIELD;
	pInfo->iWeight		= WEIGHT_SHIELD;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWShieldSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < SHIELD_COST )
		return FALSE;
	return TRUE;
}


BOOL WWShieldSpell::Deploy( void )
{
	if( m_pPlayer->m_pClass != NULL )
	{
		return DefaultDeploy(
			(char *)m_pPlayer->m_pClass->GetWizardHands(),
			(char *)m_pPlayer->m_pClass->GetWizardStaff(),
			HANDS_SHIELD_IDLE, "armor" );
	}
	else
		return DefaultDeploy( "", "", HANDS_SHIELD_IDLE, "armor" );
}


void WWShieldSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < SHIELD_COST )
	{
		RetireWeapon();
		return;
	}

	m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= SHIELD_COST;
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 ); 

#ifndef CLIENT_DLL
	m_pPlayer->pev->armorvalue += SHIELD_REPAIR;

	if( m_pPlayer->pev->armorvalue > m_pPlayer->pev->armortype )
		m_pPlayer->pev->armorvalue = m_pPlayer->pev->armortype;
#endif

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + SHIELD_DELAY;
	m_flTimeWeaponIdle	  = m_flNextPrimaryAttack;

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent, 0.0f,
		(float *)&g_vecZero, (float *)&g_vecZero, 0, 0, 0, 0, 0, 0 );
}


void WWShieldSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < SHIELD_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent, 0.0f,
		(float *)&g_vecZero, (float *)&g_vecZero, 0, 0, 1, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}


#ifndef CLIENT_DLL

class WWSpellAmmo : public CBasePlayerAmmo
{
	void Spawn( void )
	{
		Precache();
		SET_MODEL( edict(), "models/w_spellbook.mdl" );
		CBasePlayerAmmo::Spawn();
	}

	void Precache( void )
	{
		PRECACHE_MODEL( "models/w_spellbook.mdl"	);
		PRECACHE_SOUND( "items/gunpickup2.wav"		);
	}

	BOOL AddAmmo( CBaseEntity * pOther )
	{
		if(CVAR_GET_FLOAT("mp_combochaos")>0 && pOther->IsPlayer())
		{
			CBasePlayer *pPlayer=(CBasePlayer*)pOther;

			for(int i=0;i<MAX_ITEM_TYPES;i++)
			{
				if( pPlayer->m_rgpPlayerItems[i] &&
					pPlayer->m_rgpPlayerItems[i]->iItemSlot() == SLOT_COMBOSPELL &&
					pPlayer->m_rgpPlayerItems[i]->iItemPosition() == POS_COMBO)
				{
					return FALSE;
				}
			}

			if(pPlayer->m_pClass==NULL)
				return FALSE;

			int x=(int)RANDOM_FLOAT(0,2);

			switch(pPlayer->pev->playerclass)
			{
			case(WWCLASS_LIFE):
				if(x==1)
					pPlayer->GiveNamedItem("weapon_spiritwizspell");
				else
					pPlayer->GiveNamedItem("weapon_wombatspell");
				break;
			case(WWCLASS_FIRE):
				if(x==1)
					pPlayer->GiveNamedItem("weapon_meteorspell");
				else
					pPlayer->GiveNamedItem("weapon_balllightningspell");
				break;
			case(WWCLASS_ICE):
				if(x==1)
					pPlayer->GiveNamedItem("weapon_lightningcloudspell");
				else
					pPlayer->GiveNamedItem("weapon_giantplantspell");
				break;
			case(WWCLASS_NATURE):
				if(x==1)
					pPlayer->GiveNamedItem("weapon_spiritwizspell");
				else
					pPlayer->GiveNamedItem("weapon_giantplantspell");
				break;
			case(WWCLASS_LIGHTNING):
				if(x==1)
					pPlayer->GiveNamedItem("weapon_lightningcloudspell");
				else
					pPlayer->GiveNamedItem("weapon_balllightningspell");
				break;
			case(WWCLASS_DEATH):
				if(x==1)
					pPlayer->GiveNamedItem("weapon_meteorspell");
				else
					pPlayer->GiveNamedItem("weapon_skeletonspell");
				break;
			case(WWCLASS_EARTH):
				if(x==1)
					pPlayer->GiveNamedItem("weapon_tornadospell");
				else
					pPlayer->GiveNamedItem("weapon_wombatspell");
				break;
			case(WWCLASS_WIND):
				if(x==1)
					pPlayer->GiveNamedItem("weapon_tornadospell");
				else
					pPlayer->GiveNamedItem("weapon_dragonspell");
				break;			
			case(WWCLASS_DRAGON):
				if(x==1)
					pPlayer->GiveNamedItem("weapon_dragonspell");
				else
					pPlayer->GiveNamedItem("weapon_skeletonspell");
				break;			
			case(WWCLASS_ARCHMAGE):
				pPlayer->GiveNamedItem("weapon_cometspell");
				break;
			}

			return TRUE;
		}
		else if( pOther->GiveAmmo( MAX_MANA * 0.25f, "mana", MAX_MANA ) != -1 )
		{
			EMIT_SOUND( edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );
			return TRUE;
		}

		return FALSE;
	}
};


LINK_ENTITY_TO_CLASS( ww_ammo_spellbook,	WWSpellAmmo );
LINK_ENTITY_TO_CLASS( ammo_spellbook,		WWSpellAmmo );
LINK_ENTITY_TO_CLASS( ammo_9mmclip,			WWSpellAmmo );
LINK_ENTITY_TO_CLASS( ammo_9mmAR,			WWSpellAmmo );
LINK_ENTITY_TO_CLASS( ammo_9mmbox,			WWSpellAmmo );
LINK_ENTITY_TO_CLASS( ammo_buckshot,		WWSpellAmmo );
LINK_ENTITY_TO_CLASS( ammo_crossbow,		WWSpellAmmo );
LINK_ENTITY_TO_CLASS( ammo_357,				WWSpellAmmo );
LINK_ENTITY_TO_CLASS( ammo_rpgclip,			WWSpellAmmo );
LINK_ENTITY_TO_CLASS( ammo_gaussclip,		WWSpellAmmo );
LINK_ENTITY_TO_CLASS( weapon_shotgun,		WWSpellAmmo );
LINK_ENTITY_TO_CLASS( weapon_crowbar,		WWSpellAmmo );
LINK_ENTITY_TO_CLASS( weapon_9mmhandgun,	WWSpellAmmo );
LINK_ENTITY_TO_CLASS( weapon_9mmAR,			WWSpellAmmo );
LINK_ENTITY_TO_CLASS( weapon_357,			WWSpellAmmo );
LINK_ENTITY_TO_CLASS( weapon_gauss,			WWSpellAmmo );
LINK_ENTITY_TO_CLASS( weapon_rpg,			WWSpellAmmo );
LINK_ENTITY_TO_CLASS( weapon_crossbow,		WWSpellAmmo );
LINK_ENTITY_TO_CLASS( weapon_egon,			WWSpellAmmo );
LINK_ENTITY_TO_CLASS( weapon_tripmine,		WWSpellAmmo );
LINK_ENTITY_TO_CLASS( weapon_satchel,		WWSpellAmmo );
LINK_ENTITY_TO_CLASS( weapon_handgrenade,	WWSpellAmmo );
LINK_ENTITY_TO_CLASS( weapon_snark,			WWSpellAmmo );
LINK_ENTITY_TO_CLASS( weapon_hornetgun,		WWSpellAmmo );


class WWSatchelAmmo : public CBasePlayerAmmo
{
	void Spawn( void )
	{
		Precache();
		SET_MODEL( edict(), "models/w_satchels.mdl" );
		CBasePlayerAmmo::Spawn();
	}

	void Precache( void )
	{
		PRECACHE_MODEL( "models/w_satchels.mdl"	);
		PRECACHE_SOUND( "items/gunpickup2.wav"	);
	}

	BOOL AddAmmo( CBaseEntity * pOther )
	{
		if( pOther->GiveAmmo( 1, "satchels", MAX_SATCHELS ) != -1 )
		{
			EMIT_SOUND( edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );
			return TRUE;
		}

		return FALSE;
	}
};


LINK_ENTITY_TO_CLASS( ww_ammo_satchels,		WWSatchelAmmo );
LINK_ENTITY_TO_CLASS( ammo_satchels,		WWSatchelAmmo );
LINK_ENTITY_TO_CLASS( ammo_ARgrenades,		WWSatchelAmmo );
LINK_ENTITY_TO_CLASS( ammo_mp5grenades,		WWSatchelAmmo );


#endif
