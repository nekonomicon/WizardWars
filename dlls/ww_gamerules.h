#ifndef __WW_GAMERULES_H__
#define __WW_GAMERULES_H__


class WWGameRules : public CHalfLifeMultiplay
{
public:
	WWGameRules();

	virtual void		Think					( void );
	virtual void		InitHUD					( CBasePlayer * pPlayer );
	virtual void		UpdateGameMode			( CBasePlayer * pPlayer );
	virtual BOOL		ClientCommand			( CBasePlayer * pPlayer, const char * pcmd );
	virtual void		ClientUserInfoChanged	( CBasePlayer * pPlayer, char * buffer );
	virtual void		ClientDisconnected		( edict_t * pClient );

	virtual BOOL		IsTeamplay				( void );
	virtual int			PlayerRelationship		( CBaseEntity * pPlayer, CBaseEntity * pTarget );
	virtual int			IPointsForKill			( CBasePlayer * pAttacker, CBasePlayer * pKilled );

	virtual void		PlayerKilled			( CBasePlayer * pVictim, entvars_t * pKiller, entvars_t * pInflictor );
	virtual BOOL		FPlayerCanTakeDamage	( CBasePlayer * pPlayer, CBaseEntity * pAttacker );
	virtual void		DeathNotice				( CBasePlayer * pVictim, entvars_t * pKiller, entvars_t * pInflictor );

	const char *		GetGameDescription		( void ) { return GAME_NAME; };

	virtual void		ChangePlayerTeam		( CBasePlayer * pPlayer, int iTeam, BOOL bKill, BOOL bGib );
	virtual void		ChangePlayerClass		( CBasePlayer * pPlayer, int iClass, BOOL bKill, BOOL bGib );

	virtual int			TeamWithFewestPlayers	( CBasePlayer * pPlayer );

	virtual void		PlayerSpawn				( CBasePlayer * pPlayer );
	virtual void		PlayerThink				( CBasePlayer * pPlayer );

	virtual BOOL		FPlayerCanRespawn		( CBasePlayer * pPlayer );
	virtual float		FlPlayerSpawnTime		( CBasePlayer * pPlayer );
	virtual edict_t *	GetPlayerSpawnSpot		( CBasePlayer * pPlayer );

//	virtual BOOL		FAllowMonsters			( void ) { return TRUE; };

	virtual BOOL		CanHavePlayerItem		( CBasePlayer *pPlayer, CBasePlayerItem *pWeapon );

	virtual int			DeadPlayerWeapons		( CBasePlayer *pPlayer ) { return GR_PLR_DROP_GUN_NO; };
	virtual int			DeadPlayerAmmo			( CBasePlayer *pPlayer ) { return GR_PLR_DROP_AMMO_ALL; };

private:
	BOOL m_DisableDeathMessages;
	BOOL m_DisableDeathPenalty;
};


#endif // __WW_GAMERULES_H__
