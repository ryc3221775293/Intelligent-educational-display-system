#include "show.h"
#include "lcd.h"
#include "text.h"	
#include "malloc.h" 
#include "usmart.h"  
#include "sdio_sdcard.h"    
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "usart.h"
#include <stdlib.h>
extern int students;
extern int classnum;
FIL fil;
FRESULT res;
UINT bww;
FIL fil_date;
FRESULT res_date;
UINT bww_date;
char buf[200];
char buf_date[11];
int currentoday;
int weeks;
int group;
int current_class;
//读取SD卡内容，将信息存储到buf，计算本学期本教室共有几节课
void Show_SDIO(void)
{
		int n=0;
		char str;
		int len = 0;
		res=f_open (&fil,"0:/table.txt", FA_READ); //打开文件  
		f_read (&fil, buf,200,&bww);	//读取文件并将值存入buf
		f_close(&fil);//关闭文件
		res_date=f_open (&fil_date,"0:/date.txt", FA_READ); //打开文件  
		f_read (&fil_date, buf_date,200,&bww_date);	//读取文件并将值存入buf
		f_close(&fil_date);//关闭文件
		for(n=0;n>=0;n++)//统计文件中共有多少值
		{
				str = buf[n];
				if(str=='\0')
				{
					len = n;
					//printf("Space:%d\r\n",len);
					
					break;
				}
		}
		//printf("Buff:%s",(u8 *)buf);
		group = len/9;//计算本学期本教室一周要上多少课

}
void Show_Week(int month,int day)//显示当前周数
{
		int daytime=0;
		int today = 0;
		int i,j;
		int days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
		int current_month,current_day;
		current_month=(buf_date[4]-'0')*10+(buf_date[5]-'0');
		current_day=(buf_date[6]-'0')*10+(buf_date[7]-'0');
		current_class=(buf_date[8]-'0')*100+(buf_date[9]-'0')*10-(buf_date[10]-'0');
		for(i=current_month-1;i<month-1;i++)
		{
				daytime=daytime+days[i];			
		}
		for(j=0;j<month-1;j++)
		{
				today += days[j];
		}
		daytime += day;//计算从开学到现在一共多少天
		today += day;//计算从2018.1.1周一开始一共经过了多少天
		printf("today:%d",today);
		currentoday = today%7;//计算当前周几
		daytime -= current_day;
		weeks = daytime/7+1;//计算开学到现在当前周数
		Show_Str_Mid(364,30,"第  周",24,72);
		LCD_ShowxNum(388,30,weeks,2,24,0);//LCD显示周数
		if(currentoday==0)//显示当前周几
		{
				Show_Str_Mid(290,0,"周日",24,48);				
		}else if(currentoday==1)
		{
				Show_Str_Mid(290,0,"周一",24,48);
		}else if(currentoday==2)
		{
				Show_Str_Mid(290,0,"周二",24,48);
		}else if(currentoday==3)
		{
				Show_Str_Mid(290,0,"周三",24,48);
		}else if(currentoday==4)
		{
				Show_Str_Mid(290,0,"周四",24,48);
		}else if(currentoday==5)
		{
				Show_Str_Mid(290,0,"周五",24,48);
		}else if(currentoday==6)
		{
				Show_Str_Mid(290,0,"周六",24,48);
		}
		
}
void Show_Table(void)//显示课表
{
	char str1,str2,str3;
	int x,y,x0,y0;
	int m=0,m0,n0;
	
	for(x=0;x<8;x++)//绘制表格
	{
			LCD_DrawRectangle(x*100,60,100+x*100,100);
			for(y=0;y<5;y++)
			{
					LCD_DrawRectangle(x*100,100+y*70,100+x*100,170+y*70);
			}
	}
	printf("Group:%d\r\n",group);
	for(m=0;m<group;m++)
	{
			
			 
			str1=buf[0+m*9];str2=buf[1+m*9];
			m0 = (buf[2+m*9]-'0')*10+(buf[3+m*9]-'0');
			n0 = (buf[4+m*9]-'0')*10+(buf[5+m*9]-'0');
			str3 = buf[8+m*9];
			printf("m:%d,n:%d\r\n",m0,n0);
					if(str1=='1')//按照周几第几节课将课程名称填入对应位置
					{
							x0=100;
					}else if(str1=='2')
					{
							x0=200;
					}else if(str1=='3')
					{
							x0=300;
					}else if(str1=='4')
					{
							x0=400;
					}else if(str1=='5')
					{
							x0=500;
					}else if(str1=='6')
					{
							x0=600;
					}else if(str1=='7')
					{
							x0=700;
					}
					if(str2=='1')
					{
							y0=127;
					}else if(str2=='2')
					{
							y0=197;
					}
					else if(str2=='3')
					{
							y0=267;
					}
					else if(str2=='4')
					{
							y0=337;
					}
					else if(str2=='5')
					{
							y0=407;
					}
					if(weeks>=m0&&weeks<=n0)
					{
							printf("OK\r\n");
							if(str3=='1')
							{
									Show_Str_Mid(x0,y0-8,"复变函数与",16,100);
									Show_Str_Mid(x0,y0+8,"积分变换",16,100);
							}else if(str3=='2')
							{
									Show_Str_Mid(x0,y0-8,"数学物理方法",16,100);
									Show_Str_Mid(x0,y0+8,"与特殊函数",16,100);
							}else if(str3=='3')
							{
									Show_Str_Mid(x0,y0,"高等数学2-2",16,100);
							}else if(str3=='4')
							{
									Show_Str_Mid(x0,y0,"电磁学",16,100);
							}else if(str3=='5')
							{
									Show_Str_Mid(x0,y0,"单片机原理",16,100);
							}else if(str3=='6')
							{
									Show_Str_Mid(x0,y0,"通信原理",16,100);
							}
							
					}
					
			
	}
	
	//LCD显示课表基本内容
	Show_Str_Mid(100,68,"周一",24,100);
	Show_Str_Mid(200,68,"周二",24,100);
	Show_Str_Mid(300,68,"周三",24,100);
	Show_Str_Mid(400,68,"周四",24,100);
	Show_Str_Mid(500,68,"周五",24,100);
	Show_Str_Mid(600,68,"周六",24,100);
	Show_Str_Mid(700,68,"周日",24,100);
	Show_Str_Mid(0,123,"第一节",24,100);
	Show_Str_Mid(0,193,"第二节",24,100);
	Show_Str_Mid(0,263,"第三节",24,100);
	Show_Str_Mid(0,333,"第四节",24,100);
	Show_Str_Mid(0,403,"第五节",24,100);
	/*
	//if(weeks>=9&&weeks<=16)
	//{
			Show_Str_Mid(100,119,"数学物理方法",16,100);
			Show_Str_Mid(100,135,"与特殊函数",16,100);
		
			Show_Str_Mid(500,119,"数学物理方法",16,100);
			Show_Str_Mid(500,135,"与特殊函数",16,100);
		
			Show_Str_Mid(300,399,"数学物理方法",16,100);
			Show_Str_Mid(300,415,"与特殊函数",16,100);
	//}else if(weeks>=1&&weeks<=8)

	{
			Show_Str_Mid(100,119,"复变函数与",16,100);
			Show_Str_Mid(100,135,"积分变换",16,100);
		
			Show_Str_Mid(500,119,"复变函数与",16,100);
			Show_Str_Mid(500,135,"积分变换",16,100);
		
			Show_Str_Mid(300,399,"复变函数与",16,100);
			Show_Str_Mid(300,415,"积分变换",16,100);
	
	}

	Show_Str_Mid(200,127,"电磁学",16,100);
	Show_Str_Mid(300,127,"高等数学2-2",16,100);
	Show_Str_Mid(400,127,"通信原理",16,100);

	
	Show_Str_Mid(100,197,"高等数学2-2",16,100);
	Show_Str_Mid(300,197,"单片机原理",16,100);
	Show_Str_Mid(400,197,"电磁学",16,100);
	
	Show_Str_Mid(500,267,"高等数学2-2",16,100);
*/
	
	
	
}
//LCD显示课程顺延
void Show_Deferred(void)
{
		Show_Str_Mid(280,250,"课程顺延",24,240);
		Show_Str_Mid(280,280,"暂停授课",24,240);
}
//显示正在考试
void Show_Examination(void)
{
		Show_Str_Mid(280,250,"正在考试",24,240);
}
//显示调课
void Show_Lecture(int classroom)
{

		Show_Str_Mid(280,250,"已调课至",24,240);
		Show_Str_Mid(280,280,"明理楼",24,240);
		Show_Str_Mid(370,310,"理   ",24,60);
		LCD_ShowxNum(394,310,classroom,3,24,0);
		

}
//显示正在上课
void Show_Class(int classroom)
{
		int x0 = 325,y0 = 140;
		char str1,str2,str3;
		int m=0,m0,n0;
		Show_Str_Mid(280,220,"第  节课",24,240);//显示当前是第几节课
		LCD_ShowxNum(376,220,classnum,2,24,0);
		Show_Str_Mid(280,250,"正在上课",24,240);
		Show_Str_Mid(280,280,"请刷卡",24,240);
		Show_Str_Mid(370,310,"理   ",24,60);//显示当前教室
		LCD_ShowxNum(394,310,classroom,3,24,0);
		Show_Str_Mid(346,340,"已到   人",24,108);//显示已到学生人数
		//LCD_ShowxNum(394,340,students,3,24,0);
		for(m=0;m<group;m++)//显示当前上课的详细内容
	{
			
			 
			str1=buf[0+m*9];str2=buf[1+m*9];
			m0 = (buf[2+m*9]-'0')*10+(buf[3+m*9]-'0');
			n0 = (buf[4+m*9]-'0')*10+(buf[5+m*9]-'0');
			str3 = buf[8+m*9];
			printf("m:%d,n:%d\r\n",m0,n0);
					if((str1-'0')==currentoday&&(str2-'0')==classnum&&weeks>=m0&&weeks<=n0)
					{
							if(weeks>=m0&&weeks<=n0)
							{
									printf("OK\r\n");
									if(str3=='1')
									{
											Show_Str_Mid(x0,y0-12,"复变函数与",24,150);
											Show_Str_Mid(x0,y0+12,"积分变换",24,150);
											Show_Str_Mid(x0,y0+36,"老师：牛鹏斌",24,150);
									}else if(str3=='2')
									{
											Show_Str_Mid(x0,y0-12,"数学物理方法",24,150);
											Show_Str_Mid(x0,y0+12,"与特殊函数",24,150);
											Show_Str_Mid(x0,y0+36,"老师：牛鹏斌",24,150);
									}else if(str3=='3')
									{
											Show_Str_Mid(x0,y0,"高等数学2-2",24,150);
											Show_Str_Mid(x0,y0+24,"老师：肖润梅",24,150);
									}else if(str3=='4')
									{
											Show_Str_Mid(x0,y0,"电磁学",24,150);
											Show_Str_Mid(x0,y0+24,"老师：王  萍",24,150);
									}else if(str3=='5')
									{
											Show_Str_Mid(x0,y0,"单片机原理",24,150);
											Show_Str_Mid(x0,y0+24,"老师：张  昊",24,150);
									}else if(str3=='6')
									{
											Show_Str_Mid(x0,y0,"通信原理",24,150);
											Show_Str_Mid(x0,y0+24,"老师：孙慧萍",24,150);
									}
									
							}
					}
					
					
			
	}
	

}



