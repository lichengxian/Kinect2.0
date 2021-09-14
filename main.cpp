#include "myKinect.h"
#include <iostream>
#include <fstream>
#include <string>
#include "display.h"

using namespace std;
using namespace cv;

// 主函数
int main()
{
	// 初始化kinect
	CBodyBasics myKinect;
	HRESULT hr = myKinect.InitializeDefaultSensor();
	DATA res;
	// 获取保存文件名
	cout << "Please Input Save File Name:" << endl;
	string filename;
	getline(cin, filename);
	// 数据保存路径
	string path = "D:/dataset/";
	// 保存彩图视频mp4
	VideoWriter writer(path + filename + ".mp4", CAP_OPENCV_MJPEG, 25, Size(1920, 1080));
	// 保存深度视频mp4
	VideoWriter depth(path + filename + "_depth.mp4", CAP_OPENCV_MJPEG, 25, Size(512, 424));
	// 保存骨架信息txt
	string skeleton;
	ofstream out(path + filename + ".txt");
	// 记录按键
	char key = 0;

	cout << "Kinect Initialization Successfully" << endl;
	cout << "Press 'S' to Start Record" << endl;

	// 多线程显示
	queue<Color> color_q;
	thread t1(show_color, &color_q);
	t1.detach();
	queue<Depth> depth_q;
	thread t2(show_depth, &depth_q);
	t2.detach();

	// 向Kinect获取数据
	if (SUCCEEDED(hr)){
		while (1){
			clock_t s = clock();
			// 检查有什么按键按下
			if (_kbhit()) {
				key = _getch();
			}
			// 获取kinect数据
			res = myKinect.Update();
			// 保存骨架和视频流
			if (res.flag){
				color_q.push(res.color);
				depth_q.push(res.depth);
				// 开始录制
				if (key == 's') {
					// 保存视频
					writer.write(res.color.cimg);
					depth.write(res.depth.dimg);
					cout << "Record Frame:" + to_string(myKinect.frame_num) << endl;
					// 保存骨架
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
				// 停止录制
				if (key == ' ') {
					writer.release();
					depth.release();
					out.close();
					cout << "Video and TXT File Save Successfully at " << path + filename << ".mp4(.txt)" << endl;
					key = 0;
				}
			}
			// 计算每帧处理时长
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
