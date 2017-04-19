#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_rollingstone.h"


LINK_ENTITY_TO_CLASS( ww_spell_stone, WWRollingStoneSpell );


void WWRollingStoneSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_ROLLINGSTONE;
	m_iDefaultAmmo	= 0;

	FallInit();
}


void WWRollingStoneSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/stone_cast.wav"	);
	PRECACHE_MODEL( "models/spellbook.mdl"	);
	m_usEvent = PRECACHE_EVENT( 1, "events/spells/rollingstone1.sc" );
}


int WWRollingStoneSpell::iItemSlot( void )
{
	return SLOT_ROLLINGSTONE + 1;
}


int WWRollingStoneSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_ROLLINGSTONE;
	pInfo->iSlot		= SLOT_ROLLINGSTONE;
	pInfo->iPosition	= POS_ROLLINGSTONE;
	pInfo->iWeight		= WEIGHT_ROLLINGSTONE;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWRollingStoneSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < ROLLINGSTONE_COST )
		return FALSE;
	return TRUE;
}


BOOL WWRollingStoneSpell::Deploy( void )
{
	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_EARTH],
		(char *)szWizardStaves[WWCLASS_EARTH],
		HANDS_EARTH_ROLLINGSTONE_IDLE, "rollingstone" );
}


void WWRollingStoneSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < ROLLINGSTONE_COST )
	{
		RetireWeapon();
		return;
	}

	m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= ROLLINGSTONE_COST;
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + ROLLINGSTONE_DELAY;
	m_flTimeWeaponIdle	  = m_flNextPrimaryAttack;

#ifndef CLIENT_DLL
	Vector vAngles = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;
	UTIL_MakeVectors( vAngles );

	Vector vOrigin = m_pPlayer->GetGunPosition();
	vOrigin = vOrigin + gpGlobals->v_forward * 8.0f;

	CBaseEntity * pStone = Create( "proj_stone", vOrigin, vAngles, m_pPlayer->edict() );

	if( pStone != NULL )
	{
		pStone->pev->angles = UTIL_VecToAngles( gpGlobals->v_forward );
		pStone->pev->velocity = gpGlobals->v_forward * ROLLINGSTONE_SPEED;
		pStone->pev->team = m_pPlayer->pev->team;
	}
#endif

	PLAYBACK_EVENT_FULL( FEV_HOSTONLY | FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0,
		(float *)&g_vecZero, (float *)&g_vecZero, 0, 0, 0, 0, 0, 0 );
}


void WWRollingStoneSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < ROLLINGSTONE_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( FEV_HOSTONLY | FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}



#ifndef CLIENT_DLL

class WWRollingStone : public CBaseMonster
{
public:
	void		Spawn			( void );
	void		Precache		( void );
	void EXPORT StoneThink		( void );
	void EXPORT	StoneTouch		( CBaseEntity * pOther );
	BOOL		CanRepel		( CBaseEntity * pAttacker ) { return TRUE; };

private:
	float		m_flLife;
	unsigned short m_usEvent;
};


LINK_ENTITY_TO_CLASS( proj_stone, WWRollingStone );


void WWRollingStone:: Spawn( void )
{
	Precache();

	SET_MODEL	( edict(), "models/rollingstone.mdl" );
	SET_SIZE	( edict(), Vector( -4, -4, -4 ), Vector( 4, 4, 4 ) );
	SET_ORIGIN	( edict(), pev->origin );

	pev->movetype	= MOVETYPE_BOUNCE;
	pev->solid		= SOLID_BBOX;
	m_flLife		= gpGlobals->time + ROLLINGSTONE_LIFE;

	ResetSequenceInfo();

	PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f,
		pev->origin, pev->angles, 0, 0, 0, 0, 0, 0 );

	SetThink( &WWRollingStone::StoneThink );
	SetTouch( &WWRollingStone::StoneTouch );
	pev->nextthink = gpGlobals->time + 0.1f;
}


void WWRollingStone::Precache( void )
{
	PRECACHE_SOUND( "weapons/ric5.wav"	);
	PRECACHE_MODEL(	"models/rollingstone.mdl"	);

	m_usEvent = PRECACHE_EVENT( 1, "events/spells/rollingstone2.sc" );
}


void WWRollingStone::StoneThink( void )
{
	StudioFrameAdvance();

	if( gpGlobals->time >= m_flLife )
	{
		PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f,
			pev->origin, pev->angles, 0, 0, 3, 0, 0, 0 );
		UTIL_Remove( this );
	}

	pev->framerate = pev->velocity.Length() / ROLLINGSTONE_SPEED;

	pev->nextthink = gpGlobals->time + 0.1f;
}


void WWRollingStone::StoneTouch( CBaseEntity * pOther )
{
	if( pOther->pev->takedamage )
	{
		TraceResult tr = UTIL_GetGlobalTrace();

		ClearMultiDamage();
		pOther->TraceAttack( VARS( pev->owner ), ROLLINGSTONE_DAMAGE,
			pev->velocity.Normalize(), &tr, DMG_CRUSH | DMG_NEVERGIB );
		ApplyMultiDamage( pev, VARS( pev->owner ) );

		PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f,
			pev->origin, pev->angles, 0, 0, 2, 0, 0, 0 );

		UTIL_Remove( this );
	}
	else if( pev->velocity.Length() > ROLLINGSTONE_SPEED * 0.125f )
	{
		PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f,
			pev->origin, pev->angles, 0, 0, 1, 0, 0, 0 );
	}

	pev->velocity = pev->velocity * 0.75f;
}

#endif
