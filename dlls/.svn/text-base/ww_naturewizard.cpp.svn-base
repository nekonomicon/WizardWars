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
#include "ww_naturewizard.h"

#include "ww_thornbush.h"

#include "tf_globals.h"
#include "tf_globalinfo.h"
#include "tf_ent.h"


NatureWizard::NatureWizard( CBasePlayer * pPlayer ) : WWBaseWizard( pPlayer )
{
	m_flPlayerSpeed		= SPEED_MEDHIGH;
	m_iMaxHealth		= 90;
	m_iMaxArmor			= 90;

	m_hBush[0]			= NULL;
	m_hBush[1]			= NULL;

	m_hStalk			= NULL;
}


void NatureWizard::GiveSpells( void )
{
	m_pPlayer->GiveNamedItem( "ww_staff"			);
	m_pPlayer->GiveNamedItem( "ww_spell_shield"		);
	m_pPlayer->GiveNamedItem( "ww_spell_seal"		);
	m_pPlayer->GiveNamedItem( "ww_spell_missile"	);
	m_pPlayer->GiveNamedItem( "ww_spell_thornblast"	);
	m_pPlayer->GiveNamedItem( "ww_spell_beanstalk"	);
}


void NatureWizard::Think( void )
{
	if( m_hBush[0] != NULL && m_hBush[0]->pev->solid == SOLID_NOT )
		m_hBush[0]  = NULL;

	if( m_hBush[1] != NULL && m_hBush[1]->pev->solid == SOLID_NOT )
		m_hBush[1]  = NULL;

	WWBaseWizard::Think();
}


void NatureWizard::DoSpecial( int slot )
{
	int iBush = 0;
	int	iCost = 0;
	float flGive = 0.0f;
	WWThornBush * pBush = NULL;
	WWThornBush * pBush1 = (WWThornBush *)( (CBaseEntity *)m_hBush[0] );
	WWThornBush * pBush2 = (WWThornBush *)( (CBaseEntity *)m_hBush[1] );

	switch( slot )
	{
	// create new thorn bush
	case 1:
		if( !FBitSet( m_pPlayer->pev->flags, FL_ONGROUND ) )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#NotOnGround" );
			break;
		}

		if( m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] < 130 )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#LowOnMagic" );
			break;
		}

		if( pBush1 != NULL && pBush1->GetLevel() >= 2 )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#Nature_HaveStage3" );
			break;
		}
		
		if( pBush2 != NULL && pBush2->GetLevel() >= 2 )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#Nature_HaveStage3" );
			break;
		}

		iBush = -1;
		if( pBush2 == NULL )
			iBush = 1;

		if( pBush1 == NULL )
			iBush = 0;

		if( iBush == -1 )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#Nature_MoreThornplants" );
			break;
		}

		if( MakeThornBush( iBush ) )
			m_pPlayer->m_rgAmmo[ m_pPlayer->GetAmmoIndex("mana") ] -= 130;

		break;

	case 2:
		pBush = WWThornBush::FindBush( m_pPlayer, NATUREWIZARD_SEARCHRADIUS, FALSE );

		if( !pBush )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#Nature_NotCloseEnough" );
			break;
		}

		iCost = THORNBUSH_FERT_LVL2_COST;
		if( pBush->GetLevel() == 1 )
		{
			iCost = THORNBUSH_FERT_LVL3_COST;

			if( !pBush->pev->euser1 )
				break;

			CBasePlayer * pPlayer = (CBasePlayer *)CBaseEntity::Instance( pBush->pev->euser1 );

			if( !pPlayer || !pPlayer->m_pClass || pPlayer->m_pClass->GetClassNumber() != WWCLASS_NATURE )
				break;

			NatureWizard * pWizard = (NatureWizard *)pPlayer->m_pClass;

			if( pWizard->m_hBush[0] != NULL && pWizard->m_hBush[1] != NULL )
			{
				ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#Nature_2PlantsFertilize" );
				break;
			}
		}


		if( m_pPlayer->m_rgAmmo[ m_pPlayer->GetAmmoIndex("mana") ] < iCost )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#LowOnMagic" );
			return;
		}

		if( pBush->Fertilize() )
			m_pPlayer->m_rgAmmo[ m_pPlayer->GetAmmoIndex("mana") ] -= iCost;
		break;

	case 3:
		pBush = WWThornBush::FindBush( m_pPlayer, NATUREWIZARD_SEARCHRADIUS, TRUE );

		if( !pBush )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#Nature_NotCloseEnough" );
			break;
		}

		flGive = ( pBush->pev->health / pBush->pev->max_health ) * 100.0f;

		if( pBush->Harvest() )
			m_pPlayer->GiveAmmo( flGive, "mana", MAX_MANA );
		break;

	case 4:
		RemoveBush( m_hBush[0] );
		break;

	case 5:
		RemoveBush( m_hBush[1] );
		break;

	case 6:
		RemoveAllStalks();
		break;
	}
}


int NatureWizard::MakeThornBush( int iBush )
{
	if( !g_pGameRules->IsTeamplay() && CVAR_GET_FLOAT( "mp_allowdmthorns" ) != 0.0f )
	{
		ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#NoThornplantsInDM" );
		return 0;
	}

	Vector vAngles = m_pPlayer->pev->angles;
	vAngles.x = 0;
	vAngles.z = 0;
	UTIL_MakeVectors( vAngles );

	Vector vOrigin = m_pPlayer->GetGunPosition() + gpGlobals->v_forward * 16.0f;
	Vector vEndPos = vOrigin + gpGlobals->v_forward * 80.0f;
	vEndPos.z -= 128.0f;

	TraceResult tr; 
	UTIL_TraceLine( vOrigin, vEndPos, dont_ignore_monsters, m_pPlayer->edict(), &tr );

	// must build on ground
	if( tr.flFraction == 1.0f )
		return 0;

	CBaseEntity * pNoBuild = NULL;
	while( ( pNoBuild = UTIL_FindEntityByClassname( pNoBuild, "func_nobuild" ) ) != NULL )
	{
		if( pNoBuild->ContainsPoint( tr.vecEndPos ) )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#CantBuildHere" );
			return 0;
		}
	}

	Vector vMins = tr.vecEndPos;
	Vector vMaxs = tr.vecEndPos;

	vMins.x -= 30.0f;
	vMins.y -= 30.0f;
	vMaxs.x += 30.0f;
	vMaxs.y += 30.0f;
	vMaxs.z += 80.0f;

	if( !UTIL_SearchArea( vMins, vMaxs ) )
	{
		ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#OutOfRoom" );
		return 0;
	}

	vOrigin = tr.vecEndPos;

	CBaseEntity * pBush = CBaseEntity::Create( "thornbush", vOrigin, vAngles, m_pPlayer->edict() );

	if( pBush == NULL )
		return 0;
	
	pBush->pev->team	= m_pPlayer->pev->team;
	pBush->pev->skin	= m_pPlayer->pev->team;
	pBush->pev->euser1	= m_pPlayer->edict();
	pBush->pev->owner	= NULL; // WTF is this done for??

	if( m_hBush[0] == NULL )
		m_hBush[0] = pBush;
	else
		m_hBush[1] = pBush;

	return 1;
}


void NatureWizard::RemoveBush( CBaseEntity * pBush )
{
	if( !pBush )
		return;

	if( m_hBush[0] == pBush )
	{
		ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#Nature_BushKilled" );
		m_hBush[0] = NULL;
	}
	else if( m_hBush[1] == pBush )
	{
		ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#Nature_BushKilled" );
		m_hBush[1] = NULL;
	}

	UTIL_Remove( pBush );
}


void NatureWizard::AddStalk( CBaseEntity * pStalk )
{
	if( !pStalk )
		return;

	if( m_hStalk != NULL && m_hStalk != pStalk )
		pStalk->pev->euser2 = m_hStalk->edict();

	m_hStalk = pStalk;
}


void NatureWizard::RemoveAllStalks( void )
{
	if( m_hStalk == NULL )
		return;

	m_hStalk->pev->euser1 = NULL;
	m_hStalk->Killed( NULL, 0 );
	m_hStalk = NULL;

	ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#Nature_StalkKilled" );
}


void NatureWizard::StatusReport( char * pszBuffer )
{
	char szBuffer[SBAR_STRING_SIZE];
	memset( szBuffer, 0, sizeof(szBuffer) );

	int health1 = 0, health2 = 0;

	if( m_hBush[0] )
		health1 = 100 * ( m_hBush[0]->pev->health / m_hBush[0]->pev->max_health );

	if( m_hBush[1] )
		health2 = 100 * ( m_hBush[1]->pev->health / m_hBush[1]->pev->max_health );

	sprintf( szBuffer, "Plant1:%i%%%% Plant2:%i%%%% Beanstalk:%s",
		health1, health2, ( m_hStalk != NULL ? "yes" : "no" ) );

	WWBaseWizard::StatusReport( pszBuffer );

	sprintf( pszBuffer, "%s %s", pszBuffer, szBuffer );
}


void NatureWizard::Remove( void )
{
	WWBaseWizard::Remove();

	if( m_hBush[0] != NULL )
		UTIL_Remove( m_hBush[0] );

	if( m_hBush[1] != NULL )
		UTIL_Remove( m_hBush[1] );

	m_hBush[0] = NULL;
	m_hBush[1] = NULL;

	RemoveAllStalks();
}
