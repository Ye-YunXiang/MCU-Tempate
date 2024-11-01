/*********************************************************************************************/
【*】程序简介

-工程名称：SysTick—系统定时器
-实验平台: 野火STM32 霸道 开发板 

【 ！】功能简介：
利用 SysTick 产生 1s 的时基，LED以 1s 的频率闪烁。


【 ！】实验操作：
编译并下载程序到开发板，呈现红-绿-蓝每个颜色维持1s的流水灯


/*********************************************************************************************/
【*】 引脚分配

	LED_R	<-----> PB5
	LED_G	<-----> PB0
	LED_B	<-----> PB1

	对应引脚低电平则LED灯亮

 * 对于霸道V2板，在实验中，请不要拔掉旁边J73对应盖好的黄色跳帽，这样会导致LED失去3V3的电源而不能工作。



/*********************************************************************************************/
【*】程序描述：


< bsp_SysTick.c >

	1.初始化 SysTick_Config()，设定它的形参用以决定系统定时器发生中断的时间
	2.设定定时时间，利用中断次数*中断时间=定时时间编写定时函数
	3.编写中断服务函数，SysTick 每进一次中断， 之前设定好的中断次数就递减一次


* 中断时间的计算公式：TINT=VALUELOAD * TDEC= VALUELOAD/CLKAHB，其中 CLKAHB =72MHZ

PS：此程序文件中含有多个不同思路的定时函数，Delay_us、SysTick_Delay_Us、SysTick_Delay_Ms
       在main.c中调用的是SysTick_Delay_Ms，它是一个毫秒级的定时函数



< stm32f10x_it.c >

	1.在 SysTick 中断函数处调用之前编写的中断服务函数



< main.c >

	1.初始化LED端口
	2.初始化系统定时器
	3.设定死循环，在此期间会呈现红-绿-蓝每个颜色维持1s的流水灯



/*********************************************************************************************/

【*】 联系我们

-野火官网  :https://embedfire.com
-野火论坛  :http://www.firebbs.cn
-野火商城  :https://yehuosm.tmall.com/
-野火资料下载中心：http://doc.embedfire.com/products/link

/*********************************************************************************************/