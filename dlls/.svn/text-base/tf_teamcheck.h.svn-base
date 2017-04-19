#ifndef __TF_TEAMCHECK_H__
#define __TF_TEAMCHECK_H__

class TFTeamCheck : public CPointEntity
{
public:
	TFTeamCheck() { m_iTeam = 0; };

	virtual void	Spawn				( void );
	virtual void	KeyValue			( KeyValueData * pkvd );

	virtual	int		GetTeam				( void )		{ return m_iTeam;  };
	virtual void	SetTeam				( int iTeam )	{ m_iTeam = iTeam; };

private:
	int			m_iTeam;
};

#endif // __TF_TEAMCHECK_H__