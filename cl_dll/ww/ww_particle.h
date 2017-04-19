#ifndef __WW_PARTICLE_H__
#define __WW_PARTICLE_H__


const int	WW_PFLAG_NONE				= 0;
const int	WW_PFLAG_NO_ORIENT_PITCH	= 1;
const int	WW_PFLAG_NO_ORIENT_YAW		= 2;
const int	WW_PFLAG_NO_ORIENT_ROLL		= 4;
const int	WW_PFLAG_FEATHER			= 8;
const int	WW_PFLAG_COLLIDE			= 16;
const int	WW_PFLAG_DIE_ON_COLLIDE		= 32;
const int	WW_PFLAG_DIE_ON_LAST_FRAME	= 64;


class WWParticle;
class WWParticleManager;
typedef struct cl_entity_s cl_entity_t;

typedef struct pdata_s
{
	int		i1;
	int		i2;
	int		i3;
	int		i4;

	float	f1;
	float	f2;
	float	f3;
	float	f4;

	Vector	v1;
	Vector	v2;
	Vector	v3;
	Vector	v4;

	void *	p1;
	void *	p2;
	void *	p3;
	void *	p4;

} pdata_t;


#define PARTICLEAPI __fastcall
typedef int  ( PARTICLEAPI * ParticleThinkCall ) ( WWParticle *, const float & );
typedef void ( PARTICLEAPI * ParticleTouchCall ) ( WWParticle *, struct pmtrace_s * pTrace );
typedef void ( PARTICLEAPI * ParticleDeathCall ) ( WWParticle * );


class WWParticle
{
friend class WWParticle;
friend class WWParticleManager;

public:
	WWParticle( int iPriority );
	~WWParticle();

	static int			Count		( void ) { return _count; };

public:
	int	PARTICLEAPI		Draw		( void );
	int	PARTICLEAPI		Think		( const float & flFrametime );

	void				SetPrivate	( pdata_t * pPrivate, bool bFree );

	void				SetThink	( ParticleThinkCall pfnCustom );
	void				SetTouch	( ParticleTouchCall pfnCustom );
	void				SetDeath	( ParticleDeathCall pfnCustom );

	HSPRITE				Sprite		( void );
	void				SetSprite	( const char * szFile );

private:
	WWParticle *		m_pPrev;
	WWParticle *		m_pNext;

	struct model_s *	m_pModel;

	ParticleThinkCall	m_pfnCustomThink;
	ParticleTouchCall	m_pfnCustomTouch;
	ParticleDeathCall	m_pfnCustomDeath;

	bool				m_bFreeData;

	static unsigned int _count;

public:
	Vector				m_vOrigin;
	Vector				m_vVelocity;

	int					m_iFlags;
	int					m_iOrient;

	HSPRITE				m_hSprite;
	int					m_iSize[2];

	bool				m_bDraw;

	float				m_flFrame;
	float				m_flFrameRate;
	int					m_iFrameCount;

	int					m_iRenderMode;
	float				m_flRenderColor[4];
	float				m_flRenderDecay;

	float				m_flThink;
	float				m_flNextThink;

	float				m_flBirth;

	float				m_flLife;
	float				m_flDelay;
	
	float				m_flScale;
	float				m_flScaleDecay;

	float				m_flGravity;
	float				m_flGravityDecay;

	float				m_flSpeedDecay;

	float				m_flWindScale;

	int					m_iPriority;

	float				m_flTraceSize;
	float				m_flRandomSeed;

	pdata_t *			m_pPrivate;
};


#endif // __WW_PARTICLE_H__
