#ifndef __KEYBOARD_H
#define __KEYBOARD_H	 
#include "sys.h"


void Add_FR(void);	//录指纹
void Del_FR(void);	//删除指纹
void press_FR(void);//刷指纹
void ShowErrMessage(u8 ensure);//显示确认码错误信息
void AS608_load_keyboard(u16 x,u16 y,u8 **kbtbl);//加载虚拟键盘
u8  AS608_get_keynum(u16 x,u16 y);//获取键盘数
u16 GET_NUM(void);//获取数值




#endif