#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_wyvern.h"


LINK_ENTITY_TO_CLASS( ww_spell_wyvern, WWWyvernSpell );


void WWWyvernSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_WYVERN;
	m_iDefaultAmmo	= 0;

	FallInit();
}


void WWWyvernSpell::Precache( void )
{
	PRECACHE_MODEL( "models/spellbook.mdl"	);
	m_usEvent = PRECACHE_EVENT( 1, "events/spells/wyvern1.sc" );
}


int WWWyvernSpell::iItemSlot( void )
{
	return SLOT_WYVERN + 1;
}


int WWWyvernSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_WYVERN;
	pInfo->iSlot		= SLOT_WYVERN;
	pInfo->iPosition	= POS_WYVERN;
	pInfo->iWeight		= WEIGHT_WYVERN;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWWyvernSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < WYVERN_COST )
		return FALSE;
	return TRUE;
}


BOOL WWWyvernSpell::Deploy( void )
{
	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_DRAGON],
		(char *)szWizardStaves[WWCLASS_DRAGON],
		HANDS_DRAGON_WYVERN_IDLE, "wyvernspell" );
}


void WWWyvernSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < WYVERN_COST )
	{
		RetireWeapon();
		return;
	}

	m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= WYVERN_COST;
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + WYVERN_DELAY;
	m_flTimeWeaponIdle	  = m_flNextPrimaryAttack;

	Vector vAngles = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;
	UTIL_MakeVectors( vAngles );

#ifndef CLIENT_DLL
	Vector vOrigin = m_pPlayer->GetGunPosition();
	vOrigin.z -= 12.0f;
	vOrigin = vOrigin + gpGlobals->v_forward * 8.0f;

	CBaseEntity * pHunter = Create( "wyvern", vOrigin, vAngles, m_pPlayer->edict() );

	if( pHunter != NULL )
	{
		pHunter->pev->angles = UTIL_VecToAngles( gpGlobals->v_forward );
		pHunter->pev->velocity = gpGlobals->v_forward * WYVERN_SPEED;
		pHunter->pev->team = m_pPlayer->pev->team;
	}
#endif

	PLAYBACK_EVENT_FULL( FEV_HOSTONLY | FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, vAngles, 0, 0, 0, 0, 0, 0 );
}


void WWWyvernSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < WYVERN_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( FEV_HOSTONLY | FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}



#ifndef CLIENT_DLL

class WWWyvernHunter : public CBaseMonster
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


LINK_ENTITY_TO_CLASS( wyvern, WWWyvernHunter );


void WWWyvernHunter:: Spawn( void )
{
	Precache();

	SET_MODEL	( edict(), "models/npc/wyvern.mdl" );
	SET_SIZE	( edict(), Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );
	SET_ORIGIN	( edict(), pev->origin );

	pev->movetype	= MOVETYPE_FLY;
	pev->solid		= SOLID_BBOX;
	m_hTarget		= NULL;
	m_flBirth		= gpGlobals->time;
	m_flLifeTime	= gpGlobals->time + WYVERN_LIFE;

	ResetSequenceInfo();

		PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f,
			pev->origin, pev->angles, 0, 0, 0, 0, 0, 0 );

	SetThink( &WWWyvernHunter::HunterThink );
	SetTouch( &WWWyvernHunter::HunterTouch );
	pev->nextthink = gpGlobals->time + 0.1f;
}


void WWWyvernHunter::Precache( void )
{
	PRECACHE_SOUND( "npc/wyvern.wav"		);
	PRECACHE_MODEL(	"models/npc/wyvern.mdl"	);

	m_usEvent = PRECACHE_EVENT( 1, "events/spells/wyvern2.sc" );
}


void WWWyvernHunter::HunterThink( void )
{
	if( m_hTarget != NULL )
	{
		Vector vTarget = UTIL_VecToAngles( m_hTarget->pev->origin - pev->origin );
		vTarget.x = -vTarget.x;

		pev->angles.x = UTIL_ApproachAngle( vTarget.x, pev->angles.x, WYVERN_TURNSPEED * 0.1f );
		pev->angles.y = UTIL_ApproachAngle( vTarget.y, pev->angles.y, WYVERN_TURNSPEED * 0.1f );
		pev->angles.z = UTIL_ApproachAngle( vTarget.z, pev->angles.z, WYVERN_TURNSPEED * 0.1f );

		UTIL_MakeVectors( pev->angles );
		pev->velocity = gpGlobals->v_forward * WYVERN_SPEED;

		pev->nextthink = gpGlobals->time + 0.1f;
	}
	else
	{
		CBaseEntity * pSearch = NULL;

		while( ( pSearch = UTIL_FindEntityInSphere( pSearch, pev->origin, WYVERN_SEARCH ) ) != NULL )
		{
			if( !pSearch->IsAlive() || !pSearch->pev->takedamage )
				continue;

			if( IRelationship( pSearch ) < R_NO )
				continue;

			if( !FVisible( pSearch ) )
				continue;

			m_hTarget = pSearch;
			m_flLifeTime = m_flBirth + WYVERN_HUNTLIFE;
			break;
		}

		if( m_hTarget != NULL )
			PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f, pev->origin, pev->angles, 0, 0, 1, 0, 0, 0 );

		pev->nextthink = gpGlobals->time + 0.2f;
	}

	if( gpGlobals->time >= m_flLifeTime )
	{
		HunterTouch( NULL );
		SetThink( &WWWyvernHunter::SUB_Remove );
		// 1.2.4 sound loop fix
		PLAYBACK_EVENT_FULL( FEV_GLOBAL | FEV_RELIABLE, edict(), m_usEvent, 0.0f,
			pev->origin, pev->angles, 0, 0, 2, 0, 0, 0 );
	}
}


void WWWyvernHunter::HunterTouch( CBaseEntity * pOther )
{
	ClearMultiDamage();
	::RadiusDamage( pev->origin, pev, VARS( pev->owner ), WYVERN_DAMAGE, WYVERN_RADIUS, CLASS_NONE, DMG_BURN );
	ApplyMultiDamage( pev, VARS( pev->owner ) );

	PLAYBACK_EVENT_FULL( FEV_GLOBAL | FEV_RELIABLE, edict(), m_usEvent, 0.0f,
		pev->origin, pev->angles, 0, 0, 2, 0, 0, 0 );

	UTIL_Remove( this );
}

#endif
