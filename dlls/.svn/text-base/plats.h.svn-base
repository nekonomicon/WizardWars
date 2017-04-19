#ifndef __PLATS_H__
#define __PLATS_H__


#define SF_PLAT_TOGGLE		0x0001


class CBasePlatTrain : public CBaseToggle
{
public:
	virtual int	ObjectCaps( void ) { return CBaseEntity :: ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
	virtual void KeyValue( KeyValueData* pkvd);
	virtual void Precache( void );

	// This is done to fix spawn flag collisions between this class and a derived class
	virtual BOOL IsTogglePlat( void ) { return (pev->spawnflags & SF_PLAT_TOGGLE) ? TRUE : FALSE; }

	virtual int	Save( CSave &save );
	virtual int	Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];

	BYTE	m_bMoveSnd;			// sound a plat makes while moving
	BYTE	m_bStopSnd;			// sound a plat makes when it stops
	float	m_volume;			// Sound volume
};

class CFuncPlat : public CBasePlatTrain
{
public:
	virtual void Spawn( void );
	virtual void Precache( void );
	virtual void Setup( void );

	virtual void Blocked( CBaseEntity *pOther );


	virtual void EXPORT PlatUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

	virtual void EXPORT CallGoDown( void ) { GoDown(); }
	virtual void EXPORT CallHitTop( void  ) { HitTop(); }
	virtual void EXPORT CallHitBottom( void ) { HitBottom(); }

	virtual void GoUp( void );
	virtual void GoDown( void );
	virtual void HitTop( void );
	virtual void HitBottom( void );
};

class CFuncPlatRot : public CFuncPlat
{
public:
	void Spawn( void );
	void SetupRotation( void );

	virtual void	GoUp( void );
	virtual void	GoDown( void );
	virtual void	HitTop( void );
	virtual void	HitBottom( void );
	
	void			RotMove( Vector &destAngle, float time );
	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];

	Vector	m_end, m_start;
};

class CGunTarget : public CBaseMonster
{
public:
	virtual void			Spawn( void );
	virtual void			Activate( void );
	virtual void EXPORT		Next( void );
	virtual void EXPORT		Start( void );
	virtual void EXPORT		Wait( void );
	virtual void			Stop( void );

	int				BloodColor( void ) { return DONT_BLEED; }
	int				Classify( void ) { return CLASS_MACHINE; }
	int				TakeDamage( entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType );
	void			Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	Vector			BodyTarget( const Vector &posSrc ) { return pev->origin; }

	virtual int	ObjectCaps( void ) { return CBaseEntity :: ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );

	static	TYPEDESCRIPTION m_SaveData[];

private:
	BOOL			m_on;
};


#endif // __PLATS_H__
