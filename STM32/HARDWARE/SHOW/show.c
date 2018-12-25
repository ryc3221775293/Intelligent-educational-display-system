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
//��ȡSD�����ݣ�����Ϣ�洢��buf�����㱾ѧ�ڱ����ҹ��м��ڿ�
void Show_SDIO(void)
{
		int n=0;
		char str;
		int len = 0;
		res=f_open (&fil,"0:/table.txt", FA_READ); //���ļ�  
		f_read (&fil, buf,200,&bww);	//��ȡ�ļ�����ֵ����buf
		f_close(&fil);//�ر��ļ�
		res_date=f_open (&fil_date,"0:/date.txt", FA_READ); //���ļ�  
		f_read (&fil_date, buf_date,200,&bww_date);	//��ȡ�ļ�����ֵ����buf
		f_close(&fil_date);//�ر��ļ�
		for(n=0;n>=0;n++)//ͳ���ļ��й��ж���ֵ
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
		group = len/9;//���㱾ѧ�ڱ�����һ��Ҫ�϶��ٿ�

}
void Show_Week(int month,int day)//��ʾ��ǰ����
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
		daytime += day;//����ӿ�ѧ������һ��������
		today += day;//�����2018.1.1��һ��ʼһ�������˶�����
		printf("today:%d",today);
		currentoday = today%7;//���㵱ǰ�ܼ�
		daytime -= current_day;
		weeks = daytime/7+1;//���㿪ѧ�����ڵ�ǰ����
		Show_Str_Mid(364,30,"��  ��",24,72);
		LCD_ShowxNum(388,30,weeks,2,24,0);//LCD��ʾ����
		if(currentoday==0)//��ʾ��ǰ�ܼ�
		{
				Show_Str_Mid(290,0,"����",24,48);				
		}else if(currentoday==1)
		{
				Show_Str_Mid(290,0,"��һ",24,48);
		}else if(currentoday==2)
		{
				Show_Str_Mid(290,0,"�ܶ�",24,48);
		}else if(currentoday==3)
		{
				Show_Str_Mid(290,0,"����",24,48);
		}else if(currentoday==4)
		{
				Show_Str_Mid(290,0,"����",24,48);
		}else if(currentoday==5)
		{
				Show_Str_Mid(290,0,"����",24,48);
		}else if(currentoday==6)
		{
				Show_Str_Mid(290,0,"����",24,48);
		}
		
}
void Show_Table(void)//��ʾ�α�
{
	char str1,str2,str3;
	int x,y,x0,y0;
	int m=0,m0,n0;
	
	for(x=0;x<8;x++)//���Ʊ��
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
					if(str1=='1')//�����ܼ��ڼ��ڿν��γ����������Ӧλ��
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
									Show_Str_Mid(x0,y0-8,"���亯����",16,100);
									Show_Str_Mid(x0,y0+8,"���ֱ任",16,100);
							}else if(str3=='2')
							{
									Show_Str_Mid(x0,y0-8,"��ѧ������",16,100);
									Show_Str_Mid(x0,y0+8,"�����⺯��",16,100);
							}else if(str3=='3')
							{
									Show_Str_Mid(x0,y0,"�ߵ���ѧ2-2",16,100);
							}else if(str3=='4')
							{
									Show_Str_Mid(x0,y0,"���ѧ",16,100);
							}else if(str3=='5')
							{
									Show_Str_Mid(x0,y0,"��Ƭ��ԭ��",16,100);
							}else if(str3=='6')
							{
									Show_Str_Mid(x0,y0,"ͨ��ԭ��",16,100);
							}
							
					}
					
			
	}
	
	//LCD��ʾ�α��������
	Show_Str_Mid(100,68,"��һ",24,100);
	Show_Str_Mid(200,68,"�ܶ�",24,100);
	Show_Str_Mid(300,68,"����",24,100);
	Show_Str_Mid(400,68,"����",24,100);
	Show_Str_Mid(500,68,"����",24,100);
	Show_Str_Mid(600,68,"����",24,100);
	Show_Str_Mid(700,68,"����",24,100);
	Show_Str_Mid(0,123,"��һ��",24,100);
	Show_Str_Mid(0,193,"�ڶ���",24,100);
	Show_Str_Mid(0,263,"������",24,100);
	Show_Str_Mid(0,333,"���Ľ�",24,100);
	Show_Str_Mid(0,403,"�����",24,100);
	/*
	//if(weeks>=9&&weeks<=16)
	//{
			Show_Str_Mid(100,119,"��ѧ������",16,100);
			Show_Str_Mid(100,135,"�����⺯��",16,100);
		
			Show_Str_Mid(500,119,"��ѧ������",16,100);
			Show_Str_Mid(500,135,"�����⺯��",16,100);
		
			Show_Str_Mid(300,399,"��ѧ������",16,100);
			Show_Str_Mid(300,415,"�����⺯��",16,100);
	//}else if(weeks>=1&&weeks<=8)

	{
			Show_Str_Mid(100,119,"���亯����",16,100);
			Show_Str_Mid(100,135,"���ֱ任",16,100);
		
			Show_Str_Mid(500,119,"���亯����",16,100);
			Show_Str_Mid(500,135,"���ֱ任",16,100);
		
			Show_Str_Mid(300,399,"���亯����",16,100);
			Show_Str_Mid(300,415,"���ֱ任",16,100);
	
	}

	Show_Str_Mid(200,127,"���ѧ",16,100);
	Show_Str_Mid(300,127,"�ߵ���ѧ2-2",16,100);
	Show_Str_Mid(400,127,"ͨ��ԭ��",16,100);

	
	Show_Str_Mid(100,197,"�ߵ���ѧ2-2",16,100);
	Show_Str_Mid(300,197,"��Ƭ��ԭ��",16,100);
	Show_Str_Mid(400,197,"���ѧ",16,100);
	
	Show_Str_Mid(500,267,"�ߵ���ѧ2-2",16,100);
*/
	
	
	
}
//LCD��ʾ�γ�˳��
void Show_Deferred(void)
{
		Show_Str_Mid(280,250,"�γ�˳��",24,240);
		Show_Str_Mid(280,280,"��ͣ�ڿ�",24,240);
}
//��ʾ���ڿ���
void Show_Examination(void)
{
		Show_Str_Mid(280,250,"���ڿ���",24,240);
}
//��ʾ����
void Show_Lecture(int classroom)
{

		Show_Str_Mid(280,250,"�ѵ�����",24,240);
		Show_Str_Mid(280,280,"����¥",24,240);
		Show_Str_Mid(370,310,"��   ",24,60);
		LCD_ShowxNum(394,310,classroom,3,24,0);
		

}
//��ʾ�����Ͽ�
void Show_Class(int classroom)
{
		int x0 = 325,y0 = 140;
		char str1,str2,str3;
		int m=0,m0,n0;
		Show_Str_Mid(280,220,"��  �ڿ�",24,240);//��ʾ��ǰ�ǵڼ��ڿ�
		LCD_ShowxNum(376,220,classnum,2,24,0);
		Show_Str_Mid(280,250,"�����Ͽ�",24,240);
		Show_Str_Mid(280,280,"��ˢ��",24,240);
		Show_Str_Mid(370,310,"��   ",24,60);//��ʾ��ǰ����
		LCD_ShowxNum(394,310,classroom,3,24,0);
		Show_Str_Mid(346,340,"�ѵ�   ��",24,108);//��ʾ�ѵ�ѧ������
		//LCD_ShowxNum(394,340,students,3,24,0);
		for(m=0;m<group;m++)//��ʾ��ǰ�Ͽε���ϸ����
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
											Show_Str_Mid(x0,y0-12,"���亯����",24,150);
											Show_Str_Mid(x0,y0+12,"���ֱ任",24,150);
											Show_Str_Mid(x0,y0+36,"��ʦ��ţ����",24,150);
									}else if(str3=='2')
									{
											Show_Str_Mid(x0,y0-12,"��ѧ������",24,150);
											Show_Str_Mid(x0,y0+12,"�����⺯��",24,150);
											Show_Str_Mid(x0,y0+36,"��ʦ��ţ����",24,150);
									}else if(str3=='3')
									{
											Show_Str_Mid(x0,y0,"�ߵ���ѧ2-2",24,150);
											Show_Str_Mid(x0,y0+24,"��ʦ��Ф��÷",24,150);
									}else if(str3=='4')
									{
											Show_Str_Mid(x0,y0,"���ѧ",24,150);
											Show_Str_Mid(x0,y0+24,"��ʦ����  Ƽ",24,150);
									}else if(str3=='5')
									{
											Show_Str_Mid(x0,y0,"��Ƭ��ԭ��",24,150);
											Show_Str_Mid(x0,y0+24,"��ʦ����  �",24,150);
									}else if(str3=='6')
									{
											Show_Str_Mid(x0,y0,"ͨ��ԭ��",24,150);
											Show_Str_Mid(x0,y0+24,"��ʦ�����Ƽ",24,150);
									}
									
							}
					}
					
					
			
	}
	

}



