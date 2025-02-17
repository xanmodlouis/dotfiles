#include <X11/XF86keysym.h>

//=============================================================================
//                基础配置，看自己习惯
//=============================================================================
static int showsystray                    = 1;         /* 是否显示托盘栏 */
static const int newclientathead          = 0;         /* 定义新窗口在栈顶还是栈底 */
static const unsigned int borderpx        = 2;         /* 窗口边框大小 */
static const unsigned int systraypinning  = 0;         /* 托盘跟随的显示器 0代表不指定显示器 */
static const unsigned int systrayspacing  = 1;         /* 托盘间距 */
static const unsigned int systrayspadding = 5;         /* 托盘和状态栏的间隙 */
static int gappi                          = 6;        /* 窗口与窗口 缝隙大小 */
static int gappo                          = 6;        /* 窗口与边缘 缝隙大小 */
static const int _gappo                   = 12;        /* 窗口与窗口 缝隙大小 不可变 用于恢复时的默认值 */
static const int _gappi                   = 12;        /* 窗口与边缘 缝隙大小 不可变 用于恢复时的默认值 */
static const int vertpad                  = 3;         /* vertical padding of bar */
static const int sidepad                  = 3;         /* horizontal padding of bar */
static const int overviewgappi            = 5;        /* overview时 窗口与窗口 缝隙大小 */
static const int overviewgappo            = 24;        /* overview时 窗口与边缘 缝隙大小 */
static const int showbar                  = 1;         /* 是否显示状态栏 */
static const int topbar                   = 1;         /* 指定状态栏位置 0底部 1顶部 */
static const float mfact                  = 0.55;       /* 主工作区 大小比例 */
static const int   nmaster                = 1;         /* 主工作区 窗口数量 */
static const int nstack                   = 0;         /* number of clients in primary stack area */
static const unsigned int snap            = 10;        /* 边缘依附宽度 */
static const unsigned int baralpha        = 0xc0;      /* 状态栏透明度 */
static const unsigned int borderalpha     = 0xdd;      /* 边框透明度 */


//=============================================================================
// 字体配置
//=============================================================================
static const char *fonts[]               = {
      //"monospace:size=15",
			"Monaco:style=Regular:size=11",
			"Symbols Nerd Font:style=2048-em:size=17",
		  "Microsoft YaHei:size=11:style=Regular:antialias=true:autohint:true",
			"JoyPixels:size=13:antialias=true:autohint=true"
};  


//=============================================================================
/* 颜色设置 ColFg, ColBg, ColBorder */ 
//=============================================================================
static const char *colors[][3] = {        
    [SchemeNorm] = { "#bbbbbb", "#333333", "#444444" },
    [SchemeSel] = { "#ffffff", "#37474F", "#42A5F5" }, // #abd687
    [SchemeSelGlobal] = { "#ffffff", "#37474F", "#FFC0CB" },
    [SchemeHid] = { "#dddddd", NULL, NULL },
    [SchemeSystray] = { NULL, "#7799AA", NULL },
    [SchemeUnderline] = { "#7799AA", NULL, NULL }, 
    [SchemeNormTag] = { "#bbbbbb", "#333333", NULL },
    [SchemeSelTag] = { "#eeeeee", "#333333", NULL },
    [SchemeBarEmpty] = { NULL, "#111111", NULL },
};
//-----------------------------------------------------------------------------
/* 透明度设置 ColFg, ColBg, ColBorder */ 
static const unsigned int alphas[][3] = {         
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha }, 
    [SchemeSel] = { OPAQUE, baralpha, borderalpha },
    [SchemeSelGlobal] = { OPAQUE, baralpha, borderalpha },
    [SchemeNormTag] = { OPAQUE, baralpha, borderalpha }, 
    [SchemeSelTag] = { OPAQUE, baralpha, borderalpha },
    [SchemeBarEmpty] = { NULL, 0xa0a, NULL },
    [SchemeStatusText] = { OPAQUE, 0x88, NULL },
};


//=============================================================================
/* 防止误关闭，一些程序关闭快捷键不会响应 */
//=============================================================================
static const char *disablekillclient[] = {
  "wemeetapp", // 腾讯会议顶栏,防止开会时关闭桌面共享了，没有这个需求可以注释
  "tmux", // tmux不要误关了，防止有子窗口还在运行
  "QQ", // QQ关闭后会直接退出,不会最小化,微信不需要这个操作
};


//=============================================================================
//                      自启动脚本
//=============================================================================
static const char *autostartscript = "~/my_desktop/dwm/autostart/autostart.sh";
//-----------------------------------------------------------------------------
//                     状态栏启动脚本
static const char *statusbarscript = "~/my_desktop/dwm/statusbar/statusbar.py";//gxt_kt



//=============================================================================
//                 自定义 scratchpad
//=============================================================================
static const char scratchpadname[] = "scratchpad";


//=============================================================================
//                   tag icon 
//            ﮸  ﭮ 切  ﬐ // 增补图标
//            
//=============================================================================
static const char *tags[] = { "", "", "", "", "" };


//=============================================================================
// 以下为临时文件，判断启动状态或相关的，一般不用动
// 如果发现启动异常，相关脚本或任务没有执行，可以删除这些临时文件再重启电脑
//=============================================================================
/* Lockfile */ // doublepressquitPatch
static char lockfile[] = "/tmp/dwm.lock"; 
//-----------------------------------------------------------------------------
/* Super-Ctrl-Shift-Esc 热重启dwm后不会重复执行autostart脚本 */
static const char* avoid_repeat_auto_start = "/tmp/dwm_avoid_repeat_auto_start.lock"; // doublepressquitPatch
//-----------------------------------------------------------------------------
// restore after restart
#define SESSION_FILE "/tmp/dwm-session"
#define SESSION_TAG_FILE "/tmp/dwm-tag-session"


//=============================================================================
// 规则设置
//=============================================================================
static const Rule rules[] = {
//-----------------------------------------------------------------------------
  // class         instance    title      tags_mask  isfloating isglobal isnoborder monitor
    {"obs",         NULL,       NULL,      1 << 5,      0,         0,        0,        -1},
    {"chrome",      NULL,       NULL,      1 << 6,      0,         0,        0,        -1},
    {"Chromium",    NULL,       NULL,      1 << 6,      0,         0,        0,        -1},
    {"music",       NULL,       NULL,      1 << 7,      0,         0,        0,        -1},
    {"wemeetapp",   NULL,       NULL,      TAGMASK,     1,         1,        0,        -1}, // 腾讯会议有bug,需要使用global类型
    {"copyq",       NULL,       NULL,      TAGMASK,     1,         1,        0,        -1},
    {"Nitrogen",    NULL,       NULL,      TAGMASK,     1,         0,        0,        -1},
    {NULL,          NULL,     "图片查看",  TAGMASK,     1,         0,        0,        -1},  // qq image preview title
    {NULL,          NULL, "Image Preview", TAGMASK,     1,         0,        0,        -1},  // wechat image preview title
    // {"钉钉",      NULL,       NULL,      TAGMASK, 1, 1,          0,        -1 },
    // {"dingtalk",   NULL,    NULL,      TAGMASK, 1,          1,          0, -1 }, 
    // {"com.alibabainc.dingtalk",    NULL,  NULL, TAGMASK, 1,  1,  0,  -1 }, 
    // {"tblive",  NULL,     NULL,  TAGMASK, 1, 1,0,        -1 }, 
    // { NULL, NULL, "broken", TAGMASK,   1, 0, 0,    -1 }, // qq upload file's win is broken 
    // { NULL, NULL, "图片预览",  0,  1,   0,   0,  -1 },

//-----------------------------------------------------------------------------
    // 以下适用于无边框，浮动窗口，全局显示，请勿更改
    {"FG", NULL, NULL, TAGMASK, 1, 1, 1, -1},      // 浮动 + 全局
    {"FN", NULL, NULL, 0, 1, 0, 1, -1},            // 浮动 + 无边框
    {"GN", NULL, NULL, TAGMASK, 0, 1, 1, -1},      // 全局 + 无边框
    {"FGN", NULL, NULL, TAGMASK, 1, 1, 1, -1},     // 浮动 + 全局 + 无边框
    {"float", NULL, NULL, 0, 1, 0, 0, -1},         // 浮动
    {"noborder", NULL, NULL, 0, 0, 0, 1, -1},      // 无边框
    {"global", NULL, NULL, TAGMASK, 1, 1, 0, -1},  // 全局
};


//=============================================================================
// overview : win+tab
//=============================================================================
static const char *overviewtag = "OVERVIEW";
static const Layout overviewlayout = { "", overview };


//=============================================================================
/* 自定义布局
 * 有两套布局:
 *   第一套是基于yaoccc的，我写了一个tile_right做补充
 *   第二套是基于flextile布局做的更改加补充
 *
 * 建议第一次使用使用第一套布局，熟悉以后采用第二套
 * 第一套大多数情况够用了，入门第一套足够了
 * 第二套相对第一套做了很多补充，完全包含了第一套的内容
 * 第二套布局过多，很多都用不到，建议用不到的注释掉，方便快速切换想要的布局
 */
//=============================================================================
#if 0 // 0为第一套，1第二套
//-----------------------------------------------------------------------------
static const Layout layouts[] = {
  //symbol     arrange function   
    {"﬿",        tile},      /* 主次栈 */
    {"﩯",     magicgrid},   /* 网格 */
    {"TR",    tile_right},   /* 主次栈 主侧放在右侧*/
    {NULL,       NULL} //最后一个需要是NULL,NULL,cyclelayout,请勿更改
};
//-----------------------------------------------------------------------------
#else
//-----------------------------------------------------------------------------
static const Layout layouts[] = {
	/* symbol     arrange function, { nmaster, nstack, layout, master axis, stack axis, secondary stack axis } */
	{ "﬿",          flextile,         { -1, -1, SPLIT_VERTICAL, TOP_TO_BOTTOM, TOP_TO_BOTTOM, 0, NULL } }, // default tile layout
  { "﩯",        magicgrid,{0} },    /* 网格 */
	// { "><>",      NULL,             {0} },    /* no layout function means floating behavior */ // 已经被win+shift+f 替代，不用这个
	// { "[M]",      flextile,         { -1, -1, NO_SPLIT, MONOCLE, MONOCLE, 0, NULL } }, // monocle
	// { "|||",      flextile,         { -1, -1, SPLIT_VERTICAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, 0, NULL } }, // columns (col) layout //其实就是tile
	// { ">M>",      flextile,         { -1, -1, FLOATING_MASTER, LEFT_TO_RIGHT, LEFT_TO_RIGHT, 0, NULL } }, // floating master
	// { "[D]",      flextile,         { -1, -1, SPLIT_VERTICAL, TOP_TO_BOTTOM, MONOCLE, 0, NULL } }, // deck
	{ "TTT",      flextile,         { -1, -1, SPLIT_HORIZONTAL, LEFT_TO_RIGHT, LEFT_TO_RIGHT, 0, NULL } }, // bstack
	{ "===",      flextile,         { -1, -1, SPLIT_HORIZONTAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, 0, NULL } }, // bstackhoriz
	// { ":::",      flextile,         { -1, -1, SPLIT_HORIZONTAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, 0, monoclesymbols } }, // centeredmaster
	{ "gapless",      flextile,         { -1, -1, NO_SPLIT, GAPPLESSGRID, GAPPLESSGRID, 0, NULL } }, // gappless grid
	{ "[\\]",     flextile,         { -1, -1, NO_SPLIT, DWINDLE, DWINDLE, 0, NULL } }, // fibonacci dwindle
	{ "(@)",      flextile,         { -1, -1, NO_SPLIT, SPIRAL, SPIRAL, 0, NULL } }, // fibonacci spiral
	{ "[T]",      flextile,         { -1, -1, SPLIT_VERTICAL, LEFT_TO_RIGHT, TATAMI, 0, NULL } }, // tatami mats
	{ NULL,       NULL,             {0} },//最后一个需要是NULL,NULL,cyclelayout,请勿更改
 };
#endif


//=============================================================================
//                          不需要更改
//=============================================================================
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define MODKEY Mod4Mask  // Super
#define Alt Mod1Mask  // Alt
#define TAGKEYS(KEY, TAG, cmd) \
    { MODKEY,              KEY, view,       {.ui = 1 << TAG, .v = cmd} }, \
    { MODKEY|ShiftMask,    KEY, tag,        {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,  KEY, toggleview, {.ui = 1 << TAG} }, \



//=============================================================================
//                           按键配置
//  modifier            key              function          argument 
//=============================================================================
static Key keys[] = {
//=============================================================================
//           一些基础快捷键，符合配置的核心按键思想，不建议更改
//=============================================================================
  	{ MODKEY,              XK_h,       focusdir,          {.i = 0 } },  // 切换聚焦窗口
  	{ MODKEY,              XK_j,       focusdir,          {.i = 1 } },  // 切换聚焦窗口
  	{ MODKEY,              XK_k,       focusdir,          {.i = 2 } },  // 切换聚焦窗口
  	{ MODKEY,              XK_l,       focusdir,          {.i = 3 } },  // 切换聚焦窗口
//-----------------------------------------------------------------------------
    { MODKEY|ShiftMask,    XK_h,       ExchangeClient,    {.i = 0} },   // 移动窗口
    { MODKEY|ShiftMask,    XK_j,       ExchangeClient,    {.i = 1 } },  // 移动窗口
    { MODKEY|ShiftMask,    XK_k,       ExchangeClient,    {.i = 2 } },  // 移动窗口
    { MODKEY|ShiftMask,    XK_l,       ExchangeClient,    {.i = 3} },   // 移动窗口
//-----------------------------------------------------------------------------
    { MODKEY,              XK_a,     toggleoverview,    {0} },        // 显示所有tag 或 跳转到聚焦窗口的tag */
    { Mod1Mask,            XK_Tab,     focusstack,        {.i = +1} },  // 本tag内切换聚焦窗口 
    { Mod1Mask|ShiftMask,  XK_Tab,     focusstack,        {.i = -1} },  // 本tag内切换聚焦窗口  
//-----------------------------------------------------------------------------
    { MODKEY,              XK_q,       togglefloating,    {0} },        // 开启/关闭 当前窗口的float模式
    { MODKEY|ShiftMask,    XK_q,       toggleallfloating, {0} },        // 开启/关闭 当前tag 的float模式
    { MODKEY,              XK_g,       toggleglobal,      {0} },        // 开启/关闭 全局 
//-----------------------------------------------------------------------------
    { MODKEY|Alt,              XK_Left,   setmfact,          {.f = -0.01} }, // 缩小主工作区
    { MODKEY|Alt,              XK_Right,  setmfact,          {.f = +0.01} }, // 放大主工作区
//-----------------------------------------------------------------------------
    { MODKEY|ShiftMask,              XK_w,       killclient,        {0} },        // 关闭当前窗口
    { MODKEY|ControlMask|ShiftMask,  XK_w,       forcekillclient,   {0} },        // 强制关闭当前窗口
    { MODKEY|Alt,          XK_F12,  quit,              {0} },        // 退出dwm
    { MODKEY|ControlMask|ShiftMask, XK_F12, quit,      {1} },        // 重启dwm 
//-----------------------------------------------------------------------------
  

//=============================================================================
//                 一些其它快捷键，可以根据需要和习惯更改
//=============================================================================
    { MODKEY,              XK_d,       hidewin,           {0} },          // 隐藏窗口
    { MODKEY|ShiftMask,    XK_d,       restorewin,        {0} },          // 取消隐藏窗口
//-----------------------------------------------------------------------------
    { MODKEY,              XK_o,       showonlyorall,     {0} },          // 单窗口
    { MODKEY,              XK_f,     fullscreen,        {0} },          // 开启/关闭 全屏
//-----------------------------------------------------------------------------
    { MODKEY|ShiftMask,    XK_f,                 togglebar,             {0} },          // 开启/关闭 状态栏 
    { MODKEY|ControlMask|ShiftMask,              XK_f,togglesystray,    {0} },          // 开启/关闭 托盘栏
//-----------------------------------------------------------------------------
    { MODKEY|ControlMask,  XK_equal,   setgap,            {.i = +5} },    // gap增大
    { MODKEY|ControlMask,  XK_minus,   setgap,            {.i = -5} },    // gap减小
    { MODKEY|ControlMask,  XK_space,   setgap,            {.i = 0} },     // gap重置
//-----------------------------------------------------------------------------
    { MODKEY|ControlMask,              XK_k,    movewin,           {.ui = UP} },   // 移动窗口
    { MODKEY|ControlMask,              XK_j,    movewin,           {.ui = DOWN} }, // 移动窗口
    { MODKEY|ControlMask,              XK_h,    movewin,           {.ui = LEFT} }, // 移动窗口
    { MODKEY|ControlMask,              XK_l,    movewin,           {.ui = RIGHT} },// 移动窗口  

    { MODKEY|Alt,                      XK_k,    resizewin,         {.ui = V_REDUCE} },// 调整窗口 
    { MODKEY|Alt,                      XK_j,    resizewin,         {.ui = V_EXPAND} },// 调整窗口      
    { MODKEY|Alt,                      XK_h,    resizewin,         {.ui = H_REDUCE} },// 调整窗口     
    { MODKEY|Alt,                      XK_l,    resizewin,         {.ui = H_EXPAND} },// 调整窗口    
//-----------------------------------------------------------------------------
    { MODKEY,                          XK_s,    zoom,              {0} },          // 将当前聚焦窗口置为主窗口
//-----------------------------------------------------------------------------
  

//=============================================================================
//                      基础和flextile 布局相关
//=============================================================================
    { MODKEY,              XK_e,       incnmaster,        {.i = +1} },    // 改变主窗口数 1或2 都有效
    { MODKEY|ShiftMask,    XK_comma,   cyclelayout,       {.i = -1 } },   // 循环布局 都有效
    { MODKEY|ShiftMask,    XK_period,  cyclelayout,       {.i = +1 } },   // 循环布局 都有效
//-----------------------------------------------------------------------------
    { MODKEY,              XK_i,       incnstack,         {.i = +1 } },   // 增加从堆栈数 仅flextile有效
    { MODKEY,              XK_u,       incnstack,         {.i = -1 } },   // 减少从堆栈数 仅flextile有效
  	{ MODKEY|ControlMask,  XK_Return,  mirrorlayout,      {0} },          // 翻转主区域和堆栈区域 仅flextile有效
//-----------------------------------------------------------------------------
    { MODKEY|ControlMask,  XK_comma,   rotatelayoutaxis,  {.i = -1 } },   // 循环另一种布局 仅flextile有效
    { MODKEY|ControlMask,  XK_period,  rotatelayoutaxis,  {.i = +1 } },   // 循环另一种布局 仅flextile有效
//-----------------------------------------------------------------------------
    // It's just need to map one key to change layout between layouts[0] and layouts[1].
    // { MODKEY|ShiftMask,  XK_o,      selectlayout,     {.v = &layouts[0]} }, // 切换到第1个布局 
    { MODKEY|ShiftMask,   XK_space,      selectlayout,     {.v = &layouts[1]} }, // 切换到第2个布局 
//-----------------------------------------------------------------------------


//=============================================================================
//                              多显示器配置
//=============================================================================
    { MODKEY,               XK_n,        focusmon,         {.i = -1} },     // 光标移动到另一个显示器
    { MODKEY,               XK_m,        focusmon,         {.i = +1} },     // 光标移动到另一个显示器
    { MODKEY|ShiftMask,     XK_n,        tagmon,           {.i = -1} },     // 将聚焦窗口移动到另一个显示器 
    { MODKEY|ShiftMask,     XK_m,        tagmon,           {.i = +1} },     // 将聚焦窗口移动到另一个显示器   
//-----------------------------------------------------------------------------


//=============================================================================
//                              其它命令
//=============================================================================
    // Notice that if you first use copyq , Remeber config 1.disable tray show 2.Enable hidden mainwindow. Then you can use this better.
    { MODKEY,               XK_v,        spawn,   SHCMD("copyq toggle") },  // copyq
    { MODKEY,               XK_b,           togglescratch, SHCMD("alacritty -t scratchpad --class floatingTerminal")  }, // 打开/隐藏scrtachpad
    { MODKEY,               XK_Return,          spawn,   SHCMD("alacritty") },                                          // 新打开一个终端
    // { MODKEY|ShiftMask,    XK_Return,   spawn,   SHCMD("alacritty -t term-global --class globalingTerminal") }, // 新打开一个浮动终端
    { MODKEY|ShiftMask,              XK_Return,        spawn,   SHCMD("alacritty -t term-float --class floatingTerminal") }, // 新打开一个全局浮动终端
    { Alt|ShiftMask,       XK_u,        spawn,   SHCMD("alacritty -e ranger") }, // 打开资源管理器

    { Alt,                                  XK_l,                         spawn,            SHCMD("rofi -show drun -theme mine -show-icons") },
    { Alt|ShiftMask,                        XK_l,                         spawn,            SHCMD("rofi -show run -theme mine -show-icons") },
    { Alt|ControlMask|ShiftMask,            XK_l,                         spawn,            SHCMD("rofi -show ssh -theme mine -show-icons") },

    { MODKEY,                               XK_F1,                        spawn,            SHCMD("xbacklight -get -5") },
    { MODKEY,                               XK_F2,                        spawn,            SHCMD("xbacklight -get +5") },
    { MODKEY,                               XK_F3,                        spawn,            SHCMD("amixer set Master toggle") },

    { Alt,                                  XK_s,                         spawn,            SHCMD("flameshot gui -p ~/Screenshot/shot") },
    { Alt|ShiftMask,                        XK_s,                         spawn,            SHCMD("flameshot gui -d 3000 -p ~/Screenshot/shot") },
    { Alt|ControlMask|ShiftMask,            XK_s,                         spawn,            SHCMD("flameshot full -p ~/Screenshot/full") },

    { Alt|ControlMask,                      XK_l,                         spawn,            SHCMD("slock") },
    { Alt,                                  XK_u,                         spawn,            SHCMD("pcmanfm") },
    { Alt,                                  XK_y,                         spawn,            SHCMD("/home/louis/.config/Build/Qv2ray/./Qv2ray") },
    { Alt,                                  XK_k,                         spawn,            SHCMD("screenkey -p fixed -g 50%x8%+25%-11%") },
    { Alt|ShiftMask,                        XK_k,                         spawn,            SHCMD("killall screenkey") },


//-----------------------------------------------------------------------------
    // { MODKEY|ShiftMask,      XK_q,    spawn, SHCMD("kill -9 $(xprop | grep _NET_WM_PID | awk '{print $3}')") },//选中某个窗口并强制kill
    // { ShiftMask|ControlMask, XK_c,    spawn, SHCMD("xclip -o | xclip -selection c") }, // 进阶复制


//=============================================================================
//                         其它一些api可以自行启用
//                   启用时记得先检查按键避免重复定义冲突
//=============================================================================
    // { MODKEY|ShiftMask,    XK_j,        rotatestack,      {.i = +1 } },    /* rotate the stack*/
    // { MODKEY|ShiftMask,    XK_k,        rotatestack,      {.i = -1 } },    /* rotate the stack*/
//-----------------------------------------------------------------------------
    // { MODKEY|ShiftMask,    XK_Left,     viewtoleft,       {0} },      //聚焦到左边的tag 
    // { MODKEY|ShiftMask,    XK_Right,    viewtoright,      {0} },      // 聚焦到右边的tag 
//-----------------------------------------------------------------------------
    // { MODKEY|ShiftMask,    XK_Left,     tagtoleft,        {0} },      // 将本窗口移动到左边tag
    // { MODKEY|ShiftMask,    XK_Right,    tagtoright,       {0} },      // 将本窗口移动到右边tag 
//-----------------------------------------------------------------------------




//=============================================================================
    /* super key : 跳转到对应tag (可附加一条命令 若目标目录无窗口，则执行该命令) */
    /* super shift key : 将聚焦窗口移动到对应tag */
    /* key tag cmd */
    /* 注意从0开始算，会错开一个窗口 */
//=============================================================================
    TAGKEYS(XK_1, 0,  0)
    TAGKEYS(XK_2, 1,  0)
    TAGKEYS(XK_3, 2,  0)
    TAGKEYS(XK_4, 3,  0)
    TAGKEYS(XK_5, 4,  0)
//-----------------------------------------------------------------------------
    //TAGKEYS(XK_0, 8,  "linuxqq")
    //TAGKEYS(XK_w, 9,  "/opt/apps/com.qq.weixin.deepin/files/run.sh")
    //TAGKEYS(XK_l, 10, "/opt/apps/com.qq.weixin.work.deepin/files/run.sh")
    
    
//=============================================================================
//    根据相关信号执行指令，一般不需要更改，但需要注意相关指令包存在
//=============================================================================
{ 0, XF86XK_AudioMute,         spawn, SHCMD("pamixer -t;  python3 /home/gxt_kt/my_desktop/dwm/statusbar/vol.py notify ") },
{ 0, XF86XK_AudioRaiseVolume,  spawn, SHCMD("pamixer -i 5;python3 /home/gxt_kt/my_desktop/dwm/statusbar/vol.py notify ") },
{ 0, XF86XK_AudioLowerVolume,  spawn, SHCMD("pamixer -d 5;python3 /home/gxt_kt/my_desktop/dwm/statusbar/vol.py notify ") },
{ 0, XF86XK_AudioPause,        spawn, SHCMD("playerctl stop") },
{ 0, XF86XK_AudioPrev,         spawn, SHCMD("playerctl previous") },
{ 0, XF86XK_AudioNext,         spawn, SHCMD("playerctl next") },
{ 0, XF86XK_AudioPlay,         spawn, SHCMD("playerctl play") },
{ 0, XF86XK_AudioStop,         spawn, SHCMD("playerctl stop") },
{ 0, XF86XK_AudioStop,         spawn, SHCMD("playerctl stop") },
{ 0, XF86XK_MonBrightnessUp,   spawn, SHCMD("light -A 5; notify-send -r 9123 -h int:value:`light` -h string:hlcolor:#dddddd 'Backlight' " ) },
{ 0, XF86XK_MonBrightnessDown, spawn, SHCMD("light -U 5; notify-send -r 9123 -h int:value:`light` -h string:hlcolor:#dddddd 'Backlight' " ) },

};


//=============================================================================
// 按键操作和statusbar操作
// 以下内容一般不需要更改
//=============================================================================
static Button buttons[] = {
    /* click           event mask      button       function       argument  */
//=============================================================================
    /* 点击窗口标题栏操作 */
//=============================================================================
    { ClkWinTitle,         0,          Button3,     hideotherwins, {0} },                                   // 右键         |  点击标题     |  隐藏其他窗口仅保留该窗口
    { ClkWinTitle,         0,          Button1,     togglewin,     {0} },                                   // 左键         |  点击标题     |  切换窗口显示状态
//=============================================================================
    /* 点击窗口操作 */
//=============================================================================
    { ClkClientWin,        MODKEY,     Button1,     movemouse,     {0} },                                   // super+左键  |  拖拽窗口     |  拖拽窗口
    { ClkClientWin,        MODKEY,     Button3,     resizemouse,   {0} },                                   // super+右键  |  拖拽窗口     |  改变窗口大小
//=============================================================================
    /* 点击tag操作 */
//=============================================================================
    { ClkTagBar,           0,          Button1,     view,          {0} },                                   // 左键        |  点击tag      |  切换tag
	  { ClkTagBar,           0,          Button3,     toggleview,    {0} },                                   // 右键        |  点击tag      |  切换是否显示tag
    { ClkTagBar,           MODKEY,     Button1,     tag,           {0} },                                   // super+左键  |  点击tag      |  将窗口移动到对应tag
    { ClkTagBar,           0,          Button4,     viewtoleft,    {0} },                                   // 鼠标滚轮上  |  tag          |  向前切换tag
	  { ClkTagBar,           0,          Button5,     viewtoright,   {0} },                                   // 鼠标滚轮下  |  tag          |  向后切换tag

//=============================================================================
    /* 点击bar空白处 有需要自行启动 */
//=============================================================================
    // { ClkBarEmpty,         0,          Button1,     spawn,       SHCMD("~/scripts/call_rofi.sh window") },        // 左键        |  bar空白处    |  rofi 执行 window
    // { ClkBarEmpty,         0,          Button3,     spawn,       SHCMD("~/scripts/call_rofi.sh drun") },          // 右键        |  bar空白处    |  rofi 执行 drun

//=============================================================================
    /* 点击状态栏操作 */
 // 根据状态栏的信号执行  statusbarscript signal L/M/R/U/D
//=============================================================================
    { ClkStatusText,       0,          Button1,     clickstatusbar,{0} },   // 左键        |  点击状态栏   | 
    { ClkStatusText,       0,          Button2,     clickstatusbar,{0} },   // 中键        |  点击状态栏   |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal M
    { ClkStatusText,       0,          Button3,     clickstatusbar,{0} },   // 右键        |  点击状态栏   |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal R
    { ClkStatusText,       0,          Button4,     clickstatusbar,{0} },   // 鼠标滚轮上  |  状态栏       |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal U
    { ClkStatusText,       0,          Button5,     clickstatusbar,{0} },   // 鼠标滚轮下  |  状态栏       |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal D
};
