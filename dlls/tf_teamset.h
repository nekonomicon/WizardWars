#ifndef __TF_TEAMSET_H__
#define __TF_TEAMSET_H__

class TFTeamSet : public CPointEntity
{
public:
	TFTeamSet() { m_iTeam = 0; };

	virtual void	Spawn				( void );
	virtual void	KeyValue			( KeyValueData * pkvd );

	virtual void	Use					( CBaseEntity * pActivator, CBaseEntity * pCaller, USE_TYPE useType, float value );

private:
	int			m_iTeam;
};

#endif // __TF_TEAMSET_H__