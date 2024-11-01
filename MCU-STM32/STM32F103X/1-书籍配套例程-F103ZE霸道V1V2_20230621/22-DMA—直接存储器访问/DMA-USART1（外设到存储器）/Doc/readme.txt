/*********************************************************************************************/
【*】程序简介

-工程名称：直接存储器访问（外设到存储器）
-实验平台: 野火STM32-F103 霸道 开发板 
-ST固件库版本：3.5.0

/*********************************************************************************************/
 【*】实验操作：

      1.把编译好的程序下载到开发板上
      2.打开【多功能调试助手】，并配置好相关的参数      配置模式为 115200 8-N-1
      3.打开串口
      4.按下RETSET键，接收区会出现如下：
        
         DMA外设到存储器模式，用电脑向开发板串口发送数据，数据会返回到电脑。

         
  /*证明程序已经成功下载到板子上*/
       5.在发送区输入字符，点击发送数据，接收区会出现相同的字符


/*********************************************************************************************/
【*】 引脚分配

   LED灯：

	LED_R	<-----> PB5
	LED_G	<-----> PB0
	LED_B	<-----> PB1

	对应引脚低电平则LED灯亮

 * 对于霸道V2板，在实验中，请不要拔掉旁边J73对应盖好的黄色跳帽，这样会导致LED失去3V3的电源而不能工作。


   串口：

	CH340  RXD  <----->  USART1  TX  (PA9)
	CH340  TXD  <----->  USART1  RX  (PA10)


 * 开发板中的CH340的收发引脚默认通过跳帽连接到USART1， 如果想使用其他串口，
   可以把 CH340 跟 USART1 直接的连接跳帽拔掉，然后再把其他串口的 IO用杜邦线接到CH340的收发引脚即可。

/*********************************************************************************************/
【*】程序描述：

 <bsp_usart_dma.h>: 
	
	1.宏定义所选用的串口、串口的外设时钟、串口时钟使能以及串口的波特率
	 2.宏定义串口有关的GPIO时钟及其时钟使能
	 3.宏定义串口的收发引脚接口
	 4.宏定义串口的中断寄存器请求	
	 5.宏定义DMA的请求通道、外设寄存器地址、一次发送的数据量
	
			
 <bsp_usart_dma.c>:
	
	1.配置NVIC(嵌套向量中断控制器）	          
	2.配置USART(其中包括两大部分：GPIO口的初始化和USART的初始化。    
       		GPIO口的初始化和前面的一样，这里不赘述。
			USART的初始化：配置工作参数，完成配置的程序，配置串口中断优先级，使能串口接收中断，使能串口
	3.编程发送一个字节、8位数组、字符串、一个16位数的函数体
	4.重定向c库函数printf和scanf到串口
	5.DMA配置
   [*]与存储器到外设方向的程序区别是：它们的内存地址和方向的设置发生了变化



 <main.c>:

	1.调用USART的配置
	2.调用DMA的配置
	3.调用GPIO的配置
	4.使能，USART1向DMA发出RX请求


【*】注意：
	  1. <bsp_led.c>  <bsp_led.h>都和之前的配置一样，只是LED1和LED3对应的引脚调换，导致实验时，灯光闪烁的颜色不一样。
	  2.外设到存储器的通道不可以随便乱选，具体的看文档的DMA的各个通道的请求映像表。例如我们这个外设到存储器的例程，RX请求只能选DMA1的通道5


/*********************************************************************************************/

【*】 联系我们

-野火官网  :https://embedfire.com
-野火论坛  :http://www.firebbs.cn
-野火商城  :https://yehuosm.tmall.com/
-野火资料下载中心 :http://doc.embedfire.com/products/link

/*********************************************************************************************/