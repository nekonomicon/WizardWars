#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_missile.h"


LINK_ENTITY_TO_CLASS( ww_spell_missile, WWMissileSpell );


void WWMissileSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_MISSILE;
	m_iDefaultAmmo	= 0;

	FallInit();
}


void WWMissileSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/magicmissile.wav"	);
	PRECACHE_MODEL( "models/spellbook.mdl"		);
	m_usEvent = PRECACHE_EVENT( 1, "events/spells/missile1.sc" );
}



int WWMissileSpell::iItemSlot( void )
{
	return SLOT_MISSILE + 1;
}


int WWMissileSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_MISSILE;
	pInfo->iSlot		= SLOT_MISSILE;
	pInfo->iPosition	= POS_MISSILE;
	pInfo->iWeight		= WEIGHT_MISSILE;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWMissileSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < MISSILE_COST )
		return FALSE;
	return TRUE;
}


BOOL WWMissileSpell::Deploy( void )
{
	if( m_pPlayer->m_pClass != NULL )
	{
		return DefaultDeploy(
			(char *)m_pPlayer->m_pClass->GetWizardHands(),
			(char *)m_pPlayer->m_pClass->GetWizardStaff(),
			HANDS_MISSILE_IDLE, "magicmissile" );
	}
	else
		return DefaultDeploy( "", "", HANDS_MISSILE_IDLE, "magicmissile" );
}


void WWMissileSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < MISSILE_COST )
	{
		RetireWeapon();
		return;
	}

	m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= MISSILE_COST;
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + MISSILE_DELAY;
	m_flTimeWeaponIdle	  = m_flNextPrimaryAttack;

	Vector vAngles = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;
	UTIL_MakeVectors( vAngles );

#ifndef CLIENT_DLL
	Vector vOrigin = m_pPlayer->GetGunPosition();
	vOrigin.z -= 8.0f;
	vOrigin = vOrigin + gpGlobals->v_forward * 8.0f;
	vOrigin = vOrigin - gpGlobals->v_right * 8.0f;

	CBaseEntity * pMissile = Create( "proj_missile", vOrigin, vAngles, m_pPlayer->edict() );

	if( pMissile != NULL )
	{
		pMissile->pev->angles = UTIL_VecToAngles( gpGlobals->v_forward );
		pMissile->pev->velocity = gpGlobals->v_forward * MISSILE_SPEED;
		pMissile->pev->flags |= FL_SKIPLOCALHOST;
	}
#endif

	PLAYBACK_EVENT_FULL( SC( FEV_NOTHOST, 0 ), m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, vAngles, 0, 0, 0, 0, 0, 0 );
}


void WWMissileSpell::WeaponIdle( void )
{
	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < MISSILE_COST )
	{
		RetireWeapon();
		return;
	}

	PLAYBACK_EVENT_FULL( FEV_HOSTONLY | FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}



#ifndef CLIENT_DLL

class WWMagicMissile : public CBaseAnimating
{
public:
	void Spawn( void )
	{
		Precache();

		SET_MODEL	( edict(), "models/missile.mdl" );
		SET_SIZE	( edict(), Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );
		SET_ORIGIN	( edict(), pev->origin );

		pev->movetype	= MOVETYPE_FLY;
		pev->solid		= SOLID_BBOX;
		pev->rendermode	= kRenderTransAdd;
		pev->renderamt	= 255;
		pev->renderfx	= kRenderFxMana;

		ResetSequenceInfo();

		PLAYBACK_EVENT_FULL( FEV_NOTHOST, edict(), m_usEvent, 0.0f, pev->origin, pev->angles,
			0, 0, 1, 0, 0, 0 );

		SetTouch( &WWMagicMissile::MissileTouch );
		pev->nextthink = gpGlobals->time + 0.1f;
	};

	void Precache( void )
	{
		PRECACHE_SOUND(	"weapons/xbow_hitbod1.wav"	);
		PRECACHE_MODEL( "sprites/xspark2.spr"		);
		PRECACHE_MODEL(	"models/missile.mdl"		);
		m_usEvent = PRECACHE_EVENT( 1, "events/spells/missile2.sc" );
	};

	void EXPORT	MissileTouch( CBaseEntity * pOther )
	{
		if( pOther->pev->takedamage )
		{
			TraceResult tr = UTIL_GetGlobalTrace();

			ClearMultiDamage();
			pOther->TraceAttack( VARS( pev->owner ), MISSILE_DAMAGE,
				pev->velocity.Normalize(), &tr, DMG_BULLET | DMG_NEVERGIB );
			ApplyMultiDamage( pev, VARS( pev->owner ) );
		}

		// missiles are predicted local, so don't send to host
		PLAYBACK_EVENT_FULL( FEV_NOTHOST, edict(), m_usEvent, 0.0f, pev->origin, pev->angles,
			0, 0, 0, 0, 0, 0 );

		UTIL_Remove( this );
	};

private:
	unsigned short m_usEvent;
};

LINK_ENTITY_TO_CLASS( proj_missile, WWMagicMissile );


#endif
