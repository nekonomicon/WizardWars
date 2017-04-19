#ifndef __WW_SATCHELS_H__
#define __WW_SATCHELS_H__


class WWSatchel : public CBaseAnimating
{
public:
	virtual void	Spawn			( void );
	virtual void	Precache		( void );

	void			SetExplode		( float flTime );

	void EXPORT		SatchelTouch	( CBaseEntity * pOther );
	void EXPORT		SatchelThink	( void );
	void EXPORT		ExplodeThink	( void );

	virtual void	SatchelTick		( void );
	virtual void	SatchelExplode	( void );

	virtual BOOL CanRepel( CBaseEntity * pAttacker ) { return TRUE; };

protected:
	float			m_flExplodeDelay;
	float			m_flMultiDelay;
	int				m_iExplodeCount;

	unsigned short	m_usEvent;

private:
	float			m_flActivateTime;
	float			m_flExplodeTime;
};


#endif // __WW_SATCHELS_H__
