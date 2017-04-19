#ifndef __TF_ENT_H__
#define __TF_ENT_H__


const int TF_GOAL_ACTIVATION_NONE						= 0;
const int TF_GOAL_ACTIVATION_PLAYER_GLOWS				= 1;
const int TF_GOAL_ACTIVATION_REDUCE_SPEED				= 2;
const int TF_GOAL_ACTIVATION_DROP_ON_DEATH				= 4;
const int TF_GOAL_ACTIVATION_RETURN_ON_DEATH_DROP		= 8;
const int TF_GOAL_ACTIVATION_RETURN_IF_REMOVE_BY_GOAL	= 16;
const int TF_GOAL_ACTIVATION_RETURN_AFTER_PAUSE_REMOVE	= 32;
const int TF_GOAL_ACTIVATION_IF_AP_FAILS_CRITERIA		= 64;
const int TF_GOAL_ACTIVATION_REMOVE_AFTER_PAUSE			= 128;
const int TF_GOAL_ACTIVATION_KEEP_ITEM_WHEN_PLAYER_DIES	= 256;
const int TF_GOAL_ACTIVATION_GLOWS_WHEN_NOT_CARRIED		= 512;
const int TF_GOAL_ACTIVATION_PLAYER_KEEPS_EFFECTS		= 1024;
const int TF_GOAL_ACTIVATION_DROP_MODEL_ON_SPAWN		= 2048;
const int TF_GOAL_ACTIVATION_IS_DROPPABLE				= 4096;
const int TF_GOAL_ACTIVATION_IS_SOLID					= 8192;

const int TF_GOAL_ACTIVATION_PLAYER_TOUCH				= 1;
const int TF_GOAL_ACTIVATION_DETPACK_EXPLOSION			= 2;

const int TF_GOAL_EFFECT_NONE							= 0;
const int TF_GOAL_EFFECT_AP								= 1;
const int TF_GOAL_EFFECT_AP_TEAM						= 2;
const int TF_GOAL_EFFECT_AP_NONTEAM						= 4;
const int TF_GOAL_EFFECT_NONAP							= 8;
const int TF_GOAL_EFFECT_WALLS_OBSTRUCT					= 16;
const int TF_GOAL_EFFECT_SAME_ENVIRO					= 32;
const int TF_GOAL_EFFECT_CHECK_APA_INDIVIDUALLY			= 64;

const int TF_GOAL_RESULT_NONE							= 0;
const int TF_GOAL_RESULT_REMOVE_IMMEDIATELY				= 1;
const int TF_GOAL_RESULT_SUB_GOALS_APPLY_MODS			= 2;
const int TF_GOAL_RESULT_SCORES_INTERMISSION_END		= 4;
const int TF_GOAL_RESULT_SUB_GOALS_DONT_APPLY_MODS		= 8;
const int TF_GOAL_RESULT_PREVENT_SPY_DISGUISE			= 16;
const int TF_GOAL_RESULT_FORCE_RESPAWN_NO_DIE			= 32;
const int TF_GOAL_RESULT_REMOVE_BUILDINGS				= 64;


// this probably is a bad way of doing it, but it seems to work fine
#pragma warning( disable : 4243 )
template <class T, class TF> T * GetTFClassPtr( T * a, TF * b )
{
	entvars_t *pev = (entvars_t *)a;

	// allocate entity if necessary
	if (pev == NULL)
		pev = VARS(CREATE_ENTITY());

	// get the private data
	a = (T *)GET_PRIVATE(ENT(pev));

	if (a == NULL) 
	{
		// allocate private data 
		a = new(pev) T;
		a->pev = pev;
		b = (TF *)a;
		b->tfpev = pev;
	}
	return a;
}

#ifdef _WIN32
#define LINK_ENTITY_TO_TF( mapClassName, DLLClassName, TFClassName ) \
	extern "C" _declspec( dllexport ) void mapClassName( entvars_t *pev ); \
	void mapClassName( entvars_t *pev ) { GetTFClassPtr( (DLLClassName *)pev, (TFClassName *)NULL ); }
#else
#define LINK_ENTITY_TO_TF(mapClassName,DLLClassName,TFClassName) extern "C" void mapClassName( entvars_t *pev ); void mapClassName( entvars_t *pev ) { GetTFClassPtr( (DLLClassName *)pev, (TFClassName *)NULL ); }
#endif


class TFEntity
{
public:
	TFEntity();

	static void		DebugOutputAll			( void );
	void			DebugOutput				( void );
	
public:
	virtual int		GoalId					( void ) { return m_iId; };
	virtual int		GoalState				( void ) { return m_iState; };
	virtual int		GroupId					( void ) { return m_iGroupId; };
	virtual int		Owner					( void ) { return m_iOwner; };

	virtual int		Classify				( void ) { return TF_ENT_BASIC; };

	virtual BOOL	ParseSettings			( const char * pszKey, const char * pszValue );

	virtual void	GoalActivate			( CBaseEntity * pEntity ) { m_iState = TF_GOALSTATE_ACTIVE;   return; };
	virtual void	GoalInActivate			( CBaseEntity * pEntity ) { m_iState = TF_GOALSTATE_INACTIVE; return; };
	virtual void	GoalRemove				( CBaseEntity * pEntity ) { m_iState = TF_GOALSTATE_REMOVED;  return; };
	virtual void	GoalRestore				( CBaseEntity * pEntity ) { m_iState = TF_GOALSTATE_INACTIVE; return; };

	virtual void	GoalGrab				( CBaseEntity * pEntity ) { return; };
	virtual void	GoalDrop				( CBaseEntity * pEntity ) { return; };
	virtual void	GoalReturn				( CBaseEntity * pEntity ) { return; };

	virtual BOOL	CheckActivation			( CBaseEntity * pEntity );
	virtual void	ProcessGoals			( CBaseEntity * pEntity );
	virtual void	ProcessEffects			( CBaseEntity * pEntity );

	virtual void	PlayerMessage			( CBaseEntity * pEntity, CBasePlayer * pPlayer );
	virtual void	PlayerEffect			( CBaseEntity * pEntity, CBasePlayer * pPlayer );

	const char *	GoalName				( void );

	void			Debug					( const char * pszFormat, ... );
	virtual BOOL	Failed					( const char * pszReason, ... );

	virtual	BOOL	IsMoved					( void ) { return FALSE; };
	virtual CBasePlayer * GetPlayer			( void ) { return NULL; };

	virtual void	TFRemove				( void );

public:
	char		m_szName[64];

	// goal info
	int			m_iId;								// My ID is (n)
	int			m_iState;							// I am currenting (n)
	int			m_iGroupId;							// I belong to group (n)
	int			m_iOwner;							// I belong to team (n)

	int			m_iName;
	int			m_iTargetName;

	int			m_iInitialState;

	int			m_iGoalActivation;					// Activation Criteria
	int			m_iGoalEffects;						// What effects to apply
	int			m_iGoalResults;						// Results to apply if Criteria Pass

	Vector		m_vOrigin;							// Where my home is
	Vector		m_vAngles;							// Which way my home is facing

	int			m_iModel;
	int			m_iMaster;

	float		m_flPauseTime;						// Leave on groud for (n) seconds
	float		m_flDelayTime;						// Wait (n) seconds before activating
	float		m_flActiveTime;						// Wait (n) seconds between activations
	float		m_flStayActive;						// Wait (n) seconds before going inactive

	float		m_flInActiveTime;					

	// event messages
	ULONG		m_iMessages[ TF_MSG_LASTMSG ];		// Messages (broadcast, ap_speak, etc)

	// goal effects
	int			m_iTeamScores[ TF_TEAM_MAXTEAMS ];	// Add (n) points
	int			m_iTeamScore;						// Add (n) points to APA team

	int			m_iGiveFrags;						// Give (n) Frags
	int			m_iGiveLives;						// Give (n) Health
	int			m_iGiveHealth;						// Give (n) Health
	int			m_iGiveShells;						// Give (n) Shells
	int			m_iGiveNails;						// Give (n) Nails
	int			m_iGiveRockets;						// Give (n) Rockets
	int			m_iGiveCells;						// Give (n) Cells
	int			m_iGiveArmor;						// Give (n) Armor
	int			m_iGiveArmorType;					// Give (n) Armor Type
	int			m_iGiveGren1;						// Give (n) Type 1 Grenades
	int			m_iGiveGren2;						// Give (n) Type 2 Grenades
	int			m_iGiveDetpack;						// Give (n) Detpacks
	int			m_iGiveMedkit;						// Give (n) Medkit

	float		m_flGodTime;						// Give Invunerablity for (n) seconds
	float		m_flQuadTime;						// Give Super Damage for (n) seconds
	float		m_flInvisTime;						// Give Invisiblity for (n) seconds
	float		m_flRadTime;						// Give Radation Protection for (n) seconds

	unsigned int m_iHudItems;						// Display (n) icon(s) on HUD

	int			m_iGoalToActivate;					// Activate Goal (n)
	int			m_iGoalToInActivate;				// InActivate Goal (n)
	int			m_iGoalToRemove;					// Remove Goal (n)
	int			m_iGoalToRestore;					// Restore Goal (n)

	int			m_iItemToGive;						// Give Item (n)
	int			m_iItemToRemove;					// Remove Item (n)
	int			m_iItemToReturn;					// Return Item (n)
	int			m_iItemGroupToRemove;				// Remove Item from Group (n)

	int			m_iGroupToActivate;					// Activate Group (n)
	int			m_iGroupToInActivate;				// InActivate Group (n)
	int			m_iGroupToRemove;					// Remove Group (n)
	int			m_iGroupToRestore;					// Restore Group (n)

	int			m_iSpawnToRemove;					// Remove Spawn (n)
	int			m_iSpawnToRestore;					// Restore Spawn (n)
	int			m_iSpawnGroupToRemove;				// Remove Spawn (n)
	int			m_iSpawnGroupToRestore;				// Restore Spawn (n)

	int			m_iElseGoal;						// If Goal Criteria Fail, Activate Goal (n)

	// OnReturn events
	int			m_iGoalOnReturn;					// Activate Goal (n) on Return

	// OnActivate events
	int			m_iAllActive;						// if all goals in group # are active...
	int			m_iAllActiveGoal;					// .. then activate goal #

	int			m_iAllCarried;						// If all goals from Group (n) are carried
	int			m_iAllCarriedGoal;					// ... then activate goal #

	int			m_iAllCarriedSingle;				// If all goals from Group (n) are carried by 1 player
	int			m_iAllCarriedSingleGoal;			// ... then activate goal #

	// who to effect
	int			m_iApplyTeam;
	int			m_iApplyNotTeam;
	float		m_flGoalRadius;

	// criteria
	int			m_iReqTeam;							// AP must be on this team
	int			m_iReqClass;
	int			m_iTeamCheck;						// name of teamcheck ent to check
	int			m_iOwnerTeamCheck;

	int			m_iItemIsNeeded;					// If AP has Goal (n)
	int			m_iItemFromGroup;					// If AP has a goal from Group (n)
	int			m_iItemFromNotGroup;				// If AP has a goal not from Group (n)

	int			m_iItemHasMoved;					// If Item (n) has Moved
	int			m_iItemNotHasMoved;					// If Item (n) has not Moved

	int			m_iGoalIsActive;					// If Goal (n) is Active
	int			m_iGoalIsInActive;					// If Goal (n) is InActive
	int			m_iGoalIsRemoved;					// If Goal (n) is Removed

	int			m_iGroupIsActive;					// If Group (n) is Active
	int			m_iGroupIsInActive;					// If Group (n) is InActive
	int			m_iGroupIsRemoved;					// If Group (n) is Removed
	int			myCount;

public:
	static unsigned int	_count;
	static void			ClearList( void ) { m_pHead = m_pTail = NULL; _count = 0; };
	static TFEntity *	m_pHead;
	static TFEntity *	m_pTail;
	TFEntity *			m_pPrev;
	TFEntity *			m_pNext;

	entvars_t *	tfpev;
};

#endif // __TF_ENT_H__
