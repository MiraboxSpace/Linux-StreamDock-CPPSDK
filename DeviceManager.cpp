#include "DeviceManager.h"

DeviceManager::DeviceManager()
{
    this->transport=new tranSport();
    this->streamDockmaps=new std::map<char *,streamDock *>;
}

DeviceManager::~DeviceManager()
{
    delete this->streamDockmaps;
    delete this->transport;
}

std::map<char *,streamDock *> *DeviceManager::enumerate()
{
    std::vector<std::tuple<int, int, int>> products = {
        {std::make_tuple((int)USBVendorIDs::USB_VID, (int)USBProductIDs::USB_PID_STREAMDOCK_936,1)} 
    };
    for(const auto& product : products){
        struct hid_device_info *deviceInfo=this->transport->enumerate(std::get<0>(product),std::get<1>(product));
        while (deviceInfo)
        {
            streamDock *node;
            if (deviceInfo->serial_number==NULL)
            {
                deviceInfo=deviceInfo->next;
                continue;
            }
            if (std::get<2>(product)==1)
            {
                node=new streamDock293(this->transport,deviceInfo);
            }
            (*this->streamDockmaps)[deviceInfo->path]=node;
            deviceInfo=deviceInfo->next;
        }
        this->transport->freeEnumerate(deviceInfo);   
    }
    return streamDockmaps;
}


int DeviceManager::listen()
{
    struct udev *udev = udev_new();
    if (!udev) {
        std::cerr << "Failed to create udev object." << std::endl;
        return -1;
    }

    struct udev_monitor *monitor = udev_monitor_new_from_netlink(udev, "udev");
    if (!monitor) {
        std::cerr << "Failed to create udev monitor." << std::endl;
        udev_unref(udev);
        return -1;
    }
    udev_monitor_filter_add_match_subsystem_devtype(monitor, "usb", nullptr);
    udev_monitor_enable_receiving(monitor);
    int fd = udev_monitor_get_fd(monitor);
    if (fd < 0) {
        std::cerr << "Failed to get udev monitor file descriptor." << std::endl;
        udev_monitor_unref(monitor);
        udev_unref(udev);
        return -1;
    }
    std::vector<std::tuple<int, int, int>> products = {
        
        {std::make_tuple((int)USBVendorIDs::USB_VID, (int)USBProductIDs::USB_PID_STREAMDOCK_936,1)} 
    };
    int i=0;
    while (true) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        int ret = select(fd + 1, &fds, nullptr, nullptr, nullptr);
        if (ret < 0) {
            std::cerr << "select() failed." << std::endl;
            break;
        }
        if (FD_ISSET(fd, &fds)) {
            struct udev_device *dev = udev_monitor_receive_device(monitor);
            if (dev) {
                const char *action = udev_device_get_action(dev);
                const char *devnode = udev_device_get_devnode(dev);
                if (i==0)
                {
                    if (strcmp(action,"add")==0) {
                    std::cout << "Action: " << action << std::endl;
                    for(const auto& product : products){
                        sleep(1);
                        struct hid_device_info *info=this->transport->enumerate(std::get<0>(product),std::get<1>(product));
                        std::cout << info<< std::endl;
                        while (info)
                        {
                            if(streamDockmaps->find(info->path)==streamDockmaps->end())
                            {
                                (*streamDockmaps)[info->path]=new streamDock293(this->transport,info);
                                std::cout << "创建成功: "<< std::endl;
                                break;
                            }
                            info=info->next;
                        }
                    }
                    udev_device_unref(dev);
                    }else{
                        std::cout << "Action: " << action << std::endl;
                        sleep(1);
                        char *vid,*pid;
                        for(const auto& product : products){
                            struct hid_device_info *info=this->transport->enumerate(std::get<0>(product),std::get<1>(product));
                            int flag=0;
                            for (auto it = streamDockmaps->begin(); it!=streamDockmaps->end(); it++)
                            {
                                flag=0;
                                while (info)
                                {
                                    std::cout << "path" << it->first <<"   "<< info->path<< std::endl;
                                    if (strcmp(it->first,info->path)==0)
                                    {
                                        flag=1;
                                    }
                                    info=info->next;
                                }
                                 std::cout <<"1"<< std::endl;
                                if (flag==0)
                                {
                                    streamDockmaps->erase(it);
                                    std::cout << "删除成功: "<< std::endl;
                                    break;
                                }
                            }
                        }
                        udev_device_unref(dev);
                    }
                        i++;
                }else
                    i=0;
                
                }
            }
        }
    udev_monitor_unref(monitor);
    udev_unref(udev);
    return 1;
}
