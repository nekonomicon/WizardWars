#ifndef __WW_DRAGONBREATH_H__
#define __WW_DRAGONBREATH_H__


class WWDragonBreathSpell : public CBasePlayerWeapon
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

	void		Attack			( void );
	void		Fire			( const Vector & vOrigin, const Vector & vAiming );
	void		EndAttack		( void );

private:
	bool			m_bFiring;
	float			m_flPulseTime;
	unsigned short	m_usEvent1;
	unsigned short	m_usEvent2;
};


#endif // __WW_DRAGONBREATH_H__
