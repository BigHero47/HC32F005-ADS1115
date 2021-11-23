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
  #define DEV_RELAY_OUTPORT 1    //�̵����������ģ��(�˿�����)
	
#elif ((MY_DEVID & 0xF0) == 0x10)
  #define DEV_MOS_OUTPUT 1       //MOS�����
	
#elif ((MY_DEVID & 0xF0) == 0x20)
  #define DEV_GPIO_SAMPLING 1    //�˿ڼ��ģ��
	
#elif ((MY_DEVID & 0xF0) == 0x30)
  #define DEV_DAC_CONVERT 1      //DAC��ģת��ģ��
	
#elif ((MY_DEVID & 0xF0) == 0x40)
  #define DEV_ADC_SAMPLING 1     //ADC���ݲ���ģ��
	
#elif ((MY_DEVID & 0xF0) == 0x50)
  #define DEV_UART_COMMUNI 1     //����ͨѶģ��

#endif

/******************************************************************************/
/*             �������ò���                                                   */
/******************************************************************************/
#define RECVLEN 128  //���ճ���
#define BAUDRATE 115200u

/******************************************************************************/
/*             ��ʱ�����ò���                                                 */
/******************************************************************************/
#define SAMLPEPERIOD 100  //��������������
#define FEEDDOGPERIOD 250
#define ADCChannels 8
#define SENDBUFFLEN 30

/** �����ʶ */
typedef enum CommandID
{
  DataRequest = 0x01,  //��������
	PortConfig = 0x02,  //�˿�����
	EquipSearch = 0x10  //�豸��ѯ
} CommandID_t;


/** Ӧ���ʶ */
typedef enum ResponseID
{
  RecvSucess = 0x01,
	RecvError = 0x02,
  DuplicateDEVID = 0x03,
	Order = 0xFE
} ResponseID_t;


/** ��Ϣ���ͱ�־ */
typedef enum DataTypeSign
{
  ChannelBitmap = 0x01,
	GPIOSampling = 0x02,
	RelayOutput = 0x03,
	ADCSampling = 0x04

} DataTypeSign_t;



//P27������

#endif


