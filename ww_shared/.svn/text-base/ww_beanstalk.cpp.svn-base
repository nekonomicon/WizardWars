#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_beanstalk.h"

#ifndef CLIENT_DLL
#include "ww_wizards.h"
#include "ww_naturewizard.h"
#endif


LINK_ENTITY_TO_CLASS( ww_spell_beanstalk, WWBeanStalkSpell );


void WWBeanStalkSpell::Spawn( void )
{
	Precache();

	m_iId = WEAPON_BEANSTALK;
	m_bStalking  = false;

#ifndef CLIENT_DLL
	m_hLastStalk = NULL;
#endif

	FallInit();
}


void WWBeanStalkSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/beanstalk.wav" );
	m_usEvent = PRECACHE_EVENT( 1, "events/spells/beanstalk.sc" );
}


int WWBeanStalkSpell::iItemSlot( void )
{
	return SLOT_BEANSTALK + 1;
}


int WWBeanStalkSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_BEANSTALK;
	pInfo->iSlot		= SLOT_BEANSTALK;
	pInfo->iPosition	= POS_BEANSTALK;
	pInfo->iWeight		= WEIGHT_BEANSTALK;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWBeanStalkSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < BEANSTALK_COST )
		return FALSE;

	return TRUE;
}


BOOL WWBeanStalkSpell::Deploy( void )
{
	m_bStalking  = false;

#ifndef CLIENT_DLL
	m_hLastStalk = NULL;
#endif

	PLAYBACK_EVENT_FULL( FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0.0f,
		(float *)&g_vecZero, (float *)&g_vecZero, 0, 0, 1, 0, 0, 0 );

	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_NATURE],
		(char *)szWizardStaves[WWCLASS_NATURE],
		HANDS_EARTH_BIRD_IDLE, "beanstalk" );
}


void WWBeanStalkSpell::Holster( int iSkipLocal )
{
	if( m_bStalking )
		EndBuild();

	CBasePlayerWeapon::Holster( iSkipLocal ); 
}


void WWBeanStalkSpell::PrimaryAttack()
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < BEANSTALK_COST )
	{
		RetireWeapon();
		return;
	}

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + BEANSTALK_DELAY;
	m_flTimeWeaponIdle	  = m_flNextPrimaryAttack;
	
#ifndef CLIENT_DLL
	if( !FBitSet( m_pPlayer->pev->flags, FL_ONGROUND ) )
	{
		ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#NotOnGround" );
		return;
	}

	if( !m_pPlayer->m_pClass || m_pPlayer->m_pClass->GetClassNumber() != WWCLASS_NATURE )
		return;

	NatureWizard * pWizard = (NatureWizard *)m_pPlayer->m_pClass;

	if( !m_bStalking )
	{
		if( pWizard->GetStalk() != NULL )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#BeanstalkSpell_AlreadyHaveOne" );
			EndBuild();
			return;
		}

		if( Build( true ) )
		{
			pWizard->AddStalk( m_hLastStalk );

			m_pPlayer->SetSpeed( SPEED_NOMOVE, true );
			m_bStalking = true;
		}
	}

	else if( m_bStalking )
	{
		if( Build( false ) )
			pWizard->AddStalk( m_hLastStalk );
	}
#endif
}


void WWBeanStalkSpell::WeaponIdle()
{
	if( m_bStalking )
		EndBuild();

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}


BOOL WWBeanStalkSpell::Build( bool bFirst )
{
#ifndef CLIENT_DLL
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < BEANSTALK_COST )
		return FALSE;

	Vector vStalkPos;

	if( bFirst )
	{
		Vector vAngles = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;
		vAngles.x = 0;

		UTIL_MakeVectors( vAngles );

		Vector vTop, vBot;

		vTop = vBot = m_pPlayer->pev->origin + gpGlobals->v_forward * 80.0f;
		vTop.z += BEANSTALK_HEIGHT;
		vBot.z -= BEANSTALK_HEIGHT;

		TraceResult tr;
		UTIL_TraceLine( vTop, vBot, dont_ignore_monsters, ENT(pev), &tr );

		if( tr.fStartSolid || tr.fAllSolid || tr.flFraction == 1.0f )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#OutOfRoom" );
			EndBuild();
			return FALSE;
		}

		vStalkPos = tr.vecEndPos;
	}
	else
	{
		if( m_hLastStalk == NULL )
			return FALSE;

		vStalkPos = m_hLastStalk->pev->origin;
		vStalkPos.z += BEANSTALK_HEIGHT;
	}

	CBaseEntity * pNoBuild = NULL;
	while( ( pNoBuild = UTIL_FindEntityByClassname( pNoBuild, "func_nobuild" ) ) != NULL )
	{
		if( pNoBuild->ContainsPoint( vStalkPos ) )
		{
			ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#CantBuildHere" );
			EndBuild();
			return FALSE;
		}
	}
	
	Vector vMins = vStalkPos;
	Vector vMaxs = vStalkPos;

	vMins.x -= BEANSTALK_AREA;
	vMins.y -= BEANSTALK_AREA;
	vMins.z += 8.0f;
	vMaxs.x += BEANSTALK_AREA;
	vMaxs.y += BEANSTALK_AREA;
	vMaxs.z += BEANSTALK_HEIGHT;

	if( !UTIL_SearchArea( vMins, vMaxs ) )
	{
		ClientPrint( m_pPlayer->pev, HUD_PRINTCENTER, "#OutOfRoom" );
		EndBuild();
		return FALSE;
	}

	m_hLastStalk = CBaseEntity::Create( "beanstalk", vStalkPos,
		Vector( 0, RANDOM_FLOAT( -180, 180 ), 0 ), m_pPlayer->edict() );

	m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= BEANSTALK_COST;

	PLAYBACK_EVENT( 0, m_pPlayer->edict(), m_usEvent );
#endif
	return TRUE;
}


void WWBeanStalkSpell::EndBuild( void )
{
#ifndef CLIENT_DLL
	if( m_bStalking )
	{
		m_pPlayer->SetSpeed( SPEED_NOMOVE, false );
		m_bStalking = false;
	}
#endif
}


#ifndef CLIENT_DLL

class WWBeanStalk : public CBaseEntity
{
public:
	virtual const char * Name( void ) { return "BeanStalk"; };

	void Spawn( void )
	{
		Precache();

		float fArea = BEANSTALK_AREA * 0.5f;

		SET_MODEL  ( edict(), "models/beanstalk.mdl" );
		SET_SIZE   ( edict(), Vector( -fArea, -fArea, 0 ), Vector( fArea, fArea, BEANSTALK_HEIGHT ) );
		SET_ORIGIN ( edict(), pev->origin );

		pev->flags		|= FL_MONSTER;			// flag this so it is caught be UTIL_EntitiesInBox
		pev->movetype	= MOVETYPE_FLY;
		pev->solid		= SOLID_BBOX;
		pev->takedamage	= DAMAGE_YES;
		pev->health		= BEANSTALK_HEALTH;
		pev->max_health	= BEANSTALK_HEALTH;
		pev->frame		= 254;					// flag this as a ladder

		pev->rendermode	= kRenderTransColor;
		pev->renderamt	= 0;

		pev->euser1		= NULL;
		pev->euser2		= NULL;

		m_flHealthRegen	= gpGlobals->time;

		SetThink( &WWBeanStalk::StalkThink );
		pev->nextthink = gpGlobals->time + 0.1f;
	};

	void Precache( void )
	{
		PRECACHE_MODEL( "models/beanstalk.mdl" );
	};

	void EXPORT	StalkThink( void )
	{
		if( pev->owner != NULL )
			pev->euser1 = pev->owner;

		// we need to NULL pev->owner, or the owner can't climb us
		pev->owner = NULL;

		// fade in
		if( pev->rendermode != kRenderNormal )
		{
			pev->renderamt += 10.0f;

			if( pev->renderamt >= 255 )
			{
				pev->rendermode	= kRenderNormal;
				pev->renderamt	= 255;
			}
		}

		// regen health
		if( gpGlobals->time >= m_flHealthRegen )
		{
			float flGain = ( (float)Illumination() / 255.0f ) * BEANSTALK_HEALTHGAIN;

			if( flGain > 0.0f )
				TakeHealth( flGain, DMG_GENERIC );

			m_flHealthRegen = gpGlobals->time + BEANSTALK_HEALTHTIME;
		}

		pev->nextthink = gpGlobals->time + 0.1f;
	};

	void Killed( entvars_t * pevAttacker, int iGib )
	{
		if( pev->euser1 != NULL )
		{
			CBasePlayer * pPlayer = (CBasePlayer *)CBaseEntity::Instance( pev->euser1 );
			NatureWizard * pWizard = (NatureWizard *)pPlayer->m_pClass;

			pev->euser1 = NULL;
			pWizard->RemoveAllStalks();
			return;
		}

		if( pev->euser2 != NULL )
		{
			CBaseEntity * pNext = CBaseEntity::Instance( pev->euser2 );

			pev->euser2 = NULL;
			pNext->pev->euser1 = NULL;
			pNext->Killed( pevAttacker, iGib );
		}

		UTIL_Remove( this );
	};

	BOOL		CanRepel		( CBaseEntity * pAttacker ) { return FALSE; };

private:
	float		m_flHealthRegen;
};

LINK_ENTITY_TO_CLASS( beanstalk, WWBeanStalk );


#endif
