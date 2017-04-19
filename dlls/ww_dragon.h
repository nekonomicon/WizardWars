#include "monsters.h"

class WWDragon : public CBaseMonster{
public:
	void KeyValue(KeyValueData *pkvd);
	void Spawn( void );
	void Precache( void );
	void SetYawSpeed( void );
//	int  Classify ( void );
	void HandleAnimEvent( MonsterEvent_t *pEvent );
	int ShouldFadeOnDeath();
	int IRelationship(CBaseEntity *pTarget);
//	int CheckRangeAttack1(float flDot,float flDist);
	void Killed(entvars_t *pevAttacker, int iGib);
	Schedule_t *GetScheduleOfType(int type);
	void EXPORT MonsterThink();
	void Mount(CBasePlayer *pPlayer);
	void DeMount();
	void UpdateRider();
	void ShootFire();
	int HasHumanGibs(){return FALSE;}
	int HasAlienGibs(){return TRUE;}
	int LookupActivity(int activity);
	void Use(CBaseEntity *pActivator,CBaseEntity *pCaller,USE_TYPE useType,float value);
	int ObjectCaps(){return(CBaseMonster::ObjectCaps()|FCAP_IMPULSE_USE);}

	int TakeDamage( entvars_t * pevInflictor, entvars_t * pevAttacker, float flDamage, int bitsDamageType );

	float m_flClientHealth;
	float m_flClientFlame;
	float m_flRechargeTime;
	float m_flUpdateTime;
	float m_flFireTime;
	EHANDLE m_hRider;
	unsigned short m_usDragonFire;
	unsigned short m_usDragonFlap;
	edict_t *m_pOwner;

	CUSTOM_SCHEDULES;

	float	m_flNextIdleSound;
};
