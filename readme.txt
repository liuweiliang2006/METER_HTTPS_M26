V1.0 完成长按功能的上传，本版本较老版本修改内容：
1、加入TIM14的使用，用于完成串口接收完成的判断；
2、增加SIGNAL文件，本文件实现了与M26的通信功能；
3、发送的长度由128改为512，真实通讯指令超过128字节。SIM80x.h
V1.1 修改 PostCookingSecsion函数中 下面三条指令的位置 ，放在while循环中，分配-释放配对。
Send_AT_cmd[8].SendCommand =(char *)malloc(20);
Send_AT_cmd[14].SendCommand =(char *)malloc(20);
struSeverInfo = (struct SeverInfo *) malloc(sizeof(struct SeverInfo));

20200908 1812
新建文件 encode.c encode.h,并集成函数PostMeterStatus

20200909 0942
集成PostMeterWarning功能，累积共集成PostMeterStatus、PostMeterWarning、PostCookingSecsion