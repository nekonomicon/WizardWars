#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "gamerules.h"

#include "ww_shared/ww_defs.h"
#include "ww_thornbush.h"
#include "ww_wizards.h"
#include "ww_naturewizard.h"

#define THORNBUSH_LIFE_LEVEL1 250.0f//1.2.1 was 200
#define THORNBUSH_LIFE_LEVEL2 400.0f//1.2.1 was 250
#define THORNBUSH_LIFE_LEVEL3 550.0f//1.2.1 was 300

const thornbush_stats_t ThornBush[3] =
{
	{ THORNBUSH_LIFE_LEVEL1, 13.0f, 10.0f, 2.0f, 0.2f, 512.0f, 5.0f, "models/npc/thornbush1.mdl" },
	{ THORNBUSH_LIFE_LEVEL2, 15.0f, 11.0f, 1.5f, 0.2f, 768.0f, 5.5f, "models/npc/thornbush2.mdl" },
	{ THORNBUSH_LIFE_LEVEL3, 20.0f, 13.0f, 1.0f, 0.3f, 896.0f, 6.0f, "models/npc/thornbush3.mdl" },
};


LINK_ENTITY_TO_CLASS( thornbush, WWThornBush );


void WWThornBush::Spawn( void )
{
	Precache();

	SET_MODEL  ( edict(), ThornBush[0].model );
	SET_SIZE   ( edict(), Vector( -20, -20, 0 ), Vector( 20, 20, 80 ) );
	SET_ORIGIN ( edict(), pev->origin );

	pev->flags		|= FL_MONSTER;			// flag this so it is caught be UTIL_EntitiesInBox
	pev->movetype	= MOVETYPE_STEP;//1.2.1 was MOVETYPE_TOSS;
	pev->solid		= SOLID_BBOX;
	pev->takedamage	= DAMAGE_YES;
	pev->view_ofs	= Vector( 0, 0, 30 );
	pev->health		= ThornBush[0].health;
	pev->max_health	= ThornBush[0].health;

	m_flNextHunt	= 0.0f;
	m_flNextAttack	= 0.0f;
	m_flNextSpore	= 0.0f;

	m_iLevel		= 0;
	m_iEnchant		= 0;
	m_iDamageBits	= DMG_GENERIC;

	pev->frame = 0;
	pev->sequence = THORNBUSH_ANIM_GROW;
	ResetSequenceInfo();

	PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f, pev->origin, pev->angles,
		THORNBUSH_EVENT_SPAWN, 0.0f, 0, m_iLevel, m_iEnchant, 0 );

	//DROP_TO_FLOOR(edict());				// Drop to ground.

	SetTouch( NULL );
	SetThink(&WWThornBush:: GrowUpThink );
	pev->nextthink = gpGlobals->time + 0.1f;
}


void WWThornBush::Precache( void )
{
	int i;
	for( i = 0; i < 3; i++ )
		PRECACHE_MODEL( ThornBush[i].model );

	PRECACHE_MODEL( "models/bushgibs.mdl"			);
	PRECACHE_SOUND( "npc/thornbush_attack.wav"		);
	PRECACHE_SOUND( "npc/thornbush_upgrade1.wav"	);
	PRECACHE_SOUND( "npc/thornbush_upgrade2.wav"	);
	PRECACHE_SOUND( "npc/thornbush_upgrade3.wav"	);
	m_usEvent = PRECACHE_EVENT( 1, "events/thornbush.sc" );
}


void WWThornBush::GrowUpThink( void )
{
	StudioFrameAdvance(0.2);//1.2.1 was empty == 0.1?

	if( pev->effects & EF_NODRAW )
		pev->effects &= ~EF_NODRAW;

	if( m_fSequenceFinished )
	{
		pev->frame = 0;
		pev->sequence = RANDOM_LONG( THORNBUSH_ANIM_IDLE1, THORNBUSH_ANIM_IDLE5 );
		ResetSequenceInfo();

		SetTouch(&WWThornBush:: HuntTouch );
		SetThink(&WWThornBush:: HuntThink );
	}

	pev->nextthink = gpGlobals->time + 0.1f;
}


void WWThornBush::GrowDnThink( void )
{
	StudioFrameAdvance();

	if( pev->effects & EF_NODRAW )
		pev->effects &= ~EF_NODRAW;

	if( m_fSequenceFinished )
	{
		m_iLevel--;

		if( m_iLevel < 0 )
			Killed( NULL, 0 );
		else
		{
			pev->effects |= EF_NODRAW;

			pev->frame = 0;
			pev->sequence = RANDOM_LONG( THORNBUSH_ANIM_IDLE1, THORNBUSH_ANIM_IDLE5 );
			ResetSequenceInfo();

			SET_MODEL  ( edict(), ThornBush[m_iLevel].model );
			SET_SIZE   ( edict(), Vector( -20, -20, 0 ), Vector( 20, 20, 80 ) );
			SET_ORIGIN ( edict(), pev->origin );

			pev->max_health = ThornBush[m_iLevel].health;
			if( pev->health > pev->max_health )
				pev->health = pev->max_health;

			SetTouch(&WWThornBush:: HuntTouch );
			SetThink(&WWThornBush:: HuntThink );
		}
	}

	pev->nextthink = gpGlobals->time + 0.1f;
}


void WWThornBush::HuntTouch( CBaseEntity * pOther )
{
	if( IRelationship( pOther ) < R_NO )
		return;

	pOther->TakeDamage( pev, VARS(pev->euser1), ThornBush[m_iLevel].touch, DMG_GENERIC );
	m_flNextTouch = gpGlobals->time + 0.5f;
}


void WWThornBush::HuntThink( void )
{
	StudioFrameAdvance();

	if( pev->effects & EF_NODRAW )
		pev->effects &= ~EF_NODRAW;

	if( m_hTarget == NULL || gpGlobals->time >= m_flNextHunt )
	{
		FindEnemy();
		m_flNextHunt = gpGlobals->time + ThornBush[m_iLevel].hunt;
	}

	if( m_hTarget != NULL )
	{
		Vector vDiff = m_hTarget->pev->origin - pev->origin;

		if( !m_hTarget->IsAlive() || vDiff.Length() > ThornBush[m_iLevel].search + 64.0f )
		{
			m_hTarget = NULL;
			pev->nextthink = gpGlobals->time + 0.1f;
			return;
		}

		Vector vAngles = UTIL_VecToAngles( vDiff );
		pev->angles = vAngles;
		pev->angles.x = 0;

		if( gpGlobals->time > m_flNextAttack )
		{
			Fire();
			m_flNextAttack = gpGlobals->time + ThornBush[m_iLevel].delay;
		}

		if( gpGlobals->time > m_flNextSpore )
		{
			FireSpore();
			m_flNextSpore = gpGlobals->time + 1.0f;
		}

		if( gpGlobals->time > m_flNextRegen )
		{
			float flGain = ( (float)Illumination() / 255.0f ) * 20.0f;

			TakeHealth( flGain, DMG_GENERIC );
			m_flNextRegen = gpGlobals->time + ThornBush[m_iLevel].regen;
		}
	}

	if( pev->sequence == THORNBUSH_ANIM_ATTACK && m_hTarget == NULL )
	{
		pev->frame = 0;
		pev->sequence = RANDOM_LONG( THORNBUSH_ANIM_IDLE1, THORNBUSH_ANIM_IDLE5 );
		ResetSequenceInfo();
	}

	if( m_fSequenceFinished )
	{
		pev->frame = 0;
		pev->sequence = RANDOM_LONG( THORNBUSH_ANIM_IDLE1, THORNBUSH_ANIM_IDLE5 );
		ResetSequenceInfo();
	}

	if( !FBitSet( pev->flags, FL_ONGROUND ) )
		SetThink(&WWThornBush:: SUB_Remove );

	pev->nextthink = gpGlobals->time + 0.1f;
}


void WWThornBush::FindEnemy( void )
{
	CBaseEntity * pEntity = NULL;
	while( ( pEntity = UTIL_FindEntityInSphere( pEntity, pev->origin, ThornBush[m_iLevel].search ) ) != NULL )
	{
		if( pEntity == this )
			continue;

		if( !pEntity->IsAlive() || !pEntity->pev->takedamage )
			continue;

		if( !EnemyBetter( pEntity ) )
			continue;

		m_hTarget = pEntity;
		break;
	}
}


BOOL WWThornBush::EnemyBetter( CBaseEntity * pOther )
{
	if( IRelationship( pOther ) < R_NO )
		return FALSE;

	if( m_hTarget == NULL )
		return TRUE;

	if( pOther == m_hTarget )
		return FALSE;

	float flDist1 = ( pOther->pev->origin    - pev->origin ).Length();
	float flDist2 = ( m_hTarget->pev->origin - pev->origin ).Length();

	int iScore = 0;

	if( flDist1 < flDist2 )
		iScore++;

	if( pOther->pev->health < m_hTarget->pev->health )
		iScore++;

	TraceResult tr;
	UTIL_TraceLine( pev->origin, pOther->pev->origin, ignore_monsters, edict(), &tr );

	if( tr.flFraction == 1.0f )
		iScore++;
	else
	{
		UTIL_TraceLine( pev->origin, m_hTarget->pev->origin, ignore_monsters, edict(), &tr );

		if( tr.flFraction == 1.0f )
			iScore--;
	}

	if( pOther->IsPlayer() && !m_hTarget->IsPlayer() )
		iScore++;

	if( iScore > 0 )
		return TRUE;

	return FALSE;
}


void WWThornBush::Fire( void )
{
	if( m_hTarget == NULL )
		return;

//	CBaseEntity * pTarget = (CBaseEntity *)m_hTarget;

	Vector vecSrc = pev->origin + Vector( 0, 0, 40 );
	Vector vecDst = m_hTarget->BodyTarget( vecSrc );

	TraceResult tr;
	UTIL_TraceLine( vecSrc, vecDst, dont_ignore_monsters, edict(), &tr );

	if( !tr.pHit )
		return;

	CBaseEntity * pVictim = CBaseEntity::Instance( tr.pHit );

	if( !pVictim || !pVictim->IsAlive() || !pVictim->pev->takedamage )
		return;

	if( IRelationship( pVictim ) < R_NO )
		return;

	if( pev->sequence != THORNBUSH_ANIM_ATTACK )
	{
		pev->frame		= 0;
		pev->sequence	= THORNBUSH_ANIM_ATTACK;
		ResetSequenceInfo();
	}

	Vector vDir = ( vecDst - vecSrc ).Normalize();
	pev->angles = UTIL_VecToAngles( vDir );

	Vector vRand = Vector( RANDOM_FLOAT( -1, 1 ), RANDOM_FLOAT( -1, 1 ), RANDOM_FLOAT( -1, 1 ) );

	float flDmg = ThornBush[m_iLevel].damage;

	if( m_iEnchant )
		flDmg *= 1.2f;

	switch( m_iEnchant )
	{
	default:
	case 0: // normal
		break;

	case 1: // fire
		break;

	case 2: // ice
		pVictim->pev->punchangle = vRand * 20.0f;
		break;

	case 3: // lightning
		break;

	case 4: // wind
		pVictim->pev->velocity = pVictim->pev->velocity + vRand * 60.0f;
		break;
	}

	ClearMultiDamage();				// +MAH 20040801 -- fix crashes in Thornplant attacks
	m_iDamageBits |= DMG_GENERIC;	// +MAH 20040801 -- fix no damage from TP attacks - failed.
	pVictim->TraceAttack( pev, flDmg, vDir, &tr, m_iDamageBits );
	ApplyMultiDamage( pev, VARS(pev->euser1) );

	PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f, pev->origin, vDir,
		THORNBUSH_EVENT_FIRE, 0.0f, pVictim->entindex(), m_iLevel, m_iEnchant, 0 );
}



void WWThornBush::FireSpore( void )
{
	if( m_iLevel < 2 )
		return;

	if( m_hTarget == NULL )
		return;
	Vector vecSrc = pev->origin;
	Vector vecDst = m_hTarget->BodyTarget( vecSrc );
	Vector vecDir = ( vecDst - vecSrc ).Normalize();
	vecSrc = vecSrc + (vecDir * 20.0f);
	vecSrc = vecSrc + Vector(0, 0, 20.0f);

	TraceResult tr;
	UTIL_TraceLine( vecSrc, vecDst, dont_ignore_monsters, edict(), &tr );

	CBaseEntity * pVictim = CBaseEntity::Instance( tr.pHit );

	if( !pVictim || !pVictim->IsAlive() || !pVictim->pev->takedamage )
		return;

	if( IRelationship( pVictim ) < R_NO )
		return;

	CBaseEntity * pSpore = CBaseEntity::Create( "spore", vecSrc, vecDir, pev->euser1 );

	if( pSpore != NULL )
	{
		pSpore->pev->team = pev->team;
		pSpore->pev->skin = pev->team;
		pSpore->pev->velocity = vecDir * 800.0f;   // slow this way down for test

		PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f, pev->origin, pev->angles,
			THORNBUSH_EVENT_FIRESPORE, 0.0f, pSpore->entindex(), m_iLevel, m_iEnchant, 0 );
	}
}


int WWThornBush::TakeDamage( entvars_t * pevInf, entvars_t * pevAtk, float flDamage, int bitsDamageType )
{
	if( pevAtk != NULL )
	{
		CBaseEntity * pAttacker = CBaseEntity::Instance( pevAtk );

		if( !pAttacker )
			return 0;

		if( IRelationship( pAttacker ) < R_NO )
			return 0;

		if( EnemyBetter( pAttacker ) )
			m_hTarget = pAttacker;
	}

	return CBaseAnimating::TakeDamage( pevInf, pevAtk, flDamage, bitsDamageType );
}


void WWThornBush::Killed( entvars_t * pevAttacker, int iGib )
{
	if( pev->euser1 != NULL )
	{
// 1.2.6 TP Linux Crash Bug Fix #13
//		NatureWizard * pWizard = (NatureWizard *)CBaseEntity::Instance( pev->euser1 );
		CBasePlayer *pl = (CBasePlayer *)CBaseEntity::Instance( pev->euser1 );
		NatureWizard * pWizard = (NatureWizard *)pl->m_pClass;

		if( pWizard != NULL )
			pWizard->RemoveBush( this );
	}

	if( pevAttacker != NULL )
	{
		CBaseEntity * pAttacker = CBaseEntity::Instance( pevAttacker );

		if( !pAttacker || !pAttacker->IsPlayer() )
			return;

		if( !IsAlive() )
			return;

		if( IRelationship( pAttacker ) >= R_NO )
			pAttacker->pev->frags++;

		extern int gmsgDeathMsg;
		MESSAGE_BEGIN( MSG_ALL, gmsgDeathMsg );
			WRITE_BYTE( pAttacker->entindex() );
			WRITE_BYTE( -1 );
			WRITE_STRING( "thornbush" );
		MESSAGE_END();

		PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f, pev->origin, pev->angles,
			THORNBUSH_EVENT_KILLED, 0.0f, pAttacker->entindex(), m_iLevel, m_iEnchant, 0 );
	}
	else
	{
		PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f, pev->origin, pev->angles,
			THORNBUSH_EVENT_KILLED, 0.0f, 0, m_iLevel, m_iEnchant, 0 );
	}

	UTIL_Remove( this );
}


BOOL WWThornBush::Enchant( int iClass )
{
	switch( iClass )
	{
	case WWCLASS_FIRE:
		pev->renderfx	 = kRenderFxGlowShell;
		pev->renderamt	 = 1;
		pev->rendercolor = Vector( 255, 0, 0 );
		m_iEnchant		 = 1;
		m_iDamageBits	 = DMG_BURN | DMG_SLOWBURN;
		break;

	case WWCLASS_ICE:
		pev->renderfx	 = kRenderFxGlowShell;
		pev->renderamt	 = 1;
		pev->rendercolor = Vector( 0, 0, 255 );
		m_iEnchant		 = 2;
		m_iDamageBits	 = DMG_FREEZE;
		break;

	case WWCLASS_LIGHTNING:
		pev->renderfx	 = kRenderFxGlowShell;
		pev->renderamt	 = 1;
		pev->rendercolor = Vector( 255, 255, 0 );
		m_iEnchant		 = 3;
		m_iDamageBits	 = DMG_SHOCK;
		break;

	case WWCLASS_WIND:
		pev->renderfx	 = kRenderFxGlowShell;
		pev->renderamt	 = 1;
		pev->rendercolor = Vector( 128, 0, 255 );
		m_iEnchant		 = 4;
		m_iDamageBits	 = DMG_SONIC;
		break;

	default:
		return FALSE;
	}

	PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f, pev->origin, pev->angles,
		THORNBUSH_EVENT_ENCHANT, 0.0f, 0, m_iLevel, m_iEnchant, 0 );

	return TRUE;
}


BOOL WWThornBush::Fertilize( void )
{
	// already fertilizing or harvesting
	if( pev->sequence == THORNBUSH_ANIM_GROW ||
		pev->sequence == THORNBUSH_ANIM_HARVEST )
		return FALSE;

	if( m_iLevel >= 2 )
		return FALSE;

	m_iLevel++;

	SET_MODEL  ( edict(), ThornBush[m_iLevel].model );
	SET_SIZE   ( edict(), Vector( -20, -20, 0 ), Vector( 20, 20, 80 ) );
	SET_ORIGIN ( edict(), pev->origin );

	pev->effects	|= EF_NODRAW;
	pev->health		 = ThornBush[m_iLevel].health;
	pev->max_health	 = ThornBush[m_iLevel].health;

	pev->frame = 0;
	pev->sequence = THORNBUSH_ANIM_GROW;
	ResetSequenceInfo();

	PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f, pev->origin, pev->angles,
		THORNBUSH_EVENT_FERTILIZE, 0.0f, 0, m_iLevel, m_iEnchant, 0 );

	SetTouch( NULL );
	SetThink(&WWThornBush:: GrowUpThink );
	pev->nextthink = gpGlobals->time + 0.1f;

	return TRUE;
}


BOOL WWThornBush::Harvest( void )
{
	// already fertilizing or harvesting
	if( pev->sequence == THORNBUSH_ANIM_GROW ||
		pev->sequence == THORNBUSH_ANIM_HARVEST )
		return FALSE;

	if( m_iLevel < 0 )
		return FALSE;

	pev->frame = 0;
	pev->sequence = THORNBUSH_ANIM_HARVEST;
	ResetSequenceInfo();

	PLAYBACK_EVENT_FULL( 0, edict(), m_usEvent, 0.0f, pev->origin, pev->angles,
		THORNBUSH_EVENT_HARVEST, 0.0f, 0, m_iLevel, m_iEnchant, 0 );

	SetTouch( NULL );
	SetThink(&WWThornBush:: GrowDnThink );
	pev->nextthink = gpGlobals->time + 0.1f;

	return TRUE;
}


WWThornBush * WWThornBush::FindBush( CBasePlayer * pPlayer, float flDist, BOOL bOwn )
{
	if( !pPlayer )
		return NULL;

	NatureWizard * pWizard = NULL;

	if( bOwn )
	{
		if( !pPlayer->m_pClass || pPlayer->m_pClass->GetClassNumber() != WWCLASS_NATURE )
			return NULL;

		pWizard = (NatureWizard *)pPlayer->m_pClass;
	}

	CBaseEntity * pBest = NULL;
	CBaseEntity * pSearch = NULL;
	while( ( pSearch = UTIL_FindEntityInSphere( pSearch, pPlayer->pev->origin, flDist ) ) != NULL )
	{
		if( pWizard != NULL )
		{
			if( pSearch != (CBaseEntity *)pWizard->GetBush(0) &&
				pSearch != (CBaseEntity *)pWizard->GetBush(1) )
				continue;
		}
		else
		{
			if( !FClassnameIs( pSearch->pev, "thornbush" ) )
				continue;

			if( pPlayer != NULL && pPlayer->IRelationship( pSearch ) >= R_NO )
				continue;
		}

		if( !pBest )
		{
			pBest = pSearch;
			continue;
		}

		float flDist1 = ( pPlayer->pev->origin - pSearch->pev->origin ).Length();
		float flDist2 = ( pPlayer->pev->origin - pBest->pev->origin ).Length();

		if( flDist1 < flDist2 )
			pBest = pSearch;

		continue;
	}

	return (WWThornBush *)pBest;
}

// MAH 20040804 - Spore needs to be a *monster*, like the other spell effects
// class WWSpore : public CBaseAnimating
class WWSpore : public CBaseMonster
{
public:
	void Spawn( void )
	{
		Precache();

		SET_MODEL  ( edict(), "models/npc/spore.mdl" );
// +MAH 20040805 - Spore Needs a *size* so it can hit things! Therefore we
//	shall start with a 8 unit cube for it's BBOX, same size as the stone
//  from the Rolling Stone attack.
//		SET_SIZE   ( edict(), Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );
		SET_SIZE   ( edict(), Vector(-4, -4, -4), Vector (4, 4, 4) );
// -MAH 20040805

		SET_ORIGIN ( edict(), pev->origin );

		pev->movetype	= MOVETYPE_FLY;
		pev->solid		= SOLID_NOT;
		pev->health		= SPORE_HEALTH;
		pev->dmgtime	= gpGlobals->time + SPORE_DAMAGE_TIME;

		pev->sequence = 0;
		ResetSequenceInfo();

		SetTouch(&WWSpore :: SporeUntouchable );
		SetThink(&WWSpore :: SporeArmingDelayThink );
		pev->nextthink = gpGlobals->time + 0.2f;
	};

	void Precache( void )
	{
		PRECACHE_MODEL( SPORE_MODEL );
		PRECACHE_SOUND( SPORE_SOUND	);
		m_usEvent = PRECACHE_EVENT( 1, SPORE_EVENT );
	};

	// added a delay think so that we don't set 

	void EXPORT SporeArmingDelayThink( void ) {

		StudioFrameAdvance();
		pev->solid = SOLID_BBOX;
		SetTouch(&WWSpore :: SporeTouch );
		SetThink(&WWSpore :: SporeArmedThink );
		pev->nextthink = gpGlobals->time + 0.1f;
	}

	void EXPORT SporeArmedThink( void )
	{
		StudioFrameAdvance();

		if( gpGlobals->time >= pev->dmgtime )
			UTIL_Remove( this );

		pev->nextthink = gpGlobals->time + 0.1f;
	};

	void EXPORT SporeUntouchable( CBaseEntity *pother) {

		return;
	}

	void EXPORT SporeTouch( CBaseEntity * pOther )
	{
		::RadiusDamage( pev->origin, pev, VARS(pev->euser1), 100.0f, 200.0f, CLASS_NONE, DMG_POISON );
		PLAYBACK_EVENT( 0, edict(), m_usEvent );

		UTIL_Remove( this );
	};

private:
	unsigned short	m_usEvent;
};


LINK_ENTITY_TO_CLASS( spore, WWSpore );
