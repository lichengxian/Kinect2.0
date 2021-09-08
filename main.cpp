#include "myKinect.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;

int main()
{
	// 初始化kinect
	CBodyBasics myKinect;
	HRESULT hr = myKinect.InitializeDefaultSensor();
	DATA res;
	// 获取保存文件名
	cout << "Input Save File Name:" << endl;
	string filename;
	getline(cin, filename);
	// 数据保存路径
	string path = "D:/dataset/";
	// 保存视频mp4
	VideoWriter writer(path + filename + ".mp4", CAP_OPENCV_MJPEG, 25, Size(1920, 1080));
	int frame;
	// 保存骨架信息txt
	string skeleton;
	ofstream out(path + filename + ".txt");
	// 记录按键
	char key = 0;

	cout << "Kinect Initialization Successfully" << endl;

	if (SUCCEEDED(hr)){
		while (1){
			// 检查有什么按键按下
			if (_kbhit()) {
				key = _getch();
			}
			// 获取kinect信息
			float s = clock();
			res = myKinect.Update();
			// 保存骨架和视频流
			if (res.flag){
				if (key == 's') {
					writer.write(res.img);
					cout << "Record Frame:" + to_string(myKinect.frame_num) << endl;
					// 保存骨架
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
			// 计算每帧处理时长
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