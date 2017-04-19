#ifndef __WW_BEARBITE_H__
#define __WW_BEARBITE_H__


class WWBearBite : public CBasePlayerWeapon
{
public:
	BOOL		UseDecrement	( void ) { return TRUE; };

	void		Spawn			( void );
	void		Precache		( void );

	int			iItemSlot		( void );
	int			GetItemInfo		( ItemInfo * pInfo );

	BOOL		Deploy			( void );

	void		PrimaryAttack	( void );
	void		WeaponIdle		( void );

	bool		Attack			( void );

private:
	unsigned short	m_usEvent;
};


#endif // __WW_BEARBITE_H__
