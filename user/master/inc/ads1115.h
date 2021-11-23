#ifndef __ADS1115_H
#define __ADS1115_H

#include "hc32l110.h"
#include "base_types.h"

#define  I2C_DRATE 0x10    /*I2C��������*/
#define  TOUTVAL   100     /*while��ʱ����*/
#define  AVGTIMES  5

/** ADS1115���� */
#define  CHIP1 0x01      /*ADS1115 1*/
#define  CHIP2 0x02      /*ADS1115 2*/

#define  CHANNEL0 0x00      /*ADS1115 CH0*/
#define  CHANNEL1 0x01      /*ADS1115 CH1*/
#define  CHANNEL2 0x02      /*ADS1115 CH2*/
#define  CHANNEL3 0x03      /*ADS1115 CH3*/


/** ADS1115�Ĵ������� */
#define  WR_REG1 0x90      /*ADS1115 1д*/
#define  RE_REG1 0x91      /*ADS1115 1��*/
#define  WR_REG2 0x92      /*ADS1115 2д*/
#define  RE_REG2 0x93      /*ADS1115 2д*/

#define  DATA_REG  0x00		/*ת�����ݼĴ���*/
#define  CONF_REG  0x01   /*���������üĴ���*/
#define  LOTH_REG  0x02		/*��ͷ�ֵ�Ĵ���*/
#define  HITH_REG  0x03		/*��߷�ֵ�Ĵ���*/

/** ADS1115ͨ������ */
#define  AD_CH0  0xC0   /*ͨ��0*/
#define  AD_CH1  0xD0   /*ͨ��1*/
#define  AD_CH2  0xE0   /*ͨ��2*/
#define  AD_CH3  0xF0   /*ͨ��3*/

#define ADS1115_PGA_6144  6144		//FS=6.144V
#define ADS1115_PGA_4096  4096		//FS=4.096V
#define ADS1115_PGA_2048  2048		//FS=2.048V
#define ADS1115_PGA_1024  1024		//FS=1.024V
#define ADS1115_PGA_0512  512		//FS=0.512V
#define ADS1115_PGA_0256  256		//FS=0.256V

uint32_t GetAdcValue(uint8_t chip, uint32_t channel);
uint32_t AvgFilter(uint32_t adcvalue);
uint32_t ADCDataProcess(uint16_t ADC_Readbuff_i[]);
uint32_t GetOneValue(uint8_t chip, uint32_t channel);
en_result_t I2C_MasterSendStart(uint8_t u8DevAddr, uint8_t channel);
en_result_t I2C_Read(uint8_t u8DevAddr, uint8_t Reg, uint16_t Readbuff[]);
en_result_t ADS1115Read(uint8_t u8DevAddr, uint8_t Reg, uint32_t *adc_rawdata);
void I2C_init(void);


#endif
