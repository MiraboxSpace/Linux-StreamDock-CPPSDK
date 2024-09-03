#include <iostream>
#include "hidapi.h"
#include "streamDock.h"
#include "tranSport.h"
#include "streamDock293.h"
#include "DeviceManager.h"
#include <thread>
#include <unistd.h>
#define CMD_RESET			0xAA000000


void fun(streamDock *s)
{
    while(1)
    {
        unsigned char *buf=s->read();
        std::cout<<buf<<"  "<<buf+5<<" ";
        std::cout<<(int)buf[9]<<" ";
        std::cout<<(int)buf[10]<<"\n";
    }
}

int main()
{
    
    

    DeviceManager *manager=new DeviceManager();
    auto streamDocks=manager->enumerate();
    std::cout<<"发现"<<streamDocks->size()<<"台设备"<<"\n";
    //开线程监听设备插拔
    std::thread t(&DeviceManager::listen,manager);

        auto it=streamDocks->begin();
        auto s=it->second;
        s->open();
        //唤醒屏幕
        s->wakeScreen();
        //开启线程持续获取设备反馈的信息
        std::thread t1(fun,s);
        //设置屏幕亮度
        s->setBrightness(100);    
        sleep(4);
        std::string path="hy.png";
        //设置设备背景图片
        s->setBackgroundImg(path);
        sleep(4);
        //清空所有按键图标
        s->clearAllIcon();
        //设置指定按键图标
        s->setKeyImg("3.png",2);
        sleep(4);
        //清空指定按键图标
        s->cleaerIcon(1);
    

        t.join();

        t1.join();











    // s->setKeyImg("2.jpg",5);
    // new streamDock293(new tranSport(),hid_enumerate(0,0));


    // hid_device_info * info= hid_enumerate(0,0);
    // while (info)
    // {
    //     std::cout<<info->vendor_id<<"  "<<info->product_id<<"  "<<info->serial_number<<"\n";
    //     info=info->next;
    // }
    // hid_free_enumeration(info);

}


