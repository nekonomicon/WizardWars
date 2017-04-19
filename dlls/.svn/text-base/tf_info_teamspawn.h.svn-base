#ifndef __TF_INFO_TEAMSPAWN_H__
#define __TF_INFO_TEAMSPAWN_H__

const int	TF_TEAMSPAWN_ACTIVATION_ONLY_FIRST				= 0;
const int	TF_TEAMSPAWN_ACTIVATION_ALL_GET_ITEM			= 1;
const int	TF_TEAMSPAWN_ACTIVATION_ALL_GET_MESSAGE			= 2;

const int	TF_TEAMSPAWN_EFFECTS_NONE						= 0;
const int	TF_TEAMSPAWN_EFFECTS_REMOVE_AFTER_SPAWN			= 1;

const int	TF_TEAMSPAWN_RESULTS_NONE						= 0;
const int	TF_TEAMSPAWN_AP_PASS_ACTIVATED_GOALS			= 2;

class TFInfoTeamSpawn : public CPointEntity, public TFEntity
{
public:
	TFInfoTeamSpawn();
	~TFInfoTeamSpawn();

public:
	virtual void	Spawn			( void );
	virtual void	Precache		( void );

	virtual void	KeyValue		( KeyValueData * pkvd );
	virtual BOOL	ParseSettings	( const char * pszKey, const char * pszValue );

	virtual int		Classify		( void ) { return TF_ENT_INFO_TEAMSPAWN; };

	virtual BOOL	CheckSpawn		( CBasePlayer * pPlayer );
	virtual void	PlayerSpawn		( CBasePlayer * pPlayer );

private:
	BOOL			m_bHasSpawned;

};

#endif // __TF_INFO_TEAMSPAWN_H__