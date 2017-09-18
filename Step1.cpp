#include "Niblack.h"

void Step1(Mat src, Mat &OtsuMap){
	Mat SmoothMap;

	/*高斯模糊*/
	GaussianBlur(src, SmoothMap, Size(5, 5), 0, 0);
	//imshow("c", SmoothMap);
	/*大津演算法*/
	threshold(SmoothMap, OtsuMap, 150, 255, CV_THRESH_OTSU);
	
}