#include "usart.h"

struct uart_tim_style uart0={UARTCH0,TIM0};  //����0   	���ڳ�ʼ������
struct uart_tim_style uart1={UARTCH1,TIM1};	 //����1	  ���ڳ�ʼ������

char temp[50];


//UART0�����ж�
void ErrIntCallback0(void)
{
}


//UART1�жϺ���
void RxIntCallback0(void) 
{
}



//UART1�����ж�
void ErrIntCallback1(void)
{
	
}


/*************************************************************************************
*��������:void Uart_Config(struct uart_tim_style * uart_style,u32 baudrate,en_uart_mode_t uMode)
*��������:���ڳ�ʼ��
*�������:none
*�������:none
*����ֵ:none
*����˵��:
*�޸�����         �汾��                   �޸���             �޸�����
*------------------------------------------------------------------------------------
*************************************************************************************/
void Uart_Config(struct uart_tim_style * uart_style,u32 baudrate,en_uart_mode_t uMode)
{

	Bt_Stop(uart_style->TIM); //ֹͣTIM1
	Uart_DisableIrq(uart_style->UARTCH,UartRxIrq);//�رս����ж�
	Uart_EnableFunc(uart_style->UARTCH,UartTx); //�رս���ʹ�ܣ���������Ȼ����
	
	uint16_t timer=0;  //������ʱ�����ò����ʵļ���ֵ
	uint32_t pclk=0;  	//pclkƵ��

	stc_uart_config_t  stcConfig;  	//uart���ýṹ
	stc_uart_irq_cb_t stcUartIrqCb;	//uart�жϷ������ṹ
	stc_uart_multimode_t stcMulti;	//uart���ͨ�Žṹ
	stc_uart_baud_config_t stcBaud;	//uart�����ʽṹ
	stc_bt_config_t stcBtConfig;	//basetime���ýṹ
	

	DDL_ZERO_STRUCT(stcUartIrqCb);	//���uart�жϷ���ṹ
	DDL_ZERO_STRUCT(stcMulti);		//���uart���ͨ�Žṹ
	DDL_ZERO_STRUCT(stcBaud);		//���uart�����ʽṹ
	DDL_ZERO_STRUCT(stcBtConfig);	//���basetime���ýṹ					
    
	if(uart_style->UARTCH==UARTCH1)
	{
		//UART1�˿�����
		Gpio_InitIOExt(2,3,GpioDirOut,TRUE,FALSE,FALSE,FALSE);   	//����P35-Ϊ����������uart�������̵�
		Gpio_InitIOExt(2,4,GpioDirOut,TRUE,FALSE,FALSE,FALSE); 		//����P36-Ϊ����������uart�������̵�
		
		//ͨ���˿�����
		Gpio_SetFunc_UART1RX_P24();		//���������ö˿�P35Ϊuart_TX
		Gpio_SetFunc_UART1TX_P23();		//����P36Ϊuart_RX
		
	}
	else
	{
		//UART0�˿�����
		Gpio_InitIOExt(1,4,GpioDirOut,TRUE,FALSE,FALSE,FALSE);   	//����P01-Ϊ����������uart�������̵�
		Gpio_InitIOExt(1,5,GpioDirOut,TRUE,FALSE,FALSE,FALSE); 		//����P02-Ϊ����������uart�������̵�
		
		//ͨ���˿�����
		Gpio_SetFunc_UART0TX_P14();		//���������ö˿�P14Ϊuart_TX
		Gpio_SetFunc_UART0RX_P15();		//���������ö˿�P15Ϊuart_RX
	}
	
    //����ʱ��ʹ��
  Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);		//ʹ��basetime����ʱ��  //ģʽ0/2���Բ�ʹ��

	if(uart_style->UARTCH==UARTCH1)
	{
		Clk_SetPeripheralGate(ClkPeripheralUart1,TRUE);		//ʹ��uart1����ʱ��
	}
	else
	{
		Clk_SetPeripheralGate(ClkPeripheralUart0,TRUE);		//ʹ��uart0����ʱ��	
	}
	


	if(uart_style->UARTCH==UARTCH1)
	{
		stcUartIrqCb.pfnRxIrqCb = RxIntCallback1; 	//�����жϺ���
		stcUartIrqCb.pfnTxIrqCb = NULL;				//�����жϺ���
		stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback1;//֡�����жϺ���
		stcConfig.pstcIrqCb = &stcUartIrqCb;
		stcConfig.bTouchNvic = TRUE;	//����nvic�жϱ�־
	}
	else
	{
		stcUartIrqCb.pfnRxIrqCb = RxIntCallback0; 	//�����жϺ���
		stcUartIrqCb.pfnTxIrqCb = NULL;				//�����жϺ���
		stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback0;//֡�����жϺ���
		stcConfig.pstcIrqCb = &stcUartIrqCb;
		stcConfig.bTouchNvic = TRUE;	//����nvic�жϱ�־
	}

	stcConfig.enRunMode = uMode;
	stcMulti.enMulti_mode = UartNormal;// ������ģʽ����   //��������Ĵ˴���ת��������ģʽ��mode2/3���ж�����ģʽ
	stcConfig.pstcMultiMode = &stcMulti; //���ö���������
  stcBaud.bDbaud = 1u;
	stcBaud.u32Baud = baudrate;//���²�����λ��-���ò�����
	stcBaud.u8Mode = uMode;
	
	pclk = Clk_GetPClkFreq();  //��ȡpclkƵ��

	timer=Uart_SetBaudRate(uart_style->UARTCH,pclk,&stcBaud); //���uart��basetime����װ��ֵ
	stcBtConfig.enMD = BtMode2; //ģʽ2-��ʱģʽ
	stcBtConfig.enCT = BtTimer; //CT-���ö�ʱ����
	
	Bt_Init(uart_style->TIM, &stcBtConfig);//basetime��ʼ��   //����basetimer1���ú�������������
	Bt_ARRSet(uart_style->TIM,timer);		//����basetime��װ��ֵ
	Bt_Cnt16Set(uart_style->TIM,timer);	//���ü�������ֵ
	Bt_Run(uart_style->TIM);				//ʹ��basetime

//����UART�ж����ȼ�Ĭ��Ϊ3
	Uart_Init(uart_style->UARTCH, &stcConfig);  //uart��ʼ��
	Uart_EnableIrq(uart_style->UARTCH,UartRxIrq);	//uartʹ���ж�
	Uart_ClrStatus(uart_style->UARTCH,UartRxFull);	//uart�����־λ
	Uart_EnableFunc(uart_style->UARTCH,UartRx);	//uart����/����ʹ��
}




//��������
void u1_printf(char * format, ...)
{
	unsigned int length;
	va_list	ap;
	va_start(ap, format);
	vsprintf(temp, format, ap);
	va_end(ap);

	length = strlen((const char*)temp);
	Uart_ClrStatus(UARTCH1,UartTxEmpty);     //���������ɱ��  
	for (u8 i = 0; i < length; i++)
	{
		M0P_UART1->SBUF=temp[i];
		while(FALSE==Uart_GetStatus(UARTCH1,UartTxEmpty));
		Uart_ClrStatus(UARTCH1,UartTxEmpty);     //���������ɱ��  
	}
}

void u1_printf2(uint8_t *temp, int length)
{ 
	for (u8 i = 0; i < length; i++)
	{
		M0P_UART1->SBUF=temp[i];
		while(FALSE==Uart_GetStatus(UARTCH1,UartTxEmpty));
		Uart_ClrStatus(UARTCH1,UartTxEmpty);     //���������ɱ��  
	}
}

void u0_printf2(uint8_t *temp, int length)
{ 
	for (u8 i = 0; i < length; i++)
	{
		M0P_UART0->SBUF=temp[i];
		while (FALSE==Uart_GetStatus(UARTCH0,UartTxEmpty));
		Uart_ClrStatus(UARTCH0,UartTxEmpty);     //���������ɱ��  
	}
}

void u0_printf(char * format, ...)
{ 
	unsigned int length;
	va_list	ap;
	va_start(ap, format);
	vsprintf(temp, format, ap);
	va_end(ap);

	length = strlen((const char*)temp);

	Uart_ClrStatus(UARTCH0,UartTxEmpty);     //���������ɱ��  
	for (u8 i = 0; i < length; i++)
	{
		M0P_UART0->SBUF=temp[i];
		while (FALSE==Uart_GetStatus(UARTCH0,UartTxEmpty));
		Uart_ClrStatus(UARTCH0,UartTxEmpty);     //���������ɱ��  
	}
}





