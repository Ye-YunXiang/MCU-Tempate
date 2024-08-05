FatFs Module Source Files R0.11


FILES

  00readme.txt This file.
  history.txt  Revision history.    修正记录
  ffconf.h     Configuration file for FatFs module.  fat模块配置文件
  ff.h         Common include file for FatFs and application module.     常用的fatf和应用模块包含文件。
  ff.c         FatFs module.    fatf模块。
  diskio.h     Common include file for FatFs and disk I/O module.  常见的包括fatf文件和磁盘I/O模块。
  diskio.c     An example of glue function to attach existing disk I/O module to FatFs.  一个将现有磁盘I/O模块连接到fatf的glue函数示例。
  integer.h    Integer type definitions for FatFs.  fatf的整数类型定义。
  option       Optional external functions.   可选的外部函数。


  Low level disk I/O module is not included in this archive because the FatFs
  module is only a generic file system layer and not depend on any specific
  storage device. You have to provide a low level disk I/O module that written
  to control the target storage device.

低级磁盘I/O模块不包括在这个存档中，因为fatf

模块只是一个通用的文件系统层，不依赖于任何特定的

存储设备。您必须提供一个低级的磁盘I/O模块来写

控制目标存储设备。


