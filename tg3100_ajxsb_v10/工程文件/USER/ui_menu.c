#include "ui_main.h"

//һ���˵�
void Fun_GotoFileoperateWnd(void)  //1.�ļ�����
{
    Fun_WndPreDeal(DEF_FileOperate);
}

void Fun_GotoPoweAmpRegisWnd(void) //2.����ע��
{	
    Fun_WndPreDeal(DEF_PoweAmpRegis);
}

void Fun_GotoSystemConfigWnd(void) //3.ϵͳ����
{
	NextWnd_Flag = 4;
  Fun_WndPreDeal(DEF_CheckGovPassword);//�˶Թ�������
    //Fun_WndPreDeal(DEF_SystemConfig);//�˶Թ�������
}

void Fun_GotoFactoryConfigWnd(void) //4.��������
{
	NextWnd_Flag = 5;
  Fun_WndPreDeal(DEF_CheckGovPassword);//�˶Թ�������
    //Fun_WndPreDeal(DEF_SystemConfig);//�˶Թ�������
}

//�����˵�
//1.�ļ���������
void Fun_GotoRecordQueryWnd(void)  //1.¼����ѯ
{
    Fun_WndPreDeal(DEF_RecordQuery);
}

void Fun_GotoRecordDelWnd(void)   //2.¼��ɾ��
{
	NextWnd_Flag = 1;
	Fun_WndPreDeal(DEF_CheckGovPassword);//�˶Թ�������
	//Fun_WndPreDeal(DEF_RecordDel);
}

void Fun_GotoFileInputWnd(void)   //3.�ļ�����
{
	NextWnd_Flag = 2;
	Fun_WndPreDeal(DEF_CheckGovPassword);//�˶Թ�������
	//Fun_WndPreDeal(DEF_FileInput);
}

void Fun_GotoDelAllFileWnd(void)   //4.ɾ��ȫ���ļ�
{
	NextWnd_Flag = 3;
	Fun_WndPreDeal(DEF_CheckGovPassword);//�˶Թ�������
	//Fun_WndPreDeal(DEF_DelAllFile);
}

//3.ϵͳ��������
void Fun_GotoPoweAmpCfgWnd(void)  //1.��������
{
    Fun_WndPreDeal(DEF_PowerAmpCfg);
}

void Fun_GotoTimeCfgWnd(void)     //2.ʱ������
{
    Fun_WndPreDeal(DEF_TimeCfg);
}

void Fun_GotoAddressCfgWnd(void)  //3.������ַ
{
    Fun_WndPreDeal(DEF_AddressCfg);
}
void Fun_GotoPasswordCfgWnd(void) //4.��������
{
    Fun_WndPreDeal(DEF_PasswordCfg);
}

//�����˵�
void Fun_GotoUserPasswordCfgWnd(void) //1.�û���������
{
    Fun_WndPreDeal(DEF_UserPasswordCfg);
}
void Fun_GotoGovPasswordCfgWnd(void) //2.������������
{
    Fun_WndPreDeal(DEF_GovPasswordCfg);
}

//һ���˵�����
//01ϵͳ���˵�����ؼ�1
const struct StructHead_CtrlType Struct_CtrlMainMenuWnd[] =
{
	{DEF_CTRLBUTTON  ,0,24,0,0,(uint8_t*)"1.�ļ�����",1,Fun_GotoFileoperateWnd    },
	{DEF_CTRLBUTTON  ,1,24,2,0,(uint8_t*)"2.����ע��",1,Fun_GotoPoweAmpRegisWnd   },
	{DEF_CTRLBUTTON  ,2,24,4,0,(uint8_t*)"3.ϵͳ����",1,Fun_GotoSystemConfigWnd   },
	{DEF_CTRLBUTTON  ,3,24,6,0,(uint8_t*)"4.��������",1,Fun_GotoFactoryConfigWnd   },
};

//�����˵�����
//02�ļ���������ؼ�2
const struct StructHead_CtrlType Struct_CtrlFileOperateWnd[] =
{
	{DEF_CTRLBUTTON  ,0,24,0,0,(uint8_t*)"1.¼����ѯ",1,Fun_GotoRecordQueryWnd     },
	{DEF_CTRLBUTTON  ,1,24,2,0,(uint8_t*)"2.¼��ɾ��",1,Fun_GotoRecordDelWnd       },
	{DEF_CTRLBUTTON  ,2,24,4,0,(uint8_t*)"3.�ļ�����",1,Fun_GotoFileInputWnd       },
	{DEF_CTRLBUTTON  ,3,24,6,0,(uint8_t*)"4.ȫ��ɾ��",1,Fun_GotoDelAllFileWnd      },
  
};

//04ϵͳ���ý���ؼ�4
const struct StructHead_CtrlType Struct_CtrlSystemCfgWnd[] =
{
	{DEF_CTRLBUTTON  ,0,24,0,0,(uint8_t*)"1.��������",1,Fun_GotoPoweAmpCfgWnd     },
	{DEF_CTRLBUTTON  ,1,24,2,0,(uint8_t*)"2.ʱ������",1,Fun_GotoTimeCfgWnd        },
	{DEF_CTRLBUTTON  ,2,24,4,0,(uint8_t*)"3.������ַ",1,Fun_GotoAddressCfgWnd     },
  {DEF_CTRLBUTTON  ,3,24,6,0,(uint8_t*)"4.��������",1,Fun_GotoPasswordCfgWnd    },
};

//11�������ý���ؼ�11
const struct StructHead_CtrlType Struct_CtrlPasswordCfgWnd[] =
{
	{DEF_CTRLBUTTON    ,0,24,0,0,(uint8_t*)"1.�û�����",0,Fun_GotoUserPasswordCfgWnd       },
	{DEF_CTRLBUTTON    ,1,24,2,0,(uint8_t*)"2.��������",0,Fun_GotoGovPasswordCfgWnd        },
};




