#include "usart.h"

struct uart_tim_style uart0={UARTCH0,TIM0};  //串口0   	用于初始化串口
struct uart_tim_style uart1={UARTCH1,TIM1};	 //串口1	  用于初始化串口

char temp[50];


//UART0错误中断
void ErrIntCallback0(void)
{
}


//UART1中断函数
void RxIntCallback0(void) 
{
}



//UART1错误中断
void ErrIntCallback1(void)
{
	
}


/*************************************************************************************
*函数名称:void Uart_Config(struct uart_tim_style * uart_style,u32 baudrate,en_uart_mode_t uMode)
*功能描述:串口初始化
*输入参数:none
*输出参数:none
*返回值:none
*其他说明:
*修改日期         版本号                   修改人             修改内容
*------------------------------------------------------------------------------------
*************************************************************************************/
void Uart_Config(struct uart_tim_style * uart_style,u32 baudrate,en_uart_mode_t uMode)
{

	Bt_Stop(uart_style->TIM); //停止TIM1
	Uart_DisableIrq(uart_style->UARTCH,UartRxIrq);//关闭接受中断
	Uart_EnableFunc(uart_style->UARTCH,UartTx); //关闭接受使能，但发送仍然开启
	
	uint16_t timer=0;  //基础定时器设置波特率的计数值
	uint32_t pclk=0;  	//pclk频率

	stc_uart_config_t  stcConfig;  	//uart配置结构
	stc_uart_irq_cb_t stcUartIrqCb;	//uart中断服务函数结构
	stc_uart_multimode_t stcMulti;	//uart多机通信结构
	stc_uart_baud_config_t stcBaud;	//uart波特率结构
	stc_bt_config_t stcBtConfig;	//basetime配置结构
	

	DDL_ZERO_STRUCT(stcUartIrqCb);	//清空uart中断服务结构
	DDL_ZERO_STRUCT(stcMulti);		//清空uart多机通信结构
	DDL_ZERO_STRUCT(stcBaud);		//清空uart波特率结构
	DDL_ZERO_STRUCT(stcBtConfig);	//清空basetime配置结构					
    
	if(uart_style->UARTCH==UARTCH1)
	{
		//UART1端口配置
		Gpio_InitIOExt(2,3,GpioDirOut,TRUE,FALSE,FALSE,FALSE);   	//设置P35-为接下来设置uart复用做铺垫
		Gpio_InitIOExt(2,4,GpioDirOut,TRUE,FALSE,FALSE,FALSE); 		//设置P36-为接下来设置uart复用做铺垫
		
		//通道端口配置
		Gpio_SetFunc_UART1RX_P24();		//真正的设置端口P35为uart_TX
		Gpio_SetFunc_UART1TX_P23();		//设置P36为uart_RX
		
	}
	else
	{
		//UART0端口配置
		Gpio_InitIOExt(1,4,GpioDirOut,TRUE,FALSE,FALSE,FALSE);   	//设置P01-为接下来设置uart复用做铺垫
		Gpio_InitIOExt(1,5,GpioDirOut,TRUE,FALSE,FALSE,FALSE); 		//设置P02-为接下来设置uart复用做铺垫
		
		//通道端口配置
		Gpio_SetFunc_UART0TX_P14();		//真正的设置端口P14为uart_TX
		Gpio_SetFunc_UART0RX_P15();		//真正的设置端口P15为uart_RX
	}
	
    //外设时钟使能
  Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);		//使能basetime外设时钟  //模式0/2可以不使能

	if(uart_style->UARTCH==UARTCH1)
	{
		Clk_SetPeripheralGate(ClkPeripheralUart1,TRUE);		//使能uart1外设时钟
	}
	else
	{
		Clk_SetPeripheralGate(ClkPeripheralUart0,TRUE);		//使能uart0外设时钟	
	}
	


	if(uart_style->UARTCH==UARTCH1)
	{
		stcUartIrqCb.pfnRxIrqCb = RxIntCallback1; 	//接受中断函数
		stcUartIrqCb.pfnTxIrqCb = NULL;				//发送中断函数
		stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback1;//帧错误中断函数
		stcConfig.pstcIrqCb = &stcUartIrqCb;
		stcConfig.bTouchNvic = TRUE;	//设置nvic中断标志
	}
	else
	{
		stcUartIrqCb.pfnRxIrqCb = RxIntCallback0; 	//接受中断函数
		stcUartIrqCb.pfnTxIrqCb = NULL;				//发送中断函数
		stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback0;//帧错误中断函数
		stcConfig.pstcIrqCb = &stcUartIrqCb;
		stcConfig.bTouchNvic = TRUE;	//设置nvic中断标志
	}

	stcConfig.enRunMode = uMode;
	stcMulti.enMulti_mode = UartNormal;// 多主机模式配置   //测试项，更改此处来转换多主机模式，mode2/3才有多主机模式
	stcConfig.pstcMultiMode = &stcMulti; //设置多主机配置
  stcBaud.bDbaud = 1u;
	stcBaud.u32Baud = baudrate;//更新波特率位置-设置波特率
	stcBaud.u8Mode = uMode;
	
	pclk = Clk_GetPClkFreq();  //获取pclk频率

	timer=Uart_SetBaudRate(uart_style->UARTCH,pclk,&stcBaud); //获得uart给basetime的重装载值
	stcBtConfig.enMD = BtMode2; //模式2-定时模式
	stcBtConfig.enCT = BtTimer; //CT-设置定时功能
	
	Bt_Init(uart_style->TIM, &stcBtConfig);//basetime初始化   //调用basetimer1设置函数产生波特率
	Bt_ARRSet(uart_style->TIM,timer);		//设置basetime重装载值
	Bt_Cnt16Set(uart_style->TIM,timer);	//设置计数器初值
	Bt_Run(uart_style->TIM);				//使能basetime

//这里UART中断优先级默认为3
	Uart_Init(uart_style->UARTCH, &stcConfig);  //uart初始化
	Uart_EnableIrq(uart_style->UARTCH,UartRxIrq);	//uart使能中断
	Uart_ClrStatus(uart_style->UARTCH,UartRxFull);	//uart清除标志位
	Uart_EnableFunc(uart_style->UARTCH,UartRx);	//uart接收/发送使能
}




//发送数据
void u1_printf(char * format, ...)
{
	unsigned int length;
	va_list	ap;
	va_start(ap, format);
	vsprintf(temp, format, ap);
	va_end(ap);

	length = strlen((const char*)temp);
	Uart_ClrStatus(UARTCH1,UartTxEmpty);     //清除发送完成标记  
	for (u8 i = 0; i < length; i++)
	{
		M0P_UART1->SBUF=temp[i];
		while(FALSE==Uart_GetStatus(UARTCH1,UartTxEmpty));
		Uart_ClrStatus(UARTCH1,UartTxEmpty);     //清除发送完成标记  
	}
}

void u1_printf2(uint8_t *temp, int length)
{ 
	for (u8 i = 0; i < length; i++)
	{
		M0P_UART1->SBUF=temp[i];
		while(FALSE==Uart_GetStatus(UARTCH1,UartTxEmpty));
		Uart_ClrStatus(UARTCH1,UartTxEmpty);     //清除发送完成标记  
	}
}

void u0_printf2(uint8_t *temp, int length)
{ 
	for (u8 i = 0; i < length; i++)
	{
		M0P_UART0->SBUF=temp[i];
		while (FALSE==Uart_GetStatus(UARTCH0,UartTxEmpty));
		Uart_ClrStatus(UARTCH0,UartTxEmpty);     //清除发送完成标记  
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

	Uart_ClrStatus(UARTCH0,UartTxEmpty);     //清除发送完成标记  
	for (u8 i = 0; i < length; i++)
	{
		M0P_UART0->SBUF=temp[i];
		while (FALSE==Uart_GetStatus(UARTCH0,UartTxEmpty));
		Uart_ClrStatus(UARTCH0,UartTxEmpty);     //清除发送完成标记  
	}
}





