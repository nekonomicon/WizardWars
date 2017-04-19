#ifndef __TF_INFO_GOAL_H__
#define __TF_INFO_GOAL_H__


class TFInfoGoal : public CBaseAnimating, public TFEntity
{
public:
	TFInfoGoal();

public:
	void			Spawn			( void );
	void			Precache		( void );
	int				Classify		( void ) { return TF_ENT_INFO_GOAL; };
	void			KeyValue		( KeyValueData * pkvd );
	BOOL			ParseSettings	( const char * pszKey, const char * pszValue );

	void			Use				( CBaseEntity * pActivate, CBaseEntity * pCaller, USE_TYPE useType, float value );
	void EXPORT		GoalThink		( void );
	void EXPORT		GoalTouch		( CBaseEntity * pEntity );

	void			GoalActivate	( CBaseEntity * pEntity );
	void			GoalInActivate	( CBaseEntity * pEntity );
	void			GoalRemove		( CBaseEntity * pEntity );
	void			GoalRestore		( CBaseEntity * pEntity );

private:
	float			m_flLastActivate;
	float			m_flActivateTime;
	float			m_flInActiveTime;
	float			m_flNextTouch;

	EHANDLE			m_hActivator;
};

#endif // __TF_INFO_TEAMSPAWN_H__