#ifndef __WW_PARTICLEMANGER_H__
#define __WW_PARTICLEMANGER_H__


const		WW_ORIENT_NONE				= 0;
const		WW_ORIENT_PITCH				= 1;
const		WW_ORIENT_YAW				= 2;
const		WW_ORIENT_PITCHYAW			= 3;
const		WW_ORIENT_ROLL				= 4;
const		WW_ORIENT_PITCHROLL			= 5;
const		WW_ORIENT_YAWROLL			= 6;
const		WW_ORIENT_ALL				= 7;
const		WW_ORIENT_TOTAL				= 8;


enum
{
	PARTICLE_PRIORITY_NONE = 0,
	PARTICLE_PRIORITY_LOW,
	PARTICLE_PRIORITY_MEDIUM,
	PARTICLE_PRIORITY_HIGH,
};


static const Vector g_vOrients[WW_ORIENT_TOTAL] =
{
	Vector( 0, 0, 0 ),
	Vector( 1, 0, 0 ),
	Vector( 0, 1, 0 ),
	Vector( 1, 1, 0 ),
	Vector( 0, 0, 1 ),
	Vector( 1, 0, 1 ),
	Vector( 0, 1, 1 ),
	Vector( 1, 1, 1 ),
};



class WWParticleManager
{
friend class WWParticle;

public:
	WWParticleManager();
	~WWParticleManager();

	void			Clear( void );
	WWParticle *	Allocate( int iPriority );

	void			Process( float flTime );

	Vector			CalcPoint( WWParticle * pPart, float flWidth, float flHeight, float flScale, int iOrient );

private:
	WWParticle *	m_pHead;
	WWParticle *	m_pTail;

	Vector			m_vForward[WW_ORIENT_TOTAL];
	Vector			m_vRight[WW_ORIENT_TOTAL];
	Vector			m_vUp[WW_ORIENT_TOTAL];

	float			m_flLastThink;
};


extern WWParticleManager * g_pParticleManager;

extern cvar_t * effects;
extern cvar_t * part_speeds;
extern cvar_t * part_low;
extern cvar_t * part_med;
extern cvar_t * part_high;
extern cvar_t * part_max;


#endif // __WW_PARTICLEMANGER_H__