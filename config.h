/* See LICENSE file for copyright and license details. */

static const char font[] = "-misc-fixed-medium-r-normal--13-120-75-75-c-70-*-*";

static const char *tag_name[] = {"1", "2", "3", "4", "5", NULL};
static const char *tile_modes[] = {"ttwm", "rstack", "bstack", NULL};

#define SCROLLWM_CURSOR		XC_left_ptr

static const char colors[LASTColor][9] = {
	[Background]	= "#101010",
	[Default]		= "#686868",
	[Hidden]		= "#FFAA48",
	[Normal]		= "#68B0E0",
	[Sticky]		= "#288428",
	[Urgent]		= "#FF8880",
	[Title]			= "#DDDDDD",
	[TagList]		= "#424242",
};

#define ZOOM_MIN	0.2
#define WIN_MIN		30
#define ZOOM_UP		1.1
#define ZOOM_DOWN	0.89

#define DMENU		"dmenu_run -fn \"-*-terminus-bold-r-*--12-120-72-72-c-60-*-*\" -nb \"#101010\" -nf \"#484862\" -sb \"#080808\" -sf \"#FFDD0E\""
#define TERM		"urxvt" 		/* or "urxvtc","xterm","terminator",etc */
#define CMD(app)	app "&"

/* key definitions */
#define MODKEY Mod4Mask
static Key keys[] = {
	/* modifier				key			function	argument */
	/* launchers: */
	{ MODKEY,				XK_Return,	spawn,		CMD(TERM)		},
	{ MODKEY,				XK_p,		spawn,		CMD(DMENU)		},
	{ MODKEY,				XK_w,		spawn,		CMD("luakit")	},
	{ MODKEY|ShiftMask,		XK_q,		quit,		NULL			},
	{ Mod1Mask,				XK_F4,		killclient,	NULL			},
	{ MODKEY,				XK_Tab, 	cycle,		"screen"		},
	{ MODKEY|ShiftMask,		XK_Tab, 	cycle,		"visible"		},
	{ MODKEY|ControlMask,	XK_Tab, 	cycle,		"all"			},
	{ MODKEY|Mod1Mask,		XK_Tab, 	cycle,		"tag"			},
	/* select current tag */
	{ MODKEY,			XK_1,		tag,		"1"		},
	{ MODKEY,			XK_2,		tag,		"2"		},
	{ MODKEY,			XK_3,		tag,		"3"		},
	{ MODKEY,			XK_4,		tag,		"4"		},
	{ MODKEY,			XK_5,		tag,		"5"		},
	/* tag operations: hide-others, hidden,  sticky, normal(unstick+unhide) */
	{ MODKEY,			XK_o,		tagconfig,	"others"},
	{ MODKEY,			XK_h,		tagconfig,	"hide"	},
	{ MODKEY,			XK_s,		tagconfig,	"stick"	},
	{ MODKEY,			XK_n,		tagconfig,	"normal"},
	/* toggle statusbar */
	{ MODKEY,			XK_a,		tagconfig,	"bar"},
	/* assign/remove a window to/from a tag */
	{ MODKEY|Mod1Mask,	XK_1,		toggletag,	"1"		},
	{ MODKEY|Mod1Mask,	XK_2,		toggletag,	"2"		},
	{ MODKEY|Mod1Mask,	XK_3,		toggletag,	"3"		},
	{ MODKEY|Mod1Mask,	XK_4,		toggletag,	"4"		},
	{ MODKEY|Mod1Mask,	XK_5,		toggletag,	"5"		},
};

static Button buttons[] = {
	/* modifier			button		function 	arg */
	{MODKEY,			1,			window,		"move"		},
	{MODKEY,			2,			window,		"zoom"		},
	{MODKEY,			3,			window,		"resize"	},
	{MODKEY,			4,			window,		"grow"		},
	{MODKEY,			5,			window,		"shrink"	},
	{MODKEY|Mod1Mask,	1,			desktop,	"move"		},
	{MODKEY|Mod1Mask,	2,			tile,		"rstack"	},
	{MODKEY|Mod1Mask,	3,			cycle_tile,	NULL		},
	{MODKEY|Mod1Mask,	4,			desktop,	"grow"		},
	{MODKEY|Mod1Mask,	5,			desktop,	"shrink"	},
};


// vim: ts=4
