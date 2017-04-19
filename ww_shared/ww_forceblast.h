#ifndef __WW_FORCEBLAST_H__
#define __WW_FORCEBLAST_H__


class WWForceBlastSpell : public CBasePlayerWeapon
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
	unsigned short	m_usEvent;
};


#endif // __WW_FORCEBLAST_H__

