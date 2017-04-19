#ifndef __WW_DEATHWIZARD_H__
#define __WW_DEATHWIZARD_H__


class DeathWizard : public WWBaseWizard
{
public:
	DeathWizard( CBasePlayer * pPlayer );

	void		Think			( void );
	void		Equip			( void );
	void		GiveSpells		( void );
	void		DoSpecial		( int slot );
	int			GetClassNumber	( void ) { return WWCLASS_DEATH; };
	const char * GetSatchelClass( void ) { return "ww_satchel_death"; };
	const char * GetSealClass	( void ) { return "ww_seal_death"; };

private:
	BOOL		m_bMadeSoul;
};


#define DEATHWIZARD_SPECIAL_COST		25


#endif // __WW_DEATHWIZARD_H__