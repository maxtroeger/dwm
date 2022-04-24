/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 50;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "terminus:size=10", "JoyPixels:pixelsize=10:antialias=true:autohint=true" };
static const char dmenufont[]       = "terminus:size=10";
static const char col_gray1[]       = "#000000";
static const char col_gray2[]       = "#828282";
static const char col_gray3[]       = "#080808";
static const char col_gray4[]       = "#fefefe";
static const char col_cyan[]        = "#005577";
static const unsigned int baralpha = 0xda;
static const unsigned int borderalpha = 0xda;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray2, col_gray1, col_gray3 },
	[SchemeSel]  = { col_gray4, col_cyan, col_cyan  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "sxiv",    NULL,     NULL,           1,         1,          0,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "HHH",      grid },
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack,  {.ui = 0 } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray2, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_d,      spawn,          SHCMD("passmenu") },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|Mod4Mask,              XK_x,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod4Mask,              XK_z,      incrgaps,       {.i = -1 } },
/*	{ MODKEY|Mod4Mask,              XK_x,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_x,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_z,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_z,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },*/
	{ MODKEY|Mod4Mask,              XK_a,      togglegaps,     {0} },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_a,      defaultgaps,    {0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_w,      spawn,          SHCMD("web_query") },
	{ MODKEY|ShiftMask,             XK_w,      spawn,          SHCMD("web_search") },
	{ MODKEY,                       XK_n,      spawn,          SHCMD("st -e nvim +VimwikiIndex ") },
	{ MODKEY|ShiftMask,             XK_n,      spawn,          SHCMD("st -e sfeed_curses $HOME/.config/sfeed/feeds/*; kill -41 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_e,      spawn,          SHCMD("st -e neomutt; kill -42 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          SHCMD("st -e abook --datafile=$HOME/.config/abook/addressbook --config=$HOME/.config/abook/abookrc") },
	{ MODKEY,                       XK_c,      spawn,          SHCMD("st -e calcurse") },
	{ MODKEY|ShiftMask,             XK_c,      spawn,          SHCMD("webcam") },
	{ MODKEY,                       XK_r,      spawn,          SHCMD("st -e lfub") },
	{ MODKEY|ShiftMask,             XK_r,      spawn,          SHCMD("st -e htop") },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          SHCMD("st -e R -q --no-save") },
	{ MODKEY,                       XK_equal,  spawn,          SHCMD("pamixer -i 5; kill -37 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,             XK_equal,  spawn,          SHCMD("pamixer -i 15; kill -37 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_minus,  spawn,          SHCMD("pamixer -d 5; kill -37 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,             XK_minus,  spawn,          SHCMD("pamixer -d 15; kill -37 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_m,      spawn,          SHCMD("st -e ncmpcpp") },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD("pamixer -t; kill -37 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_p,      spawn,          SHCMD("mpc toggle") },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("mpc pause ; pause_mpv") },
	{ MODKEY,                       XK_comma,  spawn,          SHCMD("mpc prev") },
	{ MODKEY|ShiftMask,             XK_comma,  spawn,          SHCMD("mpc seek 0%") },
	{ MODKEY,                       XK_period, spawn,          SHCMD("mpc next") },
	{ MODKEY|ShiftMask,             XK_period, spawn,          SHCMD("mpc repeat") },
	{ MODKEY,                       XK_bracketleft, spawn,     SHCMD("mpc seek -10") },
	{ MODKEY,                       XK_bracketright, spawn,     SHCMD("mpc seek +10") },
	{ MODKEY|ShiftMask,             XK_bracketleft, spawn,     SHCMD("mpc seek -60") },
	{ MODKEY|ShiftMask,             XK_bracketright, spawn,     SHCMD("mpc seek +60") },
	{ MODKEY,                       XK_BackSpace,      spawn,          SHCMD("state_set") },
	{ MODKEY,                       XK_u,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_u,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[13]} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_space,  zoom,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_Left,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_Right, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Left,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Right, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F4,     spawn,          SHCMD("st -e pulsemixer") },
	{ MODKEY,                       XK_F5,     spawn,          SHCMD("st -e mail_up") },
	{ MODKEY|ShiftMask,             XK_F5,     spawn,          SHCMD("st -e rpimail_up") },
	{ MODKEY,                       XK_F6,     spawn,          SHCMD("st -e feed_up") },
	{ MODKEY,                       XK_F7,     spawn,          SHCMD("st -e pack_up") },
	{ MODKEY,                       XK_F8,     spawn,          SHCMD("st -e forecast_up") },
	{ MODKEY|ShiftMask,             XK_F8,     spawn,          SHCMD("doppler_up") },
	{ MODKEY,                       XK_F9,     spawn,          SHCMD("dmenumount") },
	{ MODKEY,                       XK_F10,    spawn,          SHCMD("dmenuumount") },
	{ MODKEY,                       XK_F12,    spawn,          SHCMD("dmenudock") },
	{ MODKEY|ShiftMask,             XK_F12,    spawn,          SHCMD("arandr") },
	{ 0,				XK_Print,  spawn,	   SHCMD("scrot") },
	{ ShiftMask,			XK_Print,  spawn,	   SHCMD("scrot_part") },
	{ MODKEY,			XK_Print,  spawn,	   SHCMD("dmenurec") },
	{ 0, XF86XK_AudioMute,		spawn,		SHCMD("pamixer -t; kill -37 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("pamixer -i 3; kill -37 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("pamixer -d 3; kill -37 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioPrev,		spawn,		SHCMD("mpc prev") },
	{ 0, XF86XK_AudioNext,		spawn,		SHCMD("mpc next") },
	{ 0, XF86XK_AudioPause,		spawn,		SHCMD("mpc pause") },
	{ 0, XF86XK_AudioPlay,		spawn,		SHCMD("mpc play") },
	{ 0, XF86XK_AudioStop,		spawn,		SHCMD("mpc stop") },
	{ 0, XF86XK_AudioRewind,	spawn,		SHCMD("mpc seek -10") },
	{ 0, XF86XK_AudioForward,	spawn,		SHCMD("mpc seek +10") },
	{ 0, XF86XK_AudioMedia,		spawn,		SHCMD("st -e ncmpcpp") },
	{ 0, XF86XK_AudioMicMute,	spawn,		SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle; kill -37 $(pidof dwmblocks)") },
	{ 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("xbacklight -inc 15") },
	{ 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("xbacklight -dec 15") },
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
