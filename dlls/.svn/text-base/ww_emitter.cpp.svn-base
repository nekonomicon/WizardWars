#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "effects.h"


class WWEmitter : public CPointEntity
{
public:
	void			Spawn			( void );
	void			Precache		( void );
	void			KeyValue		( KeyValueData *pkvd );

private:
	char			m_szSprite[256];
};


LINK_ENTITY_TO_CLASS( ww_emitter, WWEmitter );


void WWEmitter::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "sprite" ) )
	{
		strcpy( m_szSprite, pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else
		CPointEntity::KeyValue( pkvd );
}


void WWEmitter::Spawn( void )
{ 
	Precache();

	SetThink(&WWEmitter:: SUB_Remove );
}


void WWEmitter::Precache( void )
{
	PRECACHE_MODEL( m_szSprite );
}
