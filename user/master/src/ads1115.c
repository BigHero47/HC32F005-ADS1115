#include "ads1115.h"
#include "usartApp.h"
#include "i2c.h"

#define GPIO_I2C_SDA_PORT   2
#define GPIO_I2C_SDA_PIN	  5

#define GPIO_I2C_SCL_PORT   2
#define GPIO_I2C_SCL_PIN	  6


/*************************************************************************************
*��������:void I2C_init(void)
*��������:I2C��ʼ������
*�������:none
*�������:none
*����ֵ:none
*����˵��:
*�޸�����         �汾��                   �޸���             �޸�����
*------------------------------------------------------------------------------------
*************************************************************************************/
void I2C_init(void)
{
	stc_i2c_config_t stcI2cCfg;
	DDL_ZERO_STRUCT(stcI2cCfg);

	Gpio_InitIOExt(GPIO_I2C_SDA_PORT,GPIO_I2C_SDA_PIN,GpioDirOut,FALSE,FALSE,TRUE,FALSE);
	Gpio_InitIOExt(GPIO_I2C_SCL_PORT,GPIO_I2C_SCL_PIN,GpioDirOut,FALSE,FALSE,TRUE,FALSE);

	Gpio_SetFunc_I2CDAT_P25();
	Gpio_SetFunc_I2CCLK_P26();
	Clk_SetPeripheralGate(ClkPeripheralI2c,TRUE);

	stcI2cCfg.enFunc = I2cBaud_En;
	stcI2cCfg.u8Tm = I2C_DRATE;
	stcI2cCfg.pfnI2cCb = NULL;
	stcI2cCfg.bTouchNvic = FALSE;

	I2C_DeInit();
	I2C_Init(&stcI2cCfg);
	I2C_SetFunc(I2cMode_En);
}

/*************************************************************************************
*��������:en_result_t I2C_MasterSendStart(uint8_t u8DevAddr, uint8_t channel)
*��������:ADS1115����
*�������:u8DevAddr: оƬд��ַ
*         channel: ����ͨ��
*�������:none
*����ֵ:en_result_t
*����˵��:
*�޸�����         �汾��                   �޸���             �޸�����
*------------------------------------------------------------------------------------
*************************************************************************************/
en_result_t I2C_MasterSendStart(uint8_t u8DevAddr, uint8_t channel)
{
	int timeout = 0;
	
	/* START */
  I2C_SetFunc(I2cStart_En);  // Step5, ���� Start �ź�
	I2C_ClearIrq();

	timeout = TOUTVAL;
	while(0 == I2C_GetIrq() || timeout--);
	
	if(0x08 != I2C_GetState()) {return Error;}

	/* WR_REG */
	I2C_WriteByte(u8DevAddr);  // Step 10-1
	I2C_ClearFunc(I2cStart_En);  // Step 10-2
	I2C_ClearIrq();  // Step 10-3  M0P_I2C->CR_f.SI = 0;
	timeout = TOUTVAL;
	while(0 == I2C_GetIrq() || timeout--); // Step 11, �ȴ� I2Cx_CR.si ��Ϊ 1��SLA+W �ѷ��͵�������
	if(0x18 != I2C_GetState()) return Error;

	/* CONF_REG 0x01 */
	do{
		I2C_WriteByte(CONF_REG);  // Step 13-1
		I2C_ClearIrq();  // Step 13-2    I2Cx_CR.si = 0
  	timeout = TOUTVAL;
		while(0 == I2C_GetIrq() || timeout--);  // Step 14, �ȴ� I2Cx_CR.si ��Ϊ 1�������ѷ��͵�������
	}while(0x28 != I2C_GetState());	

	/* channel */
	do{
		I2C_WriteByte(channel);  // Step 13-1
		I2C_ClearIrq();  // Step 13-2
  	timeout = TOUTVAL;
		while(0 == I2C_GetIrq() || timeout--);  // Step 14, �ȴ� I2Cx_CR.si ��Ϊ 1�������ѷ��͵�������
	}while(0x28 != I2C_GetState());

	/* 0xe3 */
	do{
		I2C_WriteByte(0xe3);  // Step 13-1
		I2C_ClearIrq();  // Step 13-2
  	timeout = TOUTVAL;
		while(0 == I2C_GetIrq() || timeout--);  // Step 14, �ȴ� I2Cx_CR.si ��Ϊ 1�������ѷ��͵�������
	}while(0x28 != I2C_GetState());

	/* END */
	I2C_ClearIrq();  // Step 15-2   I2Cx_CR.si = 0	
  I2C_SetFunc(I2cStop_En);  // Step 15-1  I2Cx_CR.sto = 1
	I2C_ClearIrq();  // Step 15-2   I2Cx_CR.si = 0
  timeout = TOUTVAL;
	while(0 == I2C_GetIrq() || timeout--);  // Step 16, �ȴ� I2Cx_CR.si ��Ϊ 1�������ѷ��͵�������

	return Ok;
}

/*************************************************************************************
*��������:uint32_t ADCDataProcess(uint16_t ADC_Readbuff_i[2])
*��������:ADS1115���ݴ�����
*�������:uint16_t ADC_Readbuff_i[2]:�Ĵ�������������
*�������:none
*����ֵ:uint32_t:�����������
*����˵��:
*�޸�����         �汾��                   �޸���             �޸�����
*------------------------------------------------------------------------------------
*************************************************************************************/
uint32_t ADCDataProcess(uint16_t ADC_Readbuff_i[])
{
  uint32_t adc_rawdata_o;
	adc_rawdata_o = (ADC_Readbuff_i[0] << 8) + ADC_Readbuff_i[1];
	if(adc_rawdata_o >= 0x8000)  
		 adc_rawdata_o = ((0xffff - adc_rawdata_o) / 32767.0) * ADS1115_PGA_6144;
	else
		 adc_rawdata_o = (adc_rawdata_o/32768.0)*ADS1115_PGA_6144;

	return adc_rawdata_o;
}

/*************************************************************************************
*��������:en_result_t I2C_Read(uint8_t u8DevAddr, uint8_t Reg, uint16_t Readbuff[])
*��������:��ADS1115�Ĵ����ж�ȡI2C����
*�������:u8DevAddr: оƬд��ַ
*         Reg: оƬ����ַ
*�������:Readbuff[]:��ȡ�ĸߵ�λ����
*����ֵ:en_result_t
*����˵��:
*�޸�����         �汾��                   �޸���             �޸�����
*------------------------------------------------------------------------------------
*************************************************************************************/
en_result_t I2C_Read(uint8_t u8DevAddr, uint8_t Reg, uint16_t Readbuff[])
{
	int timeout = 0;
	
	/* START */
  I2C_SetFunc(I2cStart_En);  // Step5, ���� Start �ź�
	I2C_ClearIrq();
  timeout = TOUTVAL;
	while(0 == I2C_GetIrq() || timeout--);
	if(0x08 != I2C_GetState()) return Error;

	/* WR_REG */
	I2C_WriteByte(u8DevAddr);  // Step 10-1
	I2C_ClearFunc(I2cStart_En);  // Step 10-2
	I2C_ClearIrq();  // Step 10-3  M0P_I2C->CR_f.SI = 0;
  timeout = TOUTVAL;
	while(0 == I2C_GetIrq() || timeout--);  // Step 11, �ȴ� I2Cx_CR.si ��Ϊ 1��SLA+W �ѷ��͵�������
	if(0x18 != I2C_GetState()) return Error;

	/* DATA_REG */
	do{
		I2C_WriteByte(DATA_REG);  // Step 13-1
		I2C_ClearIrq();  // Step 13-2    I2Cx_CR.si = 0
    timeout = TOUTVAL;
		while(0 == I2C_GetIrq() || timeout--);  // Step 14, �ȴ� I2Cx_CR.si ��Ϊ 1�������ѷ��͵�������
	}while(0x28 != I2C_GetState());	

	/* END */
	I2C_ClearIrq();  // Step 15-2   I2Cx_CR.si = 0	
  I2C_SetFunc(I2cStop_En);  // Step 15-1  I2Cx_CR.sto = 1
	I2C_ClearIrq();  // Step 15-2   I2Cx_CR.si = 0
  timeout = TOUTVAL;
	while(0 == I2C_GetIrq() || timeout--);  // Step 16, �ȴ� I2Cx_CR.si ��Ϊ 1�������ѷ��͵�������

	/* START */
  I2C_SetFunc(I2cStart_En);  // Step5, ���� Start �ź�
	I2C_ClearIrq();
  timeout = TOUTVAL;
	while(0 == I2C_GetIrq() || timeout--);
	if(0x08 != I2C_GetState()) return Error;

	/* RE_REG 0x91 */
	I2C_WriteByte(Reg);  // Step 8-1
	I2C_ClearFunc(I2cStart_En);  // Step 8-2  CR_f.STA = 0;
	I2C_ClearIrq();  // Step 8-3 CR_f.SI = 0;
  timeout = TOUTVAL;
	while(0 == I2C_GetIrq() || timeout--);  // Step 9, �ȴ� I2Cx_CR.si ��Ϊ 1��SLA+W �ѷ��͵�������
	if(0x40 != I2C_GetState()) return Error;		

	I2C_SetFunc(I2cAck_En);  // CR_f.AA = 1;
	/* RECV DATA1 */
	I2C_ClearIrq();  // ���� I2Cx_CR.si Ϊ 0
  timeout = TOUTVAL;
	while(0 == I2C_GetIrq() || timeout--);
	Readbuff[0] = I2C_ReadByte();
	if((0x50 != I2C_GetState())) return Error;

  I2C_ClearIrq();  // ���� I2Cx_CR.si Ϊ 0
  timeout = TOUTVAL;
	while(0 == I2C_GetIrq() || timeout--);
	Readbuff[1] = I2C_ReadByte();
	if((0x50 != I2C_GetState())) return Error;
	I2C_ClearFunc(I2cAck_En);

	/* END */
	I2C_ClearIrq();
  I2C_SetFunc(I2cStop_En);
	I2C_ClearIrq();  // ���� I2Cx_CR.si Ϊ 0
  timeout = TOUTVAL;
	while(0 == I2C_GetIrq() || timeout--);  // Step 9, �ȴ� I2Cx_CR.si ��Ϊ 1
	
	return Ok;
}

/*************************************************************************************
*��������:en_result_t ADS1115Read(uint8_t u8DevAddr, uint8_t Reg, uint32_t *adc_rawdata)
*��������:ADS1115��ȡ����
*�������:u8DevAddr: оƬд��ַ
*         Reg: оƬ����ַ
*�������:adc_rawdata:ADC�ɼ���������(uint)
*����ֵ:en_result_t
*����˵��:
*�޸�����         �汾��                   �޸���             �޸�����
*------------------------------------------------------------------------------------
*************************************************************************************/
en_result_t ADS1115Read(uint8_t u8DevAddr, uint8_t Reg, uint32_t *adc_rawdata)
{
  uint16_t ADC_Readbuff[2];
	
	if(I2C_Read(u8DevAddr, Reg, ADC_Readbuff) != Ok){return Error;}  //оƬ��ȡ����
	*adc_rawdata = ADCDataProcess(ADC_Readbuff);  //���ݴ���
	return Ok;
}

/*************************************************************************************
*��������:uint32_t GetAdcValue(uint8_t chip, uint32_t channel)
*��������:ADS1115�ɼ���������
*�������:chip:оƬ��
*         channel:ͨ����
*�������:none
*����ֵ:uint32_t ADC�ɼ���������(uint)
*����˵��:
*�޸�����         �汾��                   �޸���             �޸�����
*------------------------------------------------------------------------------------
*************************************************************************************/
uint32_t GetOneValue(uint8_t chip, uint32_t channel)
{
  uint32_t ADC_data;

	if(chip == CHIP1){
		switch(channel)
		{
			case CHANNEL0:
				I2C_MasterSendStart(WR_REG1, AD_CH0);
				ADS1115Read(WR_REG1, RE_REG1, &ADC_data);
			break;
		
			case CHANNEL1:
				I2C_MasterSendStart(WR_REG1, AD_CH1);
				ADS1115Read(WR_REG1, RE_REG1, &ADC_data);
			break;
			
			case CHANNEL2:
				I2C_MasterSendStart(WR_REG1, AD_CH2);
				ADS1115Read(WR_REG1, RE_REG1, &ADC_data);
			break;
			
			case CHANNEL3:
				I2C_MasterSendStart(WR_REG1, AD_CH3);
				ADS1115Read(WR_REG1, RE_REG1, &ADC_data);
			break;
		}
	
	}
	else if(chip == CHIP2)
	{
		switch(channel)
		{
			case CHANNEL0:
				I2C_MasterSendStart(WR_REG2, AD_CH0);
				ADS1115Read(WR_REG2, RE_REG2, &ADC_data);
			break;
		
			case CHANNEL1:
				I2C_MasterSendStart(WR_REG2, AD_CH1);
				ADS1115Read(WR_REG2, RE_REG2, &ADC_data);
			break;
			
			case CHANNEL2:
				I2C_MasterSendStart(WR_REG2, AD_CH2);
				ADS1115Read(WR_REG2, RE_REG2, &ADC_data);
			break;
			
			case CHANNEL3:
				I2C_MasterSendStart(WR_REG2, AD_CH3);
				ADS1115Read(WR_REG2, RE_REG2, &ADC_data);
			break;
		}
	}
	
	return ADC_data;
}

/*************************************************************************************
*��������:uint32_t GetAdcValue(uint8_t chip, uint32_t channel)
*��������:ADS1115�ɼ����ݣ���ֵ�˲�
*�������:chip:оƬ��
*         channel:ͨ����
*�������:none
*����ֵ:uint32_t ADC�ɼ����Ĵ���ֵ�˲�������(uint)
*����˵��:
*�޸�����         �汾��                   �޸���             �޸�����
*------------------------------------------------------------------------------------
*************************************************************************************/
uint32_t GetAdcValue(uint8_t chip, uint32_t channel)
{
	uint32_t adcvalue;
	int i = 0, sum = 0;
	
	for(i = 0; i < AVGTIMES; i++){
	  adcvalue = GetOneValue(chip, channel);
		sum = sum + adcvalue;
	}
	adcvalue = sum / AVGTIMES;
	
	return adcvalue;
}


