#include "ui_main.h"

//录音查询界面
const struct StructHead_CtrlType Struct_CtrlRecordQueryWnd[] =
{
	{DEF_CTRLTEXT    ,0,8,0,0,(uint8_t*)"0000年00月00日",	0,0     },
	{DEF_CTRLTEXT    ,1,16,2,0,(uint8_t*)"00时00分00秒",	0,0     },
	{DEF_CTRLTEXT    ,2,8,6,0,(uint8_t*)"00:00  000/000",	0,0     },
};

//录音删除界面
const struct StructHead_CtrlType Struct_CtrlRecordDelWnd[] =
{
	{DEF_CTRLTEXT    ,0,8,0,0,(uint8_t*)"0000年00月00日",	0,0     },
	{DEF_CTRLTEXT    ,1,16,2,0,(uint8_t*)"00时00分00秒",	0,0     },
	{DEF_CTRLTEXT    ,2,32,5,0,(uint8_t*)"000/000",			0,0     },
};

//文件导入界面
const struct StructHead_CtrlType Struct_CtrlFileInputWnd[] =
{
	{DEF_CTRLTEXT    ,0,24,2,0,(uint8_t*)"文件导入中",	0, 0        },
	{DEF_CTRLTEXT    ,1,24,4,0,(uint8_t*)"请稍候操作",	0, 0        },
};

//删除全部文件界面
const struct StructHead_CtrlType Struct_CtrlDelAllFileWnd[] =
{
	{DEF_CTRLTEXT    ,0,24,2,0,(uint8_t*)"文件删除中",	0, 0        },
	{DEF_CTRLTEXT    ,1,24,4,0,(uint8_t*)"请稍候操作",	0, 0        },
};
