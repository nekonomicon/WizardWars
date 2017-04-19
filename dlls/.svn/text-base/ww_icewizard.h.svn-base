#ifndef __WW_ICEWIZARD_H__
#define __WW_ICEWIZARD_H__


class IceWizard : public WWBaseWizard
{
public:
	IceWizard( CBasePlayer * pPlayer );

	const char * GetWizardModel	( void );
	const char * GetWizardHands	( void );
	const char * GetWizardStaff ( void );
	int			GetClassNumber	( void ) { return WWCLASS_ICE; };
	const char * GetSatchelClass( void ) { return "ww_satchel_ice"; };
	const char * GetSealClass	( void ) { return "ww_seal_ice"; };

	BOOL		IsInvisible		( void ) { return m_bIsInvisible; };
	BOOL		IsDisguised		( void ) { return m_bIsDisguised; };

	void		StatusReport	( char * pszBuffer );

	void		Think			( void );

	void		Equip			( void );
	void		Remove			( void );
	void		GiveSpells		( void );

	void		DoSpecial		( int slot );

	void		ResetDisguise	( void );

	int			GetDisguisedWizard	( void ) { return m_iDisguisedWizard; };
	int			GetDisguisedTeam	( void ) { return m_iDisguisedTeam; };
	const char * GetDisguiseName	( void ) { return m_szDisguisedName; };

private:
	int			m_bIsInvisible;
	int			m_bIsDisguised;

	int			m_iDisguisedWizard;
	int			m_iDisguisedTeam;
	char		m_szDisguisedName[128];

	float		m_flNextAmmoUse;
};


#endif // __WW_ICEWIZARD_H__