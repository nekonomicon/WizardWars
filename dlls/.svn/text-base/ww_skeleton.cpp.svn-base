/***
*
*	Copyright (c) 1999, 2000 Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
//=========================================================
// monster template
//=========================================================
// UNDONE: Holster weapon?

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"talkmonster.h"
#include	"schedule.h"
#include	"defaultai.h"
#include	"scripted.h"
#include	"weapons.h"
#include	"soundent.h"
#include "player.h"
#include "gamerules.h"
#include "ww_icewizard.h"

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
// first flag is skeleton dying for scripted sequences?
#define		SKELETON_AE_DRAW		( 2 )
#define		SKELETON_AE_SHOOT		( 3 )
#define		SKELETON_AE_HOLSTER	( 4 )

#define	SKELETON_BODY_GUNHOLSTERED	0
#define	SKELETON_BODY_GUNDRAWN		1
#define SKELETON_BODY_GUNGONE			2

#define SKELETON_HEALTH				200

class CSkeleton : public CTalkMonster
{
public:
	void Spawn( void );
	void Precache( void );
	void SetYawSpeed( void );
	int  ISoundMask( void );
	void SkeletonFirePistol( void );
	void AlertSound( void );
	int  Classify ( void );
	void HandleAnimEvent( MonsterEvent_t *pEvent );
	
	void RunTask( Task_t *pTask );
	void StartTask( Task_t *pTask );
	virtual int	ObjectCaps( void ) { return CTalkMonster :: ObjectCaps() | FCAP_IMPULSE_USE; }
	int TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType);
	BOOL CheckRangeAttack1 ( float flDot, float flDist );
	
	void DeclineFollowing( void );

	// Override these to set behavior
	Schedule_t *GetScheduleOfType ( int Type );
	Schedule_t *GetSchedule ( void );
	MONSTERSTATE GetIdealState ( void );

	void DeathSound( void );
	void PainSound( void );
	
	void TalkInit( void );

	void TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType);
	void Killed( entvars_t *pevAttacker, int iGib );

	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];

	BOOL	m_fGunDrawn;
	float	m_painTime;
	float	m_checkAttackTime;
	BOOL	m_lastAttackCheck;

	// UNDONE: What is this for?  It isn't used?
	float	m_flPlayerDamage;// how much pain has the player inflicted on me?

	int		m_iDryBones;
	int	BloodColor( void ) { return DONT_BLEED; }
	EHANDLE m_hOwner;

	CUSTOM_SCHEDULES;

//	int IRelationship(CBaseEntity *pTarget);
};

LINK_ENTITY_TO_CLASS( monster_skeleton, CSkeleton );

TYPEDESCRIPTION	CSkeleton::m_SaveData[] = 
{
	DEFINE_FIELD( CSkeleton, m_fGunDrawn, FIELD_BOOLEAN ),
	DEFINE_FIELD( CSkeleton, m_painTime, FIELD_TIME ),
	DEFINE_FIELD( CSkeleton, m_checkAttackTime, FIELD_TIME ),
	DEFINE_FIELD( CSkeleton, m_lastAttackCheck, FIELD_BOOLEAN ),
	DEFINE_FIELD( CSkeleton, m_flPlayerDamage, FIELD_FLOAT ),
};

IMPLEMENT_SAVERESTORE( CSkeleton, CTalkMonster );


//=========================================================
// AI Schedules Specific to this monster
//=========================================================
Task_t	tlBaFollow[] =
{
	{ TASK_MOVE_TO_TARGET_RANGE,(float)128		},	// Move within 128 of target ent (client)
	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_FACE },
};

Schedule_t	slBaFollow[] =
{
	{
		tlBaFollow,
		ARRAYSIZE ( tlBaFollow ),
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND |
		bits_COND_PROVOKED,
		bits_SOUND_DANGER,
		"Follow"
	},
};


//=========================================================
// SkeletonDraw- much better looking draw schedule for when
// skeleton knows who he's gonna attack.
//=========================================================
Task_t	tlSkeletonEnemyDraw[] =
{
	{ TASK_STOP_MOVING,					0				},
	{ TASK_FACE_ENEMY,					0				},
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY,	(float) ACT_ARM },
};

Schedule_t slBarneyEnemyDraw[] = //slSkeletonEnemyDraw[] = 
{
	{
		tlSkeletonEnemyDraw,
		ARRAYSIZE ( tlSkeletonEnemyDraw ),
		0,
		0,
		"Skeleton Enemy Draw"
	}
};

Task_t	tlBaFaceTarget[] =
{
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_FACE_TARGET,			(float)0		},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_CHASE },
};

Schedule_t	slBaFaceTarget[] =
{
	{
		tlBaFaceTarget,
		ARRAYSIZE ( tlBaFaceTarget ),
		bits_COND_CLIENT_PUSH	|
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND |
		bits_COND_PROVOKED,
		bits_SOUND_DANGER,
		"FaceTarget"
	},
};


Task_t	tlIdleBaStand[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT,				(float)2		}, // repick IDLESTAND every two seconds.
	{ TASK_TLK_HEADRESET,		(float)0		}, // reset head position
};

Schedule_t	slIdleBaStand[] =
{
	{ 
		tlIdleBaStand,
		ARRAYSIZE ( tlIdleBaStand ), 
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND	|
		bits_COND_SMELL			|
		bits_COND_PROVOKED,

		bits_SOUND_COMBAT		|// sound flags - change these, and you'll break the talking code.
		//bits_SOUND_PLAYER		|
		//bits_SOUND_WORLD		|
		
		bits_SOUND_DANGER		|
		bits_SOUND_MEAT			|// scents
		bits_SOUND_CARCASS		|
		bits_SOUND_GARBAGE,
		"IdleStand"
	},
};

/*
extern Schedule_t slIdleBaStand[];
extern Schedule_t slBarneyEnemyDraw[];
extern Schedule_t slBaFaceTarget[];
extern Schedule_t slIdleBaStand[];
extern Schedule_t slBaFollow[];
*/

DEFINE_CUSTOM_SCHEDULES( CSkeleton )
{
	slBaFollow,
	slBarneyEnemyDraw,
	slBaFaceTarget,
	slIdleBaStand,
};


IMPLEMENT_CUSTOM_SCHEDULES( CSkeleton, CTalkMonster );

void CSkeleton :: StartTask( Task_t *pTask )
{
	CTalkMonster::StartTask( pTask );	
}

void CSkeleton :: RunTask( Task_t *pTask )
{
	switch ( pTask->iTask )
	{
	case TASK_RANGE_ATTACK1:
		if (m_hEnemy != NULL && (m_hEnemy->IsPlayer()))
		{
			pev->framerate = 1.5;
		}
		CTalkMonster::RunTask( pTask );
		break;
	default:
		CTalkMonster::RunTask( pTask );
		break;
	}
}

/*
int CSkeleton::IRelationship(CBaseEntity *pTarget)
{
//	CBaseMonster::IRelationship(pTarget)
	// Like owner
	if(pTarget && ENT(pTarget->pev) == pev->owner)
		return(R_AL);

	// Like things owned by owner
	if( (pev->owner) && (pTarget->pev->owner == pev->owner))
		return R_AL;

	// Like teammates
	if(g_pGameRules->IsTeamplay() && (pTarget->pev->team == pev->team))
		return R_AL;

	// Ice Wizard can trick us! Tricksy Bagginses!
	if(pTarget->IsPlayer() && pTarget->pev->playerclass == WWCLASS_ICE)
	{
		CBasePlayer* pl=((CBasePlayer*)pTarget);
		IceWizard* bw=((IceWizard*)pl->m_pClass);

		if(bw->IsInvisible() == TRUE && pTarget->pev->renderamt<20)
			return R_NO;

		if(pev->team != 0 && bw->GetDisguisedTeam() == pev->team)
			return R_NO;
	}

	// Nemisis player
	if(pTarget->IsPlayer())
		return R_NM;

	if(FClassnameIs(pTarget->pev,"monster_giantplant"))
		return R_HT;

	if(FClassnameIs(pTarget->pev,"monster_thornbush"))
		return R_HT;

	return(CBaseMonster::IRelationship(pTarget));
}
*/

//=========================================================
// ISoundMask - returns a bit mask indicating which types
// of sounds this monster regards. 
//=========================================================
int CSkeleton :: ISoundMask ( void) 
{
	return	bits_SOUND_WORLD	|
			bits_SOUND_COMBAT	|
			bits_SOUND_CARCASS	|
			bits_SOUND_MEAT		|
			bits_SOUND_GARBAGE	|
			bits_SOUND_DANGER	|
			bits_SOUND_PLAYER;
}

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int	CSkeleton :: Classify ( void )
{
	return CLASS_ALIEN_MILITARY;// Codiac 1.2 B22 CLASS_PLAYER_ALLY;
}

//=========================================================
// ALertSound - skeleton says "Freeze!"
//=========================================================
void CSkeleton :: AlertSound( void )
{
	if ( m_hEnemy != NULL )
	{
		if ( FOkToSpeak() )
		{
//			PlaySentence( "BA_ATTACK", RANDOM_FLOAT(2.8, 3.2), VOL_NORM, ATTN_IDLE );
		}
	}

}
//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CSkeleton :: SetYawSpeed ( void )
{
	int ys;

	ys = 0;

	switch ( m_Activity )
	{
	case ACT_IDLE:		
		ys = 70;
		break;
	case ACT_WALK:
		ys = 70;
		break;
	case ACT_RUN:
		ys = 90;
		break;
	default:
		ys = 70;
		break;
	}

	pev->yaw_speed = ys;
}


//=========================================================
// CheckRangeAttack1
//=========================================================
BOOL CSkeleton :: CheckRangeAttack1 ( float flDot, float flDist )
{
	if ( flDist <= 1024 && flDot >= 0.5 )
	{
		if ( gpGlobals->time > m_checkAttackTime )
		{
			TraceResult tr;
			
			Vector shootOrigin = pev->origin + Vector( 0, 0, 55 );
			CBaseEntity *pEnemy = m_hEnemy;
			Vector shootTarget = ( (pEnemy->BodyTarget( shootOrigin ) - pEnemy->pev->origin) + m_vecEnemyLKP );
			UTIL_TraceLine( shootOrigin, shootTarget, dont_ignore_monsters, ENT(pev), &tr );
			m_checkAttackTime = gpGlobals->time + 1;
			if ( tr.flFraction == 1.0 || (tr.pHit != NULL && CBaseEntity::Instance(tr.pHit) == pEnemy) )
				m_lastAttackCheck = TRUE;
			else
				m_lastAttackCheck = FALSE;
			m_checkAttackTime = gpGlobals->time + 1.5;
		}
		return m_lastAttackCheck;
	}
	return FALSE;
}


//=========================================================
// SkeletonFirePistol - shoots one round from the pistol at
// the enemy skeleton is facing.
//=========================================================
void CSkeleton :: SkeletonFirePistol ( void )
{
	Vector vecShootOrigin;

	UTIL_MakeVectors(pev->angles);
	vecShootOrigin = pev->origin + Vector( 0, 0, 55 );
	Vector vecShootDir = ShootAtEnemy( vecShootOrigin );

	Vector angDir = UTIL_VecToAngles( vecShootDir );

	vecShootOrigin=vecShootOrigin+vecShootDir*40;
	SetBlending( 0, angDir.x );

	angDir.x=angDir.x*-1;

	if(m_hOwner!=NULL)
		Create("proj_throwingbone",vecShootOrigin,angDir,m_hOwner->edict());
	else
		Create("proj_throwingbone",vecShootOrigin,angDir);

	EMIT_SOUND_DYN( ENT(pev), CHAN_WEAPON, "monster/skeleton/bonethrow.wav", 1, ATTN_NORM, 0, RANDOM_FLOAT(80,120));

	CSoundEnt::InsertSound ( bits_SOUND_COMBAT, pev->origin, 384, 0.3 );
}
		
//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//
// Returns number of events handled, 0 if none.
//=========================================================
void CSkeleton :: HandleAnimEvent( MonsterEvent_t *pEvent )
{
	switch( pEvent->event )
	{
	case SKELETON_AE_SHOOT:
		SkeletonFirePistol();
		break;

	case SKELETON_AE_DRAW:
		// skeleton's bodygroup switches here so he can pull gun from holster
		pev->body = SKELETON_BODY_GUNDRAWN;
		m_fGunDrawn = TRUE;
		break;

	case SKELETON_AE_HOLSTER:
		// change bodygroup to replace gun in holster
		pev->body = SKELETON_BODY_GUNHOLSTERED;
		m_fGunDrawn = FALSE;
		break;

	default:
		CTalkMonster::HandleAnimEvent( pEvent );
	}
}

//=========================================================
// Spawn
//=========================================================
void CSkeleton :: Spawn()
{
	Precache( );

	SET_MODEL(ENT(pev), "models/skeleton.mdl");//combos/raisedead.mdl");
	UTIL_SetSize(pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);

	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_STEP;
	m_bloodColor		= BLOOD_COLOR_RED;
	pev->health			= SKELETON_HEALTH;
	pev->view_ofs		= Vector ( 0, 0, 50 );// position of the eyes relative to monster's origin.
	m_flFieldOfView		= VIEW_FIELD_WIDE; // NOTE: we need a wide field of view so npc will notice player and say hello
	m_MonsterState		= MONSTERSTATE_NONE;

	pev->body			= 0; // gun in holster
	m_fGunDrawn			= FALSE;

	m_afCapability		= bits_CAP_HEAR | bits_CAP_TURN_HEAD | bits_CAP_DOORS_GROUP;

	if(pev->owner)
		m_hOwner=Instance(pev->owner);

	pev->owner=NULL;

	MonsterInit();
	SetUse( &CSkeleton::FollowerUse );
	Use(m_hOwner, m_hOwner, USE_SET, 0);
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CSkeleton :: Precache()
{
	PRECACHE_MODEL("models/combos/raisedead.mdl");
	PRECACHE_MODEL("models/skeleton.mdl");

	PRECACHE_SOUND("weapons/cbar_miss1.wav" );

	PRECACHE_SOUND("monster/skeleton/skfollow.wav" );
	PRECACHE_SOUND("monster/skeleton/skwait.wav" );

	PRECACHE_SOUND("barney/ba_pain1.wav");
	PRECACHE_SOUND("barney/ba_pain2.wav");
	PRECACHE_SOUND("barney/ba_pain3.wav");

	PRECACHE_SOUND("barney/ba_die1.wav");
	PRECACHE_SOUND("barney/ba_die2.wav");
	PRECACHE_SOUND("barney/ba_die3.wav");

	PRECACHE_SOUND( "monster/skeleton/bonethrow.wav" );
	PRECACHE_SOUND( "monster/skeleton/skgibs.wav" );
	
	m_iDryBones=PRECACHE_MODEL("models/bonegibs.mdl");

	UTIL_PrecacheOther("proj_throwingbone");

	// every new skeleton must call this, otherwise
	// when a level is loaded, nobody will talk (time is reset to 0)
	TalkInit();
	CTalkMonster::Precache();
}	

// Init talk data
void CSkeleton :: TalkInit()
{
	
	CTalkMonster::TalkInit();

	// scientists speach group names (group names are in sentences.txt)

	m_szGrp[TLK_ANSWER]  =	NULL;
	m_szGrp[TLK_QUESTION] =	NULL;
	m_szGrp[TLK_IDLE] =		NULL;
	m_szGrp[TLK_STARE] =	NULL;
	m_szGrp[TLK_USE] =		"monster/skeleton/skfollow.wav";//"!SK_FOLLOW";
	m_szGrp[TLK_UNUSE] =	"monster/skeleton/skwait.wav";//"!SK_WAIT";
	m_szGrp[TLK_STOP] =		NULL;
	m_szGrp[TLK_DECLINE] =	NULL;
	m_szGrp[TLK_NOSHOOT] =	NULL;
	m_szGrp[TLK_HELLO] =	NULL;

	m_szGrp[TLK_PLHURT1] =	NULL;
	m_szGrp[TLK_PLHURT2] =	NULL; 
	m_szGrp[TLK_PLHURT3] =	NULL;

	m_szGrp[TLK_PHELLO] =	NULL;	//"BA_PHELLO";		// UNDONE
	m_szGrp[TLK_PIDLE] =	NULL;	//"BA_PIDLE";			// UNDONE
	m_szGrp[TLK_PQUESTION] = NULL;		// UNDONE

	m_szGrp[TLK_SMELL] =	NULL;
	
	m_szGrp[TLK_WOUND] =	NULL;
	m_szGrp[TLK_MORTAL] =	NULL;

	// get voice for head - just one skeleton voice for now
	m_voicePitch = 100;
}


static BOOL IsFacing( entvars_t *pevTest, const Vector &reference )
{
	Vector vecDir = (reference - pevTest->origin);
	vecDir.z = 0;
	vecDir = vecDir.Normalize();
	Vector forward, angle;
	angle = pevTest->v_angle;
	angle.x = 0;
	UTIL_MakeVectorsPrivate( angle, forward, NULL, NULL );
	// He's facing me, he meant it
	if ( DotProduct( forward, vecDir ) > 0.96 )	// +/- 15 degrees or so
	{
		return TRUE;
	}
	return FALSE;
}


int CSkeleton :: TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType)
{
	if(IRelationship(Instance(pevAttacker)) < R_NO)
		flDamage=flDamage/10;
	
	// make sure friends talk about it if player hurts talkmonsters...
	int ret = CTalkMonster::TakeDamage(pevInflictor, pevAttacker, flDamage, bitsDamageType);
	if ( !IsAlive() || pev->deadflag == DEAD_DYING )
		return ret;

	if ( m_MonsterState != MONSTERSTATE_PRONE && (pevAttacker->flags & FL_CLIENT) )
	{
		m_flPlayerDamage += flDamage;

		// This is a heurstic to determine if the player intended to harm me
		// If I have an enemy, we can't establish intent (may just be crossfire)
		if ( m_hEnemy == NULL )
		{
			// If the player was facing directly at me, or I'm already suspicious, get mad
			if ( (m_afMemory & bits_MEMORY_SUSPICIOUS) || IsFacing( pevAttacker, pev->origin ) )
			{
				// Alright, now I'm pissed!
//				PlaySentence( "BA_MAD", 4, VOL_NORM, ATTN_NORM );

//				Remember( bits_MEMORY_PROVOKED );
//				StopFollowing( TRUE );
			}
			else
			{
				// Hey, be careful with that
//				PlaySentence( "BA_SHOT", 4, VOL_NORM, ATTN_NORM );
//				Remember( bits_MEMORY_SUSPICIOUS );
			}
		}
		else if ( !(m_hEnemy->IsPlayer()) && pev->deadflag == DEAD_NO )
		{
//			PlaySentence( "BA_SHOT", 4, VOL_NORM, ATTN_NORM );
		}
	}

	return ret;
}

	
//=========================================================
// PainSound
//=========================================================
void CSkeleton :: PainSound ( void )
{
/*	if (gpGlobals->time < m_painTime)
		return;
	
	m_painTime = gpGlobals->time + RANDOM_FLOAT(0.5, 0.75);

	switch (RANDOM_LONG(0,2))
	{
	case 0: EMIT_SOUND_DYN( ENT(pev), CHAN_VOICE, "barney/ba_pain1.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 1: EMIT_SOUND_DYN( ENT(pev), CHAN_VOICE, "barney/ba_pain2.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 2: EMIT_SOUND_DYN( ENT(pev), CHAN_VOICE, "barney/ba_pain3.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	}
*/
}

//=========================================================
// DeathSound 
//=========================================================
void CSkeleton :: DeathSound ( void )
{
/*
	switch (RANDOM_LONG(0,2))
	{
	case 0: EMIT_SOUND_DYN( ENT(pev), CHAN_VOICE, "barney/ba_die1.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 1: EMIT_SOUND_DYN( ENT(pev), CHAN_VOICE, "barney/ba_die2.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 2: EMIT_SOUND_DYN( ENT(pev), CHAN_VOICE, "barney/ba_die3.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	}
*/
}


void CSkeleton::TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType)
{
	switch( ptr->iHitgroup)
	{
	case HITGROUP_CHEST:
	case HITGROUP_STOMACH:
		if (bitsDamageType & (DMG_BULLET | DMG_SLASH | DMG_BLAST))
		{
			flDamage = flDamage / 2;
		}
		break;
	case 10:
		if (bitsDamageType & (DMG_BULLET | DMG_SLASH | DMG_CLUB))
		{
			flDamage -= 20;
			if (flDamage <= 0)
			{
				UTIL_Ricochet( ptr->vecEndPos, 1.0 );
				flDamage = 0.01;
			}
		}
		// always a head shot
		ptr->iHitgroup = HITGROUP_HEAD;
		break;
	}

	CTalkMonster::TraceAttack( pevAttacker, flDamage, vecDir, ptr, bitsDamageType );
}


void CSkeleton::Killed( entvars_t *pevAttacker, int iGib ){
	SetUse( NULL );	

	if(m_hOwner!=NULL)
		ClientPrint(m_hOwner->pev,HUD_PRINTCENTER,"#SkeletonSpell_LostSkeleton");

	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pev->origin );
		WRITE_BYTE( TE_BREAKMODEL);
		WRITE_COORD( pev->origin.x );
		WRITE_COORD( pev->origin.y );
		WRITE_COORD( pev->origin.z );
		WRITE_COORD( pev->size.x);
		WRITE_COORD( pev->size.y);
		WRITE_COORD( pev->size.z);
		WRITE_COORD( 0 ); 
		WRITE_COORD( 0 );
		WRITE_COORD( 50 );
		WRITE_BYTE( 10 );
		WRITE_SHORT(m_iDryBones);//model id#
		WRITE_BYTE( 0 );//# of chunks
		WRITE_BYTE( 50 );//Lifetime
		WRITE_BYTE( 0 );
	MESSAGE_END();

	EMIT_SOUND( edict(), CHAN_BODY, "monster/skeleton/skgibs.wav", 1, ATTN_NORM );

	CBaseEntity::Killed(pevAttacker,iGib);

//	CTalkMonster::Killed( pevAttacker, iGib );
}

//=========================================================
// AI Schedules Specific to this monster
//=========================================================

Schedule_t* CSkeleton :: GetScheduleOfType ( int Type )
{
	Schedule_t *psched;

	switch( Type )
	{
	case SCHED_ARM_WEAPON:
		if ( m_hEnemy != NULL )
		{
			// face enemy, then draw.
			return slBarneyEnemyDraw;
		}
		break;

	// Hook these to make a looping schedule
	case SCHED_TARGET_FACE:
		// call base class default so that skeleton will talk
		// when 'used' 
		psched = CTalkMonster::GetScheduleOfType(Type);

		if (psched == slIdleStand)
			return slBaFaceTarget;	// override this for different target face behavior
		else
			return psched;

	case SCHED_TARGET_CHASE: 

		return slBaFollow;

	case SCHED_IDLE_STAND:
		// call base class default so that scientist will talk
		// when standing during idle
		psched = CTalkMonster::GetScheduleOfType(Type);

		if (psched == slIdleStand)
		{
			// just look straight ahead.
			return slIdleBaStand;
		}
		else
			return psched;	
	}

	return CTalkMonster::GetScheduleOfType( Type );
}

//=========================================================
// GetSchedule - Decides which type of schedule best suits
// the monster's current state and conditions. Then calls
// monster's member function to get a pointer to a schedule
// of the proper type.
//=========================================================
Schedule_t *CSkeleton :: GetSchedule ( void )
{
	return CTalkMonster::GetSchedule();

	if ( HasConditions( bits_COND_HEAR_SOUND ) )
	{
		CSound *pSound;
		pSound = PBestSound();

		ASSERT( pSound != NULL );
		if ( pSound && (pSound->m_iType & bits_SOUND_DANGER) )
			return GetScheduleOfType( SCHED_TAKE_COVER_FROM_BEST_SOUND );
	}
	if ( HasConditions( bits_COND_ENEMY_DEAD ) && FOkToSpeak() )
	{
//		PlaySentence( "BA_KILL", 4, VOL_NORM, ATTN_NORM );
	}

	switch( m_MonsterState )
	{
	case MONSTERSTATE_COMBAT:
		{
// dead enemy
			if ( HasConditions( bits_COND_ENEMY_DEAD ) )
			{
				// call base class, all code to handle dead enemies is centralized there.
				return CTalkMonster :: GetSchedule();
			}

			// always act surprized with a new enemy
			if ( HasConditions( bits_COND_NEW_ENEMY ) && HasConditions( bits_COND_LIGHT_DAMAGE) )
				return GetScheduleOfType( SCHED_SMALL_FLINCH );
				
			// wait for one schedule to draw gun
			if (!m_fGunDrawn )
				return GetScheduleOfType( SCHED_ARM_WEAPON );

			if ( HasConditions( bits_COND_HEAVY_DAMAGE ) )
				return GetScheduleOfType( SCHED_TAKE_COVER_FROM_ENEMY );
		}
		break;

	case MONSTERSTATE_ALERT:	
	case MONSTERSTATE_IDLE:
		if ( HasConditions(bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE))
		{
			// flinch if hurt
			return GetScheduleOfType( SCHED_SMALL_FLINCH );
		}

		if ( m_hEnemy == NULL && IsFollowing() )
		{
			if ( !m_hTargetEnt->IsAlive() )
			{
				// UNDONE: Comment about the recently dead player here?
				StopFollowing( FALSE );
				break;
			}
			else
			{
				if ( HasConditions( bits_COND_CLIENT_PUSH ) )
				{
					return GetScheduleOfType( SCHED_MOVE_AWAY_FOLLOW );
				}
				return GetScheduleOfType( SCHED_TARGET_FACE );
			}
		}

		if ( HasConditions( bits_COND_CLIENT_PUSH ) )
		{
			return GetScheduleOfType( SCHED_MOVE_AWAY );
		}

		// try to say something about smells
		TrySmellTalk();
		break;
	}
	
	return CTalkMonster::GetSchedule();
}

MONSTERSTATE CSkeleton :: GetIdealState ( void )
{
	return CTalkMonster::GetIdealState();
}



void CSkeleton::DeclineFollowing( void )
{
//	PlaySentence( "BA_POK", 2, VOL_NORM, ATTN_NORM );
}

////////////////////////////////////////////////////
// CThrowingBone:  A bone thrown by skeletons
////////////////////////////////////////////////////

class CThrowingBone : public CGrenade
{
public:
	void Spawn( void );
	void Precache( void );
	void EXPORT MoveThink( void );
	void EXPORT ThrowingBoneTouch( CBaseEntity *pOther );

	float m_flIgniteTime;
};
LINK_ENTITY_TO_CLASS( proj_throwingbone, CThrowingBone );

//=========================================================
//=========================================================

#define THROWINGBONE_SPRITE		"models/abone_template1.mdl"
#define THROWINGBONE_VELOCITY	1800
#define THROWINGBONE_DAMAGE		40

void CThrowingBone :: Spawn( void )
{
	Precache( );
	pev->movetype = MOVETYPE_FLY;
	pev->solid = SOLID_BBOX;

	SET_MODEL(ENT(pev), THROWINGBONE_SPRITE);
	
	pev->skin = 0;
	pev->body = 0;
	pev->frame=0;

	UTIL_SetSize(pev, Vector( -5, -5, -5), Vector(5, 5, 5));
	UTIL_SetOrigin( pev, pev->origin );

	pev->classname = MAKE_STRING("throwingbone");

	SetTouch( &CThrowingBone::ThrowingBoneTouch );
	
	UTIL_MakeVectors(pev->angles);
	
	pev->angles = UTIL_VecToAngles( gpGlobals->v_forward );
	pev->velocity = gpGlobals->v_forward * THROWINGBONE_VELOCITY;
	pev->avelocity.x=-400;
	pev->gravity = 0;
	pev->nextthink = gpGlobals->time + 0.1;
	pev->dmg = THROWINGBONE_DAMAGE;
	pev->takedamage=DAMAGE_NO;

	m_flIgniteTime=gpGlobals->time;
}

//=========================================================
//=========================================================
void CThrowingBone :: ThrowingBoneTouch ( CBaseEntity *pOther )
{
	SetTouch( NULL );
	SetThink( NULL );

	if (pOther->pev->takedamage)
	{
		TraceResult tr = UTIL_GetGlobalTrace( );
		entvars_t	*pevOwner;

		pevOwner = VARS( pev->owner );

		// UNDONE: this needs to call TraceAttack instead
		ClearMultiDamage( );

		pOther->TraceAttack(pevOwner,pev->dmg, pev->velocity.Normalize(), &tr, DMG_NEVERGIB ); 
		ApplyMultiDamage( pev, pevOwner );

		// play body "thwack" sound
		switch( RANDOM_LONG(0,1) )
		{
		case 0:
			EMIT_SOUND(ENT(pev), CHAN_WEAPON, "weapons/xbow_hitbod1.wav", 1, ATTN_NORM); break;
		case 1:
			EMIT_SOUND(ENT(pev), CHAN_WEAPON, "weapons/xbow_hitbod2.wav", 1, ATTN_NORM); break;
		}
	}

	UTIL_Remove(this);
}

//=========================================================
//=========================================================
void CThrowingBone :: Precache( void )
{
	PRECACHE_SOUND("weapons/xbow_hitbod1.wav");
	PRECACHE_SOUND("weapons/xbow_hitbod2.wav");
	
	PRECACHE_MODEL(THROWINGBONE_SPRITE);
}
