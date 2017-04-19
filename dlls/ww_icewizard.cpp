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
#include "ww_icewizard.h"

#include "tf_globals.h"
#include "tf_globalinfo.h"
#include "tf_ent.h"


IceWizard::IceWizard( CBasePlayer * pPlayer ) : WWBaseWizard( pPlayer )
{
	m_flPlayerSpeed		= SPEED_MEDHIGH;
	m_iMaxHealth		= 90;
	m_iMaxArmor			= 90;

	m_bIsInvisible		= FALSE;
	m_bIsDisguised		= FALSE;
	m_iDisguisedTeam	= pPlayer->pev->team;
	m_iDisguisedWizard	= WWCLASS_ICE;
}


const char * IceWizard::GetWizardModel()
{
	if( m_bIsDisguised )
		return szWizardClasses[ m_iDisguisedWizard ];

	return szWizardClasses[ WWCLASS_ICE ];
}


const char * IceWizard::GetWizardHands()
{
	if( m_bIsDisguised )
		return szWizardHands[ m_iDisguisedWizard ];

	return szWizardHands[ WWCLASS_ICE ];
}


const char * IceWizard::GetWizardStaff()
{
	if( m_bIsDisguised )
		return szWizardStaves[ m_iDisguisedWizard ];

	return szWizardStaves[ WWCLASS_ICE ];
}


void IceWizard::Equip( void )
{
	m_bIsInvisible		= FALSE;
	m_bIsDisguised		= FALSE;
	m_iDisguisedTeam	= m_pPlayer->pev->team;
	m_iDisguisedWizard  = WWCLASS_ICE;

	WWBaseWizard::Equip();
}


void IceWizard::GiveSpells( void )
{
	m_pPlayer->GiveNamedItem( "ww_staff"			);
	m_pPlayer->GiveNamedItem( "ww_spell_shield"		);
	m_pPlayer->GiveNamedItem( "ww_spell_seal"		);
	m_pPlayer->GiveNamedItem( "ww_spell_missile"	);
	m_pPlayer->GiveNamedItem( "ww_spell_iceshard"	);
	m_pPlayer->GiveNamedItem( "ww_spell_freezeray"	);
}


void IceWizard::DoSpecial( int slot )
{
	switch( slot )
	{
	case 1:
		if( IsInvisible() )
			break;

		if( !m_pPlayer->CanSpy() )
			break;

		if( m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] < 20 )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#LowOnMagic" );
			break;
		}

		m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] -= 20;

		m_pPlayer->pev->rendermode	= kRenderTransColor;
		m_pPlayer->pev->renderamt	= 255;
		m_bIsInvisible				= TRUE;

		m_flNextAmmoUse = gpGlobals->time + 1.0f;

		break;

	case 2:
		m_pPlayer->pev->rendermode	= kRenderNormal;
		m_pPlayer->pev->renderamt	= 255;
		m_bIsInvisible				= FALSE;
		m_flNextAmmoUse				= 0.0f;

		break;

	case 3:
		if( !m_pPlayer->CanSpy() )
			break;

		if( m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] < 20 )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#LowOnMagic" );
			break;
		}

		m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] -= 20;

		if( m_pPlayer->m_pLastEnemySeen != NULL )
		{
			m_iDisguisedTeam	= m_pPlayer->m_pLastEnemySeen->pev->team;
			m_iDisguisedWizard	= m_pPlayer->m_pLastEnemySeen->pev->playerclass;

			strcpy( m_szDisguisedName, STRING( m_pPlayer->m_pLastEnemySeen->pev->netname ) );

			m_bIsDisguised = TRUE;

			m_pPlayer->SetPlayerModel();
			SetTeamColor( m_iDisguisedTeam );
		}
		else
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#Ice_NoEnemySeen" );

		break;
	}
}


void IceWizard::Think( void )
{
	if( IsInvisible() )
	{
		float t = ( m_pPlayer->pev->velocity.Length() / PlayerSpeed() ) * 0.5f;

		if( m_pPlayer->pev->button & IN_ATTACK )
			t = t + 0.5f;

		if( t > 0.9f )
			t = 0.9f;

		t *= 255.0f;

		m_pPlayer->pev->rendermode = kRenderTransColor;
		m_pPlayer->pev->renderamt  = t;

		if( gpGlobals->time >= m_flNextAmmoUse )
		{
			if( m_pPlayer->pev->velocity.Length() != 0.0 )
			{
				if( m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] < 2 )
				{
					m_pPlayer->pev->rendermode	= kRenderNormal;
					m_pPlayer->pev->renderamt	= 255;
					m_bIsInvisible				= FALSE;
				}

				if( IsInvisible() )

				{
					m_pPlayer->m_rgAmmo[m_pPlayer->GetAmmoIndex("mana")] -= 2;
					m_flNextAmmoUse = gpGlobals->time + 0.5f;
				}
			}
		}
	}


	if( IsDisguised() )
	{
		m_pPlayer->pev->weaponmodel = MAKE_STRING( szWizardStaves[ GetDisguisedWizard() ] );
		m_pPlayer->pev->viewmodel	= MAKE_STRING( szWizardHands [ GetDisguisedWizard() ] );

		if( m_pPlayer->pev->button & IN_ATTACK )
			ResetDisguise();
	}


	WWBaseWizard::Think();
}


void IceWizard::ResetDisguise( void )
{
	SetTeamColor( m_pPlayer->pev->team );

	m_bIsInvisible		= FALSE;
	m_bIsDisguised		= FALSE;
	m_iDisguisedTeam	= m_pPlayer->pev->team;
	m_iDisguisedWizard	= WWCLASS_ICE;

	m_pPlayer->pev->rendermode	= kRenderNormal;
	m_pPlayer->pev->renderamt	= 255;
	
	strcpy( m_szDisguisedName, STRING( m_pPlayer->pev->netname ) );

	m_pPlayer->pev->weaponmodel = MAKE_STRING( szWizardStaves[ GetDisguisedWizard() ] );
	m_pPlayer->pev->viewmodel	= MAKE_STRING( szWizardHands [ GetDisguisedWizard() ] );

	m_pPlayer->SetPlayerModel();
	
}


void IceWizard::StatusReport( char * pszBuffer )
{
	char szBuffer[SBAR_STRING_SIZE];
	memset( szBuffer, 0, sizeof(szBuffer) );

	if( g_pGameRules->IsTeamplay() )
	{
		if( IsInvisible() )
			sprintf( szBuffer, "Team:%s Wizard:#%s Invis:%i%%%%",
				g_pTFGlobalInfo->TeamName( m_iDisguisedTeam ),
				GetWizard( m_iDisguisedWizard ),
				100 - (int)( m_pPlayer->pev->renderamt / 255.0f * 100.0f ) );
		else
			sprintf( szBuffer, "Team:%s Wizard:#%s Invis:off",
				g_pTFGlobalInfo->TeamName( m_iDisguisedTeam ),
				GetWizard( m_iDisguisedWizard ) );
	}
	else
	{
		if( IsInvisible() )
			sprintf( szBuffer, "Wizard:#%s Invis:%i%%%%",
				GetWizard( m_iDisguisedWizard ),
				100 - (int)( m_pPlayer->pev->renderamt / 255.0f * 100.0f ) );
		else
			sprintf( szBuffer, "Wizard:#%s Invis:off", GetWizard( m_iDisguisedWizard ) );
	}


	WWBaseWizard::StatusReport( pszBuffer );

	sprintf( pszBuffer, "%s %s", pszBuffer, szBuffer );
}


void IceWizard::Remove( void )
{
	WWBaseWizard::Remove();
	ResetDisguise();
}