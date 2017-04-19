#ifndef __WW_SENTRYCRYSTAL_H__
#define __WW_SENTRYCRYSTAL_H__


class WWSentryCrystal : public CBaseMonster
{
public:
	WWSentryCrystal();

	virtual int		Classify		( void ) { return CLASS_MACHINE; };
	int				BloodColor		( void ) { return DONT_BLEED; }

	virtual int		IRelationship	( CBaseEntity * pTarget );

	void			Spawn			( void );
	virtual void	Precache		( void );
	void			KeyValue		( KeyValueData *pkvd );
	
	virtual void	TraceAttack		( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType );
	virtual int		TakeDamage		( entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType  );

	void EXPORT		SearchAndDestroy( void );
	virtual void	Fire			( CBaseEntity * pEnemy );

private:
	float			m_flNextFire;
	float			m_flLastFire;
	int				m_iszTeamcheck;
	int				m_iOwner;
	int				m_iOrient;
};


#endif // __WW_SENTRYCRYSTAL_H__