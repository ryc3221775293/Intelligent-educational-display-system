#include "keyboard.h" 

u8** kbd_tbl;
const  u8* kbd_menu[15]={"删指纹"," : ","录指纹","1","2","3","4","5","6","7","8","9","DEL","0","Enter",};//按键表
const  u8* kbd_delFR[15]={"返回"," : ","清空指纹","1","2","3","4","5","6","7","8","9","DEL","0","Enter",};//按键表
	/*
	AS608_load_keyboard(0,170,(u8**)kbd_delFR);
	num=GET_NUM();//获取返回的数值
	*/
//加载按键界面（尺寸x,y为240*150）
//x,y:界面起始坐标（240*320分辨率的时候，x必须为0）
void LCD_load_keyboard(u16 x,u16 y,u8 **kbtbl)
{
	u16 i;
	POINT_COLOR=RED;
	kbd_tbl=kbtbl;
	LCD_Fill(x,y,x+240,y+150,WHITE);
	LCD_DrawRectangle(x,y,x+240,y+150);						   
	LCD_DrawRectangle(x+80,y,x+160,y+150);	 
	LCD_DrawRectangle(x,y+30,x+240,y+60);
	LCD_DrawRectangle(x,y+90,x+240,y+120);
	POINT_COLOR=BLUE;
	for(i=0;i<15;i++)
	{
		if(i==1)//按键表第2个‘:’不需要中间显示
			Show_Str(x+(i%3)*80+2,y+7+30*(i/3),80,30,(u8*)kbd_tbl[i],16,0);	
		else
			Show_Str_Mid(x+(i%3)*80,y+7+30*(i/3),(u8*)kbd_tbl[i],16,80);
	} 
}

//得到触摸屏的输入
//x,y:键盘坐标
//返回值:（1~15,对应按键表）
u8 LCD_get_keynum(u16 x,u16 y)
{
	u16 i,j;
	static u8 key_x=0;//0,没有任何按键按下
	u8 key=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)//触摸屏被按下
	{	
		for(i=0;i<5;i++)
		{
			for(j=0;j<3;j++)
			{
			 	if(tp_dev.x[0]<(x+j*80+80)&&tp_dev.x[0]>(x+j*80)&&tp_dev.y[0]<(y+i*30+30)&&tp_dev.y[0]>(y+i*30))
				{	
					key=i*3+j+1;	 
					break;	 		   
				}
			}
			if(key)
			{	   
				if(key_x==key)key=0;
				else 
				{
					AS608_key_staset(x,y,key_x-1,0);
					key_x=key;
					AS608_key_staset(x,y,key_x-1,1);
				}
				break;
			}
	  }  
	}else if(key_x) 
	{
		AS608_key_staset(x,y,key_x-1,0);
		key_x=0;
	} 
	return key; 
}
//获取键盘数值
u16 GET_NUM(void)
{
	u8  key_num=0;
	u16 num=0;
	while(1)
	{
		key_num=AS608_get_keynum(0,170);	
		if(key_num)
		{
			if(key_num==1)return 0xFFFF;//‘返回’键
			if(key_num==3)return 0xFF00;//		
			if(key_num>3&&key_num<13&&num<99)//‘1-9’键(限制输入3位数)
				num =num*10+key_num-3;		
			if(key_num==13)num =num/10;//‘Del’键			
			if(key_num==14&&num<99)num =num*10;//‘0’键
			if(key_num==15)return num;  //‘Enter’键
		}
		LCD_ShowNum(80+15,170+7,num,6,16);
	}	
}



