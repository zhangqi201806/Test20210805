#include "ui_main.h"

//¼����ѯ����
const struct StructHead_CtrlType Struct_CtrlRecordQueryWnd[] =
{
	{DEF_CTRLTEXT    ,0,8,0,0,(uint8_t*)"0000��00��00��",	0,0     },
	{DEF_CTRLTEXT    ,1,16,2,0,(uint8_t*)"00ʱ00��00��",	0,0     },
	{DEF_CTRLTEXT    ,2,8,6,0,(uint8_t*)"00:00  000/000",	0,0     },
};

//¼��ɾ������
const struct StructHead_CtrlType Struct_CtrlRecordDelWnd[] =
{
	{DEF_CTRLTEXT    ,0,8,0,0,(uint8_t*)"0000��00��00��",	0,0     },
	{DEF_CTRLTEXT    ,1,16,2,0,(uint8_t*)"00ʱ00��00��",	0,0     },
	{DEF_CTRLTEXT    ,2,32,5,0,(uint8_t*)"000/000",			0,0     },
};

//�ļ��������
const struct StructHead_CtrlType Struct_CtrlFileInputWnd[] =
{
	{DEF_CTRLTEXT    ,0,24,2,0,(uint8_t*)"�ļ�������",	0, 0        },
	{DEF_CTRLTEXT    ,1,24,4,0,(uint8_t*)"���Ժ����",	0, 0        },
};

//ɾ��ȫ���ļ�����
const struct StructHead_CtrlType Struct_CtrlDelAllFileWnd[] =
{
	{DEF_CTRLTEXT    ,0,24,2,0,(uint8_t*)"�ļ�ɾ����",	0, 0        },
	{DEF_CTRLTEXT    ,1,24,4,0,(uint8_t*)"���Ժ����",	0, 0        },
};
