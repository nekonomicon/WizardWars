#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_skull.h"


LINK_ENTITY_TO_CLASS( ww_spell_skull, WWSkullSpell );


void WWSkullSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_SKULL;
	m_iDefaultAmmo	= 0;

	FallInit();
}


void WWSkullSpell::Precache( void )
{
	PRECACHE_MODEL( "models/spellbook.mdl"	);
	PRECACHE_MODEL( "models/p_skullspell.mdl"	);
	m_usEvent = PRECACHE_EVENT( 1, "events/spells/skull1.sc" );
}


int WWSkullSpell::iItemSlot( void )
{
	return SLOT_SKULL + 1;
}


int WWSkullSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_SKULL;
	pInfo->iSlot		= SLOT_SKULL;
	pInfo->iPosition	= POS_SKULL;
	pInfo->iWeight		= WEIGHT_SKULL;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWSkullSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < SKULL_COST )
		return FALSE;
	return TRUE;
}


BOOL WWSkullSpell::Deploy( void )
{
	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_DEATH],
		//	2004-06-08
		//	trepid_jon
		"models/p_skullspell.mdl",
		//(char *)szWizardStaves[WWCLASS_DEATH],
		HANDS_DEATH_SKULL_IDLE, "flyingskull" );
}


void WWSkullSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < SKULL_COST )
	{
		RetireWeapon();
		return;
	}

	m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= SKULL_COST;
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + SKULL_DELAY;
	m_flTimeWeaponIdle	  = m_flNextPrimaryAttack;

	Vector vAngles = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;
	UTIL_MakeVectors( vAngles );

#ifndef CLIENT_DLL
	Vector vOrigin = m_pPlayer->GetGunPosition();
	vOrigin.z -= 8.0f;
	vOrigin = vOrigin + gpGlobals->v_forward * 8.0f;
	vOrigin = vOrigin + gpGlobals->v_right * 7.0f;

	CBaseEntity * pHunter = Create( "skull", vOrigin, vAngles, m_pPlayer->edict() );

	if( pHunter != NULL )
	{
		pHunter->pev->angles = UTIL_VecToAngles( gpGlobals->v_forward );
		pHunter->pev->velocity = gpGlobals->v_forward * SKULL_SPEED;
		pHunter->pev->team = m_pPlayer->pev->team;
		pHunter->pev->skin = m_pPlayer->pev->team; // MAH 20040804
	}
#endif

	PLAYBACK_EVENT_FULL( FEV_HOSTONLY | FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, vAngles, 0, 0, 0, 0, 0, 0 );

	DefaultDeploy( (char *)szWizardHands[WWCLASS_DEATH], NULL, HANDS_DEATH_SKULL_IDLE, "flyingskull" );
}


void WWSkullSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < SKULL_COST )
	{
		RetireWeapon();
		return;
	}

	DefaultDeploy( (char *)szWizardHands[WWCLASS_DEATH], "models/p_skullspell.mdl", HANDS_DEATH_SKULL_IDLE, "flyingskull" );

	PLAYBACK_EVENT_FULL( FEV_HOSTONLY | FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}



#ifndef CLIENT_DLL

class WWSkullHunter : public CBaseMonster
{
public:
	void		Spawn			( void );
	void		Precache		( void );
	void EXPORT	HunterThink		( void );
	void EXPORT	HunterTouch		( CBaseEntity * pOther );
	BOOL		CanRepel		( CBaseEntity * pAttacker ) { return TRUE; };

private:
	EHANDLE		m_hTarget;
	float		m_flBirth;
	float		m_flLifeTime;
	unsigned short m_usEvent;
};


LINK_ENTITY_TO_CLASS( skull, WWSkullHunter );


void WWSkullHunter:: Spawn( void )
{
	Precache();

	SET_MODEL	( edict(), "models/npc/skull.mdl" );
	SET_SIZE	( edict(), Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );
	SET_ORIGIN	( edict(), pev->origin );

	pev->movetype	= MOVETYPE_FLY;
	pev->solid		= SOLID_BBOX;
	pev->renderfx	= kRenderFxRedFire;
	m_hTarget		= NULL;
	m_flBirth		= gpGlobals->time;
	m_flLifeTime	= gpGlobals->time + SKULL_LIFE;

	ResetSequenceInfo();

		PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f,
			pev->origin, pev->angles, 0, 0, 0, 0, 0, 0 );

	SetThink( &WWSkullHunter::HunterThink );
	SetTouch( &WWSkullHunter::HunterTouch );
	pev->nextthink = gpGlobals->time + 0.1f;
}


void WWSkullHunter::Precache( void )
{
	PRECACHE_SOUND( "npc/skull.wav"			);
	PRECACHE_SOUND( "npc/skull_poof.wav"	);
	PRECACHE_MODEL(	"models/npc/skull.mdl"	);

	m_usEvent = PRECACHE_EVENT( 1, "events/spells/skull2.sc" );
}


void WWSkullHunter::HunterThink( void )
{
	if( m_hTarget != NULL )
	{
		Vector vTarget = UTIL_VecToAngles( m_hTarget->pev->origin - pev->origin );
		vTarget.x = -vTarget.x;

		pev->angles.x = UTIL_ApproachAngle( vTarget.x, pev->angles.x, SKULL_TURNSPEED * 0.1f );
		pev->angles.y = UTIL_ApproachAngle( vTarget.y, pev->angles.y, SKULL_TURNSPEED * 0.1f );
		pev->angles.z = UTIL_ApproachAngle( vTarget.z, pev->angles.z, SKULL_TURNSPEED * 0.1f );

		UTIL_MakeVectors( pev->angles );
		pev->velocity = gpGlobals->v_forward * SKULL_SPEED;

		pev->nextthink = gpGlobals->time + 0.1f;
	}
	else
	{
		CBaseEntity * pSearch = NULL;

		while( ( pSearch = UTIL_FindEntityInSphere( pSearch, pev->origin, SKULL_SEARCH ) ) != NULL )
		{
			if( !pSearch->IsAlive() || !pSearch->pev->takedamage )
				continue;

			if( IRelationship( pSearch ) < R_NO )
				continue;

			if( !FVisible( pSearch ) )
				continue;

//			if( !FInViewCone( pSearch ) )
//				continue;

			m_hTarget = pSearch;
			m_flLifeTime = m_flBirth + SKULL_HUNTLIFE;
			break;
		}

		if( m_hTarget != NULL )
			PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f, pev->origin, pev->angles, 0, 0, 1, 0, 0, 0 );

		pev->nextthink = gpGlobals->time + 0.1f;
	}

	if( gpGlobals->time >= m_flLifeTime )
	{
		HunterTouch( NULL );
		SetThink( &WWSkullHunter::SUB_Remove );
		// 1.2.4 sound loop fix
		PLAYBACK_EVENT_FULL( FEV_GLOBAL | FEV_RELIABLE, edict(), m_usEvent, 0.0f,
			pev->origin, pev->angles, 0, 0, 2, 0, 0, 0 );

	}
}


void WWSkullHunter::HunterTouch( CBaseEntity * pOther )
{
	ClearMultiDamage();
	::RadiusDamage( pev->origin, pev, VARS( pev->owner ), SKULL_DAMAGE, SKULL_RADIUS,
		CLASS_NONE, DMG_BULLET | DMG_NEVERGIB );
	ApplyMultiDamage( pev, VARS( pev->owner ) );

	PLAYBACK_EVENT_FULL( FEV_GLOBAL | FEV_RELIABLE, edict(), m_usEvent, 0.0f,
		pev->origin, pev->angles, 0, 0, 2, 0, 0, 0 );

	UTIL_Remove( this );
}

#endif
