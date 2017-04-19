#ifndef __WW_SEALSPELL_H__
#define __WW_SEALSPELL_H__


class WWSealSpell : public CBasePlayerWeapon
{
public:
	BOOL		UseDecrement	( void ) { return TRUE; };

	void		Spawn			( void );
	void		Precache		( void );

	int			iItemSlot		( void );
	int			GetItemInfo		( ItemInfo * pInfo );

	BOOL		CanDeploy		( void );
	BOOL		Deploy			( void );
	void		Holster			( int iSkipLocal = 0 );

	void		PrimaryAttack	( void );
	void		WeaponIdle		( void );

	CBaseEntity	*	FindSeal	( void );

private:
	bool			m_bCharging;
	EHANDLE 		m_hSeal;
	unsigned short	m_usEvent;
};


#endif // __WW_SEALSPELL_H__

