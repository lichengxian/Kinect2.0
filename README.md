# Project Introduction
This is a C++ project on Win10, using Kinect2.0 sensor + OpenCV to get(save) Depth Data, Body Data and Color Data at 25fps. 

- Color Data
- Depth Data
- Body Data
    - Hand State
    - Body Index Data
    - Body Skeleton Data
 
Data Visualization now is available on multi-thread. Thread-main for collecting data form Kinect and saving data; Thread-1 for displaying color image; Thread-2 for displaying depth image. 

See more details in code annotations.
# Kinect2.0 sensor install
- USB3.0
- Windows 10
- [Kinect for Windows SDK 2.0](https://www.microsoft.com/en-us/download/details.aspx?id=44561)

Install [Kinect SDK v2.0_1409](https://www.microsoft.com/en-us/download/details.aspx?id=44561) and reboot, open Kinect Studio 2.0, make sure Kinect 2.0 can run successfully on Win10.

This [CSDN blog](https://blog.csdn.net/qq_36433118/article/details/88613818) can help you install Kinect2.0!
# Prerequisites
- Visual Studio 2019 （需要安装通用Windows平台开发和C++(v142)通用Windows平台工具）
- OpenCV 4.5.0 （所有dll文件都要复制到C:/Windows/System32/）

This [CSDN blog](https://blog.csdn.net/bioinformatique/article/details/105655809) can help you configure OpenCV on VS2019!

This [CSDN blog](https://blog.csdn.net/renyhui/article/details/52150984) can help you configure Kinect.h on the project!
