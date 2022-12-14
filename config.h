/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 3;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static unsigned int gappih    = 10;       /* horiz inner gap between windows */
static unsigned int gappiv    = 10;       /* vert inner gap between windows */
static unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */

static const char *fonts[]    = { "monospace:size=14",
                                  "Ping Fang SC:size=14:antialias=true:autohint=true",
                                  "Apple Color Emoji:size=14:antialias=true:autohint=true",
                                };
static char dmenufont[]       = "monospace:size=14";
static char normbgcolor[]     = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[]     = "#bbbbbb";
static char selfgcolor[]      = "#eeeeee";
static char selbordercolor[]  = "#005577";
static char selbgcolor[]      = "#005577";

static char *colors[][3] = {
	/*               fg           bg           border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "??????",      tile },    /* first entry is default */
	{ "??????",      NULL },    /* no layout function means floating behavior */
	{ "??????",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
    { MOD,  XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
    { MOD,  XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
    { MOD,  XK_o,   ACTION##stack,  {.i = 0 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "st", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    { "dmenufont",       STRING,  &dmenufont },
    { "normbgcolor",     STRING,  &normbgcolor },
    { "normbordercolor", STRING,  &normbordercolor },
    { "normfgcolor",     STRING,  &normfgcolor },
    { "selbgcolor",      STRING,  &selbgcolor },
    { "selbordercolor",  STRING,  &selbordercolor },
    { "selfgcolor",      STRING,  &selfgcolor },
    { "borderpx",        INTEGER, &borderpx },
    { "snap",            INTEGER, &snap },
    { "showbar",         INTEGER, &showbar },
    { "topbar",          INTEGER, &topbar },
    { "nmaster",         INTEGER, &nmaster },
    { "resizehints",     INTEGER, &resizehints },
    { "mfact",           FLOAT,   &mfact },
    { "gappih",          INTEGER, &gappih },
    { "gappiv",          INTEGER, &gappiv },
    { "gappoh",          INTEGER, &gappoh },
    { "gappov",          INTEGER, &gappov },
};

#include <X11/XF86keysym.h>
#include "shift-tools.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
    /* basic */
	{ Mod1Mask,                     XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },

    /* shift-tools */
    { MODKEY,                       XK_semicolon,  shiftview,   {.i = -1 } },
    { MODKEY|ShiftMask,             XK_semicolon,  shifttag,    {.i = -1 } },
    { MODKEY,                       XK_apostrophe, shiftview,   {.i = 1 } },
    { MODKEY|ShiftMask,             XK_apostrophe, shifttag,    {.i = 1 } },

    /* set layouts */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  togglefloating, {0} },
    { MODKEY,                       XK_n,      layoutscroll,   {.i = 1 } },
    { MODKEY,                       XK_p,      layoutscroll,   {.i = -1 } },

    /* gaps */
    { MODKEY,                       XK_equal,  defaultgaps,    {0} },
    { MODKEY|ShiftMask,             XK_equal,  incrigaps,      {.i = +1 } },
    { MODKEY,                       XK_minus,  incrigaps,      {.i = -1 } },

    /* client weight */
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.025} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.025} },
    { MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = -0.125} },
    { MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = +0.125} },
    { MODKEY|ShiftMask,             XK_0,      setcfact,       {.f =  0.00} },

    /* manage client stack */
    STACKKEYS(MODKEY,                          focus)
    STACKKEYS(MODKEY|ShiftMask,                push)

	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

    { 0, XK_VoidSymbol,           spawn, SHCMD("fcitx5-remote -t") },
    { 0, XF86XK_AudioMute,        spawn, SHCMD("pamixer -t") },
    { 0, XF86XK_AudioRaiseVolume, spawn, SHCMD("pamixer --allow-boost -i 5") },
    { 0, XF86XK_AudioLowerVolume, spawn, SHCMD("pamixer --allow-boost -d 5") },
    { 0, XF86XK_AudioPrev,        spawn, SHCMD("mpc prev") },
    { 0, XF86XK_AudioNext,        spawn, SHCMD("mpc next") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

