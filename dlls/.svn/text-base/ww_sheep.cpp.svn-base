/*
	sheep.cpp

	by: Alan Fischer

	for:  The Wizard Wars mod
*/

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"schedule.h"

//=========================================================
// Monster's Anim Events Go Here
//=========================================================

class CSheep : public CBaseMonster{
public:
	void Spawn();
	void Precache();
	int  Classify();
	void MonsterThink();
	void SetYawSpeed();
	int IRelationship(CBaseEntity *pTarget);
	void RunAI();
	void MovementComplete();
	BOOL ShouldAdvanceRoute(float flWaypointDist);
	BOOL ShouldGibMonster(int iGib){return 0;}
	BOOL ShouldFadeOnDeath(){return 1;}
	int BloodColor(){return DONT_BLEED;}
	void Killed(entvars_t *pevAttacker,int iGib);

	Vector m_vecGoal;
};
LINK_ENTITY_TO_CLASS(monster_sheep,CSheep);

int	CSheep :: Classify ( void ){
	return	CLASS_PLAYER_ALLY;
}

void CSheep :: Spawn(){
	Precache();

	SET_MODEL(ENT(pev),"models/npc/sheep.mdl");
	UTIL_SetSize(pev,Vector(-12,-12,-6),Vector(12,12,24));

	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_STEP;
	m_bloodColor		= BLOOD_COLOR_RED;
	pev->health			= 50;
	pev->view_ofs		= Vector ( 0, 0, 20 );// position of the eyes relative to monster's origin.
	m_flFieldOfView		= 0.5;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState		= MONSTERSTATE_NONE;
	m_IdealActivity		= ACT_WALK;
	pev->sequence		= 0;

	ResetSequenceInfo();
	MonsterInit();

	m_vecGoal=pev->origin+Vector(RANDOM_FLOAT(-50,50),RANDOM_FLOAT(-50,50),0);
}

int CSheep::IRelationship(CBaseEntity *pTarget){
	return R_NM;
}

void CSheep :: Precache(){
	PRECACHE_MODEL("models/npc/sheep.mdl");
	PRECACHE_SOUND("monster/sheep/baa.wav");
}

void CSheep :: MonsterThink(){
	if(m_fSequenceFinished){
		StudioFrameAdvance();

		if(pev->sequence==2)
			pev->sequence=1;
		else if(pev->sequence==3 || pev->sequence==4)
			pev->sequence=0;
		else if(pev->sequence==1 && RANDOM_LONG(0,3)==1)
			pev->sequence=3;
		else if(pev->sequence==0 && RANDOM_LONG(0,3)==1)
			pev->sequence=2;

		ResetSequenceInfo();

		if(RANDOM_LONG(0,3)==1)
			EMIT_SOUND(ENT(pev),CHAN_VOICE,"monster/sheep/baa.wav",.6,ATTN_NORM);
	}
	else if(pev->sequence==0 || pev->sequence==4){
		CBaseMonster::MonsterThink();
	}	
	else{
		StudioFrameAdvance();
	}

	pev->nextthink=gpGlobals->time+.1;
}

void CSheep::RunAI(){
	if(MoveToLocation(ACT_WALK,0,m_vecGoal)==0)
		m_vecGoal=pev->origin+Vector(RANDOM_FLOAT(-50,50),RANDOM_FLOAT(-50,50),0);
	
	FCheckAITrigger();

	PrescheduleThink();

	MaintainSchedule();
}

BOOL CSheep::ShouldAdvanceRoute(float flWaypointDist){
	if(CBaseMonster::ShouldAdvanceRoute(flWaypointDist)!=0)
		m_vecGoal=pev->origin+Vector(RANDOM_FLOAT(-50,50),RANDOM_FLOAT(-50,50),0);

	return 0;
}

void CSheep::SetYawSpeed ( void ){
	int ys;

	switch ( m_Activity ){
	case ACT_IDLE:
	default:
		ys = 90;
	}

	pev->yaw_speed = ys;
}

void CSheep::Killed(entvars_t *pevAttacker,int iGib){
	pev->health=0;
	
	CBaseMonster::Killed(pev,0);

	pev->sequence=4;
	pev->flags&=~FL_ONGROUND;
	pev->movetype=MOVETYPE_NOCLIP;
	pev->solid=SOLID_NOT;
	pev->velocity.z+=RANDOM_LONG(20,30);

	UTIL_SetOrigin(pev,pev->origin+Vector(0,0,1));
	SUB_StartFadeOut();
}
