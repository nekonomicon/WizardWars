#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_icepoke.h"


LINK_ENTITY_TO_CLASS( ww_spell_iceshard, WWIcePokeSpell );


void WWIcePokeSpell::Spawn( void )
{
	Precache();

	SET_MODEL( ENT(pev), "models/spellbook.mdl" );

	m_iId			= WEAPON_ICEPOKE;
	m_iDefaultAmmo	= 0;
	m_fireState		= 0;

	FallInit();
}


void WWIcePokeSpell::Precache( void )
{
	PRECACHE_SOUND( "spells/icepoke.wav"	);
	m_usEvent = PRECACHE_EVENT( 1, "events/spells/icepoke1.sc" );
}


int WWIcePokeSpell::iItemSlot( void )
{
	return SLOT_ICEPOKE + 1;
}


int WWIcePokeSpell::GetItemInfo( ItemInfo * pInfo )
{
	pInfo->iId = m_iId	= WEAPON_ICEPOKE;
	pInfo->iSlot		= SLOT_ICEPOKE;
	pInfo->iPosition	= POS_ICEPOKE;
	pInfo->iWeight		= WEIGHT_ICEPOKE;
	pInfo->pszName		= STRING( pev->classname );
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;
	pInfo->iMaxClip		= WEAPON_NOCLIP;
	pInfo->iFlags		= 0;

	return 1;
}


BOOL WWIcePokeSpell::CanDeploy( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < ICEPOKE_COST )
		return FALSE;

	return TRUE;
}


BOOL WWIcePokeSpell::Deploy( void )
{
	m_fireState			  = -1;
	m_flNextPrimaryAttack = 0.0f;
	m_flTimeWeaponIdle	  = 0.0f;

	return DefaultDeploy(
		(char *)szWizardHands[WWCLASS_ICE],
		(char *)szWizardStaves[WWCLASS_ICE],
		HANDS_ICE_ICEPOKE_IDLE, "icepoke" );
}


void WWIcePokeSpell::Holster( int skiplocal )
{
	CBasePlayerWeapon::Holster( skiplocal );
}


void WWIcePokeSpell::PrimaryAttack( void )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] < ICEPOKE_COST )
	{
		RetireWeapon();
		return;
	}

	if( m_fireState == 0 )
	{
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + ICEPOKE_DELAY;
		m_flTimeWeaponIdle	  = UTIL_WeaponTimeBase() + ICEPOKE_DELAY;

		m_fireState = -1;
		return;
	}

	if( m_fireState == -1 )
	{
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + ICEPOKE_CYCLEDELAY;
		m_flTimeWeaponIdle	  = UTIL_WeaponTimeBase() + ICEPOKE_CYCLEDELAY + ICEPOKE_CYCLETIME + ICEPOKE_DELAY;

		m_fireState = ICEPOKE_COUNT;
		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

		PLAYBACK_EVENT_FULL( FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0.0f,
			m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 2, 0, 0, 0 );
		return;
	}

	m_fireState--;

	float a = ( (float)m_fireState / (float)ICEPOKE_COUNT ) * 360.0f - 90.0f;
	float r = a * ( M_PI / 180.0f );
	float s = sinf( r );
	float c = cosf( r );

#ifndef CLIENT_DLL
	Vector vAngles = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;
	UTIL_MakeVectors( vAngles );

	Vector vOrigin = m_pPlayer->GetGunPosition();
	vOrigin.z -= 8.0f;

	vOrigin = vOrigin + gpGlobals->v_forward   * 16.0f;
	vOrigin = vOrigin + gpGlobals->v_right * s * 16.0f;
	vOrigin = vOrigin - gpGlobals->v_up    * c * 16.0f;

	CBaseEntity * pMissile = Create( "proj_iceshard", vOrigin, vAngles, m_pPlayer->edict() );

	if( pMissile != NULL )
	{
		pMissile->pev->angles = UTIL_VecToAngles( gpGlobals->v_forward );
		pMissile->pev->velocity = gpGlobals->v_forward * ICEPOKE_SPEED;
		pMissile->pev->flags |= FL_SKIPLOCALHOST;
	}
#endif

	PLAYBACK_EVENT_FULL( FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, s, c, 0, m_fireState, 0, 0 );

	m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= ICEPOKE_COST;

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + ICEPOKE_SINGLESHOT;
}


void WWIcePokeSpell::WeaponIdle( void )
{
	m_fireState = -1;

	if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	PLAYBACK_EVENT_FULL( FEV_CLIENT, m_pPlayer->edict(), m_usEvent, 0.0f,
		m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0, 0, 1, 0, 0, 0 );

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}


#ifndef CLIENT_DLL

class WWIcePoke : public CBaseAnimating
{
public:
	void		Spawn			( void );
	void		Precache		( void );
	void EXPORT	MissileTouch	( CBaseEntity * pOther );

private:
	unsigned short m_usEvent;
};


LINK_ENTITY_TO_CLASS( proj_iceshard, WWIcePoke );


void WWIcePoke:: Spawn( void )
{
	Precache();

	SET_MODEL	( edict(), "models/iceshard.mdl" );
	SET_SIZE	( edict(), Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );
	SET_ORIGIN	( edict(), pev->origin );

	pev->movetype	= MOVETYPE_FLY;
	pev->solid		= SOLID_BBOX;
	pev->rendermode	= kRenderTransAdd;
	pev->renderamt	= 250;

	ResetSequenceInfo();

	PLAYBACK_EVENT_FULL( FEV_NOTHOST, edict(), m_usEvent, 0.0f, pev->origin, pev->angles, 0, 0, 1, 0, 0, 0 );

	SetTouch( &WWIcePoke::MissileTouch );
	pev->nextthink = gpGlobals->time + 0.1f;
}


void WWIcePoke::Precache( void )
{
	PRECACHE_SOUND(	"weapons/xbow_hitbod1.wav"	);
	PRECACHE_MODEL( "sprites/xspark2.spr"		);
	PRECACHE_MODEL(	"models/iceshard.mdl"		);

	m_usEvent = PRECACHE_EVENT( 1, "events/spells/icepoke2.sc" );
}


void WWIcePoke::MissileTouch( CBaseEntity * pOther )
{
	if( pOther->pev->takedamage )
	{
		TraceResult tr = UTIL_GetGlobalTrace();

		ClearMultiDamage();
		pOther->TraceAttack( VARS( pev->owner ), ICEPOKE_DAMAGE,
			pev->velocity.Normalize(), &tr, DMG_FREEZE | DMG_NEVERGIB );
		ApplyMultiDamage( pev, VARS( pev->owner ) );

		if( pOther->IsPlayer() )
		{
			CBasePlayer * pPlayer = (CBasePlayer *)pOther;

			if( pPlayer->IRelationship( this ) >= R_NO )
			{
				if( pPlayer->GetTimer( TIMER_FROZEN ) )
					pPlayer->SetTimer( TIMER_FROZEN, ICEPOKE_FROSTTIME * 0.25f, true );
				else
					pPlayer->SetTimer( TIMER_FROSTED, ICEPOKE_FROSTTIME, true );
			}
		}
	}

	// missiles are predicted local, so don't send to host
	PLAYBACK_EVENT_FULL( FEV_NOTHOST, edict(), m_usEvent, 0.0f, pev->origin, pev->angles, 0, 0, 0, 0, 0, 0 );

	UTIL_Remove( this );
}

#endif
