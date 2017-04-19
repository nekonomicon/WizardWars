#ifndef __WW_DEFS_H__
#define __WW_DEFS_H__


#ifdef CLIENT_DLL
#define SC(x,y) (y)
const int g_iIsClient = 1;
#else
#define SC(x,y) (x)
const int g_iIsClient = 0;
#endif


enum
{
	MENU_MOTD							= 1,
	MENU_TEAM,
	MENU_CLASS,
	MENU_MAPBRIEFING,
};


enum
{
	WWCLASS_NONE						= 0,
	WWCLASS_WIND,
	WWCLASS_LIGHTNING,
	WWCLASS_FIRE,
	WWCLASS_DEATH,
	WWCLASS_LIFE,
	WWCLASS_EARTH,
	WWCLASS_DRAGON,
	WWCLASS_ICE,
	WWCLASS_NATURE,
	WWCLASS_ARCHMAGE,
	WWCLASS_RANDOM,
	WWCLASS_LASTCLASS,
};


static const char * szWizardClasses[] =
{
	"",
	"wind",
	"lightning",
	"fire",
	"death",
	"life",
	"earth",
	"dragon",
	"ice",
	"nature",
	"archmage",
	"",
	"",
};


static const char * szWizardHands[] =
{
	NULL,
	"models/hands/wind.mdl",
	"models/hands/lightning.mdl",
	"models/hands/fire.mdl",
	"models/hands/death.mdl",
	"models/hands/life.mdl",
	"models/hands/earth.mdl",
	"models/hands/dragon.mdl",
	"models/hands/ice.mdl",
	"models/hands/nature.mdl",
	"models/hands/archmage.mdl",
	NULL,
};


static const char * szWizardStaves[] =
{
	NULL,
	"models/staves/wind.mdl",
	"models/staves/lightning.mdl",
	"models/staves/fire.mdl",
	"models/staves/death.mdl",
	"models/staves/life.mdl",
	"models/staves/earth.mdl",
	"models/staves/dragon.mdl",
	"models/staves/ice.mdl",
	"models/staves/nature.mdl",
	"models/staves/archmage.mdl",
	NULL,
};


enum
{
	MAX_MANA							= 200,
	MAX_SATCHELS						= 4,
};


enum
{
	TIMER_FIRST				= 0,

	TIMER_FLOODCHECK		= TIMER_FIRST,
	TIMER_RESPAWN,
	TIMER_TEAMCHANGE,
	TIMER_CLASSCHANGE,
	TIMER_TAUNT,
	TIMER_SAVEME,
	TIMER_STATUSBAR,
	TIMER_CHATFLOOD,

	TIMER_INVULNERABILITY,
	TIMER_INVISIBILITY,
	TIMER_SUPERDAMAGE,
	TIMER_PROTECTION,
	TIMER_IMMUNITY,
	TIMER_FROZEN,
	TIMER_FROSTED,
	TIMER_ROOTED,
	TIMER_GROUNDED,

	TIMER_TOTAL,
};


static const int	HUD_GROUNDED		= 0x00000080;   //	   128
static const int	HUD_ROOTED			= 0x00000100;	//	   256
static const int	HUD_FROZEN			= 0x00000200;	//     512
static const int	HUD_FROSTED			= 0x00000400;	//	  1024    
static const int	HUD_COCKLEBUR		= 0x00000800;	//    2048
static const int	HUD_DRAGONMORPH		= 0x00001000;	//    4096
static const int	HUD_BEARMORPH		= 0x00002000;   //    8192
static const int	HUD_IMMUNITY		= 0x00004000;   //   16384
static const int	HUD_YELLOWITEM		= 0x00008000;	//   32768
static const int	HUD_GREENITEM		= 0x00010000;	//   65536
static const int	HUD_BLUEITEM		= 0x00020000;	//  131072
static const int	HUD_REDITEM			= 0x00040000;   //  262144
static const int	HUD_INVISIBILITY	= 0x00080000;   //  524288
static const int	HUD_INVULNERABILITY	= 0x00100000;   // 1048576
static const int	HUD_PROTECTION		= 0x00200000;   // 2097152
static const int	HUD_SUPERDAMAGE		= 0x00400000;   // 4194304
static const int	HUD_YELLOWITEM2		= 0x00800000;   // 8388608
static const int	HUD_GREENITEM2		= 0x01000000;   //16777216

// Player Movement Speeds 
// MAH 20040807 - moved here from ww_wizards.h

/* Codiac 1.2 B18 - more disperate speeds
#define	SPEED_HIGH						0.75f
#define SPEED_MEDHIGH					0.70f
#define SPEED_MED						0.65f
#define SPEED_MEDLOW					0.60f
#define	SPEED_LOW						0.55f
*/
#define	SPEED_HIGH						1.00f
#define SPEED_MEDHIGH					0.85f
#define SPEED_MED						0.75f
#define SPEED_MEDLOW					0.65f
#define	SPEED_LOW						0.55f

#define BEAR_SPEED						0.50f


#endif // __WW_DEFS_H__

