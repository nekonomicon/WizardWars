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

#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_defs.h"
#include "ww_wizards.h"
#include "ww_windwizard.h"
#include "ww_lightningwizard.h"
#include "ww_firewizard.h"
#include "ww_deathwizard.h"
#include "ww_lifewizard.h"
#include "ww_earthwizard.h"
#include "ww_dragonwizard.h"
#include "ww_icewizard.h"
#include "ww_naturewizard.h"
#include "ww_archmage.h"

#include "ww_satchels.h"

#include "ww_seals.h"

#include "tf_globals.h"
#include "tf_globalinfo.h"
#include "tf_ent.h"


// WWBaseWizard *	WWBaseWizard::GetRandomClass	( CBasePlayer * pPlayer );
// WWBaseWizard *	WWBaseWizard::GetNumberedClass	( CBasePlayer * pPlayer, int number );
// const char *	WWBaseWizard::GetWizard			( int num );

	
WWBaseWizard::WWBaseWizard( CBasePlayer * pPlayer )
{
	m_pPlayer			= pPlayer;
	m_bGrenadeArmed		= FALSE;
	m_flPlayerStopTime	= 0.0f;
	m_hSeal				= NULL;
	m_flNextSpecialTime	= 0.0f;

	m_iComboHits = 0;
	m_bOnDragon = FALSE;
	m_hMounted = NULL;
	m_hSpiritDragon = NULL;
	m_hGiantPlant = NULL;
}


WWBaseWizard * WWBaseWizard::GetNumberedClass( CBasePlayer * pPlayer, int number )
{
	WWBaseWizard * pWizard = NULL;

	switch( number )
	{
	case WWCLASS_WIND:
		pWizard = new WindWizard( pPlayer );
		break;

	case WWCLASS_LIGHTNING:
		pWizard = new LightningWizard( pPlayer );
		break;

	case WWCLASS_FIRE:
		pWizard = new FireWizard( pPlayer );
		break;

	case WWCLASS_DEATH:
		pWizard = new DeathWizard( pPlayer );
		break;

	case WWCLASS_LIFE:
		pWizard = new LifeWizard( pPlayer );
		break;

	case WWCLASS_EARTH:
		pWizard = new EarthWizard( pPlayer );
		break;

	case WWCLASS_DRAGON:
		pWizard = new DragonWizard( pPlayer );
		break;

	case WWCLASS_ICE:
		pWizard = new IceWizard( pPlayer );
		break;

	case WWCLASS_NATURE:
		pWizard = new NatureWizard( pPlayer );
		break;

	case WWCLASS_ARCHMAGE:
		pWizard = new ArchMage( pPlayer );
		break;

	default:
		pWizard = GetRandomClass( pPlayer );
		break;
	}

	if( !pWizard )
		pWizard = new WWBaseWizard( pPlayer );

	pWizard->m_iComboHits = 0;
	pWizard->m_bOnDragon = FALSE;
	pWizard->m_hMounted = NULL;

	return pWizard;
}


const char * WWBaseWizard::GetWizard( int num )
{
	if( num <= WWCLASS_NONE || num > WWCLASS_RANDOM )
		num = WWCLASS_ARCHMAGE;

	return szWizardClasses[num];
}


const char * WWBaseWizard::GetWizardModel( void )
{
	return szWizardClasses[ GetClassNumber() ];
}


const char * WWBaseWizard::GetWizardHands( void )
{
	return szWizardHands[ GetClassNumber() ];
}


const char * WWBaseWizard::GetWizardStaff( void )
{
	return szWizardStaves[ GetClassNumber() ];
}


WWBaseWizard * WWBaseWizard::GetRandomClass( CBasePlayer * pPlayer )
{
	int z = 0;
	int validOptions[WWCLASS_LASTCLASS];

	int i;
	for( i = 0; i < WWCLASS_LASTCLASS; i++ )
	{
		if( g_pTFGlobalInfo->IsValidClass( pPlayer->pev->team, i ) )
		{
			validOptions[z] = i;
			z++;
		}
	}

	return GetNumberedClass( pPlayer, validOptions[ RANDOM_LONG( 0, z ) ] );
}


void WWBaseWizard::Equip( void )
{
	if( GetClassNumber() == 0 )
		return;

	m_pPlayer->RemoveAllItems( FALSE, FALSE );

	GiveSpells();

	CLIENT_COMMAND( m_pPlayer->edict(), UTIL_VarArgs( "exec %s.cfg\n", GetWizard( GetClassNumber() ) ) );
	
	m_pPlayer->GiveAmmo( MAX_MANA, "mana", MAX_MANA );
	m_pPlayer->GiveAmmo( MAX_SATCHELS, "satchels", MAX_SATCHELS );

	m_pPlayer->pev->armorvalue	= m_iMaxArmor;
	m_pPlayer->pev->armortype	= m_iMaxArmor;
	m_bGrenadeArmed				= FALSE;

	m_pPlayer->SelectItem( "ww_staff" );
	m_pPlayer->UpdateSpeed();
}


int WWBaseWizard::PlayerSpeed( void )
{
	if( IsBear() )
		return BEAR_SPEED * CVAR_GET_FLOAT( "sv_maxspeed" );

	return m_flPlayerSpeed * CVAR_GET_FLOAT( "sv_maxspeed" );
}

void WWBaseWizard::RemoveGiantPlant()
{
	CBaseEntity *giantPlant = (CBaseEntity*)(GetGiantPlant());
	if(giantPlant != NULL){
		giantPlant->Killed(m_pPlayer->pev, 1000);
	}

	SetGiantPlant(NULL);
}

void WWBaseWizard::Think( void )
{
	//BOOM!  It went off in your hand... :O
	if( m_bGrenadeArmed && m_flGrenadeTime < gpGlobals->time )
	{
		m_flGrenadeTime -= 0.1f;
		TossGrenade();
	}
	
	//Toss it if he tried to throw it too early
	else if( m_bForceGrenadeToss && m_flGrenadeTime - gpGlobals->time <= 2.5 )
	{
		m_bForceGrenadeToss = FALSE;
		TossGrenade();
	}

	if( m_flPlayerStopTime != 0.0f )
	{
		m_pPlayer->pev->velocity.x = 0;
		m_pPlayer->pev->velocity.y = 0;

		if( gpGlobals->time >= m_flPlayerStopTime )
		{
			m_pPlayer->SetSpeed( SPEED_NOMOVE, false );
			m_flPlayerStopTime = 0.0f;
		}
	}
}


void WWBaseWizard::ArmGrenade( void )
{
	if( !m_pPlayer )
		return;

	// do we already have a grenade armed?
	if( m_bGrenadeArmed )
		return;

	// are we morphed?
	if( IsBear() )
		return;

	int ammoIndex = m_pPlayer->GetAmmoIndex( "satchels" );
	
	if( m_pPlayer->m_rgAmmo[ammoIndex] > 0 )
	{
		m_pPlayer->m_rgAmmo[ ammoIndex ]--;

		m_bGrenadeArmed		= TRUE;
		m_bForceGrenadeToss	= FALSE;

		m_flGrenadeTime = gpGlobals->time + 3.0f;

		EMIT_SOUND_DYN( ENT(m_pPlayer->pev), CHAN_ITEM,"items/gunpickup3.wav", 1.0f, ATTN_NORM, 0.0f, 150.0f );
	}
}


void WWBaseWizard::TossGrenade( void )
{
	if( !m_pPlayer )
		return;

	// can't toss a grenade if we don't have one armed
	if( !m_bGrenadeArmed )
		return;

	//Tried to throw it too early
	if( m_flGrenadeTime - gpGlobals->time > 2.5f )
	{
		m_bForceGrenadeToss = TRUE;
		return;
	}

	float flVel;
	Vector angThrow, vecSrc, vecThrow;

	// good god, who made this up?
	{
		angThrow = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;

		if( angThrow.x < 0.0f )
			angThrow.x = -10.0f + angThrow.x * ( ( 90.0f - 10.0f ) / 90.0f );
		else
			angThrow.x = -10.0f + angThrow.x * ( ( 90.0f + 10.0f ) / 90.0f );

		flVel = ( 90.0f - angThrow.x ) * 4.0f;

		if( flVel > 500 )
			flVel = 500;
	}

	UTIL_MakeVectors( angThrow );

	vecSrc	 = m_pPlayer->pev->origin + m_pPlayer->pev->view_ofs + gpGlobals->v_forward * 16.0f;
	vecThrow = gpGlobals->v_forward * flVel * 2.0f;

	WWSatchel * pSatchel = (WWSatchel *)CBaseEntity::Create( (char *)GetSatchelClass(),
		vecSrc, vecThrow, m_pPlayer->edict() );

	if( pSatchel != NULL )
	{
		pSatchel->SetExplode( m_flGrenadeTime );
		pSatchel->pev->velocity = vecThrow;
		pSatchel->pev->team		= m_pPlayer->pev->team;
	}

	m_bGrenadeArmed = FALSE;
}


void WWBaseWizard::StopForTime( float flTime )
{
	m_pPlayer->SetSpeed( SPEED_NOMOVE, true );

	m_pPlayer->pev->velocity.x = 0;
	m_pPlayer->pev->velocity.y = 0;

	m_flPlayerStopTime = gpGlobals->time + flTime;
}


void WWBaseWizard::DoSpecial( int slot )
{
//	UTIL_SayText( "SPECIAL HERE", m_pPlayer );
}


void WWBaseWizard::StatusReport( char * pszText )
{
	char szBuffer[SBAR_STRING_SIZE];
	memset( szBuffer, 0, sizeof(szBuffer) );

	char szTemp[SBAR_STRING_SIZE];
	memset( szTemp, 0, sizeof(szTemp) );

	if( m_pPlayer->m_pActiveItem && m_pPlayer->m_pActiveItem->pszName() )
	{
		if( !strcmp( "weapon_combospell", m_pPlayer->m_pActiveItem->pszName() ) )
		{
			switch( m_pPlayer->m_pActiveItem->pev->iuser1 )
			{
			case WWCOMBO_BALLLIGHTNING:
				sprintf( szBuffer, "#weapon_balllightningspell" );
				break;

			case WWCOMBO_COMET:
				sprintf( szBuffer, "#weapon_cometspell" );
				break;
			
			case WWCOMBO_GIANTPLANT:
				sprintf( szBuffer, "#weapon_giantplantspell" );
				break;
			
			case WWCOMBO_LIGHTNINGCLOUD:
				sprintf( szBuffer, "#weapon_lightningcloudspell" );
				break;
			
			case WWCOMBO_METEOR:
				sprintf( szBuffer, "#weapon_meteorspell" );
				break;
			
			case WWCOMBO_LIVINGDEAD:
				sprintf( szBuffer, "#weapon_skeletonspell" );
				break;
			
			case WWCOMBO_SPIRITWIZARD:
				sprintf( szBuffer, "#weapon_spiritwizspell" );
				break;
			
			case WWCOMBO_TORNADO:
				sprintf( szBuffer, "#weapon_tornadospell" );
				break;
			
			case WWCOMBO_WALLY:
				sprintf( szBuffer, "#weapon_wombatspell" );
				break;
			
			case WWCOMBO_DRAGONSPIRIT:
				sprintf( szBuffer, "#weapon_dragonspell" );
				break;
			}
		}
		else
			sprintf( szBuffer, "#%s", m_pPlayer->m_pActiveItem->pszName() );

		if( m_pPlayer->m_iLives != -1 )
		{
			sprintf( szTemp, "Lives: %d, %s", m_pPlayer->m_iLives, szBuffer );
			strcpy( szBuffer, szTemp );
		}

		if( (CBaseEntity *)m_hSeal != NULL )
		{
			WWSeal * pSeal = ((WWSeal *)((CBaseEntity*)m_hSeal));
			sprintf( szTemp, "Seal: %i%%%%, %s", (int)pSeal->m_flMana, szBuffer );
			strcpy( szBuffer, szTemp );
		}
	}
	else
	{
		if( m_pPlayer->m_iLives != -1 )
			sprintf( szBuffer, "Lives: %d", m_pPlayer->m_iLives );

		if( (CBaseEntity *)m_hSeal != NULL )
		{
			WWSeal * pSeal = ((WWSeal *)((CBaseEntity*)m_hSeal));

			if( strlen( szBuffer ) )
				sprintf( szTemp, "Seal: %i%%%%, %s", (int)pSeal->m_flMana, szBuffer );
			else
				sprintf( szTemp, "Seal: %i%%%%", (int)pSeal->m_flMana );

			strcpy( szBuffer, szTemp );
		}
	}


	strncat( pszText, szBuffer, SBAR_STRING_SIZE );

}


void WWBaseWizard::SetTeamColor( int team )
{
	m_pPlayer->pev->skin = team;
}


void WWBaseWizard::GiveSpells( void )
{
}

const char * RelationName( int iRelation )
{
	if( iRelation == R_NO )
		return "Neutral";

	if( iRelation < R_NO )
		return "Friend";

	return "Enemy";
}

int WWBaseWizard::UpdateStatusBar( CBaseEntity * pEntity, char * pszBuffer )
{
	if( !m_pPlayer || !pszBuffer )
		return 0;

	BOOL bFound = FALSE;

	const char * relation = RelationName( m_pPlayer->IRelationship( pEntity ) );

	if( pEntity->IsPlayer() )
	{
		if( pEntity->IsObserver() )
			return 0;

		// XYPHN - 040202
		//	Why was this disabled?
		if( pEntity->pev->playerclass == WWCLASS_ICE )
		{
			CBasePlayer * pl = (CBasePlayer *)pEntity;
			IceWizard   * iw = (IceWizard *)pl->m_pClass;

			if( iw->IsDisguised() )
			{
				// XYPHN - 040207
				//	If we are true friends, use the real name.
				if( g_pTFGlobalInfo->IsFriendTeam( pEntity->pev->team, m_pPlayer->pev->team ) )
					sprintf( pszBuffer, "%c(Friend) %s", 2, STRING( pEntity->pev->netname ) );
				else
				{
					// XYPHN - 040202
					//	If they are disguised, use the disgused name... if there isn't one
					//	use this player's name.
					if( strcmp( iw->GetDisguiseName(), "" ) )
						sprintf( pszBuffer, "%c(Friend) %s", 2, STRING( m_pPlayer->pev->netname ) );
					else
						sprintf( pszBuffer, "%c(Friend) %s", 2, iw->GetDisguiseName() );
				}
			}
			else if( iw->IsInvisible() )
			{
				if( pEntity->pev->renderamt >= 48 )
					sprintf( pszBuffer, "%c(%s) %s", 2, relation, STRING( pEntity->pev->netname ) );
				else
					return 1;
			}
			else
				sprintf( pszBuffer, "%c(%s) %s", 2, relation, STRING( pEntity->pev->netname ) );
		}
		else
			sprintf( pszBuffer, "%c(%s) %s", 2, relation, STRING( pEntity->pev->netname ) );

		bFound = TRUE;
	}
	else if( pEntity->Name() != NULL )
	{
		if( pEntity->pev->owner != NULL )
		{
			entvars_t * pevEnt = VARS( pEntity->pev->owner );

			// 1.2.6 #14 - TP number for retards
			CBasePlayer * pl = (CBasePlayer *)CBaseEntity::Instance( pEntity->pev->owner );
			NatureWizard * pWizard = (NatureWizard *)pl->m_pClass;

			int num = 0;

			if(pEntity == (CBaseEntity *)pWizard->GetBush(0))
				num = 1;
			else if(pEntity == (CBaseEntity *)pWizard->GetBush(1))
				num = 2;

			sprintf( pszBuffer, "%c(%s) %s's #%d %s", 2, relation, STRING( pevEnt->netname ), num, pEntity->Name() );
		}
		else if( pEntity->pev->euser1 != NULL )
		{
			entvars_t * pevEnt = VARS( pEntity->pev->euser1 );

			// 1.2.6 #14 - TP number for retards
			CBasePlayer *pl = (CBasePlayer *)CBaseEntity::Instance( pEntity->pev->euser1 );
			NatureWizard * pWizard = (NatureWizard *)pl->m_pClass;


			int num = 0;

			if(pEntity == (CBaseEntity *)pWizard->GetBush(0))
				num = 1;
			else if(pEntity == (CBaseEntity *)pWizard->GetBush(1))
				num = 2;

			sprintf( pszBuffer, "%c(%s) %s's #%d %s", 2, relation, STRING( pevEnt->netname ), num, pEntity->Name() );
		}
		else
			sprintf( pszBuffer, "%c(%s) %s", 2, relation, pEntity->Name() );

		bFound = TRUE;
	}

	// the entity isn't something we know
	if( !bFound )
		return 0;

	// if they aren't friendly
	if( m_pPlayer->IRelationship( pEntity ) >= R_NO )
		return 1;

	float health, max_health, fhealth;
	float armor,  max_armor,  farmor;

	health		= pEntity->pev->health;
	max_health	= pEntity->pev->max_health;

	armor		= pEntity->pev->armorvalue;
	max_armor	= pEntity->pev->armortype;

	if( max_health <= 0.0f ) max_health = 100.0f;
	if( max_armor  <= 0.0f ) max_armor  = 100.0f;

	fhealth = ( health / max( max_health, 0.0001f ) ) * 100.0f;
	farmor  = ( armor  / max( max_armor,  0.0001f ) ) * 100.0f;

	char szBuffer[256];
	memset( szBuffer, 0, 256 );

	// if it is a friendly player
	if( pEntity->IsPlayer() )
		sprintf( szBuffer, " %i%%%%/%i%%%%", (int)fhealth, (int)farmor );

	// if we are a healer
	else if( GetClassNumber() == WWCLASS_LIFE )
		sprintf( szBuffer, " %i%%%%/%i%%%%", (int)fhealth, (int)farmor );
	
	// if we own the entity
	else if( pEntity->pev->owner == m_pPlayer->edict() )
		sprintf( szBuffer, " %i%%%%/%i%%%%", (int)fhealth, (int)farmor );

	// if we own the entity, sometimes we use euser1
	else if( pEntity->pev->euser1 == m_pPlayer->edict() )
	{
		if( FClassnameIs( pEntity->pev, "thornbush" ) )
			sprintf( szBuffer, " %i%%%%", (int)fhealth);

		else if( FClassnameIs( pEntity->pev, "beanstalk" ) )
			sprintf( szBuffer, " %i%%%%", (int)fhealth);
		
		else
			sprintf( szBuffer, " %i%%%%/%i%%%%", (int)fhealth, (int)farmor );
	}

	strcat( pszBuffer, szBuffer );

	return 1;
}


void WWBaseWizard::Remove( void )
{
	if( (CBaseEntity *)m_hSeal != NULL )
	{
		UTIL_Remove( m_hSeal );
		m_hSeal = NULL;
	}
}
