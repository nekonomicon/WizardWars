#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"talkmonster.h"
#include	"schedule.h"
#include	"defaultai.h"
#include	"scripted.h"
#include	"animation.h"
#include	"soundent.h"


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class WWVillager : public CTalkMonster
{
public:
	WWVillager();

	void			KeyValue			( KeyValueData *pkvd );
	void			Spawn				( void );
	void			Precache			( void );
	void			TalkInit			( void );

	int				Classify			( void );
	int				ISoundMask			( void );
	void			SetYawSpeed			( void );

	void			HandleAnimEvent		( MonsterEvent_t * pEvent );

	void			PainSound			( void );
	void			DeathSound			( void );

	void			StartTask			( Task_t * pTask );
	void			RunTask				( Task_t * pTask );

	void			RunAI				( void );
	Schedule_t *	GetSchedule			( void );
	Schedule_t *	GetScheduleOfType	( int iType );
	CUSTOM_SCHEDULES;

	Vector			m_vTarget;			// where we are roaming to
	EHANDLE			m_hFriend;			// the last person we spoke to
	float			m_flLastSearch;		// last time we found someone to talk too
	float			m_flPainTime;		// last time we cried in agony
	int				m_iHead;
	int				m_iType;
	int				m_iMovement;
};

LINK_ENTITY_TO_CLASS( ww_villager, WWVillager );
LINK_ENTITY_TO_CLASS( monster_villager, WWVillager );


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define bits_MEMORY_SPEAKING (bits_MEMORY_CUSTOM1)


enum
{
	HEAD_RANDOM = -1,
	HEAD_OLD = 0,
	HEAD_RUDE,
	HEAD_NICE,
	HEAD_SMART,
};

enum
{
	TYPE_RANDOM = 0,
	TYPE_MALE,
	TYPE_FEMALE,
	TYPE_DRUNKARD,
};

enum
{
	MOVE_NONE,
	MOVE_ROAM,
};



enum
{
	SCHED_VILLAGER_RANDOM = LAST_TALKMONSTER_SCHEDULE + 1,
	SCHED_VILLAGER_ROAM,
	SCHED_VILLAGER_FIND,
	SCHED_VILLAGER_FIND_REPLY,
};


enum
{
	TASK_VILLAGER_WAIT_DISTANCE  = LAST_TALKMONSTER_TASK + 1,
	TASK_VILLAGER_FIND,
	TASK_VILLAGER_WAVE,
	TASK_VILLAGER_GREET,
	TASK_VILLAGER_LISTEN,
	TASK_VILLAGER_SPEEK,
	TASK_VILLAGER_LAUGH,
	TASK_VILLAGER_YES,
	TASK_VILLAGER_NO,
	TASK_VILLAGER_POINT,
	TASK_VILLAGER_PRAY,
	TASK_VILLAGER_ROAM,
};


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Task_t tlRoam[] =
{
	{ TASK_FORGET,					(float)bits_MEMORY_SPEAKING		},
	{ TASK_STOP_MOVING,				(float)0						},
	{ TASK_VILLAGER_ROAM,			(float)150						},
	{ TASK_WAIT_RANDOM,				(float)5.0f						},
};

Schedule_t slRoam[] =
{
	{
		tlRoam, ARRAYSIZE( tlRoam ),
		bits_COND_NEW_ENEMY			|
			bits_COND_PROVOKED		|
			bits_COND_LIGHT_DAMAGE	|
			bits_COND_HEAVY_DAMAGE	|
			bits_COND_PROVOKED,
		0,
		"Roam",
	},
};


Task_t tlFindFriend[] =
{
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_VILLAGER_ROAM		},
	{ TASK_REMEMBER,				(float)bits_MEMORY_SPEAKING		},
	{ TASK_VILLAGER_FIND,			(float)0						},
	{ TASK_FACE_TARGET,				(float)0						},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0						},
	{ TASK_VILLAGER_WAVE,			(float)1						},
	{ TASK_SET_ACTIVITY,			(float)ACT_IDLE					},
	{ TASK_GET_PATH_TO_TARGET,		(float)0						},
	{ TASK_WALK_PATH,				(float)0						},
	{ TASK_VILLAGER_WAIT_DISTANCE,	(float)64						},
	{ TASK_STOP_MOVING,				(float)0						},
	{ TASK_FACE_TARGET,				(float)0						},
	{ TASK_VILLAGER_GREET,			(float)0						},
	{ TASK_SET_ACTIVITY,			(float)ACT_IDLE					},
	{ TASK_WAIT_RANDOM,				(float)4.0f						},
	{ TASK_VILLAGER_ROAM,			(float)450						},
	{ TASK_WAIT_RANDOM,				(float)5.0f						},
	{ TASK_FORGET,					(float)bits_MEMORY_SPEAKING		},
};

Schedule_t slFindFriend[] =
{
	{
		tlFindFriend, ARRAYSIZE( tlFindFriend ),
		bits_COND_NEW_ENEMY			|
			bits_COND_PROVOKED		|
			bits_COND_LIGHT_DAMAGE	|
			bits_COND_HEAVY_DAMAGE	|
			bits_COND_HEAR_SOUND,
		bits_SOUND_COMBAT			|
			bits_SOUND_DANGER,
		"FindFriend",
	},
};


Task_t tlFindFriendReply[] =
{
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_VILLAGER_ROAM		},	// what to do if we fail
	{ TASK_REMEMBER,				(float)bits_MEMORY_SPEAKING		},	// we found some one
	{ TASK_STOP_MOVING,				(float)0						},	// stop
	{ TASK_FACE_TARGET,				(float)0						},	// look
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0						},
	{ TASK_VILLAGER_WAVE,			(float)0						},	// wave hello
	{ TASK_SET_ACTIVITY,			(float)ACT_IDLE					},	// wait
	{ TASK_GET_PATH_TO_TARGET,		(float)0						},
	{ TASK_WALK_PATH,				(float)0						},
	{ TASK_VILLAGER_WAIT_DISTANCE,	(float)64						},	// if they are close
	{ TASK_STOP_MOVING,				(float)0						},
	{ TASK_FACE_TARGET,				(float)0						},	// look
	{ TASK_WAIT_RANDOM,				(float)0.3f						},	// wait a second
	{ TASK_VILLAGER_GREET,			(float)0						},	// bow
	{ TASK_SET_ACTIVITY,			(float)ACT_IDLE					},	// idle
	{ TASK_VILLAGER_ROAM,			(float)350						},
	{ TASK_WAIT_RANDOM,				(float)5.0f						},
	{ TASK_FORGET,					(float)bits_MEMORY_SPEAKING		},	// we are in a social context
};

Schedule_t slFindFriendReply[] =
{
	{
		tlFindFriendReply, ARRAYSIZE( tlFindFriendReply ),
		bits_COND_NEW_ENEMY			|
			bits_COND_PROVOKED		|
			bits_COND_LIGHT_DAMAGE	|
			bits_COND_HEAVY_DAMAGE	|
			bits_COND_HEAR_SOUND,
		bits_SOUND_COMBAT			|
			bits_SOUND_DANGER,
		"FindFriendReply"
	},
};


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DEFINE_CUSTOM_SCHEDULES( WWVillager )
{
	slRoam,
	slFindFriend,
	slFindFriendReply,
};
IMPLEMENT_CUSTOM_SCHEDULES( WWVillager, CTalkMonster );


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
WWVillager::WWVillager() : CTalkMonster()
{
	m_iHead		= HEAD_RANDOM;
	m_iType		= TYPE_RANDOM;
	m_iMovement = MOVE_ROAM;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWVillager::Spawn( void )
{
	if( pev->body == HEAD_RANDOM )
		pev->body = RANDOM_LONG( HEAD_OLD, HEAD_SMART );

	if( m_iType == TYPE_RANDOM )
		m_iType = RANDOM_LONG( TYPE_MALE, TYPE_DRUNKARD );

	Precache();

	switch( m_iType )
	{
	default:
		SET_MODEL( ENT(pev), "models/npc/villager_m.mdl" );
		break;

	case TYPE_FEMALE:
		SET_MODEL( ENT(pev), "models/npc/villager_f.mdl" );
		break;
	}

	SET_SIZE( ENT(pev), VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX );


	if( m_iHead == HEAD_RANDOM )
		m_iHead = RANDOM_LONG( HEAD_OLD, HEAD_SMART );
	switch( m_iHead )
	{
	default:
	case HEAD_OLD:		pev->body = 0; break;
	case HEAD_RUDE:		pev->body = 1; break;
	case HEAD_NICE:		pev->body = 2; break;
	case HEAD_SMART:	pev->body = 3; break;
	}

	pev->netname	= MAKE_STRING( "ww_villager" );
	pev->health		= 50;
	pev->skin		= 0;
	pev->solid		= SOLID_SLIDEBOX;
	pev->movetype	= MOVETYPE_STEP;
	pev->view_ofs	= Vector( 0, 0, 50 );
	pev->angles		= Vector( 0, 0, 0 );

	m_vTarget		= pev->origin;
	m_hFriend		= NULL;
	m_flLastSearch	= 0.0f;

	m_bloodColor	= BLOOD_COLOR_RED;
	m_flFieldOfView	= VIEW_FIELD_WIDE;
	m_MonsterState	= MONSTERSTATE_NONE;
	m_afCapability	=
		bits_CAP_HEAR		|
		bits_CAP_SQUAD		|
		bits_CAP_TURN_HEAD	|
		bits_CAP_OPEN_DOORS	|
		bits_CAP_AUTO_DOORS	|
		bits_CAP_USE;

	MonsterInit();
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWVillager::Precache( void )
{
	PRECACHE_MODEL( "models/npc/villager_m.mdl" );
	PRECACHE_SOUND( "npc/villager/pain1_m.wav" );
	PRECACHE_SOUND( "npc/villager/pain2_m.wav" );
	PRECACHE_SOUND( "npc/villager/pain3_m.wav" );
	PRECACHE_SOUND( "npc/villager/pain4_m.wav" );
	PRECACHE_SOUND( "npc/villager/pain5_m.wav" );

	PRECACHE_MODEL( "models/npc/villager_f.mdl" );
	PRECACHE_SOUND( "npc/villager/pain1_f.wav" );
	PRECACHE_SOUND( "npc/villager/pain2_f.wav" );
	PRECACHE_SOUND( "npc/villager/pain3_f.wav" );
	PRECACHE_SOUND( "npc/villager/pain4_f.wav" );
	PRECACHE_SOUND( "npc/villager/pain5_f.wav" );

	PRECACHE_SOUND( "common/npc_step1.wav" );
	PRECACHE_SOUND( "common/npc_step2.wav" );
	PRECACHE_SOUND( "common/npc_step3.wav" );
	PRECACHE_SOUND( "common/npc_step4.wav" );

	TalkInit();
	CTalkMonster::Precache();
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWVillager::KeyValue( KeyValueData * pkvd )
{
	if( !stricmp( pkvd->szKeyName, "head" ) )
	{
		m_iHead = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}

	else if( !stricmp( pkvd->szKeyName, "type" ) )
	{
		m_iType = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}

	else if( !stricmp( pkvd->szKeyName, "movement" ) )
	{
		m_iMovement = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}

	if( !pkvd->fHandled )
		CTalkMonster::KeyValue( pkvd );
};


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWVillager::TalkInit( void )
{
	CTalkMonster::TalkInit();

	m_szFriends[0]	= "ww_villager";

	if( m_iType == TYPE_MALE )
	{
		m_szGrp[ TLK_ANSWER		] = "SC_ANSWER";
		m_szGrp[ TLK_QUESTION	] = "SC_PQUEST";
		m_szGrp[ TLK_IDLE		] = "SC_PIDLE";
		m_szGrp[ TLK_STARE		] = "SC_STARE";
		m_szGrp[ TLK_USE		] = "SC_PFOLLOW";
		m_szGrp[ TLK_UNUSE		] = "SC_PWAIT";
		m_szGrp[ TLK_DECLINE	] = "SC_POK";
		m_szGrp[ TLK_STOP		] = "SC_STOP";
		m_szGrp[ TLK_NOSHOOT	] = "SC_SCARED";
		m_szGrp[ TLK_HELLO		] = "SC_PHELLO";

		switch( pev->body )
		{
		default:
		case HEAD_OLD:		m_voicePitch = 100; break;
		case HEAD_RUDE:		m_voicePitch =  90; break;
		case HEAD_NICE:		m_voicePitch =  95; break;
		case HEAD_SMART:	m_voicePitch = 110; break;
		}
	}
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int WWVillager::Classify( void )
{
	return CLASS_HUMAN_PASSIVE;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int WWVillager::ISoundMask( void )
{
	return	bits_SOUND_WORLD	|
			bits_SOUND_COMBAT	|
			bits_SOUND_DANGER	|
			bits_SOUND_PLAYER;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWVillager::SetYawSpeed( void )
{
	int yawSpeed = 90;

	switch( m_Activity )
	{
	case ACT_IDLE:			yawSpeed = 180; break;
	case ACT_WALK:			yawSpeed = 180; break;
	case ACT_RUN:			yawSpeed = 150; break;
	case ACT_TURN_LEFT:		yawSpeed = 240; break;
	case ACT_TURN_RIGHT:	yawSpeed = 240; break;
	}

	pev->yaw_speed = yawSpeed;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWVillager::HandleAnimEvent( MonsterEvent_t * pEvent )
{
	CBaseEntity * pHurt = NULL;

	switch( pEvent->event )
	{
	case 1:
		pHurt = CheckTraceHullAttack( 70, 10, DMG_CLUB );

/*
		if ( pHurt )
			EMIT_SOUND_DYN( ENT(pev), CHAN_WEAPON, RANDOM_SOUND_ARRAY( pAttackHitSounds ), 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );
		else
			EMIT_SOUND_DYN( ENT(pev), CHAN_WEAPON, RANDOM_SOUND_ARRAY( pAttackMissSounds ), 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );
*/
		break;

	default:
		CBaseMonster::HandleAnimEvent( pEvent );
		break;
	}
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWVillager::PainSound( void )
{
	if( gpGlobals->time < m_flPainTime )
		return;
	
	m_flPainTime = gpGlobals->time + RANDOM_FLOAT( 0.5, 0.75 );

	char szSound[256];
	switch( m_iType )
	{
	case TYPE_MALE:
		sprintf( szSound, "npc/villager/pain%i_m.wav", RANDOM_LONG( 0, 4 ) );
		break;

	case TYPE_FEMALE:
		sprintf( szSound, "npc/villager/pain%i_f.wav", RANDOM_LONG( 0, 4 ) );
		break;
	};

	
	EMIT_SOUND_DYN( ENT(pev), CHAN_VOICE, szSound, 1, ATTN_NORM, 0, GetVoicePitch() );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWVillager::DeathSound( void )
{
	PainSound();
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWVillager::StartTask( Task_t * pTask )
{
	switch( pTask->iTask )
	{
	case TASK_WALK_PATH:

		if( m_iMovement == MOVE_NONE )
		{
			TaskFail();
			return;
		}
		else
			CTalkMonster::StartTask( pTask );

		break;

	case TASK_VILLAGER_WAIT_DISTANCE:

		if( m_hTargetEnt != NULL )
			TaskBegin();
		else
			TaskFail();
		break;

	case TASK_VILLAGER_FIND:

		if( m_iMovement == MOVE_NONE )
		{
			TaskFail();
			return;
		}

		m_hTargetEnt = FindNearestFriend2( m_hFriend, FALSE );

		// you need to cast to int to compare EHANDLEs
		if( m_hTargetEnt == NULL || (int)m_hFriend == (int)m_hTargetEnt )
		{
			// if we couldn't find anyone, we may be at the end of the entity list
			//	if we dont do this, it will continuously return NULL, unless a new
			//	villager is created.
			if( m_hTargetEnt == NULL )
				m_hFriend = NULL;
			TaskFail();
		}
		else
		{
			TaskComplete();
			m_hFriend = m_hTargetEnt;
			m_flLastSearch = gpGlobals->time;
		}

		break;

	case TASK_VILLAGER_WAVE:

		if( m_hTargetEnt == NULL )
		{
			TaskFail();
			break;
		}

		if( pTask->flData == 1.0f )
		{
			WWVillager *pVillager = ((WWVillager *)((CBaseEntity*)m_hTargetEnt));

			if( !pVillager )
				break;

			if( pVillager->HasMemory( bits_MEMORY_SPEAKING ) )
			{
				TaskFail();
				break;
			}

			pVillager->m_hTargetEnt = this;
			pVillager->ChangeSchedule( pVillager->GetScheduleOfType( SCHED_VILLAGER_FIND_REPLY ) );
		}

		// only wave if they are not right next to us.
		if( ( pev->origin - m_hTargetEnt->pev->origin ).Length() >= 128.0f )
		{
			pev->sequence = LookupSequence( "wave" );
			pev->frame = 0;

			ResetSequenceInfo();
			TaskBegin();
		}
		else
			TaskComplete();

		break;

	case TASK_VILLAGER_GREET:

		pev->sequence = LookupSequence( "idle4" );
		pev->frame = 0;

		PlaySentence( m_szGrp[TLK_HELLO], RANDOM_FLOAT(3, 3.5), VOL_NORM,  ATTN_IDLE );

		ResetSequenceInfo();
		TaskBegin();
		break;

	case TASK_VILLAGER_LISTEN:
		break;

	case TASK_VILLAGER_SPEEK:
		break;

	case TASK_VILLAGER_LAUGH:
		break;

	case TASK_VILLAGER_YES:
		break;

	case TASK_VILLAGER_NO:
		break;

	case TASK_VILLAGER_POINT:
		break;

	case TASK_VILLAGER_PRAY:
		break;

	case TASK_VILLAGER_ROAM:
		if( m_iMovement == MOVE_NONE )
		{
			TaskFail();
			return;
		}

		m_vTarget = pev->origin + Vector(
			RANDOM_FLOAT( -pTask->flData, pTask->flData ),
			RANDOM_FLOAT( -pTask->flData, pTask->flData ), 0 );
		MoveToLocation( ACT_WALK, 2, m_vTarget );
		break;

	default:
		CTalkMonster::StartTask( pTask );
		break;
	}
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWVillager::RunTask( Task_t * pTask )
{
//	CBaseMonster * pFriend = NULL;

	switch( pTask->iTask )
	{
	case TASK_VILLAGER_WAIT_DISTANCE:

		if( m_hTargetEnt != NULL && ( pev->origin - m_hTargetEnt->pev->origin ).Length() <= pTask->flData )
			TaskComplete();

		if( m_hTargetEnt == NULL )
			TaskFail();

		break;

	case TASK_VILLAGER_WAVE:
	case TASK_VILLAGER_GREET:
	
		if( m_fSequenceFinished )
			TaskComplete();

		break;

	case TASK_VILLAGER_LISTEN:
		TaskComplete();
		break;

	case TASK_VILLAGER_SPEEK:
		TaskComplete();
		break;

	case TASK_VILLAGER_LAUGH:
		TaskComplete();
		break;

	case TASK_VILLAGER_YES:
		TaskComplete();
		break;

	case TASK_VILLAGER_NO:
		TaskComplete();
		break;

	case TASK_VILLAGER_POINT:
		TaskComplete();
		break;

	case TASK_VILLAGER_PRAY:
		TaskComplete();
		break;

	case TASK_VILLAGER_ROAM:

		if( m_iMovement == MOVE_NONE )
			TaskFail();

		if( !IsMoving()  )
			TaskComplete();
		break;

	default:
		CTalkMonster::RunTask( pTask );
		break;
	}
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWVillager::RunAI( void )
{
	CTalkMonster::RunAI();

	// If we havent found a friend in the last minute forget our last friend so
	//	we can talk to him again.
	// This is to prevent a lack of communication when only two villagers are
	//	present.
	if( gpGlobals->time - m_flLastSearch > 30.0f )
		m_hFriend = NULL;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Schedule_t * WWVillager::GetSchedule( void )
{
	switch( m_MonsterState )
	{
	case MONSTERSTATE_ALERT:

		if( HasConditions( bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE ) )
			return GetScheduleOfType( SCHED_SMALL_FLINCH );
		break;

	case MONSTERSTATE_IDLE:

		if( !HasMemory( bits_MEMORY_SPEAKING ) )
			return GetScheduleOfType( SCHED_VILLAGER_FIND );
		break;
	}

	return CTalkMonster::GetSchedule();
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Schedule_t * WWVillager::GetScheduleOfType( int iType )
{
	switch( iType )
	{
	case SCHED_VILLAGER_RANDOM:

		switch( RANDOM_LONG( 0, 1 ) )
		{
		case 0:
			return slRoam;

		case 1:
			return slFindFriend;
		}
		break;

	case SCHED_VILLAGER_ROAM:
		return slRoam;

	case SCHED_VILLAGER_FIND:
		return slFindFriend;

	case SCHED_VILLAGER_FIND_REPLY:
		return slFindFriendReply;
	}

	Forget( bits_MEMORY_SPEAKING );

	return CTalkMonster::GetScheduleOfType( iType );
}


