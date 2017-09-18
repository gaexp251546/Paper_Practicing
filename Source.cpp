/*
Title:Parameter-free based two-stage method for binarizing degraded document images

Notice:
This code is not builded by the author in the titled paper.
Below,I use 'Step' to distribute the step in the titled paper.
Please refer the paper if you don't know the meaning of function names.

ⒸJason Wen.  Sep. 2017
*/

#include "Niblack.h"

int main(){
	system("color a");
	Mat src = imread(NAME, 0);
	Mat OtsuMap;
	Step1(src, OtsuMap);
	int windowSize = Step2(src, OtsuMap);
	//windowSize = WINSIZE;
	Step3(src, windowSize);
	imshow("last", src);
	waitKey();
}