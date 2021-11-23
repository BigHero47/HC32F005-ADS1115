#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "hc32l110.h"
#include "base_types.h"
#include "ddl.h"
#include "uart.h"
#include "usart.h"
#include "bt.h"
#include "lpm.h"
#include "gpio.h"
#include "adc.h"

#define  PRINTINFO 1

#define MY_DEVID (0x40)

#if ((MY_DEVID & 0xF0) == 0x00)
  #define DEV_RELAY_OUTPORT 1    //继电器控制输出模块(端口配置)
	
#elif ((MY_DEVID & 0xF0) == 0x10)
  #define DEV_MOS_OUTPUT 1       //MOS管输出
	
#elif ((MY_DEVID & 0xF0) == 0x20)
  #define DEV_GPIO_SAMPLING 1    //端口检测模块
	
#elif ((MY_DEVID & 0xF0) == 0x30)
  #define DEV_DAC_CONVERT 1      //DAC数模转换模块
	
#elif ((MY_DEVID & 0xF0) == 0x40)
  #define DEV_ADC_SAMPLING 1     //ADC数据采样模块
	
#elif ((MY_DEVID & 0xF0) == 0x50)
  #define DEV_UART_COMMUNI 1     //串口通讯模块

#endif

/******************************************************************************/
/*             串口配置参数                                                   */
/******************************************************************************/
#define RECVLEN 128  //接收长度
#define BAUDRATE 115200u

/******************************************************************************/
/*             定时器配置参数                                                 */
/******************************************************************************/
#define SAMLPEPERIOD 100  //传感器采样周期
#define FEEDDOGPERIOD 250
#define ADCChannels 8
#define SENDBUFFLEN 30

/** 命令标识 */
typedef enum CommandID
{
  DataRequest = 0x01,  //数据请求
	PortConfig = 0x02,  //端口配置
	EquipSearch = 0x10  //设备查询
} CommandID_t;


/** 应答标识 */
typedef enum ResponseID
{
  RecvSucess = 0x01,
	RecvError = 0x02,
  DuplicateDEVID = 0x03,
	Order = 0xFE
} ResponseID_t;


/** 信息类型标志 */
typedef enum DataTypeSign
{
  ChannelBitmap = 0x01,
	GPIOSampling = 0x02,
	RelayOutput = 0x03,
	ADCSampling = 0x04

} DataTypeSign_t;



//P27下载用

#endif


