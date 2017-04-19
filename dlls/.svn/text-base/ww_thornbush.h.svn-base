#ifndef __WW_THORNBUSH_H__
#define __WW_THORNBUSH_H__


typedef struct thornbush_stats_s
{
	float	health;
	float	damage;
	float	touch;
	float	hunt;
	float	delay;
	float	search;
	float	regen;
	char *	model;

} thornbush_stats_t;


class WWThornBush : public CBaseMonster
{
public:
	virtual const char * Name( void ) { return "ThornBush"; };

	void			Spawn		( void );
	void			Precache	( void );

	int				BloodColor	( void ) { return BLOOD_COLOR_GREEN; };
	BOOL			CanRepel	( CBaseEntity * pAttacker ) { return FALSE; };

	void EXPORT		GrowUpThink	( void );
	void EXPORT		GrowDnThink	( void );

	void EXPORT		HuntTouch	( CBaseEntity * pOther );
	void EXPORT		HuntThink	( void );
	
	void			FindEnemy	( void );
	BOOL			EnemyBetter	( CBaseEntity * pOther );

	void			Fire		( void );
	void			FireSpore	( void );

	int				TakeDamage	( entvars_t * pInf, entvars_t * pAtk, float flDamage, int bitsDamageType );
	void			Killed		( entvars_t * pevAttacker, int iGib );

	BOOL			Enchant		( int iClass );
	BOOL			Fertilize	( void );
	BOOL			Harvest		( void );

	static WWThornBush * FindBush( CBasePlayer * pPlayer, float flDist, BOOL bOwn );

	int				GetLevel	( void ) { return m_iLevel; };
	int				GetEnchant	( void ) { return m_iEnchant; };

private:
	EHANDLE			m_hTarget;

	float			m_flNextHunt;
	float			m_flNextTouch;
	float			m_flNextAttack;
	float			m_flNextSpore;
	float			m_flNextRegen;

	int				m_iLevel;
	int				m_iEnchant;
	int				m_iDamageBits;

	unsigned short	m_usEvent;
};


enum
{
	THORNBUSH_EVENT_SPAWN,
	THORNBUSH_EVENT_FIRE,
	THORNBUSH_EVENT_FIRESPORE,
	THORNBUSH_EVENT_KILLED,
	THORNBUSH_EVENT_ENCHANT,
	THORNBUSH_EVENT_FERTILIZE,
	THORNBUSH_EVENT_HARVEST,
};


enum
{
	THORNBUSH_ANIM_GROW,
	THORNBUSH_ANIM_IDLE1,
	THORNBUSH_ANIM_IDLE2,
	THORNBUSH_ANIM_IDLE3,
	THORNBUSH_ANIM_IDLE4,
	THORNBUSH_ANIM_IDLE5,
	THORNBUSH_ANIM_ATTACK,
	THORNBUSH_ANIM_HARVEST,
};

#define SPORE_HEALTH		20
#define SPORE_DAMAGE_TIME	10.0f
#define SPORE_MODEL			"models/npc/spore.mdl"
#define SPORE_SOUND			"npc/spore.wav"
#define SPORE_EVENT			"events/spore.sc"

#endif // __WW_THORNBUSH_H__
