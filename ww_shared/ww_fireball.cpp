#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_fireball.h"


LINK_ENTITY_TO_CLASS( ww_spell_fireball, WWFireBallSpell );


void WWFireBallSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_FIREBALL;
	m_iDefaultAmmo	= 0;

	FallInit();
}


void WWFireBallSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/fireball_cast.wav"	);
	PRECACHE_MODEL( "models/spellbook.mdl"		);
	PRECACHE_MODEL( "models/rollingstone.mdl"	);
	PRECACHE_MODEL( "sprites/animglow01.spr"	);

	m_usEvent = PRECACHE_EVENT( 1, "events/spells/fireball1.sc" );
}



int WWFireBallSpell::iItemSlot( void )
{
	return SLOT_FIREBALL + 1;
}


int WWFireBallSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_FIREBALL;
	pInfo->iSlot		= SLOT_FIREBALL;
	pInfo->iPosition	= POS_FIREBALL;
	pInfo->iWeight		= WEIGHT_FIREBALL;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWFireBallSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < FIREBALL_COST )
		return FALSE;
	return TRUE;
}


BOOL WWFireBallSpell::Deploy( void )
{
	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_FIRE],
		(char *)szWizardStaves[WWCLASS_FIRE],
		HANDS_FIRE_FIREBALL_IDLE, "fireball" );
}


void WWFireBallSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < FIREBALL_COST )
	{
		RetireWeapon();
		return;
	}

	m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= FIREBALL_COST;

	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + FIREBALL_DELAY;
	m_flTimeWeaponIdle	  = m_flNextPrimaryAttack;

	Vector vAngles = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;
	UTIL_MakeVectors( vAngles );

#ifndef CLIENT_DLL
	Vector vOrigin = m_pPlayer->GetGunPosition();
	vOrigin.z -= 8.0f;
	vOrigin = vOrigin + gpGlobals->v_forward * 8.0f;

	CBaseEntity * pMissile = Create( "proj_fireball", vOrigin, vAngles, m_pPlayer->edict() );

	if( pMissile != NULL )
	{
		pMissile->pev->angles = UTIL_VecToAngles( gpGlobals->v_forward );
		pMissile->pev->velocity = gpGlobals->v_forward * FIREBALL_SPEED;
		pMissile->pev->flags |= FL_SKIPLOCALHOST;
		pMissile->pev->effects |= EF_LIGHT;
	}
#endif

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, vAngles, 0, 0, 0, 0, 0, 0 );
}


void WWFireBallSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < FIREBALL_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}



#ifndef CLIENT_DLL

class WWFireBall : public CBaseAnimating
{
public:
	void		Spawn			( void );
	void		Precache		( void );
	void EXPORT	MissileTouch	( CBaseEntity * pOther );

private:
	unsigned short m_usEvent;
};


LINK_ENTITY_TO_CLASS( proj_fireball, WWFireBall );


void WWFireBall:: Spawn( void )
{
	Precache();

	SET_MODEL	( edict(), "models/rollingstone.mdl" );
	SET_SIZE	( edict(), Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );
	SET_ORIGIN	( edict(), pev->origin );

	pev->movetype		= MOVETYPE_FLY;
	pev->solid			= SOLID_BBOX;
	pev->rendermode		= kRenderNormal;
	pev->rendercolor	= Vector( 255, 0, 0 );
	pev->renderamt		= 5;
	pev->renderfx		= kRenderFxGlowShell;

	ResetSequenceInfo();

	PLAYBACK_EVENT_FULL( FEV_NOTHOST, edict(), m_usEvent, 0.0f, pev->origin, pev->angles, 0, 0, 1, 0, 0, 0 );

	SetTouch( &WWFireBall::MissileTouch );
	pev->nextthink = gpGlobals->time + 0.1f;
}


void WWFireBall::Precache( void )
{
	PRECACHE_MODEL( "sprites/zerogxplode.spr"		);
	PRECACHE_MODEL(	"models/rollingstone.mdl"		);
	PRECACHE_SOUND( "spells/fireball_ambience.wav"	);

	m_usEvent = PRECACHE_EVENT( 1, "events/spells/fireball2.sc" );
}


void WWFireBall::MissileTouch( CBaseEntity * pOther )
{
	if( pOther->pev->takedamage )
	{
		TraceResult tr = UTIL_GetGlobalTrace();

		ClearMultiDamage();
		pOther->TraceAttack( VARS( pev->owner ), FIREBALL_DAMAGE,
			pev->velocity.Normalize(), &tr, DMG_BURN | DMG_SLOWBURN );
		ApplyMultiDamage( pev, VARS( pev->owner ) );
	}

	ClearMultiDamage();
	::RadiusDamage( pev->origin, pev, VARS(pev->owner), FIREBALL_DAMAGE, FIREBALL_RADIUS,
		CLASS_NONE, DMG_BURN | DMG_SLOWBURN );
	ApplyMultiDamage( pev, VARS( pev->owner ) );

	// missiles are predicted local, so don't send to host
	PLAYBACK_EVENT_FULL( FEV_NOTHOST | FEV_GLOBAL | FEV_RELIABLE, edict(), m_usEvent, 0.0f, pev->origin, pev->angles, 0, 0, 0, 0, 0, 0 );

	UTIL_Remove( this );
}
#endif
