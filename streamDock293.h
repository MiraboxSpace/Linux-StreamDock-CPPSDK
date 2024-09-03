#ifndef _STREAMDOCK293_
#define _STREAMDOCK293_

#include "streamDock.h"
#include "tranSport.h"
#include <iostream>
#include "hidapi.h"
#include <string>
#include <vector>
#include <cstdio>
#include <string>
#include <stdio.h>
#include <jpeglib.h>
#include <string.h>
#include <algorithm>
#include <stdlib.h>
#include <sys/stat.h>
#include "png.h"

class streamDock293 :public streamDock
{
private:

public:

    streamDock293(tranSport *transport,struct hid_device_info *devInfo);
    ~streamDock293();

 

    /*
        @note:获取设备的固件版本
        @param lenth ：固件版本的长度
        @return 返回固件版本的版本号存放数组的首地址，如果出错返回空
    */
    unsigned char *getFirmVersion(int lenth );

    /*
        @note:设置设备屏幕的亮度,如果传入的值小于0，值会被拉回0，如果大于100会被回100
        @param percent ：亮度的数值1-100
        @return 成功返回0，如果出错返回-1
    */
    int setBrightness(int percent);

    /*
        @note:设置设备屏幕的背景图片
        @param path ：图片的路径
        @return 成功返回1，如果出错返回-1
    */
    int setBackgroundImg(std::string path);


     /*
        @note:接受设备发送的信息
        @return 成功返回获得的数组首地址，如果出错返回NULL
    */
    unsigned char * read();
     /*
        @note:设置设备按键的图标
        @param path ：图片的路径
        @return 成功返回1，如果出错返回-1
    */
    int setKeyImg(std::string path,int key);
    /*
        @note:设置设备屏幕的背景图片
        @param path ：存放图片数据的数组
        @return 成功返回1，如果出错返回-1
    */
};





#endif