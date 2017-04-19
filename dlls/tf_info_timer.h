#ifndef __TF_INFO_TIMER_H__
#define __TF_INFO_TIMER_H__


class TFInfoTimer : public CBaseEntity, public TFEntity
{
public:
	TFInfoTimer();

public:
	void			Spawn			( void );
	void			Precache		( void );
	int				Classify		( void ) { return TF_ENT_INFO_TIMER; };
	void			KeyValue		( KeyValueData * pkvd );
	BOOL			ParseSettings	( const char * pszKey, const char * pszValue );

	void			Use				( CBaseEntity * pActivate, CBaseEntity * pCaller, USE_TYPE useType, float value );
	void EXPORT		GoalThink		( void );
	
	void			GoalActivate	( CBaseEntity * pEntity );
	void			GoalInActivate	( CBaseEntity * pEntity );
	void			GoalRemove		( CBaseEntity * pEntity );
	void			GoalRestore		( CBaseEntity * pEntity );

private:
	float			m_flSearchTime;
	float			m_flNextCheck;
	float			m_flInActiveTime;
};

#endif // __TF_INFO_TIMER_H__