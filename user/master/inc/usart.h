#ifndef __USART_H__
#define __USART_H__

#include "main.h"
#include "bt.h"

//串口选择结构体
struct uart_tim_style
{
	en_uart_channel_t UARTCH;
	en_bt_unit_t	TIM;
};

extern struct uart_tim_style uart0;
extern struct uart_tim_style uart1;

void RxIntCallback0(void);
void ErrIntCallback0(void);
void RxIntCallback1(void);
void ErrIntCallback1(void);

void Uart_Config(struct uart_tim_style * uart_style,u32 baudrate,en_uart_mode_t uMode);
void u1_printf(char * format, ...);
void u0_printf(char * format, ...);
void u1_printf2(uint8_t *temp, int length);
void u0_printf2(uint8_t *temp, int length);


#endif
