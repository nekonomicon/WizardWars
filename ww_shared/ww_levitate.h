#ifndef __WW_LEVITATESPELL_H__
#define __WW_LEVITATESPELL_H__


class WWLevitateSpell : public CBasePlayerWeapon
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

private:
	unsigned short	m_usEvent;
};


#endif // __WW_LEVITATESPELL_H__

