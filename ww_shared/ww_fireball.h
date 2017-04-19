#ifndef __WW_FIREBALL_H__
#define __WW_FIREBALL_H__


class WWFireBallSpell : public CBasePlayerWeapon
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

	virtual BOOL CanRepel( CBaseEntity * pAttacker ) { return TRUE; };

private:
	unsigned short m_usEvent;
};


#endif // __WW_FIREBALL_H__

