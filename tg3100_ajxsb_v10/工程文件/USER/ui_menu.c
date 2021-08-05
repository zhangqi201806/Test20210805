#include "ui_main.h"

//一级菜单
void Fun_GotoFileoperateWnd(void)  //1.文件操作
{
    Fun_WndPreDeal(DEF_FileOperate);
}

void Fun_GotoPoweAmpRegisWnd(void) //2.功放注册
{	
    Fun_WndPreDeal(DEF_PoweAmpRegis);
}

void Fun_GotoSystemConfigWnd(void) //3.系统设置
{
	NextWnd_Flag = 4;
  Fun_WndPreDeal(DEF_CheckGovPassword);//核对管理密码
    //Fun_WndPreDeal(DEF_SystemConfig);//核对管理密码
}

void Fun_GotoFactoryConfigWnd(void) //4.出厂设置
{
	NextWnd_Flag = 5;
  Fun_WndPreDeal(DEF_CheckGovPassword);//核对管理密码
    //Fun_WndPreDeal(DEF_SystemConfig);//核对管理密码
}

//二级菜单
//1.文件操作下面
void Fun_GotoRecordQueryWnd(void)  //1.录音查询
{
    Fun_WndPreDeal(DEF_RecordQuery);
}

void Fun_GotoRecordDelWnd(void)   //2.录音删除
{
	NextWnd_Flag = 1;
	Fun_WndPreDeal(DEF_CheckGovPassword);//核对管理密码
	//Fun_WndPreDeal(DEF_RecordDel);
}

void Fun_GotoFileInputWnd(void)   //3.文件导入
{
	NextWnd_Flag = 2;
	Fun_WndPreDeal(DEF_CheckGovPassword);//核对管理密码
	//Fun_WndPreDeal(DEF_FileInput);
}

void Fun_GotoDelAllFileWnd(void)   //4.删除全部文件
{
	NextWnd_Flag = 3;
	Fun_WndPreDeal(DEF_CheckGovPassword);//核对管理密码
	//Fun_WndPreDeal(DEF_DelAllFile);
}

//3.系统设置下面
void Fun_GotoPoweAmpCfgWnd(void)  //1.功放设置
{
    Fun_WndPreDeal(DEF_PowerAmpCfg);
}

void Fun_GotoTimeCfgWnd(void)     //2.时间设置
{
    Fun_WndPreDeal(DEF_TimeCfg);
}

void Fun_GotoAddressCfgWnd(void)  //3.本机地址
{
    Fun_WndPreDeal(DEF_AddressCfg);
}
void Fun_GotoPasswordCfgWnd(void) //4.密码配置
{
    Fun_WndPreDeal(DEF_PasswordCfg);
}

//三级菜单
void Fun_GotoUserPasswordCfgWnd(void) //1.用户密码配置
{
    Fun_WndPreDeal(DEF_UserPasswordCfg);
}
void Fun_GotoGovPasswordCfgWnd(void) //2.管理密码配置
{
    Fun_WndPreDeal(DEF_GovPasswordCfg);
}

//一级菜单界面
//01系统主菜单界面控件1
const struct StructHead_CtrlType Struct_CtrlMainMenuWnd[] =
{
	{DEF_CTRLBUTTON  ,0,24,0,0,(uint8_t*)"1.文件操作",1,Fun_GotoFileoperateWnd    },
	{DEF_CTRLBUTTON  ,1,24,2,0,(uint8_t*)"2.功放注册",1,Fun_GotoPoweAmpRegisWnd   },
	{DEF_CTRLBUTTON  ,2,24,4,0,(uint8_t*)"3.系统设置",1,Fun_GotoSystemConfigWnd   },
	{DEF_CTRLBUTTON  ,3,24,6,0,(uint8_t*)"4.出厂设置",1,Fun_GotoFactoryConfigWnd   },
};

//二级菜单界面
//02文件操作界面控件2
const struct StructHead_CtrlType Struct_CtrlFileOperateWnd[] =
{
	{DEF_CTRLBUTTON  ,0,24,0,0,(uint8_t*)"1.录音查询",1,Fun_GotoRecordQueryWnd     },
	{DEF_CTRLBUTTON  ,1,24,2,0,(uint8_t*)"2.录音删除",1,Fun_GotoRecordDelWnd       },
	{DEF_CTRLBUTTON  ,2,24,4,0,(uint8_t*)"3.文件导入",1,Fun_GotoFileInputWnd       },
	{DEF_CTRLBUTTON  ,3,24,6,0,(uint8_t*)"4.全部删除",1,Fun_GotoDelAllFileWnd      },
  
};

//04系统设置界面控件4
const struct StructHead_CtrlType Struct_CtrlSystemCfgWnd[] =
{
	{DEF_CTRLBUTTON  ,0,24,0,0,(uint8_t*)"1.功放设置",1,Fun_GotoPoweAmpCfgWnd     },
	{DEF_CTRLBUTTON  ,1,24,2,0,(uint8_t*)"2.时间设置",1,Fun_GotoTimeCfgWnd        },
	{DEF_CTRLBUTTON  ,2,24,4,0,(uint8_t*)"3.本机地址",1,Fun_GotoAddressCfgWnd     },
  {DEF_CTRLBUTTON  ,3,24,6,0,(uint8_t*)"4.密码配置",1,Fun_GotoPasswordCfgWnd    },
};

//11密码设置界面控件11
const struct StructHead_CtrlType Struct_CtrlPasswordCfgWnd[] =
{
	{DEF_CTRLBUTTON    ,0,24,0,0,(uint8_t*)"1.用户密码",0,Fun_GotoUserPasswordCfgWnd       },
	{DEF_CTRLBUTTON    ,1,24,2,0,(uint8_t*)"2.管理密码",0,Fun_GotoGovPasswordCfgWnd        },
};




