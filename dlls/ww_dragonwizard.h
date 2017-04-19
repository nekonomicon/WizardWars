#ifndef __WW_DRAGONWIZARD_H__
#define __WW_DRAGONWIZARD_H__


class DragonWizard : public WWBaseWizard
{
public:
	DragonWizard( CBasePlayer * pPlayer );

	void		GiveSpells		( void );
	void		DoSpecial		( int slot );
	int			GetClassNumber	( void ) { return WWCLASS_DRAGON; };
	const char * GetSatchelClass( void ) { return "ww_satchel_dragon"; };
	const char * GetSealClass	( void ) { return "ww_seal_dragon"; };
};


#define DRAGONWIZARD_SPECIAL_LIFE		40
#define DRAGONWIZARD_SPECIAL_COST		100
#define DRAGONWIZARD_SPECIAL_STRENGTH	600


#endif // __WW_DRAGONWIZARD_H__