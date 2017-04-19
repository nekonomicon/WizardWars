#ifndef __WW_STAFF_H__
#define __WW_STAFF_H__


class WWStaff : public CBasePlayerWeapon
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
	void		AttackPlayer	( CBaseEntity * pVictim, TraceResult * pTrace );

	int			ComboPlayer(CBaseEntity * pEntity);

private:
	int				m_iSwingCount;
	unsigned short	m_usEvent;
};


#endif // __WW_STAFF_H__

