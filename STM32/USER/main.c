#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "touch.h"	  
#include "malloc.h" 
#include "usmart.h"  
#include "sdio_sdcard.h"    
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "timer.h"
#include "usart3.h"
#include "gizwits_product.h"
#include "show.h"
#include "rc522.h" 
#include "output.h"
/* �û�����ǰ�豸״̬�ṹ��*/
dataPoint_t currentDataPoint;

//WIFI����״̬
//wifi_sta 0: �Ͽ�
//         1: ������
u8 wifi_sta=0;

//Э���ʼ��
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MSϵͳ��ʱ
  usart3_init(9600);//WIFI��ʼ��
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
}




int main(void)
{ 

	u8 key_num;	
	u8 key;
	u8 status=0;
	extern int students;
	extern int students_flag;
	extern int months,days;
	extern int current_class;
	u8 wifi_con=0;//��¼wifi����״̬ 1:���� 0:�Ͽ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();  	//��ʼ����ʱ����
	uart_init(115200);	//��ʼ������1������Ϊ115200������֧��USMART
	BEEP_Init();  			//��ʼ��������
	//OUTPUT_Init();			 //���ģ���ʼ��
	InitRc522();				//��ʼ����Ƶ��ģ��
	
	KEY_Init();					//������ʼ�� 
 	LCD_Init();					//LCD��ʼ�� 	
	Gizwits_Init();         //Э���ʼ��
	//W25QXX_Init();			//��ʼ��W25Q128
	
	tp_dev.init();			//��ʼ��������
	usmart_dev.init(72);		//��ʼ��USMART
	//my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	exfuns_init();			//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 					//����SD�� 
 	f_mount(fs[1],"1:",1); 				//����FLASH.
	LCD_Display_Dir(1);//���ú���
	POINT_COLOR=RED;//��ɫ����
	while(font_init()) 			//����ֿ�
	{	    
		LCD_ShowString(60,50,240,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//�����ʾ
		delay_ms(200);	
	}
	while(SD_Init())//��ⲻ��SD��
	{
		Show_Str_Mid(50,30,"SD���쳣",24,96);
		delay_ms(500);					
		Show_Str_Mid(50,30,"����",24,96);
		delay_ms(500);
	}
	/*
	if(res==0X0D)//FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH
	{
		LCD_ShowString(450,30,200,16,16,"Flash Disk Formatting...");	//��ʽ��FLASH
		res=f_mkfs("1:",1,4096);//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");	//����Flash���̵�����Ϊ��ALIENTEK
			LCD_ShowString(450,30,200,16,16,"Flash Disk Format Finish");	//��ʽ�����
		}else LCD_ShowString(450,30,200,16,16,"Flash Disk Format Error ");	//��ʽ��ʧ��
		delay_ms(1000);
	}
	*/
	Show_SDIO();//��ȡSD�����ݣ�����Ϣ�洢��buf�����㱾ѧ�ڱ����ҹ��м��ڿ�
	if(!(tp_dev.touchtype&0x80))//����ǵ�����
	{
		Show_Str_Mid(0,30,"�Ƿ���д�����У׼",16,240);
		POINT_COLOR=BLUE;
		Show_Str_Mid(0,60,"��:KEY2   ��:KEY0",16,240);	
		while(1)
		{
			key_num=KEY_Scan(0);
			if(key_num==KEY0_PRES)
				break;
			if(key_num==KEY2_PRES)
			{
				LCD_Clear(WHITE);
				TP_Adjust();  	 //��ĻУ׼ 
				TP_Save_Adjdata();//����У׼����
				break;				
			}
		}
	}
	
	POINT_COLOR=BLACK;//�������Ϊ��ɫ

	printf("--------������IOT-���ܽ�����ʾϵͳ----------\r\n");
	printf("KEY1:AirLink����ģʽ\t KEY_UP:��λ\r\n\r\n");	
  while(1)
	{
		 
		 if(wifi_con!=wifi_sta)//�ж��Ƿ�������WIFI
		 {
			 wifi_con=wifi_sta;
			 wifi_con?Show_Str_Mid(600,0,"WIFI������",24,200):Show_Str_Mid(600,0,"WIFIδ����",24,200);
		 }
		 Show_Str_Mid(398,0,"����¥ ��   ",24,240);
		 LCD_ShowxNum(506,0,current_class,3,24,0);
		 gizwitsGetNTP();//����NTP����ʱ��   
		 currentDataPoint.valueStudents = students;//ѧ�������ϱ�
		 if(currentDataPoint.valueClassroom==Classroom_VALUE4)//���310�����Ͽ�
		 {
				RC522_Handel();//ˢ��
				LCD_ShowxNum(394,340,students,3,24,0);		//LCD��ʾ���� 
		 }
		 gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��
		if(wifi_sta&&status==0)//��������WIFI����ʾ��ǰ�������Լ����ܵĿα��˳���ִֻ��һ�Σ���ֹѭ��ִ�и�����������ҳ��
		{
			Show_Week(months,days);//��ʾ����
			Show_Table();//����SD��������ʾ�α�
			status=1;
		}
	  key = KEY_Scan(0);//��ȡ����ֵ

		if(key==KEY0_PRES)
		{
				
		}
		

		if(key==KEY1_PRES)//KEY1����
		{
			printf("WIFI����AirLink����ģʽ\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
		}			
		if(key==WKUP_PRES)//KEY_UP����
		{  
			printf("WIFI��λ����������������\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ 
			wifi_sta=0;//��־wifi�ѶϿ�
		}
		delay_ms(200);//��ʱ200ms
	}	
}








