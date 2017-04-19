#ifndef __WW_ICEPOKE_H__
#define __WW_ICEPOKE_H__


class WWIcePokeSpell : public CBasePlayerWeapon
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

private:
	unsigned short	m_usEvent;
};


#endif // __WW_ICEPOKE_H__
