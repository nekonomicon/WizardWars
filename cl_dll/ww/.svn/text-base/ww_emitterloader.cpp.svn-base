#include "extdll.h"
#include "entity_state.h"
#include "pm_defs.h"
#include "pm_movevars.h"
#include "../hud_iface.h"
#include "com_model.h"
#include "event_api.h"
#include "../com_weapons.h"
#include "event_flags.h"

#include "ww_emitter.h"
#include "ww_emittermanger.h"
#include "ww_particle.h"
#include "ww_particlemanger.h"

// Thanks go to Richochet for this code... :D

#define BSPVERSION	30

typedef struct
{
	int		fileofs, filelen;
} lump_t;

#define	LUMP_ENTITIES	0
#define	LUMP_PLANES		1
#define	LUMP_TEXTURES	2
#define	LUMP_VERTEXES	3
#define	LUMP_VISIBILITY	4
#define	LUMP_NODES		5
#define	LUMP_TEXINFO	6
#define	LUMP_FACES		7
#define	LUMP_LIGHTING	8
#define	LUMP_CLIPNODES	9
#define	LUMP_LEAFS		10
#define	LUMP_MARKSURFACES 11
#define	LUMP_EDGES		12
#define	LUMP_SURFEDGES	13
#define	LUMP_MODELS		14

#define	HEADER_LUMPS	15

typedef struct
{
	int			version;	
	lump_t		lumps[HEADER_LUMPS];
} dheader_t;



int WWSetKeyValue( WWEmitter * pEmitter, const char * key, const char * value )
{
	float x, y, z, w;

	if( !pEmitter )
		return 0;

	if( !stricmp( key, "classname" ) )
	{
		if( stricmp( value, "ww_emitter" ) )
			return 0;

		return 1;
	}

	else if( !stricmp( key, "origin" ) )
	{
		if( sscanf( value, "%f %f %f", &x, &y, &z ) == 3 )
		{
			pEmitter->m_vOrigin.x = x;
			pEmitter->m_vOrigin.y = y;
			pEmitter->m_vOrigin.z = z;
		}

		return 1;
	}

	else if( !stricmp( key, "angles" ) )
	{
		if( sscanf( value, "%f %f %f", &x, &y, &z ) == 3 )
		{
			pEmitter->m_vAngles.x = x;
			pEmitter->m_vAngles.y = y;
			pEmitter->m_vAngles.z = z;
		}

		return 1;
	}
	
	else if( !stricmp( key, "sprite" ) )
	{
		strcpy( pEmitter->m_szSprite, value );

		return 1;
	}

	else if( !stricmp( key, "pitch" ) )
	{
		pEmitter->m_vAngles.x = atof( value );

		return 1;
	}

	else if( !stricmp( key, "think" ) )
	{
		pEmitter->m_flThink = atof( value );

		return 1;
	}

	else if( !stricmp( key, "life" ) )
	{
		pEmitter->m_flLife = atof( value );

		return 1;
	}

	else if( !stricmp( key, "delay" ) )
	{
		pEmitter->m_flDelay = atof( value );

		return 1;
	}

	else if( !stricmp( key, "count" ) )
	{
		pEmitter->m_iCount = atoi( value );

		return 1;
	}

	else if( !stricmp( key, "spread" ) )
	{
		pEmitter->m_flSpread = atof( value );

		return 1;
	}

	else if( !stricmp( key, "think" ) )
	{
		pEmitter->m_flThink = atof( value );

		return 1;
	}

	else if( !stricmp( key, "mins" ) )
	{
		if( sscanf( value, "%f %f %f", &x, &y, &z ) == 3 )
		{
			pEmitter->m_vSpreadMin.x = x;
			pEmitter->m_vSpreadMin.y = y;
			pEmitter->m_vSpreadMin.z = z;
		}

		return 1;
	}

	else if( !stricmp( key, "maxs" ) )
	{
		if( sscanf( value, "%f %f %f", &x, &y, &z ) == 3 )
		{
			pEmitter->m_vSpreadMax.x = x;
			pEmitter->m_vSpreadMax.y = y;
			pEmitter->m_vSpreadMax.z = z;
		}

		return 1;
	}

	else if( !stricmp( key, "scale" ) )
	{
		pEmitter->m_flScale = atof( value );

		return 1;
	}

	else if( !stricmp( key, "scale_decay" ) )
	{
		pEmitter->m_flScaleDecay = atof( value );

		return 1;
	}

	else if( !stricmp( key, "gravity" ) )
	{
		pEmitter->m_flGravity = atof( value );

		return 1;
	}

	else if( !stricmp( key, "gravity_decay" ) )
	{
		pEmitter->m_flGravityDecay = atof( value );

		return 1;
	}

	else if( !stricmp( key, "speed" ) )
	{
		pEmitter->m_flSpeed = atof( value );

		return 1;
	}

	else if( !stricmp( key, "speed_decay" ) )
	{
		pEmitter->m_flSpeedDecay = atof( value );

		return 1;
	}

	else if( !stricmp( key, "wind_scale" ) )
	{
		pEmitter->m_flWindScale = atof( value );

		return 1;
	}

	else if( !stricmp( key, "render_mode" ) )
	{
		pEmitter->m_iRenderMode = atoi( value );

		return 1;
	}

	else if( !stricmp( key, "render_color" ) )
	{
		if( sscanf( value, "%f %f %f %f", &x, &y, &z, &w ) == 4 )
		{
			pEmitter->m_flRenderColor[0] = x;
			pEmitter->m_flRenderColor[1] = y;
			pEmitter->m_flRenderColor[2] = z;
			pEmitter->m_flRenderColor[3] = w;
		}

		return 1;
	}

	else if( !stricmp( key, "render_decay" ) )
	{
		pEmitter->m_flRenderDecay = atof( value );

		return 1;
	}

	else if( !stricmp( key, "spawnflags" ) )
	{
		pEmitter->m_iFlags = atoi( value );
		return 1;
	}

	else if( !stricmp( key, "priority" ) )
	{
		pEmitter->m_iFlags = atoi( value );
		return 1;
	}

	return 1;
}


char * WWParseEntity( char * buffer, WWEmitter * pEmitter, int & error )
{
	if( !pEmitter )
	{
		error = 1;
		return buffer;
	}

	char		key[256];
	char		token[ 1024 ];
	int			n;

	for( ;; )
	{	
		// Parse key
		buffer = gEngfuncs.COM_ParseFile ( buffer, token );
		if ( token[0] == '}' )
			break;

		// Ran out of input buffer?
		if( !buffer )
		{
			error = 1;
			break;
		}
		
		// Store off the key
		strcpy( key, token );

		// Fix heynames with trailing spaces
		n = strlen( key );
		while (n && key[n-1] == ' ')
		{
			key[n-1] = 0;
			n--;
		}

		// Parse value	
		buffer = gEngfuncs.COM_ParseFile( buffer, token );

		// Ran out of buffer?
		if( !buffer )
		{
			error = 1;
			break;
		}

		// Hit the end instead of a value?
		if( token[0] == '}' )
		{
			error = 1;
			break;
		}

		if( pEmitter != NULL )
		{
			// Assign k/v pair
			if( !WWSetKeyValue( pEmitter, key, token ) )
			{
				delete pEmitter;
				pEmitter = NULL;
			}
		}
	}

	// Return what's left in the stream
	return buffer;
}


void WWProcessEntities( char * buffer )
{	
	char	token[1024];
	int		error = 0;

	WWEmitter * pEmitter = NULL;
	
	// parse entities from entity lump of .bsp file
	for( ;; )
	{
		// parse the opening brace	
		buffer = gEngfuncs.COM_ParseFile( buffer, token );

		if( !buffer )
			return;
		
		// Didn't find opening brace?
		if( token[0] != '{' )
		{
			gEngfuncs.Con_Printf( "WWProcessEntities: found %s when expecting {\n", token );
			return;
		}

		pEmitter = g_pEmitterManager->Allocate();

		if( !pEmitter )
		{
			gEngfuncs.Con_Printf( "WWProcessEntities: too many emitters in map\n" );
			break;
		}
	
		// Fill in data
		buffer = WWParseEntity( buffer, pEmitter, error );

		// Check for errors and abort if any
		if( error )
		{
			if( pEmitter )
			{
				delete pEmitter;
				pEmitter = NULL;
			}

			gEngfuncs.Con_Printf ("WWProcessEntities: error parsing entities\n" );
			return;
		}

		pEmitter = NULL;
	}
}


char * WWLoadEntityLump( const char * filename )
{
	FILE		*fp;
	int			i;
	dheader_t	header;
	int			size;
	lump_t		*curLump;
	char		*buffer = NULL;

	fp = fopen( filename, "rb" );
	if ( !fp )
	{
		gEngfuncs.Con_DPrintf("WWLoadEntityLump:  Could not read BSP for map [%s].\n", filename );
		return NULL;
	}

	// Read in the .bsp header
	if ( fread(&header, sizeof(dheader_t), 1, fp) != 1 )
	{
		gEngfuncs.Con_DPrintf("WWLoadEntityLump:  Could not read BSP header for map [%s].\n", filename );
		fclose(fp);
		return NULL;
	}

	// Check the version
	i = header.version;
	if ( i != 29 && i != 30)
	{
		fclose(fp);
		gEngfuncs.Con_DPrintf("WWLoadEntityLump:  Map [%s] has incorrect BSP version (%i should be %i).\n", filename, i, BSPVERSION );
		return NULL;
	}

	// Get entity lump
	curLump = &header.lumps[ LUMP_ENTITIES ];
	// and entity lump size
	size = curLump->filelen;

	// Jump to it
	fseek( fp, curLump->fileofs, SEEK_SET );

	// Allocate sufficient memmory
	buffer = (char *)malloc( size + 1 );
	if ( !buffer )
	{
		fclose(fp);
		gEngfuncs.Con_DPrintf("WWLoadEntityLump:  Couldn't allocate %i bytes\n", size + 1 );
		return NULL;
	}

	// Read in the entity lump
	fread( buffer, size, 1, fp );

	// Terminate the string
	buffer[ size ] = '\0';

	if ( fp )
	{
		fclose(fp);
	}

	return buffer;
}


void WWLoadEmitters( const char *map )
{
	char	*buffer = NULL;
	char	filename[ MAX_PATH ];

	sprintf( filename, "%s/%s", gEngfuncs.pfnGetGameDirectory(), map );

	// Load entity lump
	buffer = WWLoadEntityLump( filename );
	if ( !buffer )
		return;

	// Process buffer and extract
	WWProcessEntities( buffer );

	// Discard buffer
	free( buffer );
}

char g_szCurrentLevel[256];
void WWCheckMap( void )
{
	if( !g_pEmitterManager )
		return;

	const char * pszLevel = gEngfuncs.pfnGetLevelName();

	if( !pszLevel || pszLevel[0] == 0 || strlen( pszLevel ) == 0 )
		return;

	// See if we've changed to a new map
	if( strnicmp( g_szCurrentLevel, pszLevel, 256 ) )
	{
		strncpy( g_szCurrentLevel, pszLevel, 256 );

		g_pEmitterManager->Clear();
		WWLoadEmitters( g_szCurrentLevel );

		gEngfuncs.Con_DPrintf( "\n==========\nWizard Wars: %i emitter(s) loaded from \"%s\".\n==========\n\n",
			WWEmitter::Count(), g_szCurrentLevel );
	}
}
