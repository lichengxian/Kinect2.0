#include <Kinect.h>
#include <opencv2/imgproc/types_c.h>
#include<opencv2\opencv.hpp>
#include <queue>
#include "display.h"
#include <windows.h>

using namespace std;
using namespace cv;

// ���߳���ʾ��ͼ
void show_color(queue<Color>* color_q)
{	
	// �Ƿ���ʾ�Ǽ�
	bool show_skeleton = true;
	// ��Ƶ���������
	cv::Mat c_img(1080, 1920, CV_8UC4);
	// ����ͼ��
	cv::Mat resize_img;
	Size dsize = Size(960, 540);
	
	while (1) {
		// ���л���̫��
		if (color_q->size() > 10) {
			cout << "color_queue size too long!" << endl;
		}
		// ����Ϊ������̹���
		if (color_q->size() < 1) {
			Sleep(1);
		}
		else {
			//�Ӷ����л�ȡ����
			Color color_data = color_q->front();
			// �������������
			c_img = color_data.cimg.clone();
			// ���Ǽ�
			if (show_skeleton) {
				drawBody(c_img, color_data.colorX, color_data.colorY);
			}
			// ����ͼ��
			resize(c_img, resize_img, dsize, 0, 0, INTER_LINEAR);
			imshow("color", resize_img);
			waitKey(1);
			color_q->pop();
		}
	}
}

// ���߳���ʾ���ͼ
void show_depth(queue<Depth>* depth_q)
{
	// �Ƿ���ʾ�Ǽ�
	bool show_skeleton = true;
	// ���ͼ���ڿ���
	cv::Mat d_img(424, 512, CV_8UC1);

	while (1) {
		// ���л���̫��
		if (depth_q->size() > 10) {
			cout << "depth_queue size too long!" << endl;
		}
		// ����Ϊ������̹���
		if (depth_q->size() < 1) {
			Sleep(1);
		}
		else {
			//�Ӷ����л�ȡ����
			Depth depth_data = depth_q->front();
			// �������������
			d_img = depth_data.dimg.clone();
			// ���Ǽ�
			if (show_skeleton) {
				drawBody(d_img, depth_data.depthX, depth_data.depthY);
			}
			imshow("depth", d_img);
			waitKey(1);
			depth_q->pop();
		}
	}
}

void drawBody(cv::Mat img, float* coorx, float* coory) 
{
	//-----------------------------------body-----------------------------------
	DrawBone(img, coorx, coory, JointType_Head, JointType_Neck);
	DrawBone(img, coorx, coory, JointType_Neck, JointType_SpineShoulder);
	DrawBone(img, coorx, coory, JointType_SpineShoulder, JointType_SpineMid);
	DrawBone(img, coorx, coory, JointType_SpineMid, JointType_SpineBase);
	DrawBone(img, coorx, coory, JointType_SpineShoulder, JointType_ShoulderRight);
	DrawBone(img, coorx, coory, JointType_SpineShoulder, JointType_ShoulderLeft);
	DrawBone(img, coorx, coory, JointType_SpineBase, JointType_HipRight);
	DrawBone(img, coorx, coory, JointType_SpineBase, JointType_HipLeft);

	//-----------------------------------Right Arm----------------------------------- 
	DrawBone(img, coorx, coory, JointType_ShoulderRight, JointType_ElbowRight);
	DrawBone(img, coorx, coory, JointType_ElbowRight, JointType_WristRight);
	DrawBone(img, coorx, coory, JointType_WristRight, JointType_HandRight);
	DrawBone(img, coorx, coory, JointType_HandRight, JointType_HandTipRight);
	DrawBone(img, coorx, coory, JointType_WristRight, JointType_ThumbRight);

	//-----------------------------------Left Arm-----------------------------------
	DrawBone(img, coorx, coory, JointType_ShoulderLeft, JointType_ElbowLeft);
	DrawBone(img, coorx, coory, JointType_ElbowLeft, JointType_WristLeft);
	DrawBone(img, coorx, coory, JointType_WristLeft, JointType_HandLeft);
	DrawBone(img, coorx, coory, JointType_HandLeft, JointType_HandTipLeft);
	DrawBone(img, coorx, coory, JointType_WristLeft, JointType_ThumbLeft);

	//-----------------------------------Right Leg-----------------------------------
	DrawBone(img, coorx, coory, JointType_HipRight, JointType_KneeRight);
	DrawBone(img, coorx, coory, JointType_KneeRight, JointType_AnkleRight);
	DrawBone(img, coorx, coory, JointType_AnkleRight, JointType_FootRight);

	//------------------------------------Left Leg-----------------------------------
	DrawBone(img, coorx, coory, JointType_HipLeft, JointType_KneeLeft);
	DrawBone(img, coorx, coory, JointType_KneeLeft, JointType_AnkleLeft);
	DrawBone(img, coorx, coory, JointType_AnkleLeft, JointType_FootLeft);
}

void DrawBone(cv::Mat img, float* coorx, float* coory, JointType joint0, JointType joint1)
{
	CvPoint p1 = cvPoint(coorx[joint0], coory[joint0]),
		    p2 = cvPoint(coorx[joint1], coory[joint1]);
	line(img, p1, p2, cvScalar(0, 255, 0), 5);
}