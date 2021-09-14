#pragma once
#include <Kinect.h>
#include <opencv2\opencv.hpp>
#include <ctime>
#include <conio.h>

using namespace std;
using namespace cv;

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

// ԭ���Ǽ���Ϣ
struct Skeleton {
	float X[25];
	float Y[25];
	float Z[25];
};

// ���ͼ�ĹǼ���Ϣ
struct Depth{
	cv::Mat dimg;
	float depthX[25];
	float depthY[25];
};

// ��ͼ���еĹǼ���Ϣ
struct Color {
	cv::Mat cimg;
	float colorX[25];
	float colorY[25];
};

// ������Ҫ�Ľ��
struct DATA {
	bool flag = false;
	Skeleton skeleton;
	Depth depth;
	Color color;
};

class CBodyBasics
{
	//kinect 2.0 ����ȿռ�ĸ�*���� 424 * 512����ɫ�ռ���1080 * 1920���ڹ�������˵��
	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;
	static const int        colorWidth = 1920;
	static const int        colorHeight = 1080;

public:
	CBodyBasics();
	~CBodyBasics();
	DATA                    Update();//��ùǼܡ�������ֵͼ�������Ϣ
	HRESULT                 InitializeDefaultSensor();//���ڳ�ʼ��kinect
	// ��������
	DATA data;
	// ֡��
	int frame_num = 0;

private:
	IKinectSensor*          m_pKinectSensor;//kinectԴ
	ICoordinateMapper*      m_pCoordinateMapper;//��������任
	IBodyFrameReader*       m_pBodyFrameReader;//���ڹǼ����ݶ�ȡ
	IDepthFrameReader*      m_pDepthFrameReader;//����������ݶ�ȡ
	IBodyIndexFrameReader*  m_pBodyIndexFrameReader;//���ڱ�����ֵͼ��ȡ
	IColorFrameReader*      m_pColorFrameReader;//������Ƶ��

	//ͨ����õ�����Ϣ���ѹǼܺͱ�����ֵͼ������
	void ProcessBody(int nBodyCount, IBody** ppBodies);
	//���Ǽܺ���
	void DrawBone(const Joint* pJoints, const ColorSpacePoint* depthSpacePosition, JointType joint0, JointType joint1);
	//���ֵ�״̬����
	void DrawHandState(const DepthSpacePoint depthSpacePosition, HandState handState);

	//��ʾͼ���Mat
	cv::Mat skeletonImg;
	cv::Mat depthImg;
	cv::Mat colorImg;
};
