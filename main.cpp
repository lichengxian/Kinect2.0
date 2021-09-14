#include "myKinect.h"
#include <iostream>
#include <fstream>
#include <string>
#include "display.h"

using namespace std;
using namespace cv;

// ������
int main()
{
	// ��ʼ��kinect
	CBodyBasics myKinect;
	HRESULT hr = myKinect.InitializeDefaultSensor();
	DATA res;
	// ��ȡ�����ļ���
	cout << "Please Input Save File Name:" << endl;
	string filename;
	getline(cin, filename);
	// ���ݱ���·��
	string path = "D:/dataset/";
	// �����ͼ��Ƶmp4
	VideoWriter writer(path + filename + ".mp4", CAP_OPENCV_MJPEG, 25, Size(1920, 1080));
	// ���������Ƶmp4
	VideoWriter depth(path + filename + "_depth.mp4", CAP_OPENCV_MJPEG, 25, Size(512, 424));
	// ����Ǽ���Ϣtxt
	string skeleton;
	ofstream out(path + filename + ".txt");
	// ��¼����
	char key = 0;

	cout << "Kinect Initialization Successfully" << endl;
	cout << "Press 'S' to Start Record" << endl;

	// ���߳���ʾ
	queue<Color> color_q;
	thread t1(show_color, &color_q);
	t1.detach();
	queue<Depth> depth_q;
	thread t2(show_depth, &depth_q);
	t2.detach();

	// ��Kinect��ȡ����
	if (SUCCEEDED(hr)){
		while (1){
			clock_t s = clock();
			// �����ʲô��������
			if (_kbhit()) {
				key = _getch();
			}
			// ��ȡkinect����
			res = myKinect.Update();
			// ����Ǽܺ���Ƶ��
			if (res.flag){
				color_q.push(res.color);
				depth_q.push(res.depth);
				// ��ʼ¼��
				if (key == 's') {
					// ������Ƶ
					writer.write(res.color.cimg);
					depth.write(res.depth.dimg);
					cout << "Record Frame:" + to_string(myKinect.frame_num) << endl;
					// ����Ǽ�
					if (out.is_open())
					{
						skeleton = to_string(myKinect.frame_num) + "\n";
						for (int i = 0; i < 25; ++i) {
							skeleton = skeleton + to_string(res.skeleton.X[i]) + "," + to_string(res.skeleton.Y[i]) + "," + to_string(res.skeleton.Z[i]) + ",";
							skeleton = skeleton + to_string(res.color.colorX[i]) + "," + to_string(res.color.colorY[i]) + ",";
							skeleton = skeleton + to_string(res.depth.depthX[i]) + "," + to_string(res.depth.depthY[i]) + "\n";
						}
						out << skeleton;
					}
				}
				// ֹͣ¼��
				if (key == ' ') {
					writer.release();
					depth.release();
					out.close();
					cout << "Video and TXT File Save Successfully at " << path + filename << ".mp4(.txt)" << endl;
					key = 0;
				}
			}
			// ����ÿ֡����ʱ��
			clock_t e = clock();
			if ((e - s) > 40) {
				cout << "Runtime:" << e - s << endl;
			}
		}
	}
	else{
		cout << "Kinect Initialization Failed" << endl;
		system("pause");
	}
}	
