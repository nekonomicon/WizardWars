#ifndef __WW_LIFEWIZARD_H__
#define __WW_LIFEWIZARD_H__


class LifeWizard : public WWBaseWizard
{
public:
	LifeWizard( CBasePlayer * pPlayer );

	void		Think			( void );

	void		Equip			( void );
	void		GiveSpells		( void );

	void		DoSpecial		( int slot );

	int			GetClassNumber	( void ) { return WWCLASS_LIFE; };
	const char * GetSatchelClass( void ) { return "ww_satchel_life"; };
	const char * GetSealClass	( void ) { return "ww_seal_life"; };

	void		Sacrafise		( void );

private:
	float		m_flNextHealthGain;
	float		m_flSacrafise;
	int			m_bMadeSoul;
	float		m_flNextSacrafise;
};


#define LIFEWIZARD_SPECIAL_COST			50


#endif // __WW_LIFEWIZARD_H__