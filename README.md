# HC32F005-ADS1115
HC32F005C6PA ADS1115
HC32时钟为22.15MHz，继而I2C传输速率如果为100k的话，根据公式



在这里将stcI2cCfg.u8Tm设置为 0x10。

I2C端口定义       SDA -> P25     SCL -> P26

ADS1115写地址可选择0x90，0x92，在这里选择了0x92
