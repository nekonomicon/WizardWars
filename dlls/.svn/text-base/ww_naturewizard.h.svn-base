#ifndef __WW_NATUREWIZARD_H__
#define __WW_NATUREWIZARD_H__


class NatureWizard : public WWBaseWizard
{
public:
	NatureWizard( CBasePlayer * pPlayer );

	int			GetClassNumber	( void ) { return WWCLASS_NATURE; };
	const char * GetSatchelClass( void ) { return "ww_satchel_nature"; };
	const char * GetSealClass	( void ) { return "ww_seal_nature"; };

	void		Think			( void );
	void		Remove			( void );
	void		GiveSpells		( void );
	void		DoSpecial		( int slot );
	int			MakeThornBush	( int bushNum );
	void		RemoveBush		( CBaseEntity * pBush );
	void		AddStalk		( CBaseEntity * pStalk );
	void		RemoveAllStalks	( void );
	int			StalkRoom		( Vector stalkPos );
	void		StatusReport	( char * pszBuffer );

	EHANDLE		GetBush			( int iBush ) { return m_hBush[iBush]; };
	EHANDLE		GetStalk		( void ) { return m_hStalk; };

private:
	EHANDLE		m_hBush[2];
	EHANDLE		m_hStalk;
};


#define NATUREWIZARD_SEARCHRADIUS		300
#define THORNBUSH_FERT_LVL2_COST		80//1.2.1 WAS 150
#define THORNBUSH_FERT_LVL3_COST		120//1.2.1 WAS 190


#endif // __WW_NATUREWIZARD_H__