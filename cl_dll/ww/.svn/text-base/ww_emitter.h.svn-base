#ifndef __WW_EMITTER_H__
#define __WW_EMITTER_H__


const int	WW_EFLAG_NONE				= 0;
const int	WW_EFLAG_NO_ORIENT_PITCH	= 1;
const int	WW_EFLAG_NO_ORIENT_YAW		= 2;
const int	WW_EFLAG_NO_ORIENT_ROLL		= 4;
const int	WW_EFLAG_FEATHER			= 8;
const int	WW_EFLAG_COLLIDE			= 16;
const int	WW_EFLAG_DIE_ON_COLLIDE		= 32;
const int	WW_EFLAG_DIE_ON_LAST_FRAME	= 64;
const int	WW_EFLAG_NO_SCREEN			= 256;
const int	WW_EFLAG_NO_BLOCKED			= 512;
const int	WW_EFLAG_NO_PITCH_SPREAD	= 1024;
const int	WW_EFLAG_NO_YAW_SPREAD		= 2048;
const int	WW_EFLAG_NO_ROLL_SPREAD		= 4096;
const int	WW_EFLAG_2D_LENGTH			= 8192;


class WWEmitter
{
public:
	WWEmitter();
	~WWEmitter();

	static int		Count( void ) { return _count; };

public:
	int				Think( float flTime );

	WWEmitter *		Prev( void );
	void			SetPrev( WWEmitter * pPart );

	WWEmitter *		Next( void );
	void			SetNext( WWEmitter * pPart );

	void			SetKeyValue( const char * key, const char * value );

private:
	WWEmitter *		m_pPrev;
	WWEmitter *		m_pNext;

	static unsigned int _count;

public:
	float			m_flThink;
	float			m_flNextThink;

	char			m_szSprite[256];

	Vector			m_vOrigin;
	Vector			m_vAngles;

	int				m_iFlags;
	int				m_iOrient;

	float			m_flLife;
	float			m_flDelay;
	int				m_iCount;
	
	float			m_flSpread;
	Vector			m_vSpreadMin;
	Vector			m_vSpreadMax;

	float			m_flScale;
	float			m_flScaleDecay;

	float			m_flGravity;
	float			m_flGravityDecay;

	float			m_flSpeed;
	float			m_flSpeedDecay;

	float			m_flWindScale;
	
	int				m_iPriority;

	int				m_iRenderMode;
	float			m_flRenderColor[4];
	float			m_flRenderDecay;
};


#endif // __WW_EMITTER_H__