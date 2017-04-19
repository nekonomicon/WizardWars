#ifndef __WW_WIZARDS_H__
#define __WW_WIZARDS_H__


class WWBaseWizard
{
public:
	WWBaseWizard( CBasePlayer * pPlayer );

	static WWBaseWizard *	GetRandomClass		( CBasePlayer * pPlayer );
	static WWBaseWizard *	GetNumberedClass	( CBasePlayer * pPlayer, int number );
	static const char *		GetWizard			( int num );
	virtual const char *	GetWizardModel		( void );
	virtual const char *	GetWizardHands		( void );
	virtual const char *	GetWizardStaff		( void );

	virtual const char *	GetSatchelClass		( void ) { return "ww_satchel"; };
	virtual const char *	GetSealClass		( void ) { return "ww_seal"; };

	virtual BOOL			CanHavePlayerItem	( CBasePlayerItem *pWeapon ) { return TRUE; };

	virtual void			Think				( void );

	virtual int				GetClassNumber		( void ) { return 0; };

	void					StopForTime			( float time );

	virtual void			StatusReport		( char * pszBuffer );
	virtual int				UpdateStatusBar		( CBaseEntity * pEntity, char * pszBuffer );

	virtual void			Equip				( void );
	virtual void			GiveSpells			( void );
	virtual void			Remove				( void );

	virtual void			DoSpecial			( int slot );

	virtual void			TossGrenade			( void );
	virtual void			ArmGrenade			( void );

	void					SetTeamColor		( int team );

	virtual BOOL			IsBear				( void ) { return FALSE; };
	virtual BOOL			IsInvisible			( void ) { return FALSE; };
	virtual BOOL			IsDisguised			( void ) { return FALSE; };

	virtual int				MaxHealth			( void ) { return m_iMaxHealth; };
	virtual int				MaxArmor			( void ) { return m_iMaxArmor;  };
	virtual int				PlayerSpeed			( void );

	EHANDLE					GetGiantPlant(void){return m_hGiantPlant;};
	void					SetGiantPlant(CBaseEntity * plant){m_hGiantPlant = plant;};
	void					RemoveGiantPlant();
	EHANDLE					GetSpiritDragon(void){return m_hSpiritDragon;};
	void					SetSpiritDragon(CBaseEntity * dragon){m_hSpiritDragon = dragon;};

protected:
	CBasePlayer *			m_pPlayer;

	BOOL					m_bGrenadeArmed;
	BOOL					m_bForceGrenadeToss;
	float					m_flGrenadeTime;

	int						m_iMaxHealth;
	int						m_iMaxArmor;
	float					m_flPlayerSpeed;
	float					m_flPlayerStopTime;

	float					m_flNextSpecialTime;
	EHANDLE					m_hGiantPlant;
	EHANDLE					m_hSpiritDragon;

public:
	EHANDLE					m_hSeal;
	int						m_iComboHits;
	EHANDLE					m_hMounted;
	BOOL					m_bOnDragon;
};


#endif // __WW_WIZARDS_H__
