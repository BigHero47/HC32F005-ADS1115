#include "usartApp.h"
#include "delay_hc.h"
#include "ads1115.h"
#include "ddl.h"
#include "i2c.h"

/*************************************************************************************
*函数名称:void systeminit(void)
*功能描述:系统初始化
*输入参数:none
*输出参数:none
*返回值:none
*其他说明:
*修改日期         版本号                   修改人             修改内容
*------------------------------------------------------------------------------------
*************************************************************************************/
void System_Init(void){
	delay_init();
	Uart_Config(&uart1,BAUDRATE,UartMode1);//串口1初始化
	I2C_init();
}

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
	uint32_t adcvalue = 0;
  System_Init();  //外设初始化


	while(1)
	{
		adcvalue = GetAdcValue(CHIP2, CHANNEL0);
		u1_printf("CH0:  %d\r\n", adcvalue);	
		delay1ms(10);
		
		
		adcvalue = GetAdcValue(CHIP2, CHANNEL1);
		u1_printf("CH1:  %d\r\n", adcvalue);
		delay1ms(10);
		
		adcvalue = GetAdcValue(CHIP2, CHANNEL2);
		u1_printf("CH2:  %d\r\n", adcvalue);	
		delay1ms(10);

		
		adcvalue = GetAdcValue(CHIP2, CHANNEL3);
		u1_printf("CH3:  %d\r\n\r\n", adcvalue);	

		delay1ms(1000);
	}
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
