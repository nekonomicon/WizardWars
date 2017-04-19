#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"

#include "ww_shared/ww_staff.h"

#ifndef CLIENT_DLL
#include "ww_thornbush.h"
#include "ww_wizards.h"
#include "ww_icewizard.h"	
#endif

LINK_ENTITY_TO_CLASS( ww_staff, WWStaff );

#define STAFF_COMBOHITS			5

char* combo_array[WWCLASS_ARCHMAGE+1][WWCLASS_ARCHMAGE+1] = {
	{"","","","","","","","","","",""},
	{"","","","","","","weapon_tornadospell","weapon_dragonspell","","",""},
	{"","","","weapon_balllightningspell","","","","","weapon_lightningcloudspell","",""},
	{"","","weapon_balllightningspell","","weapon_meteorspell","","","","","",""},
	{"","","","weapon_meteorspell","","","","weapon_skeletonspell","","",""},
	{"","","","","","","weapon_wombatspell","","","weapon_spiritwizspell",""},
	{"","weapon_tornadospell","","","","weapon_wombatspell","","","","",""},
	{"","weapon_dragonspell","","","weapon_skeletonspell","","","","","",""},
	{"","","weapon_lightningcloudspell","","","","","","","weapon_giantplantspell",""},
	{"","","","","","weapon_spiritwizspell","","","weapon_giantplantspell","",""},
	{"","","","","","","","","","",""}
};

void WWStaff::Spawn( void )
{
	Precache();

	m_iId = WEAPON_STAFF;
	m_iSwingCount = 0;

	FallInit();
}


void WWStaff::Precache( void )
{
	PRECACHE_SOUND( "spells/staff_hitbod1.wav"	);
	PRECACHE_SOUND( "spells/staff_hitbod2.wav"	);
	PRECACHE_SOUND( "spells/staff_hitbod3.wav"	);
	PRECACHE_SOUND( "spells/staff_miss1.wav"	);

	m_usEvent = PRECACHE_EVENT( 1, "events/spells/staff.sc" );
}


int WWStaff::iItemSlot( void )
{
	return SLOT_STAFF + 1;
}


int WWStaff::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_STAFF;
	pInfo->iSlot		= SLOT_STAFF;
	pInfo->iPosition	= POS_STAFF;
	pInfo->iWeight		= WEIGHT_STAFF;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= NULL;
	pInfo->iMaxAmmo1	= 0;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWStaff::Deploy( void )
{
	if( m_pPlayer->m_pClass != NULL )
	{
		return DefaultDeploy(
			(char *)m_pPlayer->m_pClass->GetWizardHands(),
			(char *)m_pPlayer->m_pClass->GetWizardStaff(),
			HANDS_IDLE, "staff" );
	}
	else
		return DefaultDeploy( "", "", HANDS_IDLE, "staff" );
}


void WWStaff::PrimaryAttack( void )
{
	if( m_flNextPrimaryAttack > UTIL_WeaponTimeBase() )
		return;

	Attack();

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + STAFF_DELAY;
	m_flTimeWeaponIdle	  = m_flNextPrimaryAttack;
}


void WWStaff::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	m_iSwingCount = 0;

	PLAYBACK_EVENT_FULL( FEV_HOSTONLY | FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles,	0, 0, 0, 0, 0, 1 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}


bool WWStaff::Attack( void )
{
	int bHit = 0;

	Vector vOrigin = m_pPlayer->GetGunPosition();
	Vector vAngles = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;

	UTIL_MakeVectors( vAngles );

	Vector vEndPos = vOrigin + gpGlobals->v_forward * STAFF_RANGE;

	TraceResult tr;
	UTIL_TraceLine( vOrigin, vEndPos, dont_ignore_monsters, m_pPlayer->edict(), &tr );

	if( tr.flFraction == 1.0f )
		UTIL_TraceHull( vOrigin, vEndPos, dont_ignore_monsters, head_hull, m_pPlayer->edict(), &tr );

	if( tr.flFraction != 1.0f )
	{
		bHit = 1;

#ifndef CLIENT_DLL
		CBaseEntity * pHit = CBaseEntity::Instance( tr.pHit );

		if( pHit != NULL )
		{
			if(!ComboPlayer(pHit))
			{
				AttackPlayer( pHit, &tr );
			}
			bHit = 2;
		}
#endif
	}

	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles,
		0, 0, m_pPlayer->pev->playerclass, m_iSwingCount, bHit, 0 );

	m_iSwingCount++;
	return ( bHit ? true : false );
}


void WWStaff::AttackPlayer( CBaseEntity * pVictim, TraceResult * pTrace )
{
#ifndef CLIENT_DLL
	if( !pVictim )
		return;

	float flDamage = STAFF_DAMAGE;
	int bitsDamage = DMG_GENERIC;

	// enchant thornbushes
	if( FClassnameIs( pVictim->pev, "thornbush" ) && m_pPlayer->IRelationship( pVictim ) < R_NO )
	{
		if( m_pPlayer->pev->playerclass == WWCLASS_NATURE )
		{
			pVictim->TakeHealth( STAFF_DAMAGE, DMG_GENERIC );
			return;
		}

		WWThornBush * pBush = (WWThornBush *)pVictim;

		if( pBush != NULL && pBush->Enchant( m_pPlayer->pev->playerclass ) )
			return;
	}

	if(!ComboPlayer(pVictim))
	{
		switch( m_pPlayer->pev->playerclass )
		{
		default:
			break;

		case WWCLASS_LIFE:

			if( pVictim->IsPlayer() && m_pPlayer->IRelationship( pVictim ) < R_NO )
			{
				CBasePlayer * pPlayer = ((CBasePlayer *)pVictim);

				pPlayer->TakeHealth( flDamage, bitsDamage );
				pPlayer->m_bitsDamageType = 0;
				return;
			}

			break;

		case WWCLASS_ICE:

			flDamage *= 1.5f;
			bitsDamage = DMG_FREEZE | DMG_SLOWFREEZE;

			if( pVictim->IsPlayer() && m_pPlayer->IRelationship( pVictim ) >= R_NO )
			{
				pVictim->pev->punchangle.x = RANDOM_FLOAT( -30, 30 );
				pVictim->pev->punchangle.y = RANDOM_FLOAT( -30, 30 );
				pVictim->pev->punchangle.z = RANDOM_FLOAT( -30, 30 );
			}

			break;

		case WWCLASS_FIRE:
			bitsDamage = DMG_BURN | DMG_SLOWBURN;
			break;

		case WWCLASS_LIGHTNING:
			bitsDamage = DMG_SHOCK;
			break;

		case WWCLASS_DEATH:
			bitsDamage = DMG_POISON | DMG_BLAST;
			break;

		case WWCLASS_EARTH:

			flDamage *= 2.0f;

			if( pVictim->IsPlayer() && m_pPlayer->IRelationship( pVictim ) >= R_NO )
			{
				extern int gmsgShake;
				MESSAGE_BEGIN( MSG_ONE, gmsgShake, NULL, pVictim->edict() );
					WRITE_SHORT( UTIL_FixedUnsigned16( 100.0f, 1 << 12 ) );
					WRITE_SHORT( UTIL_FixedUnsigned16(   1.0f, 1 << 12 ) );
					WRITE_SHORT( UTIL_FixedUnsigned16( 100.0f, 1 << 8  ) );
				MESSAGE_END();
			}

			break;

		case WWCLASS_WIND:

			flDamage *= 0.9f;

			if( pVictim->IsPlayer() && m_pPlayer->IRelationship( pVictim ) >= R_NO )
			{
				Vector vDiff = ( pVictim->pev->origin - m_pPlayer->pev->origin ).Normalize();
				pVictim->pev->velocity = vDiff * 500.0f + Vector( 0, 0, 300.0f );
			}

			break;
		}

		if( m_iSwingCount )
			flDamage *= 0.5f;

		ClearMultiDamage();
		pVictim->TraceAttack( m_pPlayer->pev, flDamage, gpGlobals->v_forward, pTrace, DMG_CLUB | bitsDamage );
		ApplyMultiDamage( pev, m_pPlayer->pev );
	}

#endif
}

// Can we combo with this player?
int WWStaff::ComboPlayer(CBaseEntity * pEntity)
{
	bool comboGo = false;

#ifndef CLIENT_DLL
	CBasePlayer * pPlayer = NULL;

	// Silently ignore non players
	if(!pEntity || !pEntity->IsPlayer())
		return(comboGo);

	// Team and allys only
	if(m_pPlayer->IRelationship( pEntity ) >= R_NO)
	{
		ClientPrint(m_pPlayer->pev, HUD_PRINTCENTER, "Other Player is on wrong team!");
		return(comboGo);
	}


	pPlayer = ((CBasePlayer *)pEntity);

	// Must have their staff armed.
	if( pPlayer &&
		pPlayer->m_pActiveItem &&
		FClassnameIs( pPlayer->m_pActiveItem->pev, "ww_staff" ) )
	{

		char comboSpell[32];

		// Combos must be allowed
		if(CVAR_GET_FLOAT("mp_allowcombos")==0)
		{
			ClientPrint(m_pPlayer->pev, HUD_PRINTCENTER, "#CombosOff");
		}
		// Player must have a team, death match
		else if (m_pPlayer->pev->team > 0)
		{
//				ALERT(at_console,"ComboHits:%s,%d\n",STRING(m_pPlayer->pev->netname),m_pPlayer->m_iComboHits);
			int myclass		= m_pPlayer->m_pClass->GetClassNumber();
			int otherClass	= pPlayer->m_pClass->GetClassNumber();

			if(combo_array[myclass][otherClass] != NULL)
			{
				strcpy(comboSpell, combo_array[myclass][otherClass]);
				comboGo=true;
			}
			else
			{
				ClientPrint(m_pPlayer->pev, HUD_PRINTCENTER, "Other Player is not a compatable combo class!");
			}
		}
		else{
			ClientPrint(m_pPlayer->pev, HUD_PRINTCENTER, "Other Player is not on a team!");
		}

		//Ok, now we know that he is the correct wiz type
		// m_iComboHits -1 means a player as comboed this turn and can't combo until
		// next spawn
		if( comboGo &&
			m_pPlayer->m_pClass->m_iComboHits != -1 &&
			pPlayer->m_pClass->m_iComboHits != -1)
		{

			// play combo hit sound
			switch( RANDOM_LONG( 0, 1 ) )
			{
			case 0:
				EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "spells/staff_combo1.wav", 1, ATTN_NORM); break;
			case 1:
				EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "spells/staff_combo2.wav", 1, ATTN_NORM); break;
			}

			m_pPlayer->m_pClass->m_iComboHits++;

			if( m_pPlayer->m_pClass->m_iComboHits > STAFF_COMBOHITS &&
				pPlayer->m_pClass->m_iComboHits > STAFF_COMBOHITS)
			{
				// BUGBUG Hitting player always gets combo
				// Is this correct behaviour? random?
				m_pPlayer->m_pClass->m_iComboHits = -1;
				pPlayer->m_pClass->m_iComboHits   =  0; // Allow this player to combo again

				MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
					WRITE_BYTE( ENTINDEX(m_pPlayer->edict()) );	// client number
					WRITE_SHORT( m_pPlayer->pev->frags );
					WRITE_SHORT( m_pPlayer->m_iDeaths );
					WRITE_BYTE ( m_pPlayer->pev->playerclass );
					WRITE_BYTE ( m_pPlayer->pev->team );
					WRITE_BYTE ( 0 );
				MESSAGE_END();
			
				m_pPlayer->GiveNamedItem(comboSpell);
			}
		}
		else if(comboGo){
			ClientPrint(m_pPlayer->pev, HUD_PRINTCENTER, "One Player has already combo'd this round!");
		}
	}
	else{
		char str[256];
		sprintf(str, "Player using %s", STRING(pPlayer->m_pActiveItem->pev->classname));
		ClientPrint(m_pPlayer->pev, HUD_PRINTCENTER, str);
	}

	// BUGBUG This means if anyone who can't combo hits a player with a staff
	// then combo count resets, is this correcrt behaviour?
	if(!comboGo && m_pPlayer->m_pClass->m_iComboHits != -1)
		m_pPlayer->m_pClass->m_iComboHits = 0;
#endif

	return(comboGo);
}

