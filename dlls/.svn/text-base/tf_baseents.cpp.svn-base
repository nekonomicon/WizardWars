#include <ctype.h>
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "saverestore.h"
#include "trains.h"
#include "gamerules.h"
#include "talkmonster.h"
#include "weapons.h"

#include "func_break.h"
#include "plats.h"
#include "healthkit.h"
#include "h_battery.h"

#include "tf_globals.h"
#include "tf_globalinfo.h"
#include "tf_teamcheck.h"
#include "tf_ent.h"
#include "tf_info_goal.h"
#include "tf_item_goal.h"
#include "tf_info_timer.h"
#include "tf_info_teamspawn.h"


//==============================================================================
// FUNC_PLAT
//==============================================================================

class TFFuncPlat : public CFuncPlat, TFEntity
{
public:
	void EXPORT PlatUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	virtual void KeyValue( KeyValueData *pkvd );
};

LINK_ENTITY_TO_TF( func_plat, TFFuncPlat, TFEntity );

void TFFuncPlat::PlatUse( CBaseEntity * pActivator, CBaseEntity * pCaller, USE_TYPE useType, float value )
{
	if( !CheckActivation( pActivator ) )
		return;

	ProcessEffects( pActivator );

	CFuncPlat::PlatUse( pActivator, pCaller, useType, value );
}

void TFFuncPlat::KeyValue( KeyValueData * pkvd )
{
	CFuncPlat::KeyValue( pkvd );

	if( !pkvd->fHandled )
		ParseSettings( pkvd->szKeyName, pkvd->szValue );

}



//==============================================================================
// FUNC_PLATROT
//==============================================================================

class TFFuncPlatRot : public CFuncPlatRot, TFEntity
{
public:
	void EXPORT PlatUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	virtual void KeyValue( KeyValueData *pkvd );
};

LINK_ENTITY_TO_TF( func_platrot, TFFuncPlatRot, TFEntity );

void TFFuncPlatRot::PlatUse( CBaseEntity * pActivator, CBaseEntity * pCaller, USE_TYPE useType, float value )
{
	if( !CheckActivation( pActivator ) )
		return;

	ProcessGoals( pActivator );

	CFuncPlatRot::PlatUse( pActivator, pCaller, useType, value );
}

void TFFuncPlatRot::KeyValue( KeyValueData * pkvd )
{
	CFuncPlatRot::KeyValue( pkvd );

	if( !pkvd->fHandled )
		ParseSettings( pkvd->szKeyName, pkvd->szValue );

}

//==============================================================================
// FUNC_GUNTARGET
//==============================================================================

class TFGunTarget : public CGunTarget, TFEntity
{
public:
	void			Activate( void );
	virtual void	KeyValue( KeyValueData *pkvd );
};

LINK_ENTITY_TO_TF( func_guntarget, TFGunTarget, TFEntity );

void TFGunTarget::KeyValue( KeyValueData * pkvd )
{
	CGunTarget::KeyValue( pkvd );

	if( !pkvd->fHandled )
		ParseSettings( pkvd->szKeyName, pkvd->szValue );

}

void TFGunTarget::Activate( void )
{
	if( !CheckActivation( m_hActivator ) )
		return;

	ProcessGoals( m_hActivator );

	CGunTarget::Activate();
}



//==============================================================================
// FUNC_HEALTHCHARGER
//==============================================================================
class TFWallHealth : public CWallHealth, TFEntity
{
public:
	void KeyValue( KeyValueData *pkvd );
	void EXPORT Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
};

LINK_ENTITY_TO_TF( func_healthcharger, TFWallHealth, TFEntity );

void TFWallHealth::Use( CBaseEntity * pActivator, CBaseEntity * pCaller, USE_TYPE useType, float value )
{
	if( !CheckActivation( pActivator ) )
		return;

	ProcessGoals( pActivator );

	CWallHealth::Use( pActivator, pCaller, useType, value );
}

void TFWallHealth::KeyValue( KeyValueData * pkvd )
{
	CWallHealth::KeyValue( pkvd );

	if( !pkvd->fHandled )
		ParseSettings( pkvd->szKeyName, pkvd->szValue );

}



//====================================================================
// FUNC_RECHARGER
//====================================================================
class TFRecharge : public CRecharge, TFEntity
{
public:
	void KeyValue( KeyValueData *pkvd );
	void EXPORT Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
};

LINK_ENTITY_TO_TF( func_recharge, TFRecharge, TFEntity );

void TFRecharge::Use( CBaseEntity * pActivator, CBaseEntity * pCaller, USE_TYPE useType, float value )
{
	if( !CheckActivation( pActivator ) )
		return;

	ProcessGoals( pActivator );

	CRecharge::Use( pActivator, pCaller, useType, value );
}

void TFRecharge::KeyValue( KeyValueData * pkvd )
{
	CRecharge::KeyValue( pkvd );

	if( !pkvd->fHandled )
		ParseSettings( pkvd->szKeyName, pkvd->szValue );

}


class TFContainEntity : public CBaseEntity
{
public:
	void Spawn( void )
	{
		pev->solid		= SOLID_NOT;
		pev->movetype	= MOVETYPE_NONE;
		pev->flags	   |= FL_WORLDBRUSH;
		pev->effects   |= EF_NODRAW;

		SET_MODEL( edict(), STRING( pev->model ) );//set size and link into world.
	}

	BOOL ContainsPoint( Vector vPoint )
	{
		if ( vPoint.x > pev->absmax.x ||
			 vPoint.y > pev->absmax.y ||
			 vPoint.z > pev->absmax.z ||
			 vPoint.x < pev->absmin.x ||
			 vPoint.y < pev->absmin.y ||
			 vPoint.z < pev->absmin.z )
			 return FALSE;

		return TRUE;
	}
};


LINK_ENTITY_TO_CLASS( func_nobuild, TFContainEntity );
LINK_ENTITY_TO_CLASS( func_nogrenades, TFContainEntity );
LINK_ENTITY_TO_CLASS( info_areamarker, CPointEntity );
