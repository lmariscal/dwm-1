/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = {
    "Terminus:style=Regular:size=11:antialias=true:autohint=true",
    "FontAwesome:style=Regular:size=11:antialias=true:autohint=true"
};
static const char dmenufont[]       = "Terminus:size=10";
static const char col_base[]        = "#323a46";
static const char col_black[]       = "#22282f";
static const char col_ltgray[]      = "#f0f5f8";
static const char col_gray[]        = "#949ba3";
static const char col_red[]         = "#8f4c48";
static const char *colors[][3]      = {
	/*               fg          bg         border   */
	[SchemeNorm] = { col_ltgray, col_black, col_red },
	[SchemeSel]  = { col_red,    col_base,  col_red  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
/* static const char *tags[] = { "", "", "", "", "" }; */

static const Rule rules[] = {
	/* class                instance    title               tags mask   isfloating  isterminal  noswallow   monitor */
	{ "St",        NULL,       NULL,               0,          0,          1,          -1,         -1 },
	{ NULL,                 NULL,       "Spotify-TUI",      1 << 8,     0,          1,          0,          -1 },
	{ NULL,                 NULL,       "tremc",            1 << 8,     0,          1,          0,          -1 },
	{ "Pavucontrol",        NULL,       NULL,               0,          1,          0,          0,          -1 },
	{ "Blueman-manager",    NULL,       NULL,               0,          1,          0,          0,          -1 },
	{ "TelegramDesktop",    NULL,       NULL,               1 << 7,     1,          0,          0,          -1 },
	{ "discord",            NULL,       NULL,               1 << 7,     0,          0,          0,          -1 },
	{ "Virt-manager",       NULL,       NULL,               1 << 3,     1,          0,          0,          -1 },
	{ NULL,                 NULL,       "win10 on QEMU/KVM",1 << 3,     1,          0,          1,          -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-c","-l", "20", "-m", dmenumon, "-fn", dmenufont, "-nb", col_base, "-nf", col_ltgray, "-sb", col_red, "-sf", col_black, "-nhb", col_base, "-nhf", col_red, "-shb", col_red, "-shf", col_ltgray, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "Scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };
/* system control */
static const char *suspend_cmd[] = { "doas", "/usr/bin/zzz", NULL };
static const char *raise_audio[] = { "/home/four/.local/bin/tools/volume.sh", "up", NULL };
static const char *lower_audio[] = { "/home/four/.local/bin/tools/volume.sh", "down", NULL };
static const char *mute_audio[] = { "/home/four/.local/bin/tools/volume.sh", "toggle", NULL };
static const char *raise_brightness[] = { "/home/four/.local/bin/tools/light.sh", "up", NULL };
static const char *lower_brightness[] = { "/home/four/.local/bin/tools/light.sh", "down", NULL };
static const char *playpause[] = { "playerctl", "--all-players", "play-pause", NULL };
static const char *nexttrack[] = { "playerctl", "--all-players", "next", NULL };
static const char *prevtrack[] = { "playerctl", "--all-players", "previous", NULL };
/* applications */
static const char *filescmd[] = { "st", "-t", "vifm", "-e", "/home/four/.config/vifm/scripts/vifmrun", NULL };
static const char *gotop[] = { "st", "-t", "go-top", "-e", "gotop", "-pf", NULL };
static const char *pavucontrol[] = { "pavucontrol", NULL };
static const char *blueman[] = { "blueman-manager", NULL };
static const char *spotify[] = { "st", "-t", "Spotify TUI", "-e", "/home/four/.local/bin/spt", NULL };
static const char *torrent[] = { "st", "-t", "tremc", "-e", "tremc", NULL };
/* dmenu scripts */
static const char *passmenu[] = { "/home/four/.local/bin/tools/passmenu", NULL };
static const char *dmenu_school[] = { "/home/four/.local/bin/dmenu/dmenu_courses.sh", NULL };
static const char *dmenu_sshot[] = { "/home/four/.local/bin/dmenu/dmenu_sshot.sh", NULL };
static const char *dmenu_unicode[] = { "/home/four/.local/bin/dmenu/dmenu_unicode.sh", NULL };

static Key keys[] = {
	/* modifier                     key         function        argument */
    /* control dwm */
	{ MODKEY,                       XK_p,       spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_grave,   togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_b,       togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_q,       killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_e,       quit,           {0} },
    { MODKEY|ShiftMask,             XK_z,       spawn,          {.v = suspend_cmd } },
    /* change window focus */
	{ MODKEY,                       XK_j,       focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,       focusstack,     {.i = -1 } },
    /* number of clients in master*/
	{ MODKEY,                       XK_i,       incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,       incnmaster,     {.i = -1 } },
    /* change size of master area */
	{ MODKEY,                       XK_h,       setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,       setmfact,       {.f = +0.05} },
    /* change size of windows in stack */
	{ MODKEY|ShiftMask,             XK_h,       setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,       setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,       setcfact,       {.f =  0.00} },
    /* make focused master */
	{ MODKEY,                       XK_Return,  zoom,           {0} },
    /* view last tag */
	{ MODKEY,                       XK_Tab,     view,           {0} },
    /* change layout */
	{ MODKEY,                       XK_t,       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,       setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,       setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,   setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,   togglefloating, {0} },
	{ MODKEY,                       XK_0,       view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,       tag,            {.ui = ~0 } },
    /* control multiple monitors */
	{ MODKEY,                       XK_comma,   focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,   tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,  tagmon,         {.i = +1 } },
    /* control gap size */
	{ MODKEY,                       XK_minus,   setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,   setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,   setgaps,        {.i = 0  } },
    /* window tags */
	TAGKEYS(                        XK_1,                       0)
	TAGKEYS(                        XK_2,                       1)
	TAGKEYS(                        XK_3,                       2)
	TAGKEYS(                        XK_4,                       3)
	TAGKEYS(                        XK_5,                       4)
	TAGKEYS(                        XK_6,                       5)
	TAGKEYS(                        XK_7,                       6)
	TAGKEYS(                        XK_8,                       7)
	TAGKEYS(                        XK_9,                       8)
	{ MODKEY|ShiftMask,             XK_Return,  spawn,          {.v = termcmd } },
    /* volume control */
    { 0, XF86XK_AudioMute,                      spawn,          {.v = mute_audio } },
    { 0, XF86XK_AudioLowerVolume,               spawn,          {.v = lower_audio } },
    { 0, XF86XK_AudioRaiseVolume,               spawn,          {.v = raise_audio } },
    /* media control */
    { 0, XF86XK_AudioNext,                      spawn,          {.v = nexttrack } },
    { 0, XF86XK_AudioPrev,                      spawn,          {.v = prevtrack } },
    { 0, XF86XK_AudioPlay,                      spawn,          {.v = playpause } },
    /* backlight control */
    { 0, XF86XK_MonBrightnessUp,                spawn,          {.v = raise_brightness } },
    { 0, XF86XK_MonBrightnessDown,              spawn,          {.v = lower_brightness } },
    /* applications: launched with Alt+Super */
    { MODKEY|Mod1Mask,              XK_r,       spawn,          {.v = gotop } },
    { MODKEY|Mod1Mask,              XK_t,       spawn,          {.v = torrent } },
    { MODKEY|Mod1Mask,              XK_f,       spawn,          {.v = filescmd } },
    { MODKEY|Mod1Mask,              XK_a,       spawn,          {.v = pavucontrol } },
    { MODKEY|Mod1Mask,              XK_b,       spawn,          {.v = blueman } },
    { MODKEY|Mod1Mask,              XK_s,       spawn,          {.v = spotify } },
    /* dmenu scripts: launched with Ctrl+Super */
    { MODKEY|ControlMask,           XK_p,       spawn,          {.v = passmenu } },
    { MODKEY|ControlMask,           XK_d,       spawn,          {.v = dmenu_school } },
    { MODKEY|ControlMask,           XK_s,       spawn,          {.v = dmenu_sshot } },
    { MODKEY|ControlMask,           XK_e,       spawn,          {.v = dmenu_unicode } },
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

