#ifndef _STREAMDOCK_
#define _STREAMDOCK_

#include "tranSport.h"
#include "hidapi.h"
#include <algorithm>
class streamDock{

    public:
        streamDock(tranSport *transport,struct hid_device_info *devInfo);
        /*
        @note:获取设备的固件版本
        @param lenth ：固件版本的长度
        @return 返回固件版本的版本号存放数组的首地址，如果出错返回空
    	*/
		virtual unsigned char *getFirmVersion(int lenth );
		/*
		@note:打开设备
		*/
        int open();

		int disconnected();
		/*
        @note:设置设备屏幕的亮度,如果传入的值小于0，值会被拉回0，如果大于100会被回100
        @param percent ：亮度的数值1-100
        @return 成功返回0，如果出错返回-1
    	*/
        virtual int setBrightness(int percent);
		/*
        @note:设置设备屏幕的背景图片
        @param path ：图片的路径
        @return 成功返回1，如果出错返回-1
    	*/
		virtual int setBackgroundImg(std::string path);
		/*
        @note:设置设备屏幕的背景图片
        @param path ：存放图片数据的数组
        @return 成功返回1，如果出错返回-1
    	*/
		virtual int setBackgroundImg(unsigned char *buffer);
		/*
        @note:接受设备发送的信息
        @return 成功返回获得的数组首地址，如果出错返回NULL
    	*/
		virtual unsigned char * read();
		/*
        @note:设置设备按键的图标
        @param path ：图片的路径
        @return 成功返回1，如果出错返回-1
    	*/
		virtual int setKeyImg(std::string path, int key);
		/*
		@note:清空按键图标
		@param index:按键的标号
		@return:成功返回1，失败返回-1
		*/
		virtual int cleaerIcon(int index);
		/*
		@note:清空所有按键图标
		@return:成功返回1，失败返回-1
		*/
		virtual int clearAllIcon();
		/*
		@note:唤醒屏幕
		@return:成功返回1，失败返回-1
		*/
		virtual int wakeScreen();
		/*
		@note:在传输数据完成后刷新显示
		*/
		int refresh();
		
		
		static std::string getFileExtension(const std::string& fileName) 
		{
        	size_t pos = fileName.find_last_of('.');
        	if (pos == std::string::npos) {
        	    return ""; // 没有找到文件扩展名
        	}
        	return fileName.substr(pos + 1);
    	}

        tranSport *transport;
    private:
            /** Platform-specific device path */
			char *path;
			/** Device Vendor ID */
			unsigned short vendor_id;
			/** Device Product ID */
			unsigned short product_id;
			/** Serial Number */
			wchar_t *serial_number;
			/** Device Release Number in binary-coded decimal,
			    also known as Device Version Number */
			unsigned short release_number;
			/** Manufacturer String */
			wchar_t *manufacturer_string;
			/** Product string */
			wchar_t *product_string;        
    

};

#endif