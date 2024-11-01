/*********************************************************************************************/

【*】程序简介

-工程名称：GPIO输出—使用固件库点亮LED灯
-实验平台: 野火STM32 霸道 开发板 

【 ！】功能简介：
控制GPIO引脚输出高低电平控制LED灯的亮灭，实现流水灯的效果

【 ！】实验操作：
编译并下载程序到开发板，复位后LED灯会以红-绿-蓝-红-绿-蓝-黄-紫-青-白-灭的顺序反复闪烁出现

/*********************************************************************************************/

【*】 引脚分配

LED：
霸道V1底板：
PB5 红灯
PB0 绿灯
PB1 蓝灯
低电平亮 实物位于板子左下角白色三色灯泡D3丝印

霸道V2底板：
PB5 红灯
PB0 绿灯
PB1 蓝灯
低电平亮  实物位于板子左下角白色三色灯泡D6丝印，左边有个J73跳帽为连接灯泡与3.3V供电

/*********************************************************************************************/

【*】程序描述：

< bsp_led.h > 

	1.宏定义LED灯对应的GPIO端口，GPIO时钟，GPIO引脚
	2.把LED的常用状态（亮、灭、状态反转）也直接定义成宏
	3.把控制R、G、B灯的不同开关状态混出不同的颜色对应的状态定义成宏



< bsp_led.c >     

- LED GPIO初始化

	1.引用结构体
	2.开启GPIO对应的外设时钟
	3.设置引脚、模式、速度
	4.初始化GPIO（根据设定参数配置 GPIO）
	5.关闭所有的LED灯

< main.c >

	1.调用LED GPIO初始化函数，初始化LED端口
	2.控制LED的亮灭

/*********************************************************************************************/

GPIO寄存器描述 《STM32参考手册中文-p75》

1、端口配置低寄存器(GPIOx_CRL)(x = A...E)
2、端口配置高寄存器(GPIOx_CRH)(x = A...E)

3、端口输入数据寄存器(GPIOx_IDR)(x = A...E)
   这些位为只读并只能以字(16位)的形式读出。读出的值为对应I/O口的状态。
   
4、端口输出数据寄存器(GPIOx_ODR)(x = A...E)
   只能以字(16bit)的形式操作，复位值全是0。写0即输出0，写1即输出1。
   
5、端口位设置/清除寄存器(GPIOx_BSRR)(x = A...E)
   高16bit写1用于清0，低16bit写1用于置位，同时写1的话低16bi有效。
   
6、端口位清除寄存器(GPIOx_BRR)(x = A...E)
   低16位写1用于置位。
   
7、端口配置锁定寄存器(GPIOx_LCKR)(x = A...E)

GPIO八种模式
typedef enum
{ GPIO_Mode_AIN = 0x0,            /* 模拟输入 */   
  GPIO_Mode_IN_FLOATING = 0x04,   /* 浮空输入，复位后的状态 */
  GPIO_Mode_IPD = 0x28,           /* 下拉输入 */
  GPIO_Mode_IPU = 0x48,           /* 上拉输入 */
  GPIO_Mode_Out_OD = 0x14,        /* 开漏输出 */
  GPIO_Mode_Out_PP = 0x10,        /* 推挽输出 */
  GPIO_Mode_AF_OD = 0x1C,         /* 复用开漏输出 */
  GPIO_Mode_AF_PP = 0x18          /* 复用推挽输出 */
}GPIOMode_TypeDef;




/*********************************************************************************************/

【*】 联系我们

-野火官网  :https://embedfire.com
-野火论坛  :http://www.firebbs.cn
-野火商城  :https://yehuosm.tmall.com/
-野火资料下载中心 :http://doc.embedfire.com/products/link

/*********************************************************************************************/