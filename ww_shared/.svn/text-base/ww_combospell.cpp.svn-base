/*
	ComboSpell.cpp
	By: Alan Fischer 
	Parts from: Valve Software and Id Software

	For the WizWars mod.
*/

#include "extdll.h"
#include "shake.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "gamerules.h"

#include "ww_shared/ww_defs.h"
#include "ww_shared/ww_weapons.h"
#include "ww_shared/ww_combospell.h"

// Define all constants here
#define COMBOSPELL_DELAY			2
#define COMBOSPELL_COST				1


enum combo_e{
	COMBO_DRAW = 0,
	COMBO_IDLE1,
	COMBO_IDLE2,
	COMBO_IDLE3,
	COMBO_HOLSTER1,
};


#define BALLLIGHTNINGSPELL_SOUND_SHOOT		"spells/combos/ball_lightning.wav"
#define BALLLIGHTNINGSPELL_DAMAGE			500
#define BALLLIGHTNINGSPELL_BOLT_WIDTH		75
#define BALLLIGHTNINGSPELL_BOLT_NOISE		60
#define BALLLIGHTNINGSPELL_BOLT_LIFE		10
#define BALLLIGHTNINGSPELL_BOLT_BRIGHTNESS	255

#define SKELETONSPELL_RADIUS		400
#define SKELETONSPELL_SOUND_CAST "spells/combos/raisedead_cast.wav"

#define SPIRITWIZ_SOUND_CAST "spells/combos/spritwiz_cast.wav"


#define TORNADOSPELL_SOUND_CAST  "spells/combos/tornado_cast.wav"

#define BALLLIGHTNING_VELOCITY				.8
#define BALLLIGHTNING_MODEL					"sprites/enter1.spr" //"sprites/enter1.spr"
#define BALLLIGHTNING_DAMAGE				600

#define COMET_VELOCITY			2.0
#define COMET_SOUND_SHOOT		"spells/combos/comet_hit.wav" 
#define COMET_SOUND_CAST		"spells/combos/comet_cast.wav" 
#define COMET_SOUND_CAST_MISS	"spells/combos/comet_miss.wav" 
#define COMET_SPRITE			"models/rpgrocket.mdl"
#define COMET_EXPLODE_DISTANCE	50
#define COMET_DAMAGE			800
#define COMET_RADIUS			200

#define GIANTPLANTSPELL_SEARCHDIST		50
#define GIANTPLANTSPELL_SOUND_CAST "spells/combos/giantplant_cast.wav"

#define GIANTPLANT_DAMAGE				40
#define GIANTPLANT_HEALTH				500
#define GIANTPLANT_HEALTHTIME			5
#define GIANTPLANT_VELOCITY				100

#define LIGHTNINGCLOUD_VELOCITY			.4
#define LIGHTNINGCLOUD_SPRITE			"sprites/poison.spr"
#define LIGHTNINGCLOUD_DAMAGE			80
#define LIGHTNINGCLOUD_RANGE			700
#define LIGHTNINGCLOUD_DELAY			.5

#define METEOR_VELOCITY			1.25
#define METEOR_SPRITE			"models/rpgrocket.mdl"
#define METEORSPELL_SOUND_HIT   "spells/combos/meteor_hit.wav"
#define METEOR_EXPLODE_DISTANCE	90
#define METEOR_DAMAGE			600
#define METEOR_RADIUS			200

#define METEORSPELL_SOUND_CAST "spells/combos/meteor_cast.wav"
#define METEORSPELL_SOUND_MISS "spells/combos/meteor_miss.wav"

#define SPIRITWIZ_VELOCITY			.3
#define SPIRITWIZ_MODEL				"models/combos/netherspirit.mdl"
#define SPIRITWIZ_DAMAGE			600
#define SPIRITWIZ_LITDAMAGE			20
#define SPIRITWIZ_HEALTH			600
#define SPIRITWIZ_SOUND				"spells/combos/spritwiz_wail.wav"

#define TORNADO_VELOCITY			.55
#define TORNADO_MODEL				"models/combos/tornado.mdl"
#define TORNADOSPELL_SOUND_WIND		"spells/combos/tornado_spin.wav"
#define TORNADO_DAMAGE				15
#define TORNADO_LIFE				30
#define TORNADO_RADIUS				100
#define TORNADO_SPIN				.5

#define WOMBAT_VELOCITY				450
#define WOMBAT_MODEL				"models/combos/wombat.mdl"
#define WOMBAT_DAMAGE				100
#define WOMBAT_HEAL					200
#define WOMBAT_HEALTH				1000
#define WOMBAT_LIFE					40

enum w_wombat_e {
	WOMBAT_RUN = 0,
	WOMBAT_JUMP,
	WOMBAT_ATTACK
};

enum wombatspell_e {
	WOMBATSPELL_IDLE1 = 0,
	WOMBATSPELL_IDLE2,
	WOMBATSPELL_IDLE3,
	WOMBATSPELL_SHOOT,
	WOMBATSPELL_SHOOT_EMPTY,
	WOMBATSPELL_RELOAD,
	WOMBATSPELL_RELOAD_NOT_EMPTY,
	WOMBATSPELL_DRAW,
	WOMBATSPELL_HOLSTER,
	WOMBATSPELL_ADD_SILENCER
};

#define SPIRITDRAGON_MODEL				"models/combos/spiritofthedragon.mdl"
#define SPIRITDRAGON_LIFE				30
#define SPIRITDRAGON_DELAY				0.2

int StalkRoom(Vector stalkPos){
	Vector mins = stalkPos;
	Vector maxs = stalkPos;
	mins.x=mins.x-GIANTPLANTSPELL_SEARCHDIST;
	mins.y=mins.y-GIANTPLANTSPELL_SEARCHDIST;
	maxs.x=maxs.x+GIANTPLANTSPELL_SEARCHDIST;
	maxs.y=maxs.y+GIANTPLANTSPELL_SEARCHDIST;
	maxs.z=maxs.z+80;

#ifndef CLIENT_DLL
	if(!UTIL_SearchArea(mins,maxs))
		return 2;
#endif
	return 1;
}

#ifndef CLIENT_DLL

/* ------------- */
/* BallLightning */
/* ------------- */

class CBallLightning:public CBaseMonster{
public:
	void Spawn();
	void Precache();
	void EXPORT BallLightningThink();
	void EXPORT BallLightningTouch(CBaseEntity *pOther);
	void Killed(entvars_t *pevAttacker,int iGib);
	int Classify(){return CLASS_PLAYER_ALLY;}

	float m_maxFrame;
	float m_flVelocity;
	float m_flDie;
};

LINK_ENTITY_TO_CLASS( monster_balllightning, CBallLightning);

void CBallLightning::Precache(){
	PRECACHE_MODEL(BALLLIGHTNING_MODEL);
}

void CBallLightning::Spawn(){
	Precache();

	SET_MODEL(ENT(pev),BALLLIGHTNING_MODEL);

	pev->solid = SOLID_BBOX;
	pev->movetype=MOVETYPE_FLY;
	pev->scale=2;
	pev->body=0;
	pev->skin=0;
	pev->renderamt=160;
	pev->rendermode=kRenderTransAdd;
	pev->sequence=0;
	pev->flags |= FL_MONSTER;
	pev->takedamage = DAMAGE_NO;
	pev->angles.x=0;

	m_flFieldOfView=-1;
	m_hEnemy=NULL;

	UTIL_MakeVectors(pev->angles);
	pev->velocity = gpGlobals->v_forward * CVAR_GET_FLOAT("sv_maxspeed")*BALLLIGHTNING_VELOCITY; //BallLightnings velocity now depends on globals maxspeed
	m_flVelocity=pev->velocity.Length();

	SetThink(&CBallLightning::BallLightningThink);
	SetTouch(&CBallLightning::BallLightningTouch);

	UTIL_SetSize(pev, Vector(0,0,0), Vector(0,0,0));
	UTIL_SetOrigin( pev, pev->origin );

	m_flDie = gpGlobals->time + 30;

	m_maxFrame=(float)MODEL_FRAMES(pev->modelindex)-1;

	pev->nextthink = gpGlobals->time + 0.1;
}

void CBallLightning::BallLightningThink(){
	pev->frame++;

	if(pev->frame>m_maxFrame)
		pev->frame=0;

	if (gpGlobals->time >= m_flDie){
		Killed(pev,0);
		return;
	}

	Look(1024);
	m_hEnemy=BestVisibleEnemy();

	if(m_hEnemy==NULL){
		Killed(pev,0);
	}

	if(m_hEnemy){
		Vector vecToEnemy;

		m_vecEnemyLKP = m_hEnemy->BodyTarget( pev->origin );
		Vector v_temp=Vector(pev->origin);
		vecToEnemy = ( m_vecEnemyLKP - v_temp ).Normalize();

		pev->velocity=vecToEnemy*m_flVelocity;
		pev->angles = UTIL_VecToAngles(pev->velocity);
	}

	pev->nextthink = gpGlobals->time + 0.1;
}

void CBallLightning::BallLightningTouch(CBaseEntity *pOther){
	if(IRelationship(pOther) <= R_NO || !pOther->pev->takedamage || !pOther->IsAlive())
		return;

	if(pev->owner!=NULL)
		pOther->TakeDamage(pev,VARS(pev->owner),BALLLIGHTNING_DAMAGE,DMG_BURN);
	else
		pOther->TakeDamage(pev,pev,BALLLIGHTNING_DAMAGE,DMG_BURN);

	Killed(pev,0);
}

void CBallLightning::Killed(entvars_t *pevAttacker,int iGib){
	CBaseEntity::Killed(pevAttacker,iGib);
}



/* ----- */
/* Comet */
/* ----- */

class CComet:public CBaseMonster{
public:
	void Spawn();
	void Precache();
	void EXPORT CometThink();
	void Killed(entvars_t *pevAttacker,int iGib);

	int m_iTrail;
	float m_flNextTrailTime;
};

void CComet::Precache(){
	PRECACHE_MODEL(COMET_SPRITE);
	PRECACHE_SOUND(COMET_SOUND_SHOOT);
	PRECACHE_SOUND(COMET_SOUND_CAST);
	PRECACHE_SOUND(COMET_SOUND_CAST_MISS);
	PRECACHE_SOUND("ambience/biggun3.wav");

	m_iTrail = PRECACHE_MODEL("sprites/smoke.spr");
}

void CComet::Spawn(){
	Precache();

	pev->solid = SOLID_BBOX;
	pev->movetype=MOVETYPE_NOCLIP;
	pev->effects |= EF_LIGHT;

	UTIL_MakeVectors(pev->angles);
	pev->velocity = gpGlobals->v_forward * CVAR_GET_FLOAT("sv_maxspeed")*COMET_VELOCITY; //Comets velocity now depends on globals maxspeed

	SET_MODEL(ENT(pev),COMET_SPRITE);

	SetThink(&CComet::CometThink);

	UTIL_SetSize(pev, Vector( 0, 0, 0), Vector(0, 0, 0));
	UTIL_SetOrigin( pev, pev->origin );

	pev->nextthink = m_flNextTrailTime = gpGlobals->time + 0.1;
}

void CComet::CometThink(){

	if(m_hEnemy == NULL){
		Killed(pev,0);
		return;
	}

	Vector vecToEnemy;

	m_vecEnemyLKP = m_hEnemy->BodyTarget(pev->origin);
	vecToEnemy = ( m_vecEnemyLKP - pev->origin ).Normalize();
	pev->velocity=(vecToEnemy*pev->velocity.Length());
	pev->angles = UTIL_VecToAngles(pev->velocity);

	if(m_flNextTrailTime <= gpGlobals->time){
		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
			WRITE_BYTE( TE_BEAMFOLLOW );
			WRITE_SHORT(entindex() );	// entity
			WRITE_SHORT(m_iTrail );	// model
			WRITE_BYTE( 20 ); // life
			WRITE_BYTE( 30 );  // width
			WRITE_BYTE( 255 );   // r, g, b
			WRITE_BYTE( 0 );   // r, g, b
			WRITE_BYTE( 0 );   // r, g, b
			WRITE_BYTE( 255 );	// brightness
		MESSAGE_END();  // move PHS/PVS data sending into here (SEND_ALL, SEND_PVS, SEND_PHS)

		m_flNextTrailTime = gpGlobals->time + 0.3;
	}

	if((m_vecEnemyLKP-pev->origin).Length()<COMET_EXPLODE_DISTANCE || !m_hEnemy->IsAlive() ){
		if(pev->owner)
			::RadiusDamage(pev->origin,pev,VARS(pev->owner),COMET_DAMAGE,COMET_RADIUS,NULL,DMG_BURN|DMG_ALWAYSGIB|DMG_BLAST);
		else
			::RadiusDamage(pev->origin,pev,pev,COMET_DAMAGE,COMET_RADIUS,NULL,DMG_BURN|DMG_ALWAYSGIB|DMG_BLAST);
		
		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY, pev->origin );
			WRITE_BYTE( TE_EXPLOSION);
			WRITE_COORD( pev->origin.x );
			WRITE_COORD( pev->origin.y );
			WRITE_COORD( pev->origin.z);
			WRITE_SHORT( g_sModelIndexFireball );
			WRITE_BYTE( 40  ); // scale * 10
			WRITE_BYTE( 15  ); // framerate
			WRITE_BYTE( TE_EXPLFLAG_NOSOUND );
		MESSAGE_END();

		EMIT_SOUND( ENT(pev), CHAN_STATIC, "ambience/biggun3.wav", 1, .1 );

		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY, pev->origin);
			WRITE_BYTE(TE_BEAMTORUS);
			WRITE_COORD(pev->origin.x);
			WRITE_COORD(pev->origin.y);
			WRITE_COORD(pev->origin.z+32);
			WRITE_COORD(pev->origin.x);
			WRITE_COORD(pev->origin.y);
			WRITE_COORD(pev->origin.z+COMET_RADIUS*3);
 			WRITE_SHORT(g_sModelIndexSmoke);
			WRITE_BYTE(0); // startframe
			WRITE_BYTE(0); // framerate
			WRITE_BYTE(10); // life
			WRITE_BYTE(30);  // width
			WRITE_BYTE(5);   // noise
			WRITE_BYTE(255);   // r, g, b
			WRITE_BYTE(0);   // r, g, b
			WRITE_BYTE(0);   // r, g, b
			WRITE_BYTE(128);	// brightness
			WRITE_BYTE(1);		// speed
		MESSAGE_END();

		CBaseEntity *pEntity=NULL;
		TraceResult tr;

		UTIL_ScreenShake( pev->origin, 120.0, 1000.0, 2.0, 4000 );
		while ((pEntity = UTIL_FindEntityInSphere( pEntity, pev->origin, 4000 )) != NULL){
			if (pEntity->IsPlayer() && pEntity->IsAlive())
			{
				UTIL_TraceLine(pev->origin+gpGlobals->v_up*2.5,pEntity->pev->origin+pEntity->pev->view_ofs,ignore_monsters,ignore_glass,ENT(pev),&tr);
				if (tr.flFraction == 1.0)
					UTIL_ScreenFade(pEntity,Vector(255,255,255),.5,.5,255,FFADE_IN);
			}
		}


		Killed(pev,0);
		return;
	}

	pev->nextthink= gpGlobals->time + 0.1;
}

void CComet::Killed(entvars_t *pevAttacker,int iGib){
	STOP_SOUND(ENT(pev), CHAN_VOICE, COMET_SOUND_SHOOT );
	
	CBaseEntity::Killed(pevAttacker,iGib);
}

LINK_ENTITY_TO_CLASS( proj_comet, CComet);

/* ---------- */
/* GiantPlant */
/* ---------- */

class CGiantPlant : public CBaseMonster{
public:
	void Spawn( void );
	void Precache( void );

	void Killed(entvars_t *pevAttacker, int iGib);
	void EXPORT TakeThis(CBaseEntity *pEntity);
	void EXPORT WatchOut();
	int TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType );
	int Classify(){return CLASS_PLAYER_ALLY;}

	float		m_flNextHealthGain;

	float		m_flNextDebugReport;
	bool		bDebugReport;

	short m_sExplodeSprite;

	Vector		vecToEnemy;
};

LINK_ENTITY_TO_CLASS( monster_giantplant, CGiantPlant );

void CGiantPlant::WatchOut(){
	StudioFrameAdvance();

	if( pev->sequence == 5 )
	{
		ResetSequenceInfo();
		pev->sequence = 0;
	}
	
	BOOL seeEnemy=FALSE;
	
	if (m_hEnemy != NULL && m_hEnemy->pev->takedamage && m_hEnemy->IsAlive() && FVisible( m_hEnemy )){
		Vector temp;
		
		m_vecEnemyLKP = Vector( m_hEnemy->BodyTarget( pev->origin ) );
		vecToEnemy = Vector(( m_vecEnemyLKP - pev->origin ).Normalize());

		temp=UTIL_VecToAngles(vecToEnemy);
		pev->angles.y=temp.y;
		pev->angles.z=temp.z;

		pev->velocity=vecToEnemy.Normalize()*GIANTPLANT_VELOCITY;
		if(pev->velocity.z>0)
			pev->velocity.z=0;

		if(pev->sequence==0){
			m_fSequenceFinished=1;
		}

		seeEnemy=TRUE;
	}

	else{//Find something else
		Look( 782 );
		m_hEnemy = BestVisibleEnemy( );
		if(m_hEnemy!=NULL && m_hEnemy->pev->takedamage && m_hEnemy->IsAlive()){
			m_fSequenceFinished=1;
			seeEnemy=TRUE;
		}

	}

	if(m_hEnemy!=NULL && (pev->origin-m_hEnemy->pev->origin).Length()<125 && pev->dmgtime <= gpGlobals->time){
		if(pev->owner!=NULL)
			m_hEnemy->TakeDamage(pev,VARS(pev->owner),600,DMG_ALWAYSGIB);
		else
			m_hEnemy->TakeDamage(pev,pev,600,DMG_ALWAYSGIB);

		pev->sequence=4;
		pev->frame=0;
		ResetSequenceInfo();

		pev->dmgtime = gpGlobals->time + 0.5;
	}

	//Change sequences if needed
	if(m_fSequenceFinished){
		if(seeEnemy){
			pev->sequence = 3;
		}
		else{
			pev->sequence = RANDOM_LONG(0,3);
		}

		pev->frame=0;
		ResetSequenceInfo();
	}

	//Gain some health
	if(m_flNextHealthGain <= gpGlobals->time && pev->health < pev->max_health)
	{
		TakeHealth(min(20,pev->max_health),DMG_GENERIC);
		m_flNextHealthGain = gpGlobals->time + GIANTPLANT_HEALTHTIME;
	}

	pev->nextthink = gpGlobals->time + 0.1;
}

void CGiantPlant::TakeThis(CBaseEntity *pEntity)
{
	if(IRelationship(pEntity) > R_NO)
	{
		m_hEnemy = pEntity;
	}
}

extern int gmsgDeathMsg;

void CGiantPlant::Killed(entvars_t *pevAttacker, int iGib)
{
	pev->owner = pev->euser1;

	if( (IRelationship(Instance(pevAttacker)) > R_NO) &&
		(FBitSet(pevAttacker->flags,FL_CLIENT))
	  )
	{
		pevAttacker->frags++;
	
		MESSAGE_BEGIN(MSG_ALL,gmsgDeathMsg);
			WRITE_BYTE(ENTINDEX(ENT(pevAttacker)));
			WRITE_BYTE(-1);
			WRITE_STRING("giantplant");
		MESSAGE_END();
	}

/*
	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY, pev->origin );
		WRITE_BYTE( TE_EXPLOSION);		
		WRITE_COORD( pev->origin.x );
		WRITE_COORD( pev->origin.y );
		WRITE_COORD( pev->origin.z+82);
		WRITE_SHORT( m_sExplodeSprite);
		WRITE_BYTE( 40  ); // scale * 10
		WRITE_BYTE( 15  ); // framerate
		WRITE_BYTE( TE_EXPLFLAG_NOSOUND );
	MESSAGE_END();
*/

	if(pev->owner!=NULL)
		ClientPrint(VARS(pev->owner),HUD_PRINTCENTER,"#GiantPlantSpell_LostPlant");

	CBaseMonster::Killed(pevAttacker,iGib);

	ResetSequenceInfo();

	SetThink( &CGiantPlant::SUB_Remove );

	pev->deadflag	= DEAD_DEAD;
	pev->takedamage	= DAMAGE_NO;
	pev->sequence	= 6;
	pev->nextthink	= gpGlobals->time + 3.0f;
}

void CGiantPlant :: Spawn( void )
{
	Precache( );

	SET_MODEL(ENT(pev), "models/combos/giantplant.mdl");

	pev->sequence=0; //Start small
	pev->frame = 0;
	pev->classname=MAKE_STRING("monster_giantplant");

	pev->movetype = MOVETYPE_STEP;
	pev->solid = SOLID_BBOX;
	pev->flags |= FL_MONSTER;
	pev->takedamage = DAMAGE_YES;
	pev->dmg = GIANTPLANT_DAMAGE;
	pev->max_health = GIANTPLANT_HEALTH;
	pev->health = pev->max_health;
	pev->gravity = 1.0;

	m_flNextHealthGain=0;
	m_flNextDebugReport= gpGlobals->time + 1;
	bDebugReport = false;
	m_flFieldOfView=-1;
	m_hEnemy = NULL;
/*
	if( pev->owner != NULL )
	{
		pev->team = VARS( pev->owner )->team;

		pev->euser1 = pev->owner;
		pev->owner  = NULL;
	}
*/
	UTIL_SetSize(pev, Vector( -50, -50, 0), Vector(50, 50, 90));

	pev->nextthink = gpGlobals->time + 0.1;
	UTIL_SetOrigin( pev, pev->origin );

	SetThink(&CGiantPlant::WatchOut);
	SetTouch(&CGiantPlant::TakeThis);

	ResetSequenceInfo();

	pev->nextthink = gpGlobals->time + 0.1;
}

void CGiantPlant :: Precache( void ){
	PRECACHE_MODEL("models/combos/giantplant.mdl");
	m_sExplodeSprite=PRECACHE_MODEL("sprites/blast.spr");

	PRECACHE_SOUND("zombie/claw_strike3.wav");
}

int CGiantPlant::TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType )
{
	if( IRelationship( Instance(pevAttacker) ) > R_NO )
	{
		m_hEnemy = Instance(pevAttacker);
	
		if( pev->sequence != 5 )
		{
			ResetSequenceInfo();
			pev->sequence = 5;
			pev->nextthink = gpGlobals->time + 0.3f;
		}
	}

	if(IRelationship(Instance(pevAttacker)) < R_NO)
		return CBaseMonster::TakeDamage(pevInflictor,pevAttacker,flDamage/10,bitsDamageType);
	else
		return CBaseMonster::TakeDamage(pevInflictor,pevAttacker,flDamage,bitsDamageType);
}

/* -------------- */
/* LightningCloud */
/* -------------- */

class CLightningCloud:public CBaseMonster{
public:
	void Spawn();
	void Precache();
	void EXPORT LightningCloudThink();
	void EXPORT LightningCloudTouch(CBaseEntity *pOther);
	void Killed(entvars_t *pevAttacker,int iGib);
	int Classify(){return CLASS_PLAYER_ALLY;}

	int m_maxFrame;
	int m_iBeam;
};

void CLightningCloud::Precache(){
	PRECACHE_MODEL(LIGHTNINGCLOUD_SPRITE);

	PRECACHE_SOUND("spells/combos/lightningcloud_zap.wav" );
}

void CLightningCloud::Spawn(){
	Precache();

	SET_MODEL(ENT(pev),LIGHTNINGCLOUD_SPRITE);

	pev->solid = SOLID_BBOX;
	pev->movetype=MOVETYPE_FLY;
	pev->body=0;
	pev->skin=0;
	pev->renderamt=255;
	pev->scale=2;
	pev->rendermode=kRenderTransAdd;
	pev->frame=0;

	m_flFieldOfView=-1;

	UTIL_MakeVectors(pev->angles);
	pev->velocity = gpGlobals->v_forward * CVAR_GET_FLOAT("sv_maxspeed")*LIGHTNINGCLOUD_VELOCITY; //LightningClouds velocity now depends on globals maxspeed

	m_maxFrame=(float)MODEL_FRAMES(pev->modelindex)-1;

	SetThink(&CLightningCloud::LightningCloudThink);
	SetTouch(&CLightningCloud::LightningCloudTouch);

	UTIL_SetSize(pev, Vector( 0, 0, 0), Vector(0, 0, 0));
	UTIL_SetOrigin( pev, pev->origin );

	pev->nextthink= gpGlobals->time + 0.1;
	m_flNextAttack= gpGlobals->time + 1.0;
}

void CLightningCloud::LightningCloudThink(){
	pev->frame++;

	if(pev->frame>m_maxFrame)
		pev->frame=0;
	
	if(m_flNextAttack<=gpGlobals->time){
		Look(LIGHTNINGCLOUD_RANGE);

		CBaseEntity *pEnt=BestVisibleEnemy();

		if(pEnt){
			MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, pev->origin );
				WRITE_BYTE( TE_BEAMENTS );
				WRITE_SHORT(entindex());
				WRITE_SHORT(pEnt->entindex());
				WRITE_SHORT(g_sModelIndexBubbles);
				WRITE_BYTE(0);		//starting frame
				WRITE_BYTE(1);		//framerate
				WRITE_BYTE(3);		//life
				WRITE_BYTE(20);		//width
				WRITE_BYTE(20);		//noise
				WRITE_BYTE(255);	//color
				WRITE_BYTE(0);
				WRITE_BYTE(0);
				WRITE_BYTE(255);	//bright
				WRITE_BYTE(1);		//scroll
			MESSAGE_END();

			TraceResult tr;

			UTIL_TraceLine(pev->origin,pEnt->pev->origin,dont_ignore_monsters,ENT(pev),&tr);

			ClearMultiDamage();

			if(pev->owner){
				pEnt->TraceAttack(VARS(pev->owner),LIGHTNINGCLOUD_DAMAGE,(pEnt->pev->origin-pev->origin),&tr,DMG_SHOCK);
				ApplyMultiDamage(pev,VARS(pev->owner));
			}
			else{
				pEnt->TraceAttack(pev,LIGHTNINGCLOUD_DAMAGE,(pEnt->pev->origin-pev->origin),&tr,DMG_SHOCK);
				ApplyMultiDamage(pev,pev);
			}

			EMIT_SOUND(ENT(pev),CHAN_VOICE,"spells/combos/lightningcloud_zap.wav",1,ATTN_NORM);
		}

		m_flNextAttack=gpGlobals->time+LIGHTNINGCLOUD_DELAY;
	}

	pev->nextthink=gpGlobals->time+.1;
}

void CLightningCloud::LightningCloudTouch(CBaseEntity *pOther){
	if(pev->owner)
		pOther->TakeDamage(pev,VARS(pev->owner),600,DMG_SHOCK);
	else
		pOther->TakeDamage(pev,pev,600,DMG_SHOCK);
	
	Killed(pev,0);
}

void CLightningCloud::Killed(entvars_t *pevAttacker,int iGib){
	CBaseEntity::Killed(pevAttacker,iGib);
}

LINK_ENTITY_TO_CLASS( proj_lightningcloud, CLightningCloud);

/* ------ */
/* Meteor */
/* ------ */

class CMeteor:public CBaseMonster{
public:
	void Spawn();
	void Precache();
	void EXPORT MeteorThink();
	void Killed(entvars_t *pevAttacker,int iGib);

	int m_iTrail;
	float m_flNextTrailTime;
};

void CMeteor::Precache(){
	PRECACHE_MODEL(METEOR_SPRITE);
	PRECACHE_SOUND(METEORSPELL_SOUND_HIT);
	PRECACHE_SOUND(METEORSPELL_SOUND_CAST);
	PRECACHE_SOUND(METEORSPELL_SOUND_MISS);
	m_iTrail = PRECACHE_MODEL("sprites/smoke.spr");
}

void CMeteor::Spawn(){
	Precache();

	pev->solid		= SOLID_BBOX;
	pev->movetype	= MOVETYPE_NOCLIP;
	pev->effects	|= EF_LIGHT;

	UTIL_MakeVectors(pev->angles);
	pev->velocity = gpGlobals->v_forward * CVAR_GET_FLOAT("sv_maxspeed")*METEOR_VELOCITY; //Meteors velocity now depends on globals maxspeed

	SET_MODEL(ENT(pev),METEOR_SPRITE);

	SetThink(&CMeteor::MeteorThink);

	UTIL_SetSize(pev, Vector( 0, 0, 0), Vector(0, 0, 0));
	UTIL_SetOrigin( pev, pev->origin );

	m_flNextTrailTime=gpGlobals->time+.1;
	pev->nextthink=gpGlobals->time+.1;
}

void CMeteor::MeteorThink()
{
	if(m_hEnemy == NULL)
	{
		Killed(pev, 0);
		return;
	}

	Vector vecToEnemy;

	m_vecEnemyLKP	= m_hEnemy->BodyTarget(pev->origin);
	vecToEnemy		= (m_vecEnemyLKP - pev->origin).Normalize();
	pev->velocity	= (vecToEnemy*pev->velocity.Length());
	pev->angles		= UTIL_VecToAngles(pev->velocity);
	
	if(m_flNextTrailTime <= gpGlobals->time)
	{
		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
			WRITE_BYTE( TE_BEAMFOLLOW );
			WRITE_SHORT(entindex() );	// entity
			WRITE_SHORT(m_iTrail );	// model
			WRITE_BYTE( 20 ); // life
			WRITE_BYTE( 30 );  // width
			WRITE_BYTE( 0 );   // r, g, b
			WRITE_BYTE( 0 );   // r, g, b
			WRITE_BYTE( 255 );   // r, g, b
			WRITE_BYTE( 255 );	// brightness
		MESSAGE_END();  // move PHS/PVS data sending into here (SEND_ALL, SEND_PVS, SEND_PHS)

		m_flNextTrailTime=gpGlobals->time+.3;
	}

	if((m_vecEnemyLKP-pev->origin).Length() < METEOR_EXPLODE_DISTANCE || !m_hEnemy->IsAlive())
	{
		if(pev->owner)
			::RadiusDamage(pev->origin,pev,VARS(pev->owner),METEOR_DAMAGE,METEOR_RADIUS,NULL,DMG_BURN|DMG_ALWAYSGIB|DMG_BLAST);
		else
			::RadiusDamage(pev->origin,pev,pev,METEOR_DAMAGE,METEOR_RADIUS,NULL,DMG_BURN|DMG_ALWAYSGIB|DMG_BLAST);
		
		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY, pev->origin );
			WRITE_BYTE( TE_EXPLOSION);
			WRITE_COORD( pev->origin.x );
			WRITE_COORD( pev->origin.y );
			WRITE_COORD( pev->origin.z);
			WRITE_SHORT( g_sModelIndexFireball );
			WRITE_BYTE( 40  ); // scale * 10
			WRITE_BYTE( 15  ); // framerate
			WRITE_BYTE( TE_EXPLFLAG_NONE );
		MESSAGE_END();

		EMIT_SOUND( ENT(pev), CHAN_STATIC, METEORSPELL_SOUND_HIT, 1, .1 );

		UTIL_ScreenShake( pev->origin, 120.0, 1000.0, 2.0, 1000 );

		Killed(pev,0);
		return;
	}

	pev->nextthink=gpGlobals->time+.1;
}

void CMeteor::Killed(entvars_t *pevAttacker,int iGib){
	CBaseEntity::Killed(pevAttacker,iGib);
}

LINK_ENTITY_TO_CLASS( proj_meteor, CMeteor);

/* --------- */
/* SpiritWiz */
/* --------- */

class CSpiritWiz:public CBaseMonster{
public:
	void Spawn();
	void Precache();
	void EXPORT SpiritWizThink();
	void EXPORT SpiritWizTouch(CBaseEntity *pOther);
	void Killed(entvars_t *pevAttacker,int iGib);
	int TakeDamage(entvars_t *pevInflictor,entvars_t *pevAttacker,float flDamage,int bitsDamageType);
	int Classify(){return CLASS_PLAYER_ALLY;}

	float m_flVelocity;
	float m_flNextMove;
	float m_flDie;
};

void CSpiritWiz::Precache(){
	PRECACHE_MODEL(SPIRITWIZ_MODEL);

	PRECACHE_SOUND(SPIRITWIZ_SOUND);
}

void CSpiritWiz::Spawn(){
	Precache();

	SET_MODEL(ENT(pev),SPIRITWIZ_MODEL);

	pev->solid = SOLID_BBOX;
	pev->movetype=MOVETYPE_FLY;
	pev->body=0;
	pev->skin=0;
	pev->renderamt=160;
	pev->rendermode=kRenderTransColor;
	pev->sequence=0;
	pev->health=SPIRITWIZ_HEALTH;
	pev->max_health=SPIRITWIZ_HEALTH;
	pev->flags |= FL_MONSTER;
	pev->takedamage = DAMAGE_YES;
	pev->angles.x=0;

	m_flFieldOfView=0;
	m_hEnemy=NULL;
	m_bloodColor=DONT_BLEED;

	UTIL_MakeVectors(pev->angles);
	pev->velocity = gpGlobals->v_forward * CVAR_GET_FLOAT("sv_maxspeed")*SPIRITWIZ_VELOCITY; //SpiritWizs velocity now depends on globals maxspeed
	m_flVelocity=pev->velocity.Length();

	SetThink(&CSpiritWiz::SpiritWizThink);
	SetTouch(&CSpiritWiz::SpiritWizTouch);

	UTIL_SetSize(pev, Vector( -18, -18, -30), Vector(18, 18, 30));
	UTIL_SetOrigin( pev, pev->origin );

	EMIT_SOUND(ENT(pev),CHAN_VOICE,SPIRITWIZ_SOUND,1,ATTN_NORM);

	m_flNextMove=gpGlobals->time+.5;
	m_flDie=gpGlobals->time+30;
	pev->nextthink=gpGlobals->time+.1;

	ResetSequenceInfo();
}

void CSpiritWiz::SpiritWizThink(){
	StudioFrameAdvance();

	if (gpGlobals->time >= m_flDie){
		Killed( pev, 0 );
		return;
	}

	Look(784);
	m_hEnemy=BestVisibleEnemy();

	if(m_hEnemy==NULL && m_flNextMove<=gpGlobals->time){
		if(pev->sequence!=0){
			pev->sequence=0;
			ResetSequenceInfo();
		}
		
		pev->velocity=pev->velocity.Normalize();
		pev->velocity.x+=RANDOM_FLOAT(-.75,.75);
		pev->velocity.y+=RANDOM_FLOAT(-.75,.75);
		pev->velocity.z=0;
		pev->velocity=pev->velocity.Normalize()*m_flVelocity*.25;

		m_flNextMove=gpGlobals->time+.5;
	}
	if(m_hEnemy!=NULL){
		if(pev->sequence!=1){
			pev->sequence=1;
			ResetSequenceInfo();
		}

		Vector vecToEnemy;

		m_vecEnemyLKP = m_hEnemy->BodyTarget( pev->origin );
		Vector v_temp=Vector(pev->origin);
		v_temp.z=v_temp.z+16;
		vecToEnemy = ( m_vecEnemyLKP - v_temp ).Normalize();

		pev->velocity=vecToEnemy*m_flVelocity;
		pev->angles = UTIL_VecToAngles(pev->velocity);
		pev->angles.x=0;

		UTIL_MakeVectors(pev->angles);

		TraceResult tr;
		UTIL_TraceLine(pev->origin,pev->origin+gpGlobals->v_forward*1000,dont_ignore_monsters,dont_ignore_glass,edict(),&tr);
	
		if(tr.pHit){
			CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);

			if(pEntity->pev->takedamage!=0){
				MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, pev->origin);
					WRITE_BYTE( TE_BEAMPOINTS);
					WRITE_COORD(pev->origin.x);
					WRITE_COORD(pev->origin.y);
					WRITE_COORD(pev->origin.z+30);
					WRITE_COORD(pEntity->pev->origin.x);
					WRITE_COORD(pEntity->pev->origin.y);
					WRITE_COORD(pEntity->pev->origin.z);
					WRITE_SHORT( g_sModelIndexBubbles ); // Beam sprite index. 
					WRITE_BYTE(0); // Starting frame 
					WRITE_BYTE(0); // Framerate 
					WRITE_BYTE(2); // How long the beam stays on. 
					WRITE_BYTE(10); // Width
					WRITE_BYTE(50); // Noise 
					WRITE_BYTE(255); 
					WRITE_BYTE(255); 
					WRITE_BYTE(255); 
					WRITE_BYTE(255); 
					WRITE_BYTE(0); // Speed, sort of.
				MESSAGE_END();

				if(pev->owner!=NULL)
					pEntity->TakeDamage(pev,VARS(pev->owner),SPIRITWIZ_LITDAMAGE,DMG_GENERIC);
				else
					pEntity->TakeDamage(pev,pev,SPIRITWIZ_LITDAMAGE,DMG_GENERIC);
			}
		}
	}

	MakeIdealYaw( pev->origin + pev->velocity );
	ChangeYaw( 15.0f );

	pev->nextthink = gpGlobals->time + 0.1f;
}

void CSpiritWiz::SpiritWizTouch(CBaseEntity *pOther){
	if(IRelationship(pOther) <= R_NO || !pOther->pev->takedamage || !pOther->IsAlive())
		return;

	if(pev->owner!=NULL)
		pOther->TakeDamage(pev,VARS(pev->owner), SPIRITWIZ_DAMAGE,DMG_NEVERGIB);
	else
		pOther->TakeDamage(pev, pev, SPIRITWIZ_DAMAGE, DMG_NEVERGIB);

	TakeDamage(pev, pev, 200, DMG_NEVERGIB);
}

void CSpiritWiz::Killed(entvars_t *pevAttacker,int iGib){
	STOP_SOUND(ENT(pev),CHAN_VOICE,SPIRITWIZ_SOUND);
	
	CBaseEntity::Killed(pevAttacker,iGib);
}

int CSpiritWiz::TakeDamage(entvars_t *pevInflictor,entvars_t *pevAttacker,float flDamage,int bitsDamageType){
	if(pevInflictor){
		Vector v=Vector(pevInflictor->origin-pev->origin);
		pev->angles = UTIL_VecToAngles(v);
		pev->angles.x=0;
	}

	return CBaseMonster::TakeDamage(pevInflictor,pevAttacker,flDamage,bitsDamageType);
}

LINK_ENTITY_TO_CLASS( monster_spiritwiz, CSpiritWiz);

/* ------- */
/* Tornado */
/* ------- */

class CTornado:public CBaseMonster{
public:
	void Spawn();
	void Precache();
	void EXPORT TornadoThink();
	void EXPORT TornadoTouch(CBaseEntity *pOther);
	int Classify(){return CLASS_PLAYER_ALLY;}

	float m_flVelocity;
	float m_flDie;
	float m_flNextSound;
};

void CTornado::Precache(){
	PRECACHE_MODEL(TORNADO_MODEL);

	PRECACHE_SOUND(TORNADOSPELL_SOUND_WIND);
	PRECACHE_SOUND(TORNADOSPELL_SOUND_CAST);
}

void CTornado::Spawn(){
	Precache();

	SET_MODEL(ENT(pev),TORNADO_MODEL);

	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_BOUNCE;
	pev->body=0;
	pev->skin=0;
	pev->renderamt = 160;
	pev->rendermode = kRenderTransColor;
	pev->flags |= FL_MONSTER;
	pev->takedamage = DAMAGE_NO;
	pev->angles.x = 0;
	pev->dmgtime = gpGlobals->time + TORNADO_SPIN;

	m_flNextSound = 0.0f;

	m_flFieldOfView = -1;
	m_hEnemy = NULL;

	UTIL_MakeVectors(pev->angles);
	pev->velocity = gpGlobals->v_forward * CVAR_GET_FLOAT("sv_maxspeed")*TORNADO_VELOCITY; //Tornados velocity now depends on globals maxspeed
	m_flVelocity=pev->velocity.Length();

	UTIL_SetSize(pev, Vector(0,0,0), Vector(0,0,0));
	UTIL_SetOrigin( pev, pev->origin );

	m_flDie=gpGlobals->time+TORNADO_LIFE;

	SetThink(&CTornado::TornadoThink);
	SetTouch(&CTornado::TornadoTouch);

	pev->sequence=0;
	ResetSequenceInfo();
	
	EMIT_SOUND_DYN(edict(),CHAN_VOICE,TORNADOSPELL_SOUND_WIND,1.0,ATTN_NORM,0,100);

	pev->nextthink=gpGlobals->time+.1;
}

void CTornado::TornadoThink()
{
	StudioFrameAdvance();

	if(pev->velocity.z>10)
		pev->velocity.z=10;

	if (gpGlobals->time >= m_flDie){
		STOP_SOUND(edict(),CHAN_VOICE,TORNADOSPELL_SOUND_WIND);
		UTIL_Remove(this);
		return;
	}

	Look(1024);
	m_hEnemy=BestVisibleEnemy();

	if(m_hEnemy){
		Vector vecToEnemy;

		m_vecEnemyLKP = m_hEnemy->BodyTarget( pev->origin );
		Vector v_temp=Vector(pev->origin);
		vecToEnemy = ( m_vecEnemyLKP - v_temp ).Normalize();

		pev->velocity.x=(vecToEnemy*m_flVelocity).x;
		pev->velocity.y=(vecToEnemy*m_flVelocity).y;
	}

	CBaseEntity *pEntity=NULL;
	Vector vec;

	while((pEntity=UTIL_FindEntityInSphere(pEntity,pev->origin,TORNADO_RADIUS)) != NULL)
	{
		if(pEntity->pev->takedamage && IRelationship(pEntity) > R_NO)
		{
			if(pev->owner)
				pEntity->TakeDamage(pev,VARS(pev->owner),TORNADO_DAMAGE,DMG_ALWAYSGIB);
			else
				pEntity->TakeDamage(pev,pev,TORNADO_DAMAGE,DMG_ALWAYSGIB);
		}

		if( (pEntity->pev->movetype != MOVETYPE_NONE) &&
			(pEntity->pev->takedamage) && 
			(IRelationship(pEntity) > R_NO) &&
			(pEntity->pev->velocity.z >- 200)
		  )
		{
			vec=pev->origin-pEntity->pev->origin;

			vec.z=0;
			pEntity->pev->velocity=vec;
			pEntity->pev->velocity.z=500/vec.Length()+pEntity->pev->velocity.z;
			pEntity->pev->velocity=pEntity->pev->velocity*3;
			
			if(pEntity->pev->velocity.z>1200)
				pEntity->pev->velocity.z=1200;
		}
	}

	if( gpGlobals->time > m_flNextSound )
	{
		EMIT_SOUND_DYN(edict(),CHAN_VOICE,TORNADOSPELL_SOUND_WIND,1.0,ATTN_NORM,0,100);
		m_flNextSound = gpGlobals->time + 1.0f;
	}

	pev->dmgtime = gpGlobals->time + TORNADO_SPIN;

	pev->nextthink = gpGlobals->time+.1;
}

void CTornado::TornadoTouch(CBaseEntity *pOther){
	pev->velocity.z = 10;
}

LINK_ENTITY_TO_CLASS( monster_tornado, CTornado);

/* ------ */
/* Wombat */
/* ------ */

class CWombat : public CGrenade
{
	void Spawn( void );
	void Precache( void );
	int  Classify( void ){return CLASS_PLAYER_ALLY;}
	void EXPORT SuperBounceTouch( CBaseEntity *pOther );
	void EXPORT HuntThink( void );
	int  BloodColor( void ) { return DONT_BLEED; }
	void Killed( entvars_t *pevAttacker, int iGib );
	int TakeDamage(entvars_t *pevInflictor,entvars_t *pevAttacker,float flDamage,int bitsDamageType);
	void GibMonster();

	virtual int		Save( CSave &save ); 
	virtual int		Restore( CRestore &restore );
	
	static	TYPEDESCRIPTION m_SaveData[];

	static float m_flNextBounceSoundTime;

	EHANDLE m_hTarget;
	float m_flDie;
	Vector m_vecTarget;
	float m_flNextHunt;
	float m_flNextHit;
	Vector m_posPrev;
	int g_sSacrificeSprite;
};

float CWombat::m_flNextBounceSoundTime = 0;

LINK_ENTITY_TO_CLASS( monster_wombat, CWombat );
TYPEDESCRIPTION	CWombat::m_SaveData[] = 
{
	DEFINE_FIELD( CWombat, m_flDie, FIELD_TIME ),
	DEFINE_FIELD( CWombat, m_vecTarget, FIELD_VECTOR ),
	DEFINE_FIELD( CWombat, m_flNextHunt, FIELD_TIME ),
	DEFINE_FIELD( CWombat, m_flNextHit, FIELD_TIME ),
	DEFINE_FIELD( CWombat, m_posPrev, FIELD_POSITION_VECTOR ),
};

IMPLEMENT_SAVERESTORE( CWombat, CGrenade );

#define SQUEEK_DETONATE_DELAY	15.0

void CWombat :: Spawn( void )
{
	Precache( );
	// motor
	pev->movetype = MOVETYPE_BOUNCE;
	pev->solid = SOLID_BBOX;

	SET_MODEL(ENT(pev),WOMBAT_MODEL);
	UTIL_SetSize(pev, Vector( -4, -4, 0), Vector(4, 4, 8));
	UTIL_SetOrigin( pev, pev->origin );

	SetTouch( &CWombat::SuperBounceTouch );
	SetThink( &CWombat::HuntThink );
	pev->nextthink = gpGlobals->time + 0.1;
	m_flNextHunt = gpGlobals->time + 1E6;

	pev->flags |= FL_MONSTER;
	pev->takedamage		= DAMAGE_YES;//DAMAGE_AIM;
	pev->health			= WOMBAT_HEALTH;
	pev->max_health		= WOMBAT_HEALTH;
	pev->gravity		= 0.5;
	pev->friction		= 0.5;

	pev->dmg = WOMBAT_DAMAGE;

	m_flDie = gpGlobals->time + WOMBAT_LIFE;

	m_flFieldOfView = 0; // 180 degrees

	m_flNextBounceSoundTime = gpGlobals->time;// reset each time a wombat is spawned.

	pev->sequence = WOMBAT_RUN;
	ResetSequenceInfo( );

	EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, "monster/wombat/wblaunch.wav", 1.0, ATTN_NORM, 0, 150);
}

void CWombat::Precache( void )
{
	PRECACHE_MODEL(WOMBAT_MODEL);
	PRECACHE_SOUND("common/bodysplat.wav");
	PRECACHE_SOUND("monster/wombat/wbattack1.wav");
	PRECACHE_SOUND("monster/wombat/wbattack2.wav");
	PRECACHE_SOUND("monster/wombat/wbattack3.wav");
	PRECACHE_SOUND("monster/wombat/wblaunch.wav");
	PRECACHE_SOUND("monster/wombat/wbstrength.wav");

	g_sSacrificeSprite = PRECACHE_MODEL( "sprites/blueflare1.spr" );

}

void CWombat :: Killed( entvars_t *pevAttacker, int iGib )
{
	pev->model = iStringNull;// make invisible
	SetThink( &CWombat::SUB_Remove );
	SetTouch( NULL );
	pev->nextthink = gpGlobals->time + 0.1;

	// since squeak grenades never leave a body behind, clear out their takedamage now.
	// Squeaks do a bit of radius damage when they pop, and that radius damage will
	// continue to call this function unless we acknowledge the Squeak's death now. (sjb)
	pev->takedamage = DAMAGE_NO;

	EMIT_SOUND(ENT(pev),CHAN_VOICE,"common/bodysplat.wav",1,ATTN_NORM);

	UTIL_BloodDrips( pev->origin, g_vecZero, BloodColor(), 80 );

	CBaseEntity::Killed( pevAttacker, GIB_ALWAYS );
}

void CWombat::GibMonster(){
	UTIL_BloodDrips(pev->origin,g_vecZero,BloodColor(),80);
}

int CWombat::TakeDamage(entvars_t *pevInflictor,entvars_t *pevAttacker,float flDamage,int bitsDamageType){
	if(CBaseMonster::IRelationship(Instance(pevAttacker)) > R_NO)
		m_hEnemy=Instance(pevAttacker);

	return CBaseMonster::TakeDamage(pevInflictor,pevAttacker,flDamage,bitsDamageType);
}

void CWombat::HuntThink( void )
{
	// ALERT( at_console, "think\n" );

	if (!IsInWorld())
	{
		SetTouch( NULL );
		UTIL_Remove( this );
		return;
	}

	if(m_flDie<=gpGlobals->time){
		Killed(pev,TRUE);
	}

	StudioFrameAdvance( );
	pev->nextthink = gpGlobals->time + 0.1;

	// float
	if (pev->waterlevel != 0)
	{
		if (pev->movetype == MOVETYPE_BOUNCE)
		{
			pev->movetype = MOVETYPE_FLY;
		}
		pev->velocity = pev->velocity * 0.9;
		pev->velocity.z += 8.0;
	}
	else if (pev->movetype == MOVETYPE_FLY)
	{
		pev->movetype = MOVETYPE_BOUNCE;
	}

	// return if not time to hunt
	if (m_flNextHunt > gpGlobals->time)
		return;

	m_flNextHunt = gpGlobals->time + 2.0;

//	CBaseEntity *pOther = NULL;
	Vector vecDir;
	TraceResult tr;

	Vector vecFlat = pev->velocity;
	vecFlat.z = 0;
	vecFlat = vecFlat.Normalize( );

	UTIL_MakeVectors( pev->angles );

	if (m_hEnemy == NULL || !m_hEnemy->IsAlive())
	{
		// find target, bounce a bit towards it.
		Look( 512 );
		m_hEnemy = BestVisibleEnemy( );
	}

	if (m_hEnemy != NULL)
	{
		if (FVisible( m_hEnemy ))
		{
			vecDir = m_hEnemy->EyePosition() - pev->origin;
			m_vecTarget = vecDir.Normalize( );
		}

		float flVel = pev->velocity.Length();
		float flAdj = 50.0 / (flVel + 10.0);

		if (flAdj > 1.2)
			flAdj = 1.2;
		
		// ALERT( at_console, "think : enemy\n");

		// ALERT( at_console, "%.0f %.2f %.2f %.2f\n", flVel, m_vecTarget.x, m_vecTarget.y, m_vecTarget.z );

		pev->velocity = pev->velocity * flAdj + m_vecTarget * WOMBAT_VELOCITY;
	}

	if (pev->flags & FL_ONGROUND)
	{
		pev->sequence=WOMBAT_RUN;
		ResetSequenceInfo();
	}
	else if(m_hEnemy!=NULL && (pev->origin-m_hEnemy->pev->origin).Length()<100){
		if( (IRelationship(m_hEnemy) == R_NM) &&
 			(pev->sequence != WOMBAT_ATTACK)
		  )
		{
			pev->sequence=WOMBAT_ATTACK;
			ResetSequenceInfo();
		}
// Codiac 1.2 B22 Wombat should attack more things 
//		else if( (IRelationship(m_hEnemy) == R_HT) &&
		else if( (IRelationship(m_hEnemy) >= R_DL) &&
			     (pev->sequence != WOMBAT_JUMP)
			   )
		{
			pev->sequence = WOMBAT_JUMP;
			ResetSequenceInfo();
		}
	}

	if ((pev->origin - m_posPrev).Length() < 1.0)
	{
		pev->velocity.x = RANDOM_FLOAT( -100, 100 );
		pev->velocity.y = RANDOM_FLOAT( -100, 100 );
	}
	m_posPrev = pev->origin;

	pev->angles = UTIL_VecToAngles( pev->velocity );
	pev->angles.z = 0;
	pev->angles.x = 0;
}


void CWombat::SuperBounceTouch( CBaseEntity *pOther ){
	TraceResult tr = UTIL_GetGlobalTrace( );

	// don't hit the guy that launched this grenade
	if ( pev->owner && pOther->edict() == pev->owner )
		return;

	pev->angles.x = 0;
	pev->angles.z = 0;

	// avoid bouncing too much
	if (m_flNextHit > gpGlobals->time)
		return;

	if( (pOther->pev->takedamage) &&
		(m_flNextAttack < gpGlobals->time) && 
		(IRelationship(pOther) == R_NM)
	  )
	{
		float flRndSound = RANDOM_FLOAT ( 0 , 1 );

		if ( flRndSound <= 0.33 )
			EMIT_SOUND(ENT(pev), CHAN_VOICE, "monster/wombat/wbattack1.wav", 1, ATTN_NORM);
		else if (flRndSound <= 0.66)
			EMIT_SOUND(ENT(pev), CHAN_VOICE, "monster/wombat/wbattack2.wav", 1, ATTN_NORM);
		else 
			EMIT_SOUND(ENT(pev), CHAN_VOICE, "monster/wombat/wbattack3.wav", 1, ATTN_NORM);

//		// make sure it's me who has touched them
//		if (tr.pHit == pOther->edict())
//		{
//			// and it's not another squeakgrenade
//			if (tr.pHit->v.modelindex != pev->modelindex)
//			{
				// ALERT( at_console, "hit enemy\n");
				ClearMultiDamage( );
				pOther->TraceAttack(pev, WOMBAT_DAMAGE, gpGlobals->v_forward, &tr, DMG_SLASH ); 
				if (pev->owner != NULL)
					ApplyMultiDamage( pev, VARS(pev->owner) );
				else
					ApplyMultiDamage( pev, pev );

				pev->dmg += WOMBAT_DAMAGE; // add more explosion damage

				// make bite sound
				m_flNextAttack = gpGlobals->time + 0.5;
//			}
//		}
//		else
//		{
			// ALERT( at_console, "been hit\n");
//		}
	}
	else if( (pOther->pev->takedamage) &&
		     (m_flNextAttack < gpGlobals->time) &&
			 (IRelationship(pOther) == R_HT)
		   )
	{
		EMIT_SOUND(ENT(pev), CHAN_VOICE, "monster/wombat/wbstrength.wav", 1, ATTN_NORM);

		pOther->TakeHealth(WOMBAT_HEAL,DMG_GENERIC);

		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY, pev->origin );
			WRITE_BYTE( TE_EXPLOSION);
			WRITE_COORD( pev->origin.x );
			WRITE_COORD( pev->origin.y );
			WRITE_COORD( pev->origin.z);
			WRITE_SHORT( g_sSacrificeSprite );
			WRITE_BYTE( 20 ); // scale * 10
			WRITE_BYTE( 1 ); // framerate
			WRITE_BYTE( TE_EXPLFLAG_NOSOUND );
		MESSAGE_END();
		
		if(pOther->IsPlayer())
			((CBasePlayer*)pOther)->m_bitsDamageType=0;
		m_hTarget=pOther;
		m_hEnemy=NULL;
	}
	else{
		pev->sequence=WOMBAT_RUN;
		ResetSequenceInfo();

		if (!(pev->flags & FL_ONGROUND)){
			// play bounce sound
			float flRndSound = RANDOM_FLOAT ( 0 , 1 );

			if ( flRndSound <= 0.33 )
				EMIT_SOUND(ENT(pev), CHAN_VOICE, "monster/wombat/wbattack1.wav", .5, ATTN_NORM);
			else if (flRndSound <= 0.66)
				EMIT_SOUND(ENT(pev), CHAN_VOICE, "monster/wombat/wbattack2.wav", .5, ATTN_NORM);
			else 
				EMIT_SOUND(ENT(pev), CHAN_VOICE, "monster/wombat/wbattack3.wav", .5, ATTN_NORM);
		}
	}

	m_flNextHit = gpGlobals->time + 0.1;
	m_flNextHunt = gpGlobals->time;

	// in multiplayer, we limit how often wombats can make their bounce sounds to prevent overflows.
	if ( gpGlobals->time < m_flNextBounceSoundTime ){
		// too soon!
		return;
	}

	m_flNextBounceSoundTime = gpGlobals->time + 0.5;// half second.
}

/* ------------ */
/* SpiritDragon */
/* ------------ */

class CSpiritDragon:public CBaseMonster{
public:
	void Spawn();
	void Precache();
	void EXPORT SpiritDragonThink();
	int Classify(){return CLASS_PLAYER_ALLY;}

	float m_flDie;
	unsigned short m_usDragonFire;
};

LINK_ENTITY_TO_CLASS( monster_spiritdragon,CSpiritDragon );

void CSpiritDragon::Precache(){
	PRECACHE_MODEL(SPIRITDRAGON_MODEL);
	PRECACHE_SOUND("spells/combos/spritdragon_cast.wav");
	UTIL_PrecacheOther("proj_dragonfire");

	m_usDragonFire=PRECACHE_EVENT(1,"events/monsters/dragonfire.sc");
}

void CSpiritDragon::Spawn(){
	Precache();

	SET_MODEL(ENT(pev),SPIRITDRAGON_MODEL);

	pev->solid = SOLID_NOT;
	pev->movetype=MOVETYPE_FLY;
	pev->body=0;
	pev->skin=0;
	pev->renderamt=190;
	pev->rendermode=kRenderTransColor;
	pev->flags |= FL_MONSTER;
	pev->takedamage = DAMAGE_NO;
	pev->angles.x=0;

	UTIL_SetSize(pev, Vector(0,0,0), Vector(0,0,0));
	UTIL_SetOrigin( pev, pev->origin );

	m_flDie = gpGlobals->time + SPIRITDRAGON_LIFE;

	SetThink(&CSpiritDragon::SpiritDragonThink);

	pev->sequence=0;
	ResetSequenceInfo();
	
	pev->nextthink = gpGlobals->time + SPIRITDRAGON_DELAY;
}

void CSpiritDragon::SpiritDragonThink(){
	StudioFrameAdvance();

	CBaseEntity *pOwner=Instance(pev->owner);

	if(!pOwner || !pOwner->IsAlive() || gpGlobals->time >= m_flDie)
	{
		UTIL_Remove(this);
		return;
	}

	pev->velocity=(pOwner->pev->origin+Vector(0,0,40)-pev->origin)*6;
	pev->angles = pOwner->pev->angles;

	if(pOwner->pev->button & IN_ATTACK){
		Vector vecAngles;
		Vector vecVelocity;
		Vector vecOrigin;

		vecAngles = pOwner->pev->v_angle;
		UTIL_MakeVectors(vecAngles);
		vecVelocity=gpGlobals->v_forward*1400;
		vecVelocity[0]+=RANDOM_FLOAT(-100,100);
		vecVelocity[1]+=RANDOM_FLOAT(-100,100);
		vecVelocity[2]+=RANDOM_FLOAT(-100,100);
		vecOrigin =	pev->origin	+ gpGlobals->v_forward * 50	+ gpGlobals->v_up *	40;

		CBaseEntity *pEnt=Create("proj_dragonfire",vecOrigin,vecAngles,pOwner->edict());
		pEnt->pev->velocity=vecVelocity;
		pEnt->pev->nextthink = gpGlobals->time + .3;
		pEnt->pev->dmg=80;

		vecAngles = UTIL_VecToAngles(vecVelocity);

		PLAYBACK_EVENT_FULL(0,edict(),m_usDragonFire,0,(float *)&vecOrigin,(float *)&vecAngles,0,0,0,0,1,0);

		pev->sequence=1;
	}
	else{
		pev->sequence=0;
	}

	pev->nextthink = gpGlobals->time + SPIRITDRAGON_DELAY;
}

#endif

LINK_ENTITY_TO_CLASS( weapon_combospell, CComboSpell);

#define LINK_ENTITY_TO_CLASS2(mapClassName,DLLClassName,type) \
	extern "C" EXPORT void mapClassName( entvars_t *pev ); \
	void mapClassName( entvars_t *pev ) { \
		pev->iuser1 = type; \
		GetClassPtr((DLLClassName *)pev); \
	}

LINK_ENTITY_TO_CLASS2( weapon_balllightningspell,	CComboSpell, WWCOMBO_BALLLIGHTNING );
LINK_ENTITY_TO_CLASS2( weapon_cometspell,			CComboSpell, WWCOMBO_COMET );
LINK_ENTITY_TO_CLASS2( weapon_giantplantspell,		CComboSpell, WWCOMBO_GIANTPLANT );
LINK_ENTITY_TO_CLASS2( weapon_lightningcloudspell,	CComboSpell, WWCOMBO_LIGHTNINGCLOUD );
LINK_ENTITY_TO_CLASS2( weapon_meteorspell,			CComboSpell, WWCOMBO_METEOR );
LINK_ENTITY_TO_CLASS2( weapon_skeletonspell,		CComboSpell, WWCOMBO_LIVINGDEAD );
LINK_ENTITY_TO_CLASS2( weapon_spiritwizspell,		CComboSpell, WWCOMBO_SPIRITWIZARD );
LINK_ENTITY_TO_CLASS2( weapon_tornadospell,			CComboSpell, WWCOMBO_TORNADO );
LINK_ENTITY_TO_CLASS2( weapon_wombatspell,			CComboSpell, WWCOMBO_WALLY );
LINK_ENTITY_TO_CLASS2( weapon_dragonspell,			CComboSpell, WWCOMBO_DRAGONSPIRIT );

void CComboSpell::Spawn()
{
	pev->classname = MAKE_STRING("weapon_combospell");
	Precache();

	m_iId = WEAPON_COMBOSPELL;
	SET_MODEL(ENT(pev), "models/w_spellbook.mdl");
	m_iDefaultAmmo=1;

	FallInit();// get ready to fall down.
//	m_iIdling = false;
}

void CComboSpell::Precache( void )
{
	PRECACHE_MODEL("models/combos/v_combospell.mdl");

	PrecacheBallLightning();
	PrecacheComet();
	PrecacheGiantPlant();
	PrecacheLightningCloud();
	PrecacheMeteor();
	PrecacheSkeleton();
	PrecacheSpiritWiz();
	PrecacheTornado();
	PrecacheWombat();
	PrecacheDragon();
}

int CComboSpell::GetItemInfo(ItemInfo *pInfo)
{
	pInfo->pszName = "weapon_combospell";
	pInfo->iMaxClip = WEAPON_NOCLIP;
	pInfo->iSlot = SLOT_COMBOSPELL;
	pInfo->iPosition = POS_COMBO;
	pInfo->iFlags = 0;
	pInfo->iId = m_iId = WEAPON_COMBOSPELL;
	pInfo->iWeight = WEIGHT_COMBO;
//	p->pszAmmo1 = "rockets";
//	p->iMaxAmmo1 = 1;
//	p->pszAmmo2 = "ARgrenades";
//	p->iMaxAmmo2 = 4;
	pInfo->pszAmmo1		= "mana";
	pInfo->iMaxAmmo1	= MAX_MANA;
	pInfo->pszAmmo2		= "satchels";
	pInfo->iMaxAmmo2	= MAX_SATCHELS;

	return 1;
}

BOOL CComboSpell::Deploy( )
{
	char *pText="";
	
	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + COMBOSPELL_DELAY;

	switch(pev->iuser1)
	{
	case WWCOMBO_BALLLIGHTNING:
		pText="#Have_BallLightningSpell";
		break;
	case WWCOMBO_COMET:
		pText="#Have_CometSpell";
		break;
	case WWCOMBO_GIANTPLANT:
		pText="#Have_GiantPlantSpell";
		break;
	case WWCOMBO_LIGHTNINGCLOUD:
		pText="#Have_LightningCloudSpell";
		break;
	case WWCOMBO_METEOR:
		pText="#Have_MeteorSpell";
		break;
	case WWCOMBO_LIVINGDEAD:
		pText="#Have_SkeletonSpell";
		break;
	case WWCOMBO_SPIRITWIZARD:
		pText="#Have_SpiritWizSpell";
		break;
	case WWCOMBO_TORNADO:
		pText="#Have_TornadoSpell";
		break;
	case WWCOMBO_WALLY:
		pText="#Have_WombatSpell";
		break;
	case WWCOMBO_DRAGONSPIRIT:
		pText="#Have_DragonSpell";
		break;
	default:
		ClientPrint(m_pPlayer->pev, HUD_PRINTCENTER, "Invalid Combo Spell!");
		return FALSE;
	}

	ClientPrint(m_pPlayer->pev,HUD_PRINTCENTER,pText);

// Codiac - base skin on type
	pev->skin = pev->iuser1;

//	return DefaultDeploy(GetHandModel(m_pPlayer->pev->playerclass),GetStaffModel(m_pPlayer->pev->playerclass),FirstPersonAnims[FPANIMS_MMIDLE],ThirdPersonAnims[TPANIMS_TWOHANDED]);
	return DefaultDeploy("models/combos/v_combospell.mdl" , (char *)m_pPlayer->m_pClass->GetWizardStaff(),COMBO_DRAW, "trip");
}

void CComboSpell::PrimaryAttack()
{
	int s = 0;
	
	if(m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] < COMBOSPELL_COST)
		return;

//BUGBUG - Need fire animations in v model
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);
//	SendWeaponAnim(FirstPersonAnims[FPANIMS_MMFIRE]);

	m_flNextPrimaryAttack= UTIL_WeaponTimeBase() + COMBOSPELL_DELAY; 

	UTIL_MakeVectors(m_pPlayer->pev->v_angle+m_pPlayer->pev->punchangle);

	switch(pev->iuser1)
	{
	case WWCOMBO_BALLLIGHTNING:
		s = FireBallLightning();
		break;
	case WWCOMBO_COMET:
		s = FireComet();
		break;
	case WWCOMBO_GIANTPLANT:
		s = FireGiantPlant();
		break;
	case WWCOMBO_LIGHTNINGCLOUD:
		s = FireLightningCloud();
		break;
	case WWCOMBO_METEOR:
		s = FireMeteor();
		break;
	case WWCOMBO_LIVINGDEAD:
		s = FireSkeleton();
		break;
	case WWCOMBO_SPIRITWIZARD:
		s = FireSpiritWiz();
		break;
	case WWCOMBO_TORNADO:
		s = FireTornado();
		break;
	case WWCOMBO_WALLY:
		s = FireWombat();
		break;
	case WWCOMBO_DRAGONSPIRIT:
		s = FireDragon();
		break;
	}

	if(s)
	{
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= COMBOSPELL_COST;
	}

// 1.2.9 One shot per combo!
//	if(m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] < COMBOSPELL_COST )
//	{
#ifndef CLIENT_DLL
		g_pGameRules->GetNextBestWeapon(m_pPlayer,this);
		m_pPlayer->RemovePlayerItem(this);
		m_pPlayer->pev->weapons		&= ~(1<<m_iId);// take item off hud
		m_pPlayer->pev->renderfx	= kRenderFxNone;
		m_pPlayer->pev->rendercolor	= Vector(0,0,0);
//		Drop();
#endif
//	}

	// Codiac 1.2 B11 - Use looping idle sequence
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.5;
//	m_iIdling = false;
}

int CComboSpell::iItemSlot( void ) { return SLOT_COMBOSPELL+1; }

void CComboSpell::WeaponIdle( void )
{
	static int iAnim = COMBO_IDLE2;

	ResetEmptySound( );

	m_pPlayer->GetAutoaimVector( AUTOAIM_10DEGREES );

	if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;

	if(iAnim != COMBO_IDLE1)
	{
		iAnim = COMBO_IDLE1;
	}
	else
	{
		iAnim = RANDOM_LONG(COMBO_IDLE2, COMBO_IDLE3);
	}

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 2.0;

	SendWeaponAnim(iAnim);
}


void CComboSpell::PrecacheDragon()
{
	UTIL_PrecacheOther("monster_spiritdragon");
}

int CComboSpell::FireDragon()
{
#ifndef CLIENT_DLL
	CBaseEntity *dragon = (CBaseEntity*)(m_pPlayer->m_pClass->GetSpiritDragon());
	if(dragon)
	{
		dragon->SetThink(NULL);
		UTIL_Remove(dragon);
	}

	CBaseEntity *pDragon = CBaseEntity::Create("monster_spiritdragon", m_pPlayer->pev->origin + Vector(0, 0, 25), m_pPlayer->pev->v_angle, m_pPlayer->edict());

	m_pPlayer->m_pClass->SetSpiritDragon(pDragon);
#endif
	EMIT_SOUND( m_pPlayer->edict(), CHAN_WEAPON, "spells/combos/spritdragon_cast.wav", 1.0f, ATTN_NORM );

	return 1;
}


void CComboSpell::PrecacheBallLightning(){
	PRECACHE_MODEL("models/w_spellbook.mdl");

	PRECACHE_SOUND (BALLLIGHTNINGSPELL_SOUND_SHOOT);

	UTIL_PrecacheOther("monster_balllightning");
}

int CComboSpell::FireBallLightning()
{
#ifndef CLIENT_DLL
	Vector AimingDir=gpGlobals->v_forward;
	Vector GunPosition=m_pPlayer->GetGunPosition();
	Vector EndPoint=GunPosition+AimingDir*8192;

	TraceResult TResult;
	edict_t* EntityToIgnore;
	EntityToIgnore = ENT( m_pPlayer->pev );

	UTIL_TraceLine( GunPosition, EndPoint, dont_ignore_monsters, EntityToIgnore, &TResult );
	if( TResult.fAllSolid ) return 1;

	CBaseEntity* Victim = CBaseEntity :: Instance( TResult.pHit );
	if( ( Victim != NULL) && ( Victim->pev->takedamage ))
	{ 
		ClearMultiDamage(); 
		Victim->TraceAttack( m_pPlayer->pev, // Person who shot Victim 
	                 BALLLIGHTNINGSPELL_DAMAGE, // Amount of damage 
                     AimingDir, 
                     &TResult, 
                     DMG_SHOCK ); // Damage type. 
		ApplyMultiDamage(pev,m_pPlayer->pev);
	}

	for(int i=0;i<4;i++){
		// Draw the Lightning
		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY, TResult.vecEndPos );
			WRITE_BYTE( TE_BEAMPOINTS ); 
			WRITE_COORD( TResult.vecEndPos.x ); 
			WRITE_COORD( TResult.vecEndPos.y ); 
			WRITE_COORD( TResult.vecEndPos.z ); 
			WRITE_COORD( TResult.vecEndPos.x + RANDOM_FLOAT(-1024,1024) ); 
			WRITE_COORD( TResult.vecEndPos.y + RANDOM_FLOAT(-1024,1024) ); 
			WRITE_COORD( TResult.vecEndPos.z + 1024 );
			WRITE_SHORT( g_sModelIndexBubbles ); // Beam sprite index. 
			WRITE_BYTE( 0 ); // Starting frame 
			WRITE_BYTE( 0 ); // Framerate 
			WRITE_BYTE( BALLLIGHTNINGSPELL_BOLT_LIFE ); // How long the beam stays on. 
			WRITE_BYTE( BALLLIGHTNINGSPELL_BOLT_WIDTH ); 
			WRITE_BYTE( BALLLIGHTNINGSPELL_BOLT_NOISE ); // Noise 
			WRITE_BYTE( (int)RANDOM_FLOAT(0,256) ); 
			WRITE_BYTE( 0 ); 
			WRITE_BYTE( (int)RANDOM_FLOAT(0,256) ); 
			WRITE_BYTE( BALLLIGHTNINGSPELL_BOLT_BRIGHTNESS ); 
			WRITE_BYTE( 0 ); // Speed, sort of.
		MESSAGE_END( );
	}

	// Draw the Explosion
	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY, pev->origin );
		WRITE_BYTE( TE_EXPLOSION);
		WRITE_COORD( TResult.vecEndPos.x );
		WRITE_COORD( TResult.vecEndPos.y );
		WRITE_COORD( TResult.vecEndPos.z );
		WRITE_SHORT( g_sModelIndexFireball );
		WRITE_BYTE( 35 ); // m_iTeam * 10
		WRITE_BYTE( 12  ); // framerate
		WRITE_BYTE( TE_EXPLFLAG_NOSOUND );
	MESSAGE_END();

	EMIT_SOUND( ENT(m_pPlayer->pev),CHAN_WEAPON,BALLLIGHTNINGSPELL_SOUND_SHOOT,1,ATTN_NORM);

	CBaseEntity *pEnt	= CBaseEntity::Create("monster_balllightning",TResult.vecEndPos,Vector(0,0,0),ENT(pev->owner));
	pEnt->pev->team		= m_pPlayer->pev->team;

	UTIL_ScreenShake( pEnt->pev->origin, 120.0, 1000.0, 2.0, 1000 );
#endif
	return 1;
}


void CComboSpell::PrecacheComet(){
	PRECACHE_SOUND("ambience/hawk1.wav");
	
	UTIL_PrecacheOther("proj_comet");
}

int CComboSpell::FireComet()
{
#ifndef CLIENT_DLL
	float tmp;
	CBaseEntity *pEnt;
	
	tmp=m_pPlayer->m_flFieldOfView;
	m_pPlayer->m_flFieldOfView=.25;
	m_pPlayer->Look(1024);
	m_pPlayer->m_flFieldOfView=tmp;

	pEnt = m_pPlayer->BestVisibleEnemy( );

	if(pEnt == NULL || m_pPlayer->IRelationship(pEnt) < R_NO)
	{
		EMIT_SOUND( ENT(m_pPlayer->pev), CHAN_VOICE, COMET_SOUND_CAST_MISS,1,ATTN_NORM);
	}
	else
	{
		Vector temp;
		CComet *met;

		temp=pEnt->pev->origin;
		temp.z=temp.z+1024;
		met=(CComet*)CBaseEntity::Create("proj_comet",temp,temp,pev->owner);
		met->m_hEnemy=pEnt;
		met->pev->team=m_pPlayer->pev->team;
		EMIT_SOUND( ENT(m_pPlayer->pev), CHAN_VOICE, COMET_SOUND_CAST,1,ATTN_NORM);
	}
#endif

	return 1;
}


void CComboSpell::PrecacheGiantPlant()
{
	UTIL_PrecacheOther("monster_giantplant");

	PRECACHE_SOUND(GIANTPLANTSPELL_SOUND_CAST);
}

int CComboSpell::FireGiantPlant()
{
#ifndef CLIENT_DLL
	Vector temp;
	Vector m_bushPos;
	TraceResult TResult; 

	UTIL_MakeVectors( m_pPlayer->pev->v_angle );
		
	Vector v_for=gpGlobals->v_forward;
	v_for.z=0;v_for.Normalize();
	m_bushPos = m_pPlayer->GetGunPosition()+v_for*120;

	//Find the ground beneath the Bush
	temp.x=m_bushPos.x;
	temp.y=m_bushPos.y;
	temp.z=m_bushPos.z;
	temp.z=temp.z-96;

	UTIL_TraceLine( m_bushPos, temp, dont_ignore_monsters, ENT(m_pPlayer->pev), &TResult );

	if(TResult.vecEndPos.z==temp.z){
		ClientPrint(m_pPlayer->pev,HUD_PRINTCENTER,"#OutOfRoom");
		return 0;
	}
	if(StalkRoom(TResult.vecEndPos)==2){
		ClientPrint(m_pPlayer->pev,HUD_PRINTCENTER,"#OutOfRoom");
		return 0;
	}

	temp = m_pPlayer->pev->angles;
	temp.y=0;

	CBaseEntity *pEnt=CBaseEntity::Create("monster_giantplant",TResult.vecEndPos,temp,ENT(pev->owner));	
	pEnt->pev->team=m_pPlayer->pev->team;
#endif
	EMIT_SOUND(ENT(m_pPlayer->pev),CHAN_WEAPON,GIANTPLANTSPELL_SOUND_CAST,1,ATTN_NORM);

#ifndef CLIENT_DLL
	//Remove any other Giant Plants
	CBaseEntity *giantPlant = (CBaseEntity*)(m_pPlayer->m_pClass->GetGiantPlant());

	if(giantPlant != NULL){
		giantPlant->Killed(m_pPlayer->pev, 1000);
	}

	m_pPlayer->m_pClass->SetGiantPlant(pEnt);
#endif

	return 1;
}


void CComboSpell::PrecacheLightningCloud(){
	UTIL_PrecacheOther("proj_lightningcloud");
}

int CComboSpell::FireLightningCloud(){
#ifndef CLIENT_DLL
	UTIL_MakeVectors( m_pPlayer->pev->v_angle );
	
	Vector v_for=gpGlobals->v_forward;
	Vector v_temp=m_pPlayer->pev->origin;

	v_temp.z=v_temp.z+24;
	v_for=v_temp+v_for*48;

	Vector v_ang=v_for-v_temp;
	v_ang.z=-v_ang.z;
	v_ang=UTIL_VecToAngles(v_ang);

	CBaseEntity *pEnt;

	pEnt=CBaseEntity::Create("proj_lightningcloud",v_for,v_ang,pev->owner);
	pEnt->pev->team=m_pPlayer->pev->team;
#endif
	EMIT_SOUND(ENT(m_pPlayer->pev),CHAN_WEAPON,"spells/combos/lightningcloud.wav",1,ATTN_NORM);

	return 1;
}


int CComboSpell::FireMeteor(){
#ifndef CLIENT_DLL
	int tmp;
	CBaseEntity *pEnt;
	
	tmp = m_pPlayer->m_flFieldOfView;
	m_pPlayer->m_flFieldOfView = .25;
	m_pPlayer->Look(2048);
	m_pPlayer->m_flFieldOfView = tmp;

	pEnt = m_pPlayer->BestVisibleEnemy();

	if(pEnt == NULL || m_pPlayer->IRelationship(pEnt) < R_NO)
	{
		//OUT:FriendlyCombos?
		//pEnt=m_pPlayer;
		EMIT_SOUND( ENT(m_pPlayer->pev), CHAN_VOICE, METEORSPELL_SOUND_MISS, 1, ATTN_NORM);
	}
	else
	{
		Vector temp;
		CMeteor *met;
		float x,y,z;

		x		= RANDOM_LONG(-1,1);
		y		= RANDOM_LONG(-1,1);
		z		= RANDOM_LONG(0,1);
		temp	= Vector(x,y,z).Normalize();
		temp	= temp * 2048 + pEnt->pev->origin;
		met		= (CMeteor*)CBaseEntity::Create("proj_meteor", temp, temp, pev->owner);
		met->m_hEnemy = pEnt;
		met->pev->team = m_pPlayer->pev->team;

		x		= RANDOM_LONG(-1,1);
		y		= RANDOM_LONG(-1,1);
		z		= RANDOM_LONG(0,1);
		temp	= Vector(x,y,z).Normalize();
		temp	= temp*2048+pEnt->pev->origin;
		met		= (CMeteor*)CBaseEntity::Create("proj_meteor",temp,temp,pev->owner);
		met->m_hEnemy	= pEnt;
		met->pev->team	= m_pPlayer->pev->team;

		x		= RANDOM_LONG(-1,1);
		y		= RANDOM_LONG(-1,1);
		z		= RANDOM_LONG(0,1);
		temp	= Vector(x,y,z).Normalize();
		temp	= temp*2048+pEnt->pev->origin;
		met		= (CMeteor*)CBaseEntity::Create("proj_meteor",temp,temp,pev->owner);
		met->m_hEnemy	= pEnt;
		met->pev->team	= m_pPlayer->pev->team;
		EMIT_SOUND( ENT(m_pPlayer->pev), CHAN_VOICE, METEORSPELL_SOUND_CAST,1,ATTN_NORM);
	}
#endif

	return 1;
}

void CComboSpell::PrecacheMeteor(){
	UTIL_PrecacheOther("proj_meteor");
}


/* -------- */
/* Skeleton */
/* -------- */

void CComboSpell::PrecacheSkeleton(){
	UTIL_PrecacheOther("monster_skeleton");
	PRECACHE_SOUND(SKELETONSPELL_SOUND_CAST);
}

int CComboSpell::FireSkeleton()
{
#ifndef CLIENT_DLL
	Vector v_for = Vector(m_pPlayer->pev->angles);
	UTIL_MakeVectors(v_for);
	v_for = Vector(gpGlobals->v_forward);
	Vector v_temp = Vector(m_pPlayer->pev->origin);

	v_temp.z = v_temp.z + 24;
	v_for = v_temp + v_for * 56;

	Vector v_ang = Vector(v_for-v_temp);
	v_ang.z = -v_ang.z;
	v_ang = UTIL_VecToAngles(v_ang);

	CBaseEntity *pBody = NULL;
	int success = 0;
	TraceResult tr;

	while((pBody=UTIL_FindEntityInSphere(pBody, m_pPlayer->pev->origin, SKELETONSPELL_RADIUS))!=NULL)
	{
		if( !pBody )
			break;

		if( pBody->edict() == pev->owner )
			continue;

		if( FBitSet( pBody->pev->effects, EF_NODRAW ) )
			continue;

		if(m_pPlayer->IRelationship(pBody) > R_NO && pBody->IsPlayer() )
		{
			UTIL_TraceLine(m_pPlayer->pev->origin, pBody->pev->origin, dont_ignore_monsters, m_pPlayer->edict(), &tr);

			if(tr.flFraction==1.0 || tr.pHit == pBody->edict())
			{
				pBody->TakeDamage(pev, m_pPlayer->pev, 1000, DMG_CRUSH);
				
				MakeSkeleton(pBody->pev->origin);

				success = 1;
			}
		}
		else if(FClassnameIs(pBody->pev, "bodyque" ) )
		{
			if(FClassnameIs(pBody->pev,"bodyque") && UTIL_FindEntityGeneric("player",pBody->pev->origin,50))
			{
				ClientPrint(m_pPlayer->pev,HUD_PRINTCENTER,"#SkeletonSpell_TooClose");
				return 0;
			}

			pBody->pev->effects |= EF_NODRAW;

			MakeSkeleton(pBody->pev->origin);

			success = 1;
		}
	}
	if(success == 0)
	{
		ClientPrint(m_pPlayer->pev, HUD_PRINTCENTER, "#DeathWiz_NoBody");

		return 0;
	}
	else
	{
		MESSAGE_BEGIN(MSG_BROADCAST,SVC_TEMPENTITY,m_pPlayer->pev->origin);
			WRITE_BYTE(TE_BEAMTORUS);
			WRITE_COORD(m_pPlayer->pev->origin.x);
			WRITE_COORD(m_pPlayer->pev->origin.y);
			WRITE_COORD(m_pPlayer->pev->origin.z);
			WRITE_COORD(m_pPlayer->pev->origin.x);
			WRITE_COORD(m_pPlayer->pev->origin.y);
			WRITE_COORD(m_pPlayer->pev->origin.z + 300);
	 		WRITE_SHORT(g_sModelIndexSmoke);
			WRITE_BYTE(0); // startframe
			WRITE_BYTE(0); // framerate
			WRITE_BYTE(5); // life
			WRITE_BYTE(20);  // width
			WRITE_BYTE(20);   // noise
			WRITE_BYTE(255);   // r, g, b
			WRITE_BYTE(0);   // r, g, b
			WRITE_BYTE(0);   // r, g, b
			WRITE_BYTE(255);	// brightness
			WRITE_BYTE(1);		// speed
		MESSAGE_END();

		EMIT_SOUND(ENT(m_pPlayer->pev),CHAN_WEAPON,SKELETONSPELL_SOUND_CAST,1,ATTN_NORM);
	}
#endif
	return 1;
}

void CComboSpell::MakeSkeleton(Vector vOrigin)
{
/* Who the *&%&*% writes this shit and doesn't comment it?
	vOrigin.z += 48;
	int offset = 20;
	int t;
	TraceResult tr;
	Vector v1;

	v1 = vOrigin;
	v1.x += offset;
	UTIL_TraceLine(vOrigin, v1, dont_ignore_monsters, edict(), &tr);
	t=offset+vOrigin.x - tr.vecEndPos.x;
	vOrigin.x = vOrigin.x - t;

	v1 = vOrigin;
	v1.x -= offset;
	UTIL_TraceLine(vOrigin, v1, dont_ignore_monsters, edict(), &tr);
	t = offset-vOrigin.x + tr.vecEndPos.x;
	vOrigin.x = vOrigin.x + t;

	v1 = vOrigin;
	v1.y += offset;
	UTIL_TraceLine(vOrigin, v1, dont_ignore_monsters, edict(), &tr);
	t = offset + vOrigin.y - tr.vecEndPos.y;
	vOrigin.y = vOrigin.y - t;

	v1 = vOrigin;
	v1.y -= offset;
	UTIL_TraceLine(vOrigin,v1,dont_ignore_monsters,edict(),&tr);
	t = offset - vOrigin.y + tr.vecEndPos.y;
	vOrigin.y = vOrigin.y + t;

	v1 = vOrigin;
	v1.z += offset;
	UTIL_TraceLine(vOrigin,v1,dont_ignore_monsters,edict(),&tr);
	t = offset + vOrigin.z-tr.vecEndPos.z;
	vOrigin.z = vOrigin.z-t;
*/

	CBaseMonster *pEnt	= (CBaseMonster*)CBaseMonster::Create("monster_skeleton", vOrigin, Vector(0, 0, 0), ENT(pev->owner));
	pEnt->pev->team		= m_pPlayer->pev->team;
	pEnt->pev->euser1	= pev->owner;
}

void CComboSpell::PrecacheSpiritWiz(){
	UTIL_PrecacheOther("monster_spiritwiz");
	PRECACHE_SOUND(SPIRITWIZ_SOUND_CAST);
}

int CComboSpell::FireSpiritWiz(){
#ifndef CLIENT_DLL
// wtf is this...
	/*
	Vector v_for=Vector(m_pPlayer->pev->angles);
	UTIL_MakeVectors(v_for);
	v_for=Vector(gpGlobals->v_forward);
	Vector v_temp=Vector(m_pPlayer->pev->origin);

	v_temp.z=v_temp.z+24;
	v_for=v_temp+v_for*56;

	Vector v_ang=Vector(v_for-v_temp);
	v_ang.z=-v_ang.z;
	v_ang=UTIL_VecToAngles(v_ang);
*/

	CBaseEntity *pEnt=CBaseEntity::Create("monster_spiritwiz", m_pPlayer->pev->origin + ((gpGlobals->v_forward * 56) + (gpGlobals->v_up * 24)), m_pPlayer->pev->v_angle,ENT(pev->owner));
	pEnt->pev->team=m_pPlayer->pev->team;

	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, SPIRITWIZ_SOUND_CAST, 1, ATTN_NORM);
#endif
	return 1;
}


void CComboSpell::PrecacheTornado(){
	UTIL_PrecacheOther("monster_tornado");
}

int CComboSpell::FireTornado(){
#ifndef CLIENT_DLL
	Vector AimingDir = gpGlobals->v_forward;
	Vector vTemp=m_pPlayer->pev->origin+gpGlobals->v_forward*100;

	CBaseEntity *pEnt=CBaseEntity::Create("monster_tornado",vTemp,Vector(0,0,0),ENT(pev->owner));
	pEnt->pev->team=m_pPlayer->pev->team;

	EMIT_SOUND_DYN(edict(),CHAN_VOICE,TORNADOSPELL_SOUND_CAST,1.0,ATTN_NORM,0,100);
#endif
	return 1;
}

void CComboSpell::PrecacheWombat(){
	UTIL_PrecacheOther("monster_wombat");
}

int CComboSpell::FireWombat(){
#ifndef CLIENT_DLL
	UTIL_MakeVectors( m_pPlayer->pev->v_angle );
	TraceResult tr;
	Vector trace_origin;

	trace_origin = m_pPlayer->pev->origin;
	if ( m_pPlayer->pev->flags & FL_DUCKING ){
		trace_origin = trace_origin - ( VEC_HULL_MIN - VEC_DUCK_HULL_MIN );
	}

	// find place to toss monster
	UTIL_TraceLine( trace_origin + gpGlobals->v_forward * 20, trace_origin + gpGlobals->v_forward * 64, dont_ignore_monsters, NULL, &tr );

	if (tr.fAllSolid == 0 && tr.fStartSolid == 0 && tr.flFraction > 0.25){
		CBaseEntity *pWombat = CBaseEntity::Create( "monster_wombat", tr.vecEndPos, m_pPlayer->pev->v_angle, m_pPlayer->edict() );

		pWombat->pev->velocity = gpGlobals->v_forward * WOMBAT_VELOCITY + m_pPlayer->pev->velocity;
		pWombat->pev->team = m_pPlayer->pev->team;
	}
#endif
	return 1;
}

int CComboSpell::AddToPlayer(CBasePlayer *pPlayer)
{
#ifndef CLIENT_DLL
	pPlayer->pev->renderfx = kRenderFxGlowShell;
	pPlayer->pev->rendercolor=Vector(255,0,0);
	return CBasePlayerWeapon::AddToPlayer(pPlayer);
#else
	return FALSE;
#endif
}

void CComboSpell::Drop(){
#ifndef CLIENT_DLL
	m_pPlayer->pev->renderfx=kRenderFxNone;
	m_pPlayer->pev->rendercolor=Vector(0,0,0);
	CBasePlayerWeapon::Drop();
#endif
}
