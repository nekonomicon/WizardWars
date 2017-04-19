#ifndef __WW_WYVERN_H__
#define __WW_WYVERN_H__


class WWWyvernSpell : public CBasePlayerWeapon
{
public:
	BOOL		UseDecrement	( void ) { return TRUE; };

	void		Spawn			( void );
	void		Precache		( void );

	int			iItemSlot		( void );
	int			GetItemInfo		( ItemInfo * pInfo );

	BOOL		CanDeploy		( void );
	BOOL		Deploy			( void );

	void		PrimaryAttack	( void );
	void		WeaponIdle		( void );

private:
	unsigned short m_usEvent;
};


#endif // __WW_WYVERN_H__
