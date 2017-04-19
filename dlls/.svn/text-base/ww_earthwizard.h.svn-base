#ifndef __WW_EARTHWIZARD_H__
#define __WW_EARTHWIZARD_H__


class EarthWizard : public WWBaseWizard
{
public:
	EarthWizard( CBasePlayer * pPlayer );

	const char * GetWizardModel	( void );

	void		Think();

	void		Equip			( void );
	void		Remove			( void );
	void		GiveSpells		( void );

	void		DoSpecial		( int slot );

	int			GetClassNumber	( void ) { return WWCLASS_EARTH; };
	const char * GetSatchelClass( void ) { return "ww_satchel_earth"; };
	const char * GetSealClass	( void ) { return "ww_seal_earth"; };
	BOOL		IsBear			( void ) { return m_bIsBear; };

	BOOL		CanHavePlayerItem( CBasePlayerItem *pWeapon );

	void		BecomeBear		( void );
	void		BecomeWizard	( void );

private:
	BOOL		m_bIsBear;
	float		m_flNextAmmoUse;
};


#define EARTHWIZARD_SPECIAL_COST		20


#endif // __WW_EARTHWIZARD_H__