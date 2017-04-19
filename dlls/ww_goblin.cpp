#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"squadmonster.h"
#include	"schedule.h"
#include	"effects.h"
#include	"weapons.h"
#include	"soundent.h"


class WWGoblin : public CSquadMonster
{
public:
	void	Spawn			( void );
	void	Precache		( void );
	void	SetYawSpeed		( void );
	int		ISoundMask		( void ) { return bits_SOUND_WORLD | bits_SOUND_COMBAT | bits_SOUND_DANGER | bits_SOUND_PLAYER; };
	int		Classify		( void ) { return CLASS_ALIEN_MILITARY; };
	int		IRelationship	( CBaseEntity *pTarget );
	void	HandleAnimEvent	( MonsterEvent_t *pEvent );
	void	CallForHelp		( char *szClassname, float flDist, EHANDLE hEnemy, Vector &vecLocation );
	int		TakeDamage		( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType );

	void	DeathSound		( void );
	void	PainSound		( void );
	void	AlertSound		( void );

	Schedule_t *	GetSchedule			( void );
	Schedule_t *	GetScheduleOfType	( int Type );
	CUSTOM_SCHEDULES;

	static const char *pAttackHitSounds[];
	static const char *pAttackMissSounds[];
	static const char *pPainSounds[];
	static const char *pDeathSounds[];
};
LINK_ENTITY_TO_CLASS( ww_goblin, WWGoblin );
LINK_ENTITY_TO_CLASS( monster_goblin, WWGoblin );


const char * WWGoblin::pAttackHitSounds[] = 
{
	"npc/goblin/claw1.wav",
	"npc/goblin/claw2.wav",
	"npc/goblin/claw3.wav",
};


const char * WWGoblin::pAttackMissSounds[] = 
{
	"npc/goblin/miss1.wav",
	"npc/goblin/miss2.wav",
};


const char * WWGoblin::pPainSounds[] = 
{
	"npc/goblin/pain1.wav",
	"npc/goblin/pain2.wav",
};


const char * WWGoblin::pDeathSounds[] = 
{
	"npc/goblin/die1.wav",
	"npc/goblin/die2.wav",
};


int WWGoblin::IRelationship( CBaseEntity *pTarget )
{
	if ( pTarget->IsPlayer() )
		if ( pev->spawnflags & SF_MONSTER_WAIT_UNTIL_PROVOKED && !( m_afMemory & bits_MEMORY_PROVOKED ) )
			return R_NO;
	return CBaseMonster::IRelationship( pTarget );
}


void WWGoblin::CallForHelp( char *szClassname, float flDist, EHANDLE hEnemy, Vector &vecLocation )
{
	if ( FStringNull( pev->netname ))
		return;

	CBaseEntity *pEntity = NULL;

	while ((pEntity = UTIL_FindEntityByString( pEntity, "netname", STRING( pev->netname ))) != NULL)
	{
		float d = (pev->origin - pEntity->pev->origin).Length();
		if (d < flDist)
		{
			CBaseMonster *pMonster = pEntity->MyMonsterPointer( );
			if (pMonster)
			{
				pMonster->m_afMemory |= bits_MEMORY_PROVOKED;
				pMonster->PushEnemy( hEnemy, vecLocation );
			}
		}
	}
}


void WWGoblin::AlertSound( void )
{
	if ( m_hEnemy != NULL )
	{
		SENTENCEG_PlayRndSz( ENT(pev), "SLV_ALERT", 0.85, ATTN_NORM, 0, 90 );

		CallForHelp( "monster_goblin", 512, m_hEnemy, m_vecEnemyLKP );
	}
}


void WWGoblin::PainSound( void )
{
	if( RANDOM_LONG( 0, 2 ) == 0 )
	{
		EMIT_SOUND_DYN( ENT(pev), CHAN_WEAPON, pPainSounds[ RANDOM_LONG(0,ARRAYSIZE(pPainSounds)-1) ], 1.0, ATTN_NORM, 0, 90 );
	}
}


void WWGoblin::DeathSound( void )
{
	EMIT_SOUND_DYN( ENT(pev), CHAN_WEAPON, pDeathSounds[ RANDOM_LONG(0,ARRAYSIZE(pDeathSounds)-1) ], 1.0, ATTN_NORM, 0, 90 );
}


void WWGoblin::SetYawSpeed ( void )
{
	int ys;

	switch ( m_Activity )
	{
	case ACT_WALK:		
		ys = 50;	
		break;
	case ACT_RUN:		
		ys = 70;
		break;
	case ACT_IDLE:		
		ys = 50;
		break;
	default:
		ys = 90;
		break;
	}

	pev->yaw_speed = ys;
}


void WWGoblin::Spawn()
{
	Precache();

	SET_MODEL( ENT(pev), "models/npc/goblin.mdl");

	UTIL_SetSize(pev, Vector( -16, -16, 0 ), Vector( 16, 16, 64 ) );

	pev->netname		= MAKE_STRING( "ww_goblin" );
	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_STEP;
	pev->effects		= 0;
	pev->health			= 50;
	pev->team			= 0;
	pev->view_ofs		= Vector ( 0, 0, 64 );
	m_flFieldOfView		= VIEW_FIELD_WIDE;
	m_bloodColor		= BLOOD_COLOR_GREEN;
	m_MonsterState		= MONSTERSTATE_NONE;
	m_afCapability		=
		bits_CAP_HEAR |
		bits_CAP_SQUAD |
		bits_CAP_TURN_HEAD |
		bits_CAP_DOORS_GROUP |
		bits_CAP_RANGE_ATTACK1;

	m_HackedGunPos = Vector( 32, 0, 26 );

	MonsterInit();
}

void WWGoblin::Precache()
{
	int i;

	PRECACHE_MODEL( "models/npc/goblin.mdl" );
	PRECACHE_MODEL( "sprites/lgtning.spr" );

	for ( i = 0; i < ARRAYSIZE( pAttackHitSounds ); i++ )
		PRECACHE_SOUND((char *)pAttackHitSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pAttackMissSounds ); i++ )
		PRECACHE_SOUND((char *)pAttackMissSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pPainSounds ); i++ )
		PRECACHE_SOUND((char *)pPainSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pDeathSounds ); i++ )
		PRECACHE_SOUND((char *)pDeathSounds[i]);
}	


int WWGoblin::TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType )
{
	// don't slash one of your own
	if( IRelationship( Instance(pevAttacker) ) < R_DL )
		return 0;

	m_afMemory |= bits_MEMORY_PROVOKED;

	return CSquadMonster::TakeDamage( pevInflictor, pevAttacker, flDamage, bitsDamageType );
}


void WWGoblin::HandleAnimEvent( MonsterEvent_t *pEvent )
{
	CBaseEntity * pStone = NULL;

	switch( pEvent->event )
	{
	case 2500:
		ALERT( at_aiconsole, "Throw a rock" );

		if( m_hEnemy == NULL )
			break;

		pStone = CBaseEntity::Create( "proj_rollingstone", GetGunPosition(), pev->angles, edict() );
		pStone->pev->velocity = VecCheckToss( pStone->pev, pStone->pev->origin, m_hEnemy->pev->origin, 1.0f );
		break;

	default:
		CSquadMonster::HandleAnimEvent( pEvent );
		break;
	}
}


Task_t tlGoblinRockThrow[] =
{
	{ TASK_STOP_MOVING,				0					},
	{ TASK_FACE_ENEMY,				0					},
	{ TASK_SET_ACTIVITY,			(float)ACT_RELOAD	},
	{ TASK_WAIT,					0.1f				},
	{ TASK_WAIT_RANDOM,				0.2f				},
	{ TASK_RANGE_ATTACK1,			0					},
	{ TASK_SET_ACTIVITY,			(float)ACT_IDLE		},
	{ TASK_WAIT_RANDOM,				0.8f				},
};

Schedule_t slGoblinRockThrow[] =
{
	{
		tlGoblinRockThrow,
		ARRAYSIZE( tlGoblinRockThrow ),
		bits_COND_NEW_ENEMY		|
		bits_COND_ENEMY_DEAD	|
		bits_COND_ENEMY_TOOFAR	|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE,
		0,
		"Goblin Rock Throw",
	},
};

DEFINE_CUSTOM_SCHEDULES( WWGoblin )
{
	slGoblinRockThrow,
};

IMPLEMENT_CUSTOM_SCHEDULES( WWGoblin, CSquadMonster );


Schedule_t * WWGoblin::GetSchedule( void )
{
	if( HasConditions( bits_COND_HEAR_SOUND ) )
	{
		CSound *pSound;
		pSound = PBestSound();

		ASSERT( pSound != NULL );

		if( pSound && ( pSound->m_iType & bits_SOUND_DANGER ) )
			return GetScheduleOfType( SCHED_TAKE_COVER_FROM_BEST_SOUND );

		if( pSound && ( pSound->m_iType & bits_SOUND_COMBAT ) )
			m_afMemory |= bits_MEMORY_PROVOKED;
	}

	switch( m_MonsterState )
	{
	case MONSTERSTATE_COMBAT:
		// call base class, all code to handle dead enemies is centralized there.
		if( HasConditions( bits_COND_ENEMY_DEAD ) )
			return CBaseMonster::GetSchedule();

		if( pev->health < 50 )
		{
			if( !HasConditions( bits_COND_CAN_RANGE_ATTACK1 ) || pev->health < 10 )
			{
				m_failSchedule = SCHED_CHASE_ENEMY;

				if( HasConditions( bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE) )
					return GetScheduleOfType( SCHED_TAKE_COVER_FROM_ENEMY );

				// exposed
				if( HasConditions( bits_COND_SEE_ENEMY ) && HasConditions( bits_COND_ENEMY_FACING_ME ) )
					return GetScheduleOfType( SCHED_TAKE_COVER_FROM_ENEMY );
			}
		}
		break;
	}

	return CSquadMonster::GetSchedule();
}


Schedule_t * WWGoblin::GetScheduleOfType( int Type ) 
{
	switch( Type )
	{
	case SCHED_FAIL:
		if( HasConditions( bits_COND_CAN_RANGE_ATTACK1 ) )
			return &slGoblinRockThrow[0];
		break;

	case SCHED_RANGE_ATTACK1:
	case SCHED_RANGE_ATTACK2:
		return &slGoblinRockThrow[0];
	}

	return CSquadMonster::GetScheduleOfType( Type );
}