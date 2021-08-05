#ifndef __UI_MAIN_H__
#define __UI_MAIN_H__

#include "stm32f10x.h"

struct StructHead_CtrlType
{
	uint8_t CtrlType;                               /*�ؼ�����		*/
	uint8_t	CtrlID;                                 /*�ؼ�ID		*/
	uint8_t	Ctrlx;                                  /*�ؼ�x����     */
	uint8_t	Ctrly;                                  /*�ؼ�y����     */
	uint8_t	CtrlSize;                               /*�ؼ���С      */
	uint8_t	*CtrlData;                              /*�ؼ����е�����*/
	uint8_t	CtrlDispMode;                           /*�ؼ���ʾģʽ  */
	void (*CtrlFun)(void);                           /*�ص�����      */
};

struct StructHead_SysWndType
{
	uint8_t	SelfWnd;                                /*����ID		*/
	uint8_t	CtrlNum;                                /*�ؼ�����		*/
	const struct StructHead_CtrlType *SelfCtrlType; /*���ڰ����ؼ�	*/
	void (*NowWndFun)(void);                         /*���ڻص�����	*/		
};

//�ؼ�����
#define DEF_CTRLTEXT				0			/*�ı��ؼ�			*/
#define DEF_CTRLEDIT				1			/*�༭�ؼ�			*/
#define DEF_CTRLPASSWORD            2			/*�����ؼ�        */
#define DEF_CTRLBUTTON				3 			/*��ť�ؼ�			*/
#define DEF_CTRLRADIO				4			/*��ѡ��ؼ�        */
/********************************************************************/
/*���																*/
#define DEF_CUSBIT					0x01		//���״̬��־λ
#define DEF_CUSFLASH				0x02		//�����˸��־λ
/********************************************************************/
/*�༭�ؼ���                                                        */
#define DEF_MAXEDITSUM				12			/*�༭���������    */	
#define DEF_MAXEDITLENGTH			32			/*�༭����󳤶�    */

/*��������															*/
#define DEF_PATROLWND               0			//��ؽ���            
#define DEF_MENUWND                 1			//���˵�����          
#define DEF_FileOperate             2			//�ļ���������		1��
#define DEF_PoweAmpRegis            3			//����ע�����		1��
#define DEF_SystemConfig            4           //ϵͳ���ý���      1��
#define DEF_RecordQuery             5           //¼����ѯ����   	2��
#define DEF_RecordDel               6           //¼��ɾ������   	2��
#define DEF_FileInput               7           //�ļ��������	 	2��
#define DEF_PowerAmpCfg             8           //�������ý���		2��
#define DEF_TimeCfg                 9           //ʱ�����ý���      
#define DEF_AddressCfg              10          //������ַ����
#define DEF_PasswordCfg             11          //�������ý���
#define DEF_UserPasswordCfg         12          //�û��������ý���
#define DEF_GovPasswordCfg          13          //�����������ý���
#define DEF_CheckUserPassword		    14          //�˶��û��������
#define DEF_CheckGovPassword		    15          //�˶Թ����������
#define DEF_DelAllFile					    16          //ɾ��ȫ���ļ�����
#define DEF_FactoryConfig				    17          //�ָ��������ý���
#define DEF_PasswordSave				    18          //���뱣��ȷ�Ͻ���


#define DEF_INPUTNUMWND             200			//����������

extern u8 Cfg_PowAmp1to10_InitSta_OneEnter_Flag;

extern u8 FileInputWnd_Flag,FileInputFinish_Flag;
extern u8 DelAllFileWnd_Flag,DelAllFile_Flag;

extern uint8_t Area_No_Stratup_Dis;

extern uint8_t Bar_Process_Count;
extern uint8_t Time_Adjust_Play;
extern u8 RecordQueryWnd_Flag,RecordDelWnd_Flag,CfgNativeAddressWnd_Flag;
extern u8 UserPasswordWnd_Flag,GovPasswordWnd_Flag,CheckUserPasswordWnd_Flag,CheckGovPasswordWnd_Flag,NextWnd_Flag,PasswordSaveWnd_Flag;
extern u8 PlayBack_Play_Flag;

extern uint8_t  mTimeBuffer[6];//������ʱ����
extern uint16_t CurWndFocus;                               //���ڽ���λ��
extern uint16_t OldWndFocus;                               //

extern uint8_t  WndUpdate;                                 //���ڸ���
extern uint8_t  WndNum;                                    //����Ƕ�׸���
extern uint8_t  InputSum;                                  //�����ַ�����
extern uint8_t  WndGrp[6];                                 //������

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
void GBK_to_hex_Asiic(u8 data,u8 Location);//������ -8080������-2020��λ��
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
