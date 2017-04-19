#ifndef __WW_CUSTOMHUD_H__
#define __WW_CUSTOMHUD_H__



#define SAFE_DELETE(x)	{if(x!=NULL){delete x;x=NULL;}}


class CustomHUD;
class CustomHUD_Image;
class CustomHUD_Weapon;


class CustomHUD : public Panel
{
public:
	CustomHUD();
	~CustomHUD();

	void				Init( void );
	void				Destroy( void );

	void				Resize( void );

	void				Update( void );
	void				Hide( void );
	void				Show( void );

protected:
	void				ResizePanel( CustomHUD_Image * pPanel, int x, int y, float fPercent );

	virtual void		paintBackground( void );

	int					m_iHealth;
	int					m_iTargetHealth;
	int					m_iMana;
	int					m_iTargetMana;
	int					m_iArmor;
	int					m_iTargetArmor;
	int					m_iSatchel;

	int					m_iCurWeapon;
	int					m_iCurWeaponId;

	CustomHUD_Image *	m_pHealth;
	CustomHUD_Image *	m_pHealthBulb;
	CustomHUD_Image *	m_pMana;
	CustomHUD_Image *	m_pManaBulb;
	CustomHUD_Image *	m_pLeftHand;
	CustomHUD_Image *	m_pRightHand;
	CustomHUD_Image *	m_pArmor;
	CustomHUD_Image *	m_pArmorBG;
	CustomHUD_Image *	m_pSatchel1;
	CustomHUD_Image *	m_pSatchel2;
	CustomHUD_Image *	m_pSatchel3;
	CustomHUD_Image *	m_pSatchel4;

	TextGrid *			m_pChatGrid;

	cvar_t *			m_pShowHud;
	bool				m_bVisible;

	Dar<CustomHUD_Weapon *>	m_Weapons;
};



class CustomHUD_Image : public Panel
{
public:
	CustomHUD_Image( const char * pszFile );
	~CustomHUD_Image();

	void			setOffset( int x, int y );
	void			getOffset( int & x, int & y );

	void			getImageSize( int & w, int & h );

protected:
	void			paint( void );
	virtual void	paintBackground( void );

	BitmapTGA *		_image;
	int				_offset[2];
};


class CustomHUD_Weapon
{
public:
	CustomHUD_Weapon();
	~CustomHUD_Weapon();

	BitmapTGA * _image;
	int			_pos[2];
	int			_size[2];
};


#endif // __WW_CUSTOMHUD_H__

