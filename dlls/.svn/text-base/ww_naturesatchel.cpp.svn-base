#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "gamerules.h"
#include "effects.h"

#include "ww_satchels.h"


class NatureSatchel : public WWSatchel
{
public:
	virtual void	Spawn			( void );
	virtual void	Precache		( void );

	virtual void	SatchelExplode	( void );
};


LINK_ENTITY_TO_CLASS( ww_satchel_nature, NatureSatchel );


void NatureSatchel::Spawn( void )
{
	WWSatchel::Spawn();


	m_iExplodeCount = 3;
	m_flMultiDelay	= 0.5f;
}


void NatureSatchel::Precache( void )
{
	PRECACHE_MODEL( "models/satchels/nature.mdl"	);
	PRECACHE_SOUND( "satchels/nature.wav"			);
	m_usEvent = PRECACHE_EVENT( 1, "events/satchels/nature.sc" );
	SET_MODEL( edict(), "models/satchels/nature.mdl" );
}


void NatureSatchel::SatchelExplode( void )
{
	::RadiusDamage( pev->origin, pev, VARS(pev->owner), 50.0f, 100.0f, CLASS_NONE, DMG_ACID );

	Vector vThrow;
	vThrow.x = RANDOM_FLOAT( -100.0f, 100.0f );
	vThrow.y = RANDOM_FLOAT( -100.0f, 100.0f );
	vThrow.z = RANDOM_FLOAT(  200.0f, 300.0f );

	Vector vOrigin = pev->origin;
	vOrigin.x += RANDOM_FLOAT( -16.0f, 16.0f );
	vOrigin.y += RANDOM_FLOAT( -16.0f, 16.0f );
	vOrigin.z += RANDOM_FLOAT(   8.0f, 16.0f );

	CBaseEntity * pBur = CBaseEntity::Create( "cocklebur", vOrigin, UTIL_VecToAngles( vThrow ), pev->owner );

	if( pBur != NULL )
	{
		pBur->pev->velocity = vThrow;
		pBur->pev->team		= pev->team;
	}

	PLAYBACK_EVENT( 0, edict(), m_usEvent );
}



class CockleBur : public CBaseEntity
{
public:
	void		Spawn		( void );
	void		Precache	( void );

	void		Killed		( entvars_t * pevAttacker, int iGib );
	int			TakeDamage	( entvars_t * pevInflictor, entvars_t * pevAttacker, float flDamage, int bitsDamageType );

	void EXPORT	BurThink	( void );
	void EXPORT	BurTouch	( CBaseEntity * pOther );

private:
	float			m_flLifeTime;
	float			m_flDamageTime;
	EHANDLE			m_hVictim;
	unsigned short	m_usEvent;
};


LINK_ENTITY_TO_CLASS( cocklebur, CockleBur );


void CockleBur::Spawn( void )
{
	Precache();

	SET_MODEL  ( edict(), "models/npc/cocklebur.mdl" );
	SET_SIZE   ( edict(), Vector( -2, -2, -2 ), Vector( 2, 2, 4 ) );
	SET_ORIGIN ( edict(), pev->origin );

	pev->movetype	= MOVETYPE_TOSS;
	pev->solid		= SOLID_BBOX;
	pev->health		= 25;
	pev->takedamage	= DAMAGE_YES;

	m_flLifeTime	= gpGlobals->time + 30.0f;
	m_flDamageTime	= 0.0f;
	m_hVictim		= NULL;

	SetTouch(&CockleBur:: BurTouch );
	SetThink(&CockleBur:: BurThink );
	pev->nextthink = gpGlobals->time + 0.1f;
}


void CockleBur::Precache( void )
{
	PRECACHE_MODEL( "models/npc/cocklebur.mdl"	);
	PRECACHE_SOUND( "npc/cocklebur.wav"			);
	m_usEvent = PRECACHE_EVENT( 1, "events/cocklebur.sc" );
}


void CockleBur::Killed( entvars_t * pevAttacker, int iGib )
{
	m_hVictim = NULL;
	PLAYBACK_EVENT_SHORT( 0, edict(), m_usEvent, 0, 0, 2, 0, 0, 0 );

	CBaseEntity::Killed( pevAttacker, iGib );
}

int CockleBur::TakeDamage( entvars_t * pevInflictor, entvars_t * pevAttacker, float flDamage, int bitsDamageType )
{
	if( bitsDamageType & DMG_ACID )
		return 0;

	CBaseEntity * pAttacker = (CBaseEntity *)CBaseEntity::Instance( pevAttacker );

	if( !pAttacker || !pAttacker->IsPlayer() )
		return 0;

	return CBaseEntity::TakeDamage( pevInflictor, pevAttacker, flDamage, bitsDamageType );
}


void CockleBur::BurThink( void )
{
	if( pev->owner != NULL )
	{
		pev->euser1 = pev->owner;
		pev->owner = NULL;
	}

	if( gpGlobals->time >= m_flLifeTime )
	{
		Killed( NULL, 0 );
		return;
	}

	if( (int)m_hVictim != NULL )
	{
		if( !m_hVictim->IsAlive() )
		{
			m_hVictim = NULL;
			m_flLifeTime = 0.0f;
			// 1.2.6 #11 burrs not dieing, must do one more think
			pev->nextthink = gpGlobals->time + 0.1f;
			return;
		}

		if( ( pev->origin - m_hVictim->pev->origin ).Length2D() <= 20.0f )
		{
			if( gpGlobals->time >= m_flDamageTime )
			{
				m_hVictim->TakeDamage( pev, VARS(pev->euser1), 4.5f, DMG_ACID );
				m_flDamageTime = gpGlobals->time + 1.0f;
			}

			pev->velocity = Vector( 0, 0, 0 );
		}
		else
		{
			Vector vDiff = ( m_hVictim->pev->origin - pev->origin ).Normalize();
			pev->velocity = vDiff * 600.0f;
		}
	}

	pev->nextthink = gpGlobals->time + 0.1f;
}


void CockleBur::BurTouch( CBaseEntity * pOther )
{
	if( !pOther->IsPlayer() || !pOther->IsAlive() )
		return;

	if( pev->euser1 == NULL )
		return;

	CBasePlayer * pOwner = (CBasePlayer *)CBaseEntity::Instance( pev->euser1 );

	if( pOwner != NULL && pOwner->IRelationship( pOther ) < R_NO )
		return;

	if( !(int)m_hVictim || pOther != (CBaseEntity *)m_hVictim )
		PLAYBACK_EVENT_SHORT( 0, edict(), m_usEvent, 0, 0, 1, 0, 0, 0 );

	pev->movetype	= MOVETYPE_FLY;
	m_hVictim		= pOther;

	SetTouch( NULL );
}

