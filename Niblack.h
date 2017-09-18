#include<iostream>
#include<opencv2/opencv.hpp>

//#define k 0.1
#define NAME "cc.png"
#define WINSIZE 15

using namespace std;
using namespace cv;

void Step1(Mat src, Mat &OtsuMap);
void Step3(Mat src, int windowSize);
void giveMeGradientMap(Mat src, int windowSize, Mat MeanGradientFinal,float &MaxGradient);
int Step2(Mat src, Mat &OtsuMap);
