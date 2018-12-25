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
/* 用户区当前设备状态结构体*/
dataPoint_t currentDataPoint;

//WIFI连接状态
//wifi_sta 0: 断开
//         1: 已连接
u8 wifi_sta=0;

//协议初始化
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MS系统定时
  usart3_init(9600);//WIFI初始化
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
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
	u8 wifi_con=0;//记录wifi连接状态 1:连接 0:断开
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();  	//初始化延时函数
	uart_init(115200);	//初始化串口1波特率为115200，用于支持USMART
	BEEP_Init();  			//初始化蜂鸣器
	//OUTPUT_Init();			 //输出模块初始化
	InitRc522();				//初始化射频卡模块
	
	KEY_Init();					//按键初始化 
 	LCD_Init();					//LCD初始化 	
	Gizwits_Init();         //协议初始化
	//W25QXX_Init();			//初始化W25Q128
	
	tp_dev.init();			//初始化触摸屏
	usmart_dev.init(72);		//初始化USMART
	//my_mem_init(SRAMIN);		//初始化内部内存池 
	exfuns_init();			//为fatfs相关变量申请内存  
 	f_mount(fs[0],"0:",1); 					//挂载SD卡 
 	f_mount(fs[1],"1:",1); 				//挂载FLASH.
	LCD_Display_Dir(1);//设置横屏
	POINT_COLOR=RED;//红色字体
	while(font_init()) 			//检查字库
	{	    
		LCD_ShowString(60,50,240,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//清除显示
		delay_ms(200);	
	}
	while(SD_Init())//检测不到SD卡
	{
		Show_Str_Mid(50,30,"SD卡异常",24,96);
		delay_ms(500);					
		Show_Str_Mid(50,30,"请检查",24,96);
		delay_ms(500);
	}
	/*
	if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		LCD_ShowString(450,30,200,16,16,"Flash Disk Formatting...");	//格式化FLASH
		res=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");	//设置Flash磁盘的名字为：ALIENTEK
			LCD_ShowString(450,30,200,16,16,"Flash Disk Format Finish");	//格式化完成
		}else LCD_ShowString(450,30,200,16,16,"Flash Disk Format Error ");	//格式化失败
		delay_ms(1000);
	}
	*/
	Show_SDIO();//读取SD卡内容，将信息存储到buf，计算本学期本教室共有几节课
	if(!(tp_dev.touchtype&0x80))//如果是电阻屏
	{
		Show_Str_Mid(0,30,"是否进行触摸屏校准",16,240);
		POINT_COLOR=BLUE;
		Show_Str_Mid(0,60,"是:KEY2   否:KEY0",16,240);	
		while(1)
		{
			key_num=KEY_Scan(0);
			if(key_num==KEY0_PRES)
				break;
			if(key_num==KEY2_PRES)
			{
				LCD_Clear(WHITE);
				TP_Adjust();  	 //屏幕校准 
				TP_Save_Adjdata();//保存校准参数
				break;				
			}
		}
	}
	
	POINT_COLOR=BLACK;//字体更新为黑色

	printf("--------机智云IOT-智能教务显示系统----------\r\n");
	printf("KEY1:AirLink连接模式\t KEY_UP:复位\r\n\r\n");	
  while(1)
	{
		 
		 if(wifi_con!=wifi_sta)//判断是否已连接WIFI
		 {
			 wifi_con=wifi_sta;
			 wifi_con?Show_Str_Mid(600,0,"WIFI已连接",24,200):Show_Str_Mid(600,0,"WIFI未连接",24,200);
		 }
		 Show_Str_Mid(398,0,"明理楼 理   ",24,240);
		 LCD_ShowxNum(506,0,current_class,3,24,0);
		 gizwitsGetNTP();//请求NTP网络时间   
		 currentDataPoint.valueStudents = students;//学生人数上报
		 if(currentDataPoint.valueClassroom==Classroom_VALUE4)//如果310教室上课
		 {
				RC522_Handel();//刷卡
				LCD_ShowxNum(394,340,students,3,24,0);		//LCD显示人数 
		 }
		 gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
		if(wifi_sta&&status==0)//开机连接WIFI后显示当前的周数以及本周的课表，此程序只执行一次，防止循环执行覆盖其他命令页面
		{
			Show_Week(months,days);//显示周数
			Show_Table();//根据SD卡内容显示课表
			status=1;
		}
	  key = KEY_Scan(0);//读取按键值

		if(key==KEY0_PRES)
		{
				
		}
		

		if(key==KEY1_PRES)//KEY1按键
		{
			printf("WIFI进入AirLink连接模式\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
		}			
		if(key==WKUP_PRES)//KEY_UP按键
		{  
			printf("WIFI复位，请重新配置连接\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位 
			wifi_sta=0;//标志wifi已断开
		}
		delay_ms(200);//延时200ms
	}	
}








