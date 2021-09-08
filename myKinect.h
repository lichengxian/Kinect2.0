#pragma once
#include <Kinect.h>
#include <opencv2\opencv.hpp>
#include <ctime>
#include <conio.h>

// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

// 返回想要的结果
struct DATA {
	cv::Mat img;
	bool flag = false;
	float X[25];
	float Y[25];
	float Z[25];
};


class CBodyBasics
{
	//kinect 2.0 的深度空间的高*宽是 424 * 512，彩色空间是1080 * 1920，在官网上有说明
	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;
	static const int        colorWidth = 1920;
	static const int        colorHeight = 1080;

public:
	CBodyBasics();
	~CBodyBasics();
	DATA                    Update();//获得骨架、背景二值图和深度信息
	HRESULT                 InitializeDefaultSensor();//用于初始化kinect
	// 返回数据
	DATA data;
	// 帧数
	int frame_num = 0;
	// 是否显示骨架
	bool show_skeleton = false;
	// 表示骨架已经成功获取
	bool s_flag = true;

private:
	IKinectSensor*          m_pKinectSensor;//kinect源
	ICoordinateMapper*      m_pCoordinateMapper;//用于坐标变换
	IBodyFrameReader*       m_pBodyFrameReader;//用于骨架数据读取
	IDepthFrameReader*      m_pDepthFrameReader;//用于深度数据读取
	IBodyIndexFrameReader*  m_pBodyIndexFrameReader;//用于背景二值图读取
	IColorFrameReader*      m_pColorFrameReader;//用于视频流

	//通过获得到的信息，把骨架和背景二值图画出来
	void                    ProcessBody(int nBodyCount, IBody** ppBodies);
	//画骨架函数
	void DrawBone(const Joint* pJoints, const ColorSpacePoint* depthSpacePosition, JointType joint0, JointType joint1);
	//画手的状态函数
	void DrawHandState(const DepthSpacePoint depthSpacePosition, HandState handState);

	//显示图像的Mat
	cv::Mat skeletonImg;
	cv::Mat depthImg;
	cv::Mat colorImg;
};

