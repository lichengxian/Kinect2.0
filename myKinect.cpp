#include "myKinect.h"
#include <iostream>
#include <opencv2/imgproc/types_c.h>
#include<opencv2\opencv.hpp>

using namespace cv;
using namespace std;

/// Initializes the default Kinect sensor
HRESULT CBodyBasics::InitializeDefaultSensor()
{
	// �����ж�ÿ�ζ�ȡ�����ĳɹ����
	HRESULT hr;

	// ����kinect
	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr)){
		return hr;
	}

	// �ҵ�kinect�豸
	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the other reader
		IBodyFrameSource* pBodyFrameSource = NULL;//��ȡ�Ǽ�
		IDepthFrameSource* pDepthFrameSource = NULL;//��ȡ�����Ϣ
		IBodyIndexFrameSource* pBodyIndexFrameSource = NULL;//��ȡ������ֵͼ
		IColorFrameSource* pColorFrameSource = NULL; //��ȡ��ͼ��

		// ��kinect
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

		// ��ͼ��
		if (SUCCEEDED(hr)) {
			hr = m_pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
		}

		if (SUCCEEDED(hr)) {
			hr = pColorFrameSource->OpenReader(&m_pColorFrameReader);
		}
		
		// �ͷ�
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

	//skeletonImg,���ڻ�������ֵͼ(���˵�Mask)��MAT
	//skeletonImg.create(cDepthHeight, cDepthWidth, CV_8UC3);
	//skeletonImg.setTo(0);

	//depthImg,���ڻ������Ϣ��MAT
	//depthImg.create(cDepthHeight, cDepthWidth, CV_8UC1);
	//depthImg.setTo(0);

	//colorImg,���ڻ���ͼ����MAT
	colorImg.create(colorHeight, colorWidth, CV_8UC4);
	colorImg.setTo(0);

	return hr;
}

/// Main processing function
DATA CBodyBasics::Update()
{
	//ÿ�������skeletonImg
	//skeletonImg.setTo(0);

	data.flag = false;

	//�����ʧ��kinect���򲻼�������
	if (!m_pBodyFrameReader)
	{
		return data;
	}

	IBodyFrame* pBodyFrame = NULL;//�Ǽ���Ϣ
	IDepthFrame* pDepthFrame = NULL;//�����Ϣ
	IBodyIndexFrame* pBodyIndexFrame = NULL;//������ֵͼ
	IColorFrame* pColorFrame = NULL;//��ͼ��

	//��¼ÿ�β����ĳɹ����
	HRESULT hr = S_OK;

	//---------------------------------------��ȡ������ֵͼ����ʾ---------------------------------
	//if (SUCCEEDED(hr)){
	//	hr = m_pBodyIndexFrameReader->AcquireLatestFrame(&pBodyIndexFrame);//��ñ�����ֵͼ��Ϣ
	//}
	//if (SUCCEEDED(hr)){
	//	BYTE *bodyIndexArray = new BYTE[cDepthHeight * cDepthWidth];//������ֵͼ��8λuchar�������Ǻ�ɫ��û���ǰ�ɫ
	//	pBodyIndexFrame->CopyFrameDataToArray(cDepthHeight * cDepthWidth, bodyIndexArray);

	//	//�ѱ�����ֵͼ����MAT��
	//	uchar* skeletonData = (uchar*)skeletonImg.data;
	//	for (int j = 0; j < cDepthHeight * cDepthWidth; ++j){
	//		*skeletonData = bodyIndexArray[j]; ++skeletonData;
	//		*skeletonData = bodyIndexArray[j]; ++skeletonData;
	//		*skeletonData = bodyIndexArray[j]; ++skeletonData;
	//	}
	//	delete[] bodyIndexArray;
	//}
	//SafeRelease(pBodyIndexFrame);//����Ҫ�ͷţ�����֮���޷�����µ�frame����

	//---------------------------------------��ȡ������ݲ���ʾ---------------------------------
	//if (SUCCEEDED(hr)){
	//	hr = m_pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);//����������
	//}
	//if (SUCCEEDED(hr)){
	//	UINT16 *depthArray = new UINT16[cDepthHeight * cDepthWidth];//���������16λunsigned int
	//	pDepthFrame->CopyFrameDataToArray(cDepthHeight * cDepthWidth, depthArray);

	//	//��������ݻ���MAT��
	//	uchar* depthData = (uchar*)depthImg.data;
	//	for (int j = 0; j < cDepthHeight * cDepthWidth; ++j){
	//		*depthData = depthArray[j];++depthData;
	//	}
	//	delete[] depthArray;
	//}
	//SafeRelease(pDepthFrame);//����Ҫ�ͷţ�����֮���޷�����µ�frame����
	//imshow("depthImg", depthImg);
	//cv::waitKey(5);

	//---------------------------------------��ȡ��ͼ���ݲ���ʾ---------------------------------
	if (SUCCEEDED(hr)) {
		hr = m_pColorFrameReader->AcquireLatestFrame(&pColorFrame);//��ò�ͼ����
	}
	if (SUCCEEDED(hr)) {
		pColorFrame->CopyConvertedFrameDataToArray(colorWidth * colorHeight * 4, colorImg.data, ColorImageFormat::ColorImageFormat_Bgra);
		frame_num += 1;
		data.img = colorImg;
		data.flag = true;
	}
	SafeRelease(pColorFrame);//����Ҫ�ͷţ�����֮���޷�����µ�frame����
	//imshow("colorImg", colorImg);
	//cv::waitKey(5);

	//---------------------------------------��ȡ�Ǽܲ���ʾ---------------------------------
	if (SUCCEEDED(hr)){
		hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);//��ȡ�Ǽ���Ϣ
	}
	if (SUCCEEDED(hr))
	{
		IBody* ppBodies[BODY_COUNT] = { 0 };//ÿһ��IBody����׷��һ���ˣ��ܹ�����׷��������

		if (SUCCEEDED(hr))
		{
			//��kinect׷�ٵ����˵���Ϣ���ֱ�浽ÿһ��IBody��
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}

		if (SUCCEEDED(hr))
		{
			//��ÿһ��IBody�������ҵ����ĹǼ���Ϣ�����һ�����
			ProcessBody(BODY_COUNT, ppBodies);
		}

		for (int i = 0; i < _countof(ppBodies); ++i)
		{
			SafeRelease(ppBodies[i]);//�ͷ�����
		}
	}
	SafeRelease(pBodyFrame);//����Ҫ�ͷţ�����֮���޷�����µ�frame����
	return data;
}

/// Handle new body data
void CBodyBasics::ProcessBody(int nBodyCount, IBody** ppBodies)
{
	//��¼��������Ƿ�ɹ�
	HRESULT hr;

	//����ÿһ��IBody
	for (int i = 0; i < nBodyCount; ++i)
	{
		IBody* pBody = ppBodies[i];
		if (pBody)//��û�и���������pBody�������bTracked��ʲô����
		{
			BOOLEAN bTracked = false;
			hr = pBody->get_IsTracked(&bTracked);

			if (SUCCEEDED(hr) && bTracked)
			{
				Joint joints[JointType_Count];//�洢�ؽڵ���
				// HandState leftHandState = HandState_Unknown;//����״̬
				// HandState rightHandState = HandState_Unknown;//����״̬

				// ��ȡ������״̬
				// pBody->get_HandLeftState(&leftHandState);
				// pBody->get_HandRightState(&rightHandState);

				// �洢�������ϵ�еĹؽڵ�λ��
				//DepthSpacePoint *SpacePosition = new DepthSpacePoint[_countof(joints)];
				ColorSpacePoint* SpacePosition = new ColorSpacePoint[_countof(joints)];

				// ��ùؽڵ���
				hr = pBody->GetJoints(_countof(joints), joints);

				if (SUCCEEDED(hr))
				{	
					// ��ʾ�Ǽ��Ѿ��ɹ���ȡ
					if (s_flag) {
						cout << "Skeleton Get Successfully" << endl;
						s_flag = false;
					}
					// ����Ǽ�
					for (int j = 0; j < _countof(joints); ++j)
					{
						// ���ؽڵ���������������ϵ��-1~1��ת���������ϵ��424*512��
						//m_pCoordinateMapper->MapCameraPointToDepthSpace(joints[j].Position, &depthSpacePosition[j]);
						// ���ؽڵ���������������ϵ��-1~1��ת����ͼ����ϵ��1080*1920��
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

//���ֵ�״̬
void CBodyBasics::DrawHandState(const DepthSpacePoint depthSpacePosition, HandState handState)
{
	//����ͬ�����Ʒ��䲻ͬ��ɫ
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
	default://���û��ȷ�������ƣ��Ͳ�Ҫ��
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
		//�ǳ�ȷ���ĹǼܣ��ð�ɫֱ��
	    line(colorImg, p1, p2, cvScalar(0, 255, 0), 5);
	}
	else
	{
		//��ȷ���ĹǼܣ��ú�ɫֱ��
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
