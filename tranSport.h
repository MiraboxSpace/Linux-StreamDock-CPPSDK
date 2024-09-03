#ifndef _TRANSPOT_
#define _TRANSPOT_

#include <iostream>
#include "hidapi.h"
#include <stdio.h>
#include <sys/stat.h>
class tranSport
{
private:
    hid_device *handle;
public:
    tranSport();
    //打开设备
    int open(char *path);
    //获取设备固件版本号 lenth缓冲区大小建议513
    unsigned char *getInputReport(int lenth);
    //获取设备的反馈信息，data 缓冲区首地址，lenth缓冲区长度
    int read(unsigned char *data,unsigned long lenth);
    //向设备下发数据， data 数据的首地址，数据的长度，当数据长度不够513，自动补齐513
    int write(unsigned char *data,unsigned long lenth);
    //释放enumerate返回的数据列表
    void freeEnumerate(hid_device_info *devs);
    //返回设备信息列表
    hid_device_info *enumerate(int vid, int pid);
    //设置设备屏幕亮度
    int setBrightness(int percent);
    //设置设备背景图片
    int setBackgroundImg(unsigned char *buffer);
    //设置设备按键图标
    int setKeyImg(std::string path,int key);
    //清空设备某个按键图标
    int keyClear(int key);
    //清空设备所有按键图标
    int keyAllClear();
    //唤醒设备屏幕
    int wakeScreen();
    //在数据传输完成后刷新显示
    int refresh();
    //断开设备连接
    int disconnected();
    //关闭设备
    void close();

};

#endif