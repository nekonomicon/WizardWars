#ifndef __WW_BEANSTALK_H__
#define __WW_BEANSTALK_H__


class WWBeanStalkSpell : public CBasePlayerWeapon
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

	BOOL		Build			( bool bFirst );
	void		EndBuild		( void );

private:
	bool			m_bStalking;
	EHANDLE			m_hLastStalk;
	unsigned short	m_usEvent;
};


#endif // __WW_BEANSTALK_H__
