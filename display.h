#include <Kinect.h>
#include <opencv2\opencv.hpp>
#include <queue>
#include "myKinect.h"

using namespace std;
using namespace cv;

void show_color(queue<Color>* color_q);
void show_depth(queue<Depth>* depth_q);
void DrawBone(cv::Mat img, float* coorx, float* coory, JointType joint0, JointType joint1);
void drawBody(cv::Mat img, float* coorx, float* coory);
