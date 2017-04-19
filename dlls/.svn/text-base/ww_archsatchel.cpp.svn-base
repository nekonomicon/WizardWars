#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "gamerules.h"
#include "effects.h"

#include "ww_satchels.h"


class ArchMageSatchel : public WWSatchel
{
public:
	virtual void	Spawn			( void );
	virtual void	SatchelExplode	( void );
};


LINK_ENTITY_TO_CLASS( ww_satchel_archmage, ArchMageSatchel );


void ArchMageSatchel::Spawn( void )
{
	WWSatchel::Spawn();

//	pev->effects |= EF_NODRAW;
	m_iExplodeCount = 1;
}


void ArchMageSatchel::SatchelExplode( void )
{
	if( !pev->owner )
		return;
/*
	entvars_t * pown = VARS( pev->owner );

	if( !pown )
		return;

	pown->health += 30;
	if( pown->health > pown->max_health )
		pown->health = pown->max_health;

	pown->armorvalue += 30;
	if( pown->armorvalue > pown->armortype )
		pown->armorvalue = pown->armortype;
*/

	CBasePlayer * pPlayer = (CBasePlayer *)CBaseEntity::Instance( pev->owner );
	
	if( !pPlayer )
	{
		UTIL_Remove( this );
		return;
	}

	CBaseEntity * pVictim = NULL;
	while( ( pVictim = UTIL_FindEntityInSphere( pVictim, pev->origin, 175.0f ) ) != NULL )
	{
		if( !pVictim->IsPlayer() || !pVictim->IsAlive())
			continue;

		if( pPlayer->IRelationship( pVictim ) < R_NO )
		{
			((CBasePlayer*)pVictim)->GiveArmor(0, 30);

			pVictim->GiveAmmo( 30, "mana", MAX_MANA );
		}
		else
		{
			((CBasePlayer *)pVictim)->GiveArmor(0, -30);

			int ammoIndex = ((CBasePlayer *)pVictim)->GetAmmoIndex( "mana" );
			((CBasePlayer *)pVictim)->m_rgAmmo[ ammoIndex ] -= 30;

			if( ((CBasePlayer *)pVictim)->m_rgAmmo[ammoIndex] > 0 )
			{
				((CBasePlayer *)pVictim)->m_rgAmmo[ ammoIndex ] = 0;
			}
		}
	}
	
	PLAYBACK_EVENT( 0, edict(), m_usEvent );
}
