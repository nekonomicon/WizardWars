#ifndef __WW_FIREWIZARD_H__
#define __WW_FIREWIZARD_H__

class FireWizard : public WWBaseWizard
{
public:
	FireWizard( CBasePlayer *pPlayer );

	void		GiveSpells		( void );

	void		DoSpecial		( int slot );

	int			GetClassNumber	( void ) { return WWCLASS_FIRE; };
	const char * GetSatchelClass( void ) { return "ww_satchel_fire"; };
	const char * GetSealClass	( void ) { return "ww_seal_fire"; };
};


#define FIREWIZARD_SPECIAL_COST			100
#define FIREWIZARD_SPECIAL_SOUND		"spells/fissure_cast.wav"
#define FIREWIZARD_FISSURE_DAMAGE		500
#define FIREWIZARD_FISSURE_RADIUS		206.0// 1.2.1 WAS 512

#endif // __WW_FIREWIZARD_H__