#include "ui_main.h"

//功放设置界面
const struct StructHead_CtrlType Struct_CtrlPowerAmpCfgWnd[] =
{
	{DEF_CTRLTEXT    , 0,0,0,0,(uint8_t*)"配置01号功放",0,0        },	
	{DEF_CTRLTEXT    , 1,0,2,0,(uint8_t*)"请按确认键"  ,0,0        },
	{DEF_CTRLTEXT    , 2,0,4,0,(uint8_t*)"保存01号功放",0,0        },		
};

//时间设置界面
const struct StructHead_CtrlType Struct_CtrlSysTimeCfgWnd[] =
{
	{DEF_CTRLTEXT    , 0,  8,0,0,(uint8_t*)"请输入当前时间"  ,0,0 },
  {DEF_CTRLTEXT    , 1,  8,4,2,(uint8_t*)"20"             ,0,0 },
	{DEF_CTRLTEXT    , 2, 40,4,0,(uint8_t*)"年"             ,0,0 },
	{DEF_CTRLTEXT    , 3, 56,4,1,0                        ,0,0 },
	{DEF_CTRLTEXT    , 4, 72,4,0,(uint8_t*)"月"              ,0,0 },
	{DEF_CTRLTEXT    , 5, 88,4,1,0                        ,0,0 },
	{DEF_CTRLTEXT    , 6,104,4,0,(uint8_t*)"日"              ,0,0 },
	{DEF_CTRLTEXT    , 7, 16,6,1,0                        ,0,0 },
	{DEF_CTRLTEXT    , 8, 32,6,0,(uint8_t*)"时"              ,0,0 },
	{DEF_CTRLTEXT    , 9, 48,6,1,0                        ,0,0 },
	{DEF_CTRLTEXT    ,10, 64,6,0,(uint8_t*)"分"              ,0,0 },
	{DEF_CTRLTEXT    ,11, 80,6,1,0                        ,0,0 },
	{DEF_CTRLTEXT    ,12, 96,6,0,(uint8_t*)"秒"              ,0,0 },
};

//本机地址界面
const struct StructHead_CtrlType Struct_CtrlAddressCfgWnd[] =
{
	{DEF_CTRLTEXT    ,0,0,3,0,(uint8_t*)"本机地址:000",0,0        },
};


//密码设置界面
const struct StructHead_CtrlType Struct_CtrlUserPasswordCfgWnd[] =
{
	{DEF_CTRLTEXT    ,0,0,0,0,(uint8_t*)"请输入新用户密码",0,0    },
	{DEF_CTRLTEXT    ,1,48,4,0,(uint8_t*)"****",0,0       },
};

//密码设置界面
const struct StructHead_CtrlType Struct_CtrlGovPasswordCfgWnd[] =
{
	{DEF_CTRLTEXT    ,0,0,0,0,(uint8_t*)"请输入新管理密码",0,0    },
	{DEF_CTRLTEXT    ,1,48,4,0,(uint8_t*)"****",0,0       },
};

//密码确认保存界面
const struct StructHead_CtrlType Struct_CtrlPasswordSaveWnd[] =
{
	{DEF_CTRLTEXT    ,0,16,2,0,(uint8_t*)"保存新密码吗",0,0    },
	{DEF_CTRLTEXT    ,1,24,4,0,(uint8_t*)"确认键保存",0,0     },
	{DEF_CTRLTEXT    ,2,24,6,0,(uint8_t*)"取消键退出",0,0     },
};

//核对用户密码界面
const struct StructHead_CtrlType Struct_CtrlCheckUserPasswordWnd[] =
{
	{DEF_CTRLTEXT    ,0,0,0,0,(uint8_t*)"请输入用户密码",0,0    },
	{DEF_CTRLTEXT    ,1,48,4,0,(uint8_t*)"****",0,0       },
};

//核对管理密码界面
const struct StructHead_CtrlType Struct_CtrlCheckGovPasswordWnd[] =
{
	{DEF_CTRLTEXT    ,0,0,0,0,(uint8_t*)"请输入管理密码",0,0    },
	{DEF_CTRLTEXT    ,1,48,4,0,(uint8_t*)"****",0,0       },
};

//恢复出厂设置界面
const struct StructHead_CtrlType Struct_CtrlFactoryCfgWnd[] =
{
	{DEF_CTRLTEXT    ,0,24,2,0,(uint8_t*)"恢复设置中",	0, 0        },
	{DEF_CTRLTEXT    ,1,24,4,0,(uint8_t*)"请稍候操作",	0, 0        },
};


