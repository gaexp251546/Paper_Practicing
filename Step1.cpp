#include "Niblack.h"

void Step1(Mat src, Mat &OtsuMap){
	Mat SmoothMap;

	/*�����ҽk*/
	GaussianBlur(src, SmoothMap, Size(5, 5), 0, 0);
	//imshow("c", SmoothMap);
	/*�j�z�t��k*/
	threshold(SmoothMap, OtsuMap, 150, 255, CV_THRESH_OTSU);
	
}