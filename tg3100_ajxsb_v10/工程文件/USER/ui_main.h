#ifndef __UI_MAIN_H__
#define __UI_MAIN_H__

#include "stm32f10x.h"

struct StructHead_CtrlType
{
	uint8_t CtrlType;                               /*控件类型		*/
	uint8_t	CtrlID;                                 /*控件ID		*/
	uint8_t	Ctrlx;                                  /*控件x坐标     */
	uint8_t	Ctrly;                                  /*控件y坐标     */
	uint8_t	CtrlSize;                               /*控件大小      */
	uint8_t	*CtrlData;                              /*控件持有的数据*/
	uint8_t	CtrlDispMode;                           /*控件显示模式  */
	void (*CtrlFun)(void);                           /*回调函数      */
};

struct StructHead_SysWndType
{
	uint8_t	SelfWnd;                                /*窗口ID		*/
	uint8_t	CtrlNum;                                /*控件个数		*/
	const struct StructHead_CtrlType *SelfCtrlType; /*窗口包含控件	*/
	void (*NowWndFun)(void);                         /*窗口回调函数	*/		
};

//控件类型
#define DEF_CTRLTEXT				0			/*文本控件			*/
#define DEF_CTRLEDIT				1			/*编辑控件			*/
#define DEF_CTRLPASSWORD            2			/*密码框控件        */
#define DEF_CTRLBUTTON				3 			/*按钮控件			*/
#define DEF_CTRLRADIO				4			/*单选框控件        */
/********************************************************************/
/*光标																*/
#define DEF_CUSBIT					0x01		//光标状态标志位
#define DEF_CUSFLASH				0x02		//光标闪烁标志位
/********************************************************************/
/*编辑控件宏                                                        */
#define DEF_MAXEDITSUM				12			/*编辑框最大数量    */	
#define DEF_MAXEDITLENGTH			32			/*编辑框最大长度    */

/*窗口类型															*/
#define DEF_PATROLWND               0			//监控界面            
#define DEF_MENUWND                 1			//主菜单界面          
#define DEF_FileOperate             2			//文件操作界面		1级
#define DEF_PoweAmpRegis            3			//功放注册界面		1级
#define DEF_SystemConfig            4           //系统设置界面      1级
#define DEF_RecordQuery             5           //录音查询界面   	2级
#define DEF_RecordDel               6           //录音删除界面   	2级
#define DEF_FileInput               7           //文件导入界面	 	2级
#define DEF_PowerAmpCfg             8           //功放配置界面		2级
#define DEF_TimeCfg                 9           //时间设置界面      
#define DEF_AddressCfg              10          //本机地址界面
#define DEF_PasswordCfg             11          //密码设置界面
#define DEF_UserPasswordCfg         12          //用户密码设置界面
#define DEF_GovPasswordCfg          13          //管理密码设置界面
#define DEF_CheckUserPassword		    14          //核对用户密码界面
#define DEF_CheckGovPassword		    15          //核对管理密码界面
#define DEF_DelAllFile					    16          //删除全部文件界面
#define DEF_FactoryConfig				    17          //恢复出厂设置界面
#define DEF_PasswordSave				    18          //密码保存确认界面


#define DEF_INPUTNUMWND             200			//输入号码界面

extern u8 Cfg_PowAmp1to10_InitSta_OneEnter_Flag;

extern u8 FileInputWnd_Flag,FileInputFinish_Flag;
extern u8 DelAllFileWnd_Flag,DelAllFile_Flag;

extern uint8_t Area_No_Stratup_Dis;

extern uint8_t Bar_Process_Count;
extern uint8_t Time_Adjust_Play;
extern u8 RecordQueryWnd_Flag,RecordDelWnd_Flag,CfgNativeAddressWnd_Flag;
extern u8 UserPasswordWnd_Flag,GovPasswordWnd_Flag,CheckUserPasswordWnd_Flag,CheckGovPasswordWnd_Flag,NextWnd_Flag,PasswordSaveWnd_Flag;
extern u8 PlayBack_Play_Flag;

extern uint8_t  mTimeBuffer[6];//年月日时分秒
extern uint16_t CurWndFocus;                               //窗口焦点位置
extern uint16_t OldWndFocus;                               //

extern uint8_t  WndUpdate;                                 //窗口更新
extern uint8_t  WndNum;                                    //窗口嵌套个数
extern uint8_t  InputSum;                                  //输入字符总数
extern uint8_t  WndGrp[6];                                 //窗口组

void ui_WndProc(void);
void ui_ShowMainMenu(void);
void ui_RemoveAllWnd(void);
void ui_ReDrawWnd(void);
void ui_MenuToMainWnd(void);
void ui_ShowSys485Error(int nAddr,int nCmd);
void ui_ShowSelfCheckWnd(void);

void Fun_WndPreDeal(uint8_t wnd);
void Fun_WndExitDeal(void);

void delay_ms(u16 ms);
void Bar_Proceess_Init_Interface(u8 y);
void Bar_Change(u8 type,u8 y);

int DrawDateTimeLine(int bReDraw);

void Emer_Interface(void);
void Record_Interface(void);
void MP3_Interface(void);

void LINE1_Interface(void);
void LINE2_Interface(void);

void U_Interface(void);
void U_No_Interface(void);

void MIC_Interface(void);
void MIC_No_Interface(void);


void GBK_to_hex_quwei(u8 H,u8 L,u8 Location);
void GBK_to_hex_Asiic(u8 data,u8 Location);//机内码 -8080国标码-2020区位码
void Clear_Location_Code(void);
void Clear_16_Location_Code(void);

void Reset_MP3_Display(void);
void MP3_Song_Current_Num_Display(void);
void MP3_Song_Sum_Num_Display(void);
void MP3_Song_Name_Display(void);

void PlayBack_Current_Num_Display(void);	
void PlayBack_Current_Sum_Display(void);
void Playback_Current_Time_Display(void);
void PlayBack_History_Time_Dispaly(void);
void Emergency_broadcast_Current_Time_Display(void);
void Record_Current_Time_Display(void);
void PlayBack_Del_Current_Num_Display(void);
void PlayBack_Del_Sum_Display(void);
void FileInputFinish_Display(void);
void DelAllFileFinish_Display(void);
void FactoryCfgFinish_Display(void);

void Com_itoa(uint16_t i, uint8_t *a);
void Byte2Str(u8 nValue,u8 *pBuf);

#endif
