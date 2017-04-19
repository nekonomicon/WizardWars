/*
	dragon.cpp

	by: Alan Fischer

	for:  The Wizard Wars mod
*/

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"schedule.h"
#include	"flyingmonster.h"
#include	"player.h"
#include	"ww_dragon.h"
#include	"weapons.h"
#include	"gamerules.h"
//=========================================================
// Monster's Anim Events Go Here
//=========================================================

#define DRAGON_HEALTH			400.0
#define DRAGON_FLAME			47.0
#define DRAGON_RECHARGETIME		2.0
#define DRAGON_RECHARGEAMOUNT	6
#define DRAGON_UPDATETIME		.25
#define DRAGON_NEXTFIRE			.03

extern int gmsgDragon;

void WWDragon :: Spawn(){
	Precache( );

	SET_MODEL(ENT(pev), "models/npc/dragon.mdl");
	UTIL_SetSize( pev, Vector( -80, -80, 0 ), Vector( 80, 80, 64 ) );

	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_STEP;
	m_bloodColor		= BLOOD_COLOR_GREEN;
	pev->health			= DRAGON_HEALTH;
	pev->view_ofs		= Vector ( 0, -36, 36 );// position of the eyes relative to monster's origin.
	m_flFieldOfView		= 0.5;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState		= MONSTERSTATE_NONE;

	m_flNextIdleSound	= 0.0f;

	MonsterInit();
}
LINK_ENTITY_TO_CLASS( monster_dragon, WWDragon );

void WWDragon :: Precache(){
	PRECACHE_MODEL("models/npc/dragon.mdl");
	PRECACHE_MODEL("models/npc/v_dragon.mdl");
	PRECACHE_MODEL("models/rpgrocket.mdl");

	PRECACHE_SOUND( "monster/dragon/dragon_fly1.wav" );
	PRECACHE_SOUND( "monster/dragon/dragon_idle1.wav" );
	PRECACHE_SOUND( "monster/dragon/dragon_attack1.wav" );
	PRECACHE_SOUND( "monster/dragon/dragon_pain1.wav" );
	PRECACHE_SOUND( "monster/dragon/dragon_die1.wav" );
// Codiac 1.2 B18 - better fire sound
	PRECACHE_SOUND( "monster/dragon/dragon_fire.wav" );

	UTIL_PrecacheOther("proj_dragonfire");

	m_usDragonFire=PRECACHE_EVENT(1,"events/monsters/dragonfire.sc");
	m_usDragonFlap=PRECACHE_EVENT(1,"events/monsters/dragonflap.sc");
}	

void WWDragon::KeyValue(KeyValueData *pkvd){
	if(FStrEq(pkvd->szKeyName,"hostile")){
		pev->iuser1=atoi(pkvd->szValue);
		pkvd->fHandled=TRUE;
	}
	else if(FStrEq(pkvd->szKeyName,"mountable")){
		pev->iuser2=atoi(pkvd->szValue);
		pkvd->fHandled=TRUE;
	}
	else if(FStrEq(pkvd->szKeyName,"team_no")){
		pev->team=atoi(pkvd->szValue);
		pkvd->fHandled=TRUE;
	}
	else{
		CBaseMonster::KeyValue(pkvd);
	}
}

/*
int	WWDragon :: Classify ( void ){
	if(pev->iuser1)
		return CLASS_FACTION_B;// Codiac 1.2 B22 CLASS_ALIEN_MONSTER;
	else
		return CLASS_PLAYER_ALLY;
}
*/
void WWDragon::MonsterThink(){
	
	if( gpGlobals->time >= m_flNextIdleSound )
	{
		if( !( pev->flags & FL_ONGROUND ) )
			EMIT_SOUND( edict(), CHAN_BODY, "monster/dragon/dragon_fly1.wav", 1.0f, ATTN_NORM );
		else
			EMIT_SOUND( edict(), CHAN_BODY, "monster/dragon/dragon_idle1.wav", 1.0f, ATTN_NORM );

		m_flNextIdleSound = gpGlobals->time + 2.0f;
	}

	if(m_hRider){
		if(m_hRider->IsAlive())
			UpdateRider();
		else
			DeMount();

		pev->nextthink=gpGlobals->time+.1;
	}
	else{
		CBaseMonster::MonsterThink();
	}
}

int WWDragon::ShouldFadeOnDeath(){
	return TRUE;
}

int WWDragon::IRelationship(CBaseEntity *pTarget){

	if(pev->iuser1)
	{
		if(pev->team != 0 && pev->team == pTarget->pev->team)
			return R_AL;
		else
			return R_NM;
	}
	else{
		if(pev->team == 0 || pev->team == pTarget->pev->team)
			return R_AL;
		else
			return R_NO;
	}

	return R_NO;

//	return CBaseMonster::IRelationship( pTarget );
}

void WWDragon :: SetYawSpeed ( void ){
	int ys;

	switch ( m_Activity ){
	case ACT_IDLE:
	default:
		ys = 90;
	}

	pev->yaw_speed = ys;
}

void WWDragon :: HandleAnimEvent( MonsterEvent_t *pEvent ){
	switch( pEvent->event )
	{
	case 1:
		ShootFire();
		break;
	default:
		CBaseMonster::HandleAnimEvent( pEvent );
		break;
	}
}

/*
int WWDragon::CheckRangeAttack1(float flDot,float flDist){
	return FALSE;
}
*/

Task_t	tlDragonAttack1[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_FACE_IDEAL,			(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
};

Schedule_t	slDragonAttack1[] =
{
	{ 
		tlDragonAttack1,
		ARRAYSIZE ( tlDragonAttack1 ), 
		bits_COND_CAN_MELEE_ATTACK1 |
		bits_COND_HEAR_SOUND |
		bits_COND_HEAVY_DAMAGE, 
		0,
		"Slave Range Attack1"
	},
};

DEFINE_CUSTOM_SCHEDULES(WWDragon)
{
	slDragonAttack1,
};

IMPLEMENT_CUSTOM_SCHEDULES(WWDragon,CBaseMonster);

Schedule_t* WWDragon::GetScheduleOfType(int Type){
	switch(Type){
		case SCHED_RANGE_ATTACK1:
		{
			return &slDragonAttack1[ 0 ];
		}
		break;
	}

	return CBaseMonster::GetScheduleOfType( Type );
}

void WWDragon::Mount(CBasePlayer *pPlayer)
{
	if(pev->iuser2==FALSE)
	{
		ClientPrint(pPlayer->pev, HUD_PRINTCENTER, "Dragon not mountable!");
		return;
	}

	if(IRelationship(pPlayer) > R_NO)
	{
		ClientPrint(pPlayer->pev, HUD_PRINTCENTER, "Dragon has wrong relationship!");
		return;
	}

	if(pPlayer->m_pClass->m_bOnDragon || m_hRider != NULL)
	{
		ClientPrint(pPlayer->pev, HUD_PRINTCENTER, "Dragon or you are already mounted!");
		return;
	}

	if((pev->team) && pPlayer->pev->team != pev->team){
		ClientPrint(pPlayer->pev,HUD_PRINTCENTER,"You cannot ride enemy dragons");
		return;
	}
	
	m_hRider = (CBaseEntity*)pPlayer;
	pPlayer->m_pClass->m_hMounted = this;

	SET_MODEL(ENT(pev), "models/npc/v_dragon.mdl");

	if(pPlayer->m_pActiveItem){
		pPlayer->m_pActiveItem->Holster( );
		pPlayer->m_pActiveItem=NULL;
	}

	pPlayer->pev->viewmodel = MAKE_STRING("models/npc/v_dragon.mdl");
	
	m_pOwner		= pev->owner;
	pev->owner		= pPlayer->edict();
	pev->aiment		= pPlayer->edict();
	pev->movetype	= MOVETYPE_FOLLOW;

	SetActivity(ACT_FLY);
	ResetSequenceInfo();

	pPlayer->pev->flags &= ~FL_ONGROUND;
	pPlayer->pev->origin = pev->origin;
	pPlayer->pev->origin.z += 50;

	g_engfuncs.pfnSetPhysicsKeyValue(pPlayer->edict(),"dragon","1");
	g_engfuncs.pfnSetClientMaxspeed(ENT(pPlayer->pev),550);

	pPlayer->m_pClass->m_bOnDragon = TRUE;

	pev->health			= DRAGON_HEALTH;
	pev->armorvalue		= DRAGON_FLAME;
	m_flClientHealth	= pev->health;
	m_flClientFlame		= pev->armorvalue;
	m_flUpdateTime		= 0;
	m_flRechargeTime	= 0;
	m_flFireTime		= gpGlobals->time + DRAGON_NEXTFIRE;

	MESSAGE_BEGIN(MSG_ONE,gmsgDragon,NULL,pPlayer->pev);
		WRITE_BYTE(100);
		WRITE_BYTE(100);
	MESSAGE_END();

	UpdateRider();
}

extern unsigned short g_usDeathFlash;

void WWDragon::Killed(entvars_t *pevAttacker,int iGib){
	EMIT_SOUND( edict(), CHAN_BODY, "monster/dragon/dragon_die1.wav", 1, ATTN_NORM );

	DeMount();

	CBaseMonster::Killed(pevAttacker,iGib);
}

/*
BUGBUG! verify m_hMounted && m_bOnDragon are all copied over!
Also check combo stuff in player.cpp and such
mosters being precached?
*/
void WWDragon::DeMount(){
	if(m_hRider==NULL)
		return;

	CBasePlayer *pPlayer=(CBasePlayer*)(CBaseEntity*)m_hRider;

	if(pPlayer->IsAlive() && pPlayer->m_pClass!=NULL){
		g_engfuncs.pfnSetClientMaxspeed(ENT(pPlayer->pev),pPlayer->m_pClass->PlayerSpeed());
	}

	g_engfuncs.pfnSetPhysicsKeyValue(pPlayer->edict(),"dragon","0");

	pPlayer->pev->velocity.z += 100;
	pPlayer->m_pClass->m_hMounted	= NULL;
	pPlayer->m_pClass->m_bOnDragon	= FALSE;

	SET_MODEL(ENT(pev),"models/npc/dragon.mdl");

	pev->owner		= m_pOwner;
	pev->aiment		= NULL;
	pev->movetype	= MOVETYPE_STEP;
	pev->solid		= SOLID_NOT;

	UTIL_SetSize( pev, Vector( -90, -90, 0 ), Vector( 90, 90, 64 ) );
	UTIL_SetOrigin( pev, pev->origin );

	CBaseMonster::Killed(pev,0);

	MESSAGE_BEGIN(MSG_ONE,gmsgDragon,NULL,pPlayer->pev);
		WRITE_BYTE(0);
		WRITE_BYTE(0);
	MESSAGE_END();

	m_hRider->pev->viewmodel=0;
	((CBasePlayer*)(CBaseEntity*)m_hRider)->SelectLastItem();

	m_hRider = NULL;
}

int WWDragon::TakeDamage( entvars_t * pevInflictor, entvars_t * pevAttacker, float flDamage, int bitsDamageType )
{
	EMIT_SOUND( edict(), CHAN_BODY, "monster/dragon/dragon_pain1.wav", 1.0f, ATTN_NORM );

	return CBaseMonster::TakeDamage( pevInflictor, pevAttacker, flDamage, bitsDamageType );
}


void WWDragon::UpdateRider()
{
	StudioFrameAdvance();

	if(m_fSequenceFinished)
	{
		if(m_hRider->pev->velocity.z > 1)
		{
			int flags = FEV_NOTHOST;

			PLAYBACK_EVENT(flags,m_hRider->edict(),m_usDragonFlap);

			pev->sequence = 0;
		}
		else{
			pev->sequence=1;
		}

		ResetSequenceInfo();
	}

	if( m_hRider->pev->button & IN_ATTACK &&
		pev->armorvalue > 2 &&
		(gpGlobals->time > m_flFireTime)
	  )
	{
		m_flFireTime = gpGlobals->time + DRAGON_NEXTFIRE;
		pev->armorvalue -= 2.5;
		ShootFire();
	}

	if(m_flRechargeTime <= gpGlobals->time)
	{
		m_flRechargeTime += DRAGON_RECHARGETIME;
		pev->armorvalue += DRAGON_RECHARGEAMOUNT;
		if(pev->armorvalue > DRAGON_FLAME)
			pev->armorvalue = DRAGON_FLAME;
	}

	if( ( pev->health != m_flClientHealth ||
		  pev->armorvalue != m_flClientFlame
		 ) &&
		 m_flUpdateTime <= gpGlobals->time)
	{
		m_flUpdateTime		= gpGlobals->time + DRAGON_UPDATETIME;
		m_flClientHealth	= pev->health;
		m_flClientFlame		= pev->armorvalue;

		MESSAGE_BEGIN(MSG_ONE,gmsgDragon,NULL,m_hRider->pev);
			WRITE_BYTE((m_flClientHealth * 100.0) / DRAGON_HEALTH);
			WRITE_BYTE((m_flClientFlame * 100.0)  / DRAGON_FLAME);
		MESSAGE_END();
	}

	m_hRider->pev->viewmodel = MAKE_STRING("models/npc/v_dragon.mdl");
}

void WWDragon::ShootFire()
{
	CBaseEntity *pEnt;
	Vector vecOrigin;
	Vector vecVelocity;
	Vector vecAngles;
	edict_t *pOwner = m_pOwner; //edict();

	if(m_hRider){
		UTIL_MakeVectors(m_hRider->pev->v_angle);
		vecOrigin = pev->origin+gpGlobals->v_forward * 70 + gpGlobals->v_up * -10;
		pOwner = m_hRider->edict();
		vecAngles = m_hRider->pev->v_angle;
	}
	else{
		UTIL_MakeVectors(pev->angles);
		vecOrigin = pev->origin + gpGlobals->v_forward * 70 + gpGlobals->v_up * 70;
		if(m_hEnemy)
			vecAngles = UTIL_VecToAngles(m_hEnemy->pev->origin-vecOrigin);
		else
			vecAngles = pev->angles;
		vecAngles.x *= -1;
	}
	
	pEnt = Create("proj_dragonfire",vecOrigin,vecAngles,pOwner);

	UTIL_MakeVectors(vecAngles);
	vecVelocity=gpGlobals->v_forward * 1400;
	vecVelocity[0] += RANDOM_FLOAT(-140,140);
	vecVelocity[1] += RANDOM_FLOAT(-140,140);
	vecVelocity[2] += RANDOM_FLOAT(-140,140);
	pEnt->pev->velocity = vecVelocity;
	pEnt->pev->team = pev->team;

	vecAngles = UTIL_VecToAngles(pEnt->pev->velocity);

	PLAYBACK_EVENT_FULL(0,edict(),m_usDragonFire,0,(float *)&vecOrigin,(float *)&vecAngles,0,0,0,0,0,0);
}

void WWDragon::Use(CBaseEntity *pActivator,CBaseEntity *pCaller,USE_TYPE useType,float value){
	if(!IsAlive())
	{
		ClientPrint(pActivator->pev, HUD_PRINTCENTER, "Dragon is dead!");
		return;
	}

	if( pev->solid == SOLID_NOT )
	{
		ClientPrint(pActivator->pev, HUD_PRINTCENTER, "Dragon is not solid!");
		return;
	}

	if(pActivator->IsPlayer() && ((CBasePlayer*)pActivator)->m_pClass->m_hMounted==NULL && IRelationship(pActivator) <= R_NO){
		Mount((CBasePlayer*)pActivator);
	}
	else
	{
		if(!pActivator->IsPlayer())
		{
			ClientPrint(pActivator->pev, HUD_PRINTCENTER, "You are not a player!");
		}
		else if(((CBasePlayer*)pActivator)->m_pClass->m_hMounted != NULL)
		{
			ClientPrint(pActivator->pev, HUD_PRINTCENTER, "You are already mounted!");
		}
		else if(IRelationship(pActivator) > R_NO)
		{
			ClientPrint(pActivator->pev, HUD_PRINTCENTER, "You have wrong relationship!");
		}
	}
}

/* ********** */
/* DragonFire */
/* ********** */

#define DRAGONFIRE_DAMAGE 20
#define MDRAGONFIRE_DAMAGE 30
#define DRAGONFIRE_RADIUS 180

class WWDragonFire:public CBaseEntity{
	void Spawn( void );
	void Precache( void );
	void EXPORT DragonFireTouch( CBaseEntity *pOther );
};
LINK_ENTITY_TO_CLASS( proj_dragonfire, WWDragonFire );

void WWDragonFire::Precache(){
	PRECACHE_MODEL("sprites/xspark4.spr");
	PRECACHE_MODEL("sprites/gargeye1.spr");
	PRECACHE_MODEL("sprites/fexplo1.spr");
}

void WWDragonFire::Spawn()
{
	Precache();
	pev->movetype=MOVETYPE_FLY;
	pev->solid=SOLID_BBOX;

	SET_MODEL(ENT(pev),"sprites/xspark4.spr");
	UTIL_SetSize(pev,Vector(0,0,0),Vector(0,0,0));
	UTIL_SetOrigin( pev, pev->origin );

	pev->effects |= EF_NODRAW;
	pev->classname = MAKE_STRING("proj_dragonfire");

	SetThink(&WWDragonFire::SUB_Remove);
	SetTouch(&WWDragonFire::DragonFireTouch);
	
	pev->nextthink=gpGlobals->time + 10;
	pev->dmg = DRAGONFIRE_DAMAGE;
}

void WWDragonFire::DragonFireTouch(CBaseEntity *pOther)
{
	SetThink(NULL);
	SetTouch(NULL);

	CBasePlayer *pPlr = NULL;

	if(pev->owner)
		pPlr = (CBasePlayer*)CBaseEntity::Instance(pev->owner);
	
	if(pPlr && !pPlr->m_pClass->m_bOnDragon)
		::RadiusDamage(pev->origin, pev, VARS(pev->owner), pev->dmg, DRAGONFIRE_RADIUS, 0, DMG_BURN|DMG_BLAST|DMG_SLOWBURN);
	else if (pPlr && pPlr->m_pClass->m_bOnDragon)
		::RadiusDamage(pev->origin, pev, VARS(pev->owner), MDRAGONFIRE_DAMAGE, DRAGONFIRE_RADIUS, 0, DMG_BURN|DMG_BLAST|DMG_SLOWBURN);
	else
		::RadiusDamage(pev->origin, pev, pev, pev->dmg, DRAGONFIRE_RADIUS, 0, DMG_BURN|DMG_BLAST|DMG_SLOWBURN);

	UTIL_Remove(this);
}
