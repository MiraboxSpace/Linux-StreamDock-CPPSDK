#ifndef _DEVICEMANAGER_
#define _DEVICEMANAGER_

#include "tranSport.h"
#include "streamDock.h"
#include "productIDs.h"
#include "streamDock293.h"
#include "hidapi.h"
#include <map>
#include <tuple>
#include <vector>
#include <iostream>
#include <libudev.h>
#include <unistd.h>
class DeviceManager
{
private:
    tranSport *transport;
    std::map<char *,streamDock *> *streamDockmaps;
    struct hid_device_info *deviceInfo;
public:
    DeviceManager(/* args */);
    ~DeviceManager();
    //遍历所有在线的设备，并为设备创建对应的设备类
    std::map<char *,streamDock *> *enumerate();
    //监听设备插拔建议开线程
    int listen();
};


#endif