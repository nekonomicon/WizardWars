#ifndef __TF_GLOBALINFO_H__
#define __TF_GLOBALINFO_H__

static const char *	TF_VERSION								= "TeamFortress 2.1";


static const int	TF_GAME_NONE							= 0;
static const int	TF_GAME_MULTISKIN						= 1;
static const int	TF_GAME_CLASS_PERSISTENCE				= 2;
static const int	TF_GAME_CHEAT_CHECKING					= 4;
static const int	TF_GAME_FORTRESS_MAP					= 8;
static const int	TF_GAME_RESPAWN_DELAY1					= 16;
static const int	TF_GAME_RESPAWN_DELAY2					= 32;
static const int	TF_GAME_AUTO_TEAM						= 64;
static const int	TF_GAME_TEAM_FRAGS						= 128;

enum
{
	TF_GFLAG_NONE						= 0,

	TF_GFLAG_ITEM1,
	TF_GFLAG_ITEM2,
	TF_GFLAG_ITEM3,
	TF_GFLAG_ITEM4,

	TF_GFLAG_LASTFLAG,
};

enum
{
	TF_GMSG_NONE						= 0,

	TF_GMSG_ITEM_HOME_OWNER,
	TF_GMSG_ITEM_HOME_NON_OWNER,

	TF_GMSG_ITEM_DROPPED_OWNER,
	TF_GMSG_ITEM_DROPPED_NON_OWNER,

	TF_GMSG_ITEM_CARRIED_OWNER,
	TF_GMSG_ITEM_CARRIED_NON_OWNER,

	TF_GMSG_LASTMSG,
};

class TFGlobalInfo
{
public:
	TFGlobalInfo();
	~TFGlobalInfo();

public:
	int				NumTeams				( void );
	const char *	TeamName				( int iTeam ) { return STRING( m_iTeamName[ iTeam ] ); };
	int				MaxPlayers				( int iTeam ) { return m_iMaxPlayers[ iTeam ]; };
	int				ValidClasses			( int iTeam );
	int				ValidTeams				( void );

	BOOL			IsValidTeam				( int iTeam );
	BOOL			IsValidClass			( int iTeam, int iClass );

	int				TeamScore				( int iTeam );
	void			SetTeamScore			( int iTeam, int iScore );
	void			AddTeamScore			( int iTeam, int iScore );

	int				TeamLives				( int iTeam );
	void			SetTeamLives			( int iTeam, int iScore );
	void			AddTeamLives			( int iTeam, int iScore );

	BOOL			IsFriendTeam			( int iTeam1, int iTeam2 );

	float			GetSpawnDelay			( void );

	BOOL			CanJoinTeam				( CBasePlayer * pPlayer, int iTeam  );
	BOOL			CanChangeClass			( CBasePlayer * pPlayer, int iClass );

	BOOL			CanRespawn				( CBasePlayer * pPlayer );

	float			DamageRatio				( CBasePlayer * pPlayer, CBaseEntity * pAttacker );
	float			BlastDamageRatio		( CBasePlayer * pPlayer, CBaseEntity * pAttacker );
	float			ArmorDamageRatio		( CBasePlayer * pPlayer, CBaseEntity * pAttacker );

	int				GivePlayerPoints		( CBasePlayer * pPlayer, int iPoints );
	void			UpdateFrags				( void );

	BOOL			ParseSettings			( const char * pszKey, const char * pszValue );
	void			UpdateSettings			( int iImpulse );

	CBaseEntity *	GetPlayerSpawn			( CBasePlayer * pPlayer );
	void			PlayerSpawn				( CBasePlayer * pPlayer, CBaseEntity * pSpawn );

	BOOL			ClientCommand			( CBasePlayer * pPlayer, const char * cmd );

	void			AddGlobalTeam			( int iTeam );
	void			AddGlobalClass			( int iClass );

	void			DestroyBuildings		( void ) { return; };

private:
	CBaseEntity *	m_pLastSpawnEntity	[ TF_TEAM_MAXTEAMS  ];

private:
	int			m_iszVersion;
	int			m_iszLocalCmd;

	int			m_iScore				[ TF_TEAM_MAXTEAMS	];
	int			m_iLives				[ TF_TEAM_MAXTEAMS	];
	int			m_iMaxPlayers			[ TF_TEAM_MAXTEAMS	];
	int			m_iAllies				[ TF_TEAM_MAXTEAMS	];
	int			m_iClassLimits			[ TF_TEAM_MAXTEAMS	];

	int			m_iTeamName				[ TF_TEAM_MAXTEAMS	];

	int			m_iFlagInfoItem			[ TF_GFLAG_LASTFLAG	];
	int			m_iszFlagMsg			[ TF_GMSG_LASTMSG	];

	int			m_iTeams;
	int			m_iDefaultTeams;
	int			m_iNumTeams;

	int			m_iGlobalTeams;
	int			m_iGlobalClasses;

	BOOL		m_bNoHook;

	BOOL		m_bMultiSkin;
	BOOL		m_bClassPersistence;
	BOOL		m_bCheatChecking;
	BOOL		m_bFortressMap;
	BOOL		m_bRespawnDelay1;
	BOOL		m_bRespawnDelay2;
	BOOL		m_bAutoTeam;
	BOOL		m_bTeamFrags;
};


extern cvar_t tf_debug;
extern TFGlobalInfo * g_pTFGlobalInfo;


#endif // __TF_GLOBALINFO_H__