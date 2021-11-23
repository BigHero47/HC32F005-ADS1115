#ifndef __GPIOAPP_H__
#define __GPIOAPP_H__

#include "gpio.h"

#define BIT(num, n)  ((num & (1 << n)) > 0)

void Gpio_Init(void);
void gpioctrl(int i, boolean_t bVal);


/******************************************************************************/
/*             需要读取或配置的(继电器)IO口                                   */
/******************************************************************************/
#define GPIO1_PORT    	1
#define GPIO1_PIN		    5

#define GPIO2_PORT    	1
#define GPIO2_PIN		    4

#define GPIO3_PORT    	2
#define GPIO3_PIN		    5

#define GPIO4_PORT    	2
#define GPIO4_PIN		    6

#define GPIO5_PORT    	3
#define GPIO5_PIN		    2

#define GPIO6_PORT    	3
#define GPIO6_PIN		    3

#define GPIO7_PORT    	3
#define GPIO7_PIN		    4

#define GPIO8_PORT    	3
#define GPIO8_PIN		    5

#define GPIO_485CTL_PORT   	3
#define GPIO_485CTL_PIN	    6

#define GPIO_I2C_SDA_PORT   2
#define GPIO_I2C_SDA_PIN	  5

#define GPIO_I2C_SCL_PORT   2
#define GPIO_I2C_SCL_PIN	  6




#endif
