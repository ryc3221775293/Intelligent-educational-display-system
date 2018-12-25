/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "usart3.h"
#include "lcd.h"
#include "text.h"
#include "show.h"
#include "rc522.h" 
static uint32_t timerMsCount;
int x0=0,x1=0,x2=0,x3=0,x4=0;
int months,days;
int currentmin,classnum;
/** Current datapoint */
extern dataPoint_t currentDataPoint;
extern u8 wifi_sta;
extern int students;
extern int students_flag;
/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_Examination:
        currentDataPoint.valueExamination = dataPointPtr->valueExamination;
        GIZWITS_LOG("Evt: EVENT_Examination %d \n", currentDataPoint.valueExamination);
        if(0x01 == currentDataPoint.valueExamination)//如果正在考试开关打开
        {
          //user handle
					
					LCD_Clear(WHITE);//清屏
					
					Show_Examination();//显示正在考试
        }
				else//正在考试开关关闭
				{
					LCD_Clear(WHITE);//清屏
					Show_Table();//显示课表
				}
        break;
      case EVENT_Deferred:
        currentDataPoint.valueDeferred = dataPointPtr->valueDeferred;
        GIZWITS_LOG("Evt: EVENT_Deferred %d \n", currentDataPoint.valueDeferred);
        if(0x01 == currentDataPoint.valueDeferred)//如果正在课程顺延打开
        {
          //user handle
					
					LCD_Clear(WHITE);//清屏
					
					Show_Deferred();//显示考试顺延
        }
				else
				{
					LCD_Clear(WHITE);//清屏
					Show_Table();//显示课表
				}
        break;

      case EVENT_Classroom:
        currentDataPoint.valueClassroom = dataPointPtr->valueClassroom;
        GIZWITS_LOG("Evt: EVENT_Classroom %d\n", currentDataPoint.valueClassroom);
        switch(currentDataPoint.valueClassroom)
        {
          case Classroom_VALUE0://当前教室为None
						students_flag=1;
						students = 0;//学生人数清空
						LCD_Clear(WHITE);//清屏
						Show_Table();//显示课表
            //user handle
            break;
          case Classroom_VALUE1:
            //user handle
            break;
          case Classroom_VALUE2:
            //user handle
            break;
          case Classroom_VALUE3:
            //user handle
            break;
          case Classroom_VALUE4://选择310教室
						students_flag=1;
						students = 0;//学生人数清空
						LCD_Clear(WHITE);//清屏
						Show_Class(310);//显示310教室详细情况
						
            //user handle
            break;
          case Classroom_VALUE5:
            //user handle
            break;
          case Classroom_VALUE6:
            //user handle
            break;
          case Classroom_VALUE7:
            //user handle
            break;
          default:
            break;
        }
        break;
      case EVENT_Lecture:
        currentDataPoint.valueLecture = dataPointPtr->valueLecture;
        GIZWITS_LOG("Evt: EVENT_Lecture %d\n", currentDataPoint.valueLecture);
        switch(currentDataPoint.valueLecture)
        {
          case Lecture_VALUE0://调课选择None
						LCD_Clear(WHITE);//清屏
						Show_Table();//显示课表
            //user handle
            break;
          case Lecture_VALUE1://调课选择307
						LCD_Clear(WHITE);//清屏
						Show_Lecture(307);//显示调课至307
            //user handle
            break;
          case Lecture_VALUE2://同上
						LCD_Clear(WHITE);
						Show_Lecture(308);
            //user handle
            break;
          case Lecture_VALUE3:
						LCD_Clear(WHITE);
						Show_Lecture(309);
            //user handle
            break;
          case Lecture_VALUE4:
						LCD_Clear(WHITE);
						Show_Lecture(310);
            //user handle
            break;
          case Lecture_VALUE5:
						LCD_Clear(WHITE);
						Show_Lecture(311);
            //user handle
            break;
          case Lecture_VALUE6:
						LCD_Clear(WHITE);
						Show_Lecture(312);
            //user handle
            break;
          case Lecture_VALUE7:
						LCD_Clear(WHITE);
						Show_Lecture(313);
            //user handle
            break;
          default:
            break;
        }
        break;



      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
 
        break;
      case WIFI_DISCON_ROUTER:
 
        break;
      case WIFI_CON_M2M:wifi_sta=1;//wifi设备已连接//第二处，添加
 
        break;
      case WIFI_DISCON_M2M:wifi_sta=0;//wifi设备断开//第三处，添加
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
				//获取网络时间
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
				//显示年月日
				LCD_ShowxNum(20,0,ptime->year,4,24,0);
				LCD_ShowChar(68,0,'-',24,0);
				LCD_ShowxNum(80,0,ptime->month,2,24,0);
				LCD_ShowChar(104,0,'-',24,0);
				LCD_ShowxNum(126,0,ptime->day,2,24,0);
				//显示时分秒
				LCD_ShowxNum(180,0,ptime->hour,2,24,0);
				LCD_ShowChar(204,0,':',24,0);
				LCD_ShowxNum(216,0,ptime->minute,2,24,0);
				LCD_ShowChar(240,0,':',24,0);
				LCD_ShowxNum(252,0,ptime->second,2,24,0);
				//显示周几
				Show_Week(ptime->month,ptime->day);
				months = ptime->month;
				days = ptime->day;
				currentmin = ptime->hour*60+ptime->minute;
			
				//计算当前是第几节课
				if(currentmin>=(8*60)&&currentmin<=(9*60+50))
				{
						classnum = 1;
				}else if(currentmin>=(10*60+10)&&currentmin<=(12*60))
				{
						classnum = 2;
				}else if(currentmin>=(14*60+30)&&currentmin<=(16*60+20))
				{
						classnum = 3;
				}else if(currentmin>=(16*60+30)&&currentmin<=(18*60+20))
				{
						classnum = 4;
				}else if(currentmin>=(19*60)&&currentmin<=(21*60+50))
				{
						classnum = 5;
				}
			break;
      case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
      #else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
    break;
      default:
        break;
    }
  }

  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
/*
void userHandle(void)
{
 
    currentDataPoint.valueStudents = ;//Add Sensor Data Collection

    
    
}
*/
/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
      currentDataPoint.valueExamination = ;
      currentDataPoint.valueDeferred = ;
      currentDataPoint.valueClassroom = ;
      currentDataPoint.valueLecture = ;
      currentDataPoint.valueStudents = ;
    */

}


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);//关闭所有中断
    NVIC_SystemReset();//复位
}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
  gizTimerMs();
}

/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
  uint8_t value = 0;
  //value = USART_ReceiveData(USART2);//STM32 test demo
  gizPutData(&value, 1);
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1，Input Param Illegal
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
    
    #ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);
    }
    GIZWITS_LOG("\n");
    #endif

    for(i=0; i<len; i++)
    {
        //USART_SendData(UART, buf[i]);//STM32 test demo
        //Serial port to achieve the function, the buf[i] sent to the module
				USART_SendData(USART3,buf[i]);
        while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕
        if(i >=2 && buf[i] == 0xFF)
        {
          //Serial port to achieve the function, the 0x55 sent to the module
          //USART_SendData(UART, 0x55);//STM32 test demo
					USART_SendData(USART3,0x55);
          while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕 
        }
    }


    
    return len;
}


