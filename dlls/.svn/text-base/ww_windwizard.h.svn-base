#ifndef __WW_WINDWIZARD_H__
#define __WW_WINDWIZARD_H__


class WindWizard : public WWBaseWizard
{
public:
	WindWizard( CBasePlayer * pPlayer );

public:
	void		Think			( void );
	void		Remove			( void );

	void		Equip			( void );
	void		GiveSpells		( void );

	void		DoSpecial		( int slot );
	
	int			GetClassNumber	( void ) { return WWCLASS_WIND; };
	const char * GetSatchelClass( void ) { return "ww_satchel_wind"; };
	const char * GetSealClass	( void ) { return "ww_seal_wind"; };

private:
	float		m_flNextCloneTime;
};


#define WINDWIZARD_SPECIAL_COST			20
#define WINDWIZARD_SPECIAL_DELAY		5


#endif // __WW_WINDWIZARD_H__