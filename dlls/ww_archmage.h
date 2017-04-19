#ifndef __WW_ARCHMAGE_H__
#define __WW_ARCHMAGE_H__


class ArchMage : public WWBaseWizard
{
public:
	ArchMage( CBasePlayer * pPlayer );

	int			GetClassNumber	( void ) { return WWCLASS_ARCHMAGE; };
	void		Equip			( void );
	void		GiveSpells		( void );

	const char * GetSatchelClass( void ) { return "ww_satchel_archmage"; };
};


#endif // __WW_ARCHMAGE_H__