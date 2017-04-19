#ifndef __WW_LIGHTNINGWIZARD_H__
#define __WW_LIGHTNINGWIZARD_H__

class LightningWizard : public WWBaseWizard
{
public:
	LightningWizard( CBasePlayer * pPlayer );

	void		GiveSpells		( void );

	void		DoSpecial		( int slot );

	int			GetClassNumber	( void ) { return WWCLASS_LIGHTNING; };
	const char * GetSatchelClass( void ) { return "ww_satchel_lightning"; };
	const char * GetSealClass	( void ) { return "ww_seal_lightning"; };

private:
	int			m_iZoomMode;
};

enum
{
	WWZOOM_NORMAL						= 0,
	WWZOOM_2X,
	WWZOOM_4X,
	WWZOOM_8X,
};

#endif // __WW_LIGHTNINGWIZARD_H__