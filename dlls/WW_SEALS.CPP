#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "animation.h"
#include "saverestore.h"

#include "ww_seals.h"


LINK_ENTITY_TO_CLASS( ww_seal, WWSeal );


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWSeal::Spawn( void )
{
	Precache();

	SET_MODEL ( ENT(pev), "models/seals/default.mdl" );
	SET_SIZE  ( ENT(pev), Vector( -24, -24, -1 ), Vector( 24, 24, 24 ) );
	SET_ORIGIN( ENT(pev), pev->origin );

	SetThink(&WWSeal:: SealThink );
	SetTouch(&WWSeal:: SealTouch );

	pev->health			= 1;
	pev->skin			= 0;
	pev->body			= 0;
	pev->effects		= 0;
	pev->solid			= SOLID_TRIGGER;
	pev->movetype		= MOVETYPE_TOSS;
	pev->takedamage		= DAMAGE_NO;
	pev->rendermode		= kRenderTransAdd;
	pev->renderamt		= 0;
	pev->renderfx		= 0;
	pev->angles.x		= 0;
	pev->sequence		= 0;
	pev->iuser4			= 0;

	m_bActive			= false;
	m_flMana			= 0.0f;
	m_flMaxMana			= 100.0f;
	m_flClientMana		= 0.0f;
	m_flShowTime		= 0.0f;
	m_flNextProcess		= gpGlobals->time + 999.0f;
	m_hActivator		= NULL;

	pev->nextthink		= gpGlobals->time + 0.1f;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWSeal::Precache( void )
{
	PRECACHE_MODEL( "models/seals/default.mdl" );
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int WWSeal::Classify( void )
{
	return SEAL_CLASSIFY;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWSeal::Process( CBaseEntity * pOther )
{
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
float WWSeal::GiveMana( float flMana )
{
	// if we already have enough mana
	if( m_flMana >= m_flMaxMana )
		return flMana;

	float flGive = flMana;

	// if adding this much mana will be more then enough, reduce how much we will give
	if( m_flMana + flGive > m_flMaxMana )
		flGive = m_flMaxMana - m_flMana;

	// give mana
	m_flMana += flGive;

	// if we have full mana, go ahead and activate the seal
	if( m_flMana >= m_flMaxMana )
	{
		ResetSequenceInfo();
		pev->iuser4 = 999;
		pev->renderfx = kRenderFxSurplusMana;
		m_flNextProcess = gpGlobals->time + 2.0f;
		m_bActive = true;

		Activate();
	}

	// return any remaining mana
	return flMana - flGive;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EXPORT WWSeal::SealThink( void )
{
	// drop to the ground
	if( pev->movetype == MOVETYPE_TOSS )
	{
		if( FBitSet( pev->flags, FL_ONGROUND ) )
			pev->movetype = MOVETYPE_NONE;

		pev->nextthink = gpGlobals->time + 0.1f;
		return;
	}

	// if we have full mana, go ahead and activate the seal
	if( !m_bActive && m_flMana >= m_flMaxMana )
	{
		ResetSequenceInfo();
		pev->iuser4 = 999;
		pev->renderfx = kRenderFxSurplusMana;
		m_flNextProcess = gpGlobals->time + 2.0f;
		m_bActive = true;

		Activate();
	}

	// only animate if we are active
	if( m_bActive )
		StudioFrameAdvance();

	// update the alpha and skin if we need to
	if( m_flMana != m_flClientMana )
	{
		pev->renderamt = 50 + ( m_flMana / m_flMaxMana ) * 150;
		pev->skin = pev->team;

		m_flClientMana = m_flMana;
	}

	if( gpGlobals->time >= m_flNextProcess && m_hActivator != NULL )
		m_hActivator = NULL;

	// if we are currently visible to all
	if( m_flShowTime != 0.0f )
	{
		if( gpGlobals->time >= m_flShowTime )
		{ 
			pev->iuser4 = 999; // show to team-mates
			m_flShowTime = 0.0f;
		}
		else
			pev->iuser4 = 0;
	}

	// if we ran out of mana, go inactive
	if( m_bActive && m_flMana <= 0.0f )
	{
		m_bActive = false;
		m_flMana = 0.0f;
		pev->framerate = 0;
		pev->renderfx = 0;

		Deactivate();
	}

	Tick();

	pev->nextthink = gpGlobals->time + 0.1f;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWSeal::SealTouch( CBaseEntity * pOther )
{
	if( !m_bActive )
		return;

	if( !pOther->IsPlayer() || !pOther->IsAlive() )
		return;

	if( pOther == (CBaseEntity *)m_hActivator )
		return;

	if( m_flNextProcess > gpGlobals->time )
		return;

	m_hActivator = pOther;
	m_flNextProcess = gpGlobals->time + 0.1f;

	Process( pOther );
}


