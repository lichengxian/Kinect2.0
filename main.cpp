#include "myKinect.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;

int main()
{
	// ��ʼ��kinect
	CBodyBasics myKinect;
	HRESULT hr = myKinect.InitializeDefaultSensor();
	DATA res;
	// ��ȡ�����ļ���
	cout << "Input Save File Name:" << endl;
	string filename;
	getline(cin, filename);
	// ���ݱ���·��
	string path = "D:/dataset/";
	// ������Ƶmp4
	VideoWriter writer(path + filename + ".mp4", CAP_OPENCV_MJPEG, 25, Size(1920, 1080));
	int frame;
	// ����Ǽ���Ϣtxt
	string skeleton;
	ofstream out(path + filename + ".txt");
	// ��¼����
	char key = 0;

	cout << "Kinect Initialization Successfully" << endl;

	if (SUCCEEDED(hr)){
		while (1){
			// �����ʲô��������
			if (_kbhit()) {
				key = _getch();
			}
			// ��ȡkinect��Ϣ
			float s = clock();
			res = myKinect.Update();
			// ����Ǽܺ���Ƶ��
			if (res.flag){
				if (key == 's') {
					writer.write(res.img);
					cout << "Record Frame:" + to_string(myKinect.frame_num) << endl;
					// ����Ǽ�
					if (out.is_open())
					{
						skeleton = skeleton + to_string(myKinect.frame_num) + "\n";
						for (int i = 0; i < 25; ++i) {
							skeleton = skeleton + to_string(res.X[i]) + "," + to_string(res.Y[i]) + "," + to_string(res.Z[i]) + "\n";
						}
					}
				}
				if (key == ' ') {
					writer.release();
					out << skeleton;
					out.close();
					cout << "Video and TXT File Save Successfully at " << path + filename << ".mp4(.txt)" << endl;
					key = 0;
				}
			}
			float e = clock();
			// ����ÿ֡����ʱ��
			if (e - s > 30) {
				cout << "Runtime:" << e - s << endl;
			}
		}
	}
	else{
		cout << "Kinect Initialization Failed" << endl;
		system("pause");
	}
}