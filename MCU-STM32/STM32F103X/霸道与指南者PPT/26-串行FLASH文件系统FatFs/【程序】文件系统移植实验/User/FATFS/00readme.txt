FatFs Module Source Files R0.11


FILES

  00readme.txt This file.
  history.txt  Revision history.    ������¼
  ffconf.h     Configuration file for FatFs module.  fatģ�������ļ�
  ff.h         Common include file for FatFs and application module.     ���õ�fatf��Ӧ��ģ������ļ���
  ff.c         FatFs module.    fatfģ�顣
  diskio.h     Common include file for FatFs and disk I/O module.  �����İ���fatf�ļ��ʹ���I/Oģ�顣
  diskio.c     An example of glue function to attach existing disk I/O module to FatFs.  һ�������д���I/Oģ�����ӵ�fatf��glue����ʾ����
  integer.h    Integer type definitions for FatFs.  fatf���������Ͷ��塣
  option       Optional external functions.   ��ѡ���ⲿ������


  Low level disk I/O module is not included in this archive because the FatFs
  module is only a generic file system layer and not depend on any specific
  storage device. You have to provide a low level disk I/O module that written
  to control the target storage device.

�ͼ�����I/Oģ�鲻����������浵�У���Ϊfatf

ģ��ֻ��һ��ͨ�õ��ļ�ϵͳ�㣬���������κ��ض���

�洢�豸���������ṩһ���ͼ��Ĵ���I/Oģ����д

����Ŀ��洢�豸��


