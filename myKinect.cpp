#include "myKinect.h"
#include <iostream>
#include <opencv2/imgproc/types_c.h>
#include<opencv2\opencv.hpp>

using namespace cv;
using namespace std;

/// Initializes the default Kinect sensor
HRESULT CBodyBasics::InitializeDefaultSensor()
{
	// 用于判断每次读取操作的成功与否
	HRESULT hr;

	// 搜索kinect
	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr)){
		return hr;
	}

	// 找到kinect设备
	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the other reader
		IBodyFrameSource* pBodyFrameSource = NULL;//读取骨架
		IDepthFrameSource* pDepthFrameSource = NULL;//读取深度信息
		IBodyIndexFrameSource* pBodyIndexFrameSource = NULL;//读取背景二值图
		IColorFrameSource* pColorFrameSource = NULL; //读取彩图流

		// 打开kinect
		hr = m_pKinectSensor->Open();

		// coordinatemapper
		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		// bodyframe
		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
		}

		// depth frame
		if (SUCCEEDED(hr)){
			hr = m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
		}

		if (SUCCEEDED(hr)){
			hr = pDepthFrameSource->OpenReader(&m_pDepthFrameReader);
		}

		// body index frame
		if (SUCCEEDED(hr)){
			hr = m_pKinectSensor->get_BodyIndexFrameSource(&pBodyIndexFrameSource);
		}

		if (SUCCEEDED(hr)){
			hr = pBodyIndexFrameSource->OpenReader(&m_pBodyIndexFrameReader);
		}

		// 彩图流
		if (SUCCEEDED(hr)) {
			hr = m_pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
		}

		if (SUCCEEDED(hr)) {
			hr = pColorFrameSource->OpenReader(&m_pColorFrameReader);
		}
		
		// 释放
		SafeRelease(pBodyFrameSource);
		SafeRelease(pDepthFrameSource);
		SafeRelease(pBodyIndexFrameSource);
		SafeRelease(pColorFrameSource);
	}

	if (!m_pKinectSensor || FAILED(hr))
	{
		cout << "Kinect initialization failed!" << endl;
		return E_FAIL;
	}

	//skeletonImg,用于画背景二值图(扣人的Mask)的MAT
	//skeletonImg.create(cDepthHeight, cDepthWidth, CV_8UC3);
	//skeletonImg.setTo(0);

	//depthImg,用于画深度信息的MAT
	//depthImg.create(cDepthHeight, cDepthWidth, CV_8UC1);
	//depthImg.setTo(0);

	//colorImg,用于画彩图流的MAT
	colorImg.create(colorHeight, colorWidth, CV_8UC4);
	colorImg.setTo(0);

	return hr;
}

/// Main processing function
DATA CBodyBasics::Update()
{
	//每次先清空skeletonImg
	//skeletonImg.setTo(0);

	data.flag = false;

	//如果丢失了kinect，则不继续操作
	if (!m_pBodyFrameReader)
	{
		return data;
	}

	IBodyFrame* pBodyFrame = NULL;//骨架信息
	IDepthFrame* pDepthFrame = NULL;//深度信息
	IBodyIndexFrame* pBodyIndexFrame = NULL;//背景二值图
	IColorFrame* pColorFrame = NULL;//彩图流

	//记录每次操作的成功与否
	HRESULT hr = S_OK;

	//---------------------------------------获取背景二值图并显示---------------------------------
	//if (SUCCEEDED(hr)){
	//	hr = m_pBodyIndexFrameReader->AcquireLatestFrame(&pBodyIndexFrame);//获得背景二值图信息
	//}
	//if (SUCCEEDED(hr)){
	//	BYTE *bodyIndexArray = new BYTE[cDepthHeight * cDepthWidth];//背景二值图是8位uchar，有人是黑色，没人是白色
	//	pBodyIndexFrame->CopyFrameDataToArray(cDepthHeight * cDepthWidth, bodyIndexArray);

	//	//把背景二值图画到MAT里
	//	uchar* skeletonData = (uchar*)skeletonImg.data;
	//	for (int j = 0; j < cDepthHeight * cDepthWidth; ++j){
	//		*skeletonData = bodyIndexArray[j]; ++skeletonData;
	//		*skeletonData = bodyIndexArray[j]; ++skeletonData;
	//		*skeletonData = bodyIndexArray[j]; ++skeletonData;
	//	}
	//	delete[] bodyIndexArray;
	//}
	//SafeRelease(pBodyIndexFrame);//必须要释放，否则之后无法获得新的frame数据

	//---------------------------------------获取深度数据并显示---------------------------------
	//if (SUCCEEDED(hr)){
	//	hr = m_pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);//获得深度数据
	//}
	//if (SUCCEEDED(hr)){
	//	UINT16 *depthArray = new UINT16[cDepthHeight * cDepthWidth];//深度数据是16位unsigned int
	//	pDepthFrame->CopyFrameDataToArray(cDepthHeight * cDepthWidth, depthArray);

	//	//把深度数据画到MAT中
	//	uchar* depthData = (uchar*)depthImg.data;
	//	for (int j = 0; j < cDepthHeight * cDepthWidth; ++j){
	//		*depthData = depthArray[j];++depthData;
	//	}
	//	delete[] depthArray;
	//}
	//SafeRelease(pDepthFrame);//必须要释放，否则之后无法获得新的frame数据
	//imshow("depthImg", depthImg);
	//cv::waitKey(5);

	//---------------------------------------获取彩图数据并显示---------------------------------
	if (SUCCEEDED(hr)) {
		hr = m_pColorFrameReader->AcquireLatestFrame(&pColorFrame);//获得彩图数据
	}
	if (SUCCEEDED(hr)) {
		pColorFrame->CopyConvertedFrameDataToArray(colorWidth * colorHeight * 4, colorImg.data, ColorImageFormat::ColorImageFormat_Bgra);
		frame_num += 1;
		data.img = colorImg;
		data.flag = true;
	}
	SafeRelease(pColorFrame);//必须要释放，否则之后无法获得新的frame数据
	//imshow("colorImg", colorImg);
	//cv::waitKey(5);

	//---------------------------------------获取骨架并显示---------------------------------
	if (SUCCEEDED(hr)){
		hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);//获取骨架信息
	}
	if (SUCCEEDED(hr))
	{
		IBody* ppBodies[BODY_COUNT] = { 0 };//每一个IBody可以追踪一个人，总共可以追踪六个人

		if (SUCCEEDED(hr))
		{
			//把kinect追踪到的人的信息，分别存到每一个IBody中
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}

		if (SUCCEEDED(hr))
		{
			//对每一个IBody，我们找到他的骨架信息，并且画出来
			ProcessBody(BODY_COUNT, ppBodies);
		}

		for (int i = 0; i < _countof(ppBodies); ++i)
		{
			SafeRelease(ppBodies[i]);//释放所有
		}
	}
	SafeRelease(pBodyFrame);//必须要释放，否则之后无法获得新的frame数据
	return data;
}

/// Handle new body data
void CBodyBasics::ProcessBody(int nBodyCount, IBody** ppBodies)
{
	//记录操作结果是否成功
	HRESULT hr;

	//对于每一个IBody
	for (int i = 0; i < nBodyCount; ++i)
	{
		IBody* pBody = ppBodies[i];
		if (pBody)//还没有搞明白这里pBody和下面的bTracked有什么区别
		{
			BOOLEAN bTracked = false;
			hr = pBody->get_IsTracked(&bTracked);

			if (SUCCEEDED(hr) && bTracked)
			{
				Joint joints[JointType_Count];//存储关节点类
				// HandState leftHandState = HandState_Unknown;//左手状态
				// HandState rightHandState = HandState_Unknown;//右手状态

				// 获取左右手状态
				// pBody->get_HandLeftState(&leftHandState);
				// pBody->get_HandRightState(&rightHandState);

				// 存储深度坐标系中的关节点位置
				//DepthSpacePoint *SpacePosition = new DepthSpacePoint[_countof(joints)];
				ColorSpacePoint* SpacePosition = new ColorSpacePoint[_countof(joints)];

				// 获得关节点类
				hr = pBody->GetJoints(_countof(joints), joints);

				if (SUCCEEDED(hr))
				{	
					// 表示骨架已经成功获取
					if (s_flag) {
						cout << "Skeleton Get Successfully" << endl;
						s_flag = false;
					}
					// 处理骨架
					for (int j = 0; j < _countof(joints); ++j)
					{
						// 将关节点坐标从摄像机坐标系（-1~1）转到深度坐标系（424*512）
						//m_pCoordinateMapper->MapCameraPointToDepthSpace(joints[j].Position, &depthSpacePosition[j]);
						// 将关节点坐标从摄像机坐标系（-1~1）转到彩图坐标系（1080*1920）
						m_pCoordinateMapper->MapCameraPointToColorSpace(joints[j].Position, &SpacePosition[j]);
						data.X[j] = joints[j].Position.X;
						data.Y[j] = joints[j].Position.Y;
						data.Z[j] = joints[j].Position.Z;
					}

					if (show_skeleton) {
						//-----------------------------------hand state left-----------------------------------
						// DrawHandState(depthSpacePosition[JointType_HandLeft], leftHandState);
						// DrawHandState(depthSpacePosition[JointType_HandRight], rightHandState);

						//-----------------------------------body-----------------------------------
						DrawBone(joints, SpacePosition, JointType_Head, JointType_Neck);
						DrawBone(joints, SpacePosition, JointType_Neck, JointType_SpineShoulder);
						DrawBone(joints, SpacePosition, JointType_SpineShoulder, JointType_SpineMid);
						DrawBone(joints, SpacePosition, JointType_SpineMid, JointType_SpineBase);
						DrawBone(joints, SpacePosition, JointType_SpineShoulder, JointType_ShoulderRight);
						DrawBone(joints, SpacePosition, JointType_SpineShoulder, JointType_ShoulderLeft);
						DrawBone(joints, SpacePosition, JointType_SpineBase, JointType_HipRight);
						DrawBone(joints, SpacePosition, JointType_SpineBase, JointType_HipLeft);

						//-----------------------------------Right Arm----------------------------------- 
						DrawBone(joints, SpacePosition, JointType_ShoulderRight, JointType_ElbowRight);
						DrawBone(joints, SpacePosition, JointType_ElbowRight, JointType_WristRight);
						DrawBone(joints, SpacePosition, JointType_WristRight, JointType_HandRight);
						DrawBone(joints, SpacePosition, JointType_HandRight, JointType_HandTipRight);
						DrawBone(joints, SpacePosition, JointType_WristRight, JointType_ThumbRight);

						//-----------------------------------Left Arm-----------------------------------
						DrawBone(joints, SpacePosition, JointType_ShoulderLeft, JointType_ElbowLeft);
						DrawBone(joints, SpacePosition, JointType_ElbowLeft, JointType_WristLeft);
						DrawBone(joints, SpacePosition, JointType_WristLeft, JointType_HandLeft);
						DrawBone(joints, SpacePosition, JointType_HandLeft, JointType_HandTipLeft);
						DrawBone(joints, SpacePosition, JointType_WristLeft, JointType_ThumbLeft);

						//-----------------------------------Right Leg-----------------------------------
						DrawBone(joints, SpacePosition, JointType_HipRight, JointType_KneeRight);
						DrawBone(joints, SpacePosition, JointType_KneeRight, JointType_AnkleRight);
						DrawBone(joints, SpacePosition, JointType_AnkleRight, JointType_FootRight);

						//------------------------------------Left Leg-----------------------------------
						DrawBone(joints, SpacePosition, JointType_HipLeft, JointType_KneeLeft);
						DrawBone(joints, SpacePosition, JointType_KneeLeft, JointType_AnkleLeft);
						DrawBone(joints, SpacePosition, JointType_AnkleLeft, JointType_FootLeft);
					}
				}
				delete[] SpacePosition;
			}
		}
	}
	//cv::imshow("skeletonImg", colorImg);
	//cv::waitKey(5);
}

//画手的状态
void CBodyBasics::DrawHandState(const DepthSpacePoint depthSpacePosition, HandState handState)
{
	//给不同的手势分配不同颜色
	CvScalar color;
	switch (handState){
	case HandState_Open:
		color = cvScalar(255, 0, 0);
		break;
	case HandState_Closed:
		color = cvScalar(0, 255, 0);
		break;
	case HandState_Lasso:
		color = cvScalar(0, 0, 255);
		break;
	default://如果没有确定的手势，就不要画
		return;
	}

	circle(skeletonImg,
		cvPoint(depthSpacePosition.X, depthSpacePosition.Y),
		20, color, -1);
}


/// Draws one bone of a body (joint to joint)
void CBodyBasics::DrawBone(const Joint* pJoints, const ColorSpacePoint* depthSpacePosition, JointType joint0, JointType joint1)
{
	TrackingState joint0State = pJoints[joint0].TrackingState;
	TrackingState joint1State = pJoints[joint1].TrackingState;

	// If we can't find either of these joints, exit
	if ((joint0State == TrackingState_NotTracked) || (joint1State == TrackingState_NotTracked))
	{
		return;
	}

	// Don't draw if both points are inferred
	if ((joint0State == TrackingState_Inferred) && (joint1State == TrackingState_Inferred))
	{
		return;
	}

	CvPoint p1 = cvPoint(depthSpacePosition[joint0].X, depthSpacePosition[joint0].Y),
		p2 = cvPoint(depthSpacePosition[joint1].X, depthSpacePosition[joint1].Y);

	// We assume all drawn bones are inferred unless BOTH joints are tracked
	if ((joint0State == TrackingState_Tracked) && (joint1State == TrackingState_Tracked))
	{
		//非常确定的骨架，用白色直线
	    line(colorImg, p1, p2, cvScalar(0, 255, 0), 5);
	}
	else
	{
		//不确定的骨架，用红色直线
		line(colorImg, p1, p2, cvScalar(0, 0, 255), 2);
	}
}


/// Constructor
CBodyBasics::CBodyBasics() :
m_pKinectSensor(NULL),
m_pCoordinateMapper(NULL),
m_pBodyFrameReader(NULL){}

/// Destructor
CBodyBasics::~CBodyBasics()
{
	SafeRelease(m_pBodyFrameReader);
	SafeRelease(m_pCoordinateMapper);

	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}
	SafeRelease(m_pKinectSensor);
}
