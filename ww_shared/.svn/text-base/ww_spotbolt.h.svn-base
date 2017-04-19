#ifndef __WW_SPOTBOLT_H__
#define __WW_SPOTBOLT_H__


class WWSpotBoltSpell : public CBasePlayerWeapon
{
public:
	BOOL		UseDecrement	( void ) { return TRUE; };

	void		Spawn			( void );
	void		Precache		( void );

	int			iItemSlot		( void );
	int			GetItemInfo		( ItemInfo * pInfo );

	BOOL		CanDeploy		( void );
	BOOL		Deploy			( void );
	void		Holster			( int skiplocal = 0 );

	void		PrimaryAttack	( void );
	void		WeaponIdle		( void );

	void		Fire			( void );
	void		MisFire			( void );

	void		CreateEffect	( void );
	void		UpdateEffect	( void );
	void		DestroyEffect	( void );

private:
	bool			m_bCharging;
	bool			m_bReady;
	bool			m_bFull;
	float			m_flChargeTime;
	CSprite *		m_pSprite;
	unsigned short	m_usEvent1;
	unsigned short	m_usEvent2;
};

#endif // __WW_SPOTBOLT_H__

