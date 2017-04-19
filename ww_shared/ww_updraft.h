#ifndef __WW_UPDRAFTSPELL_H__
#define __WW_UPDRAFTSPELL_H__


class WWUpdraftSpell : public CBasePlayerWeapon
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
	unsigned short	m_usEvent1;
	unsigned short	m_usEvent2;
};


#endif // __WW_UPDRAFTSPELL_H__

