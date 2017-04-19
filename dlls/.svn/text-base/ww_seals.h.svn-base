#ifndef __WW_SEALS_H__
#define __WW_SEALS_H__


#define SEAL_CLASSIFY		50


class WWSeal : public CBaseAnimating
{
public:
	virtual void	Spawn		( void );
	virtual void	Precache	( void );
	int				Classify	( void );

	virtual void	Process		( CBaseEntity * pOther );

	float			GiveMana	( float flMana );

	void EXPORT		SealThink	( void );
	void EXPORT		SealTouch	( CBaseEntity * pOther );

	virtual void	Tick		( void ) {};
	virtual void	Activate	( void ) {};
	virtual void	Deactivate	( void ) {};

	bool			m_bActive;
	float			m_flMana;			// how much mana do we have?
	float			m_flMaxMana;		// how much mana can we have max?
	float			m_flClientMana;		// how much mana did we have last time we updated?
	float			m_flShowTime;		// how long are we to remain visible?
	float			m_flNextProcess;	// how long until we can become active again.
	EHANDLE			m_hActivator;
};


#endif // __WW_SEALS_H__
