#ifndef __TF_ITEM_GOAL_H__
#define __TF_ITEM_GOAL_H__


class TFItemGoal : public CBaseAnimating, public TFEntity
{
public:
	TFItemGoal();

public:
	void			Spawn			( void );
	void			Precache		( void );
	int				Classify		( void ) { return TF_ENT_ITEM_GOAL; };
	void			KeyValue		( KeyValueData * pkvd );

	void			Use				( CBaseEntity * pActivate, CBaseEntity * pCaller, USE_TYPE useType, float value );
	void EXPORT		GoalThink		( void );
	void EXPORT		GoalTouch		( CBaseEntity * pEntity );
	void EXPORT		FallThink		( void );

	void			PlaceItem		( void );

	CBasePlayer *	GetPlayer		( void ) { if( m_hPlayer != NULL ) return ((CBasePlayer *)((CBaseEntity*)m_hPlayer)); return NULL; };

	BOOL			ParseSettings	( const char * pszKey, const char * pszValue );

	void			Attach			( CBasePlayer * pPlayer );
	void			Detach			( void );

	void			GoalActivate	( CBaseEntity * pEntity );
	void			GoalInActivate	( CBaseEntity * pEntity );
	void			GoalRemove		( CBaseEntity * pEntity );
	void			GoalRestore		( CBaseEntity * pEntity );

	void			GoalGrab		( CBaseEntity * pEntity );
	void			GoalDrop		( CBaseEntity * pEntity );
	void			GoalReturn		( CBaseEntity * pEntity );

	static void		DropItems		( CBasePlayer * pPlayer, BOOL bForce = FALSE );

	BOOL			IsMoved			( void ) { return m_bMoved; };

private:
	EHANDLE			m_hPlayer;
	EHANDLE			m_hActivator;

	bool			m_bMoved;

	float			m_flLastActivate;
	float			m_flActivateTime;
	float			m_flInActiveTime;
	float			m_flReturnTime;
	float			m_flDropReturnTime;
	float			m_flNoTouch;
	float			m_flNextTouch;
};

#endif // __TF_INFO_GOAL_H__