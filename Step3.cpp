#include"Niblack.h"


void giveMeGradientMap(Mat src, int windowSize, Mat MeanGradientFinal, float &MaxGradient){
	Mat GradientHorizontal(src.rows,src.cols,CV_32F);
	Mat GradientVertical(src.rows, src.cols, CV_32F);

	/*水平梯度*/
	Mat kernelHorizontal(3, 3, CV_32F, Scalar(0));
	kernelHorizontal.at<float>(0,0) = -1.0;
	kernelHorizontal.at<float>(0,2) = 1.0;
	kernelHorizontal.at<float>(1,0) = -2.0;
	kernelHorizontal.at<float>(1,2) = 2.0;
	kernelHorizontal.at<float>(2,0) = -1.0;
	kernelHorizontal.at<float>(2,2) = 1.0;

	filter2D(src, GradientHorizontal, GradientHorizontal.depth(), kernelHorizontal);
	imshow("GradientHorizontal", GradientHorizontal);

	/*垂直梯度*/
	Mat kernelVertical(3, 3, CV_32F, Scalar(0));
	kernelVertical.at<float>(0, 0) = -1.0;
	kernelVertical.at<float>(0, 1) = -2.0;
	kernelVertical.at<float>(0, 2) = -1.0;
	kernelVertical.at<float>(2, 0) = 1.0;
	kernelVertical.at<float>(2, 1) = 2.0;
	kernelVertical.at<float>(2, 2) = 1.0;

	filter2D(src, GradientVertical, GradientHorizontal.depth(), kernelVertical);
	imshow("GradientVertical", GradientVertical);


	/*做平方相加開根號 以及取整張圖最大的梯度*/
	for (int i = 0; i < src.rows; i++)
	for (int j = 0; j < src.cols; j++){
		MeanGradientFinal.at<float>(i, j) = sqrt(pow(GradientHorizontal.at<float>(i, j), 2) + pow(GradientVertical.at<float>(i, j), 2));
		if (MaxGradient<MeanGradientFinal.at<float>(i, j))MaxGradient = MeanGradientFinal.at<float>(i, j);
	}

	/*
	//為了秀出梯度圖才寫的 這步驟我寫在核心公式中
	for (int i = 0; i < src.rows; i++)
	for (int j = 0; j < src.cols; j++){
		MeanGradientFinal.at<float>(i, j) /= MaxGradient;
	}
	imshow("output", MeanGradientFinal);
	waitKey();
	*/
}

float giveMeK(Mat src, Mat MeanGray, Mat MeanGradientFinal, float MaxGradient,float k){
	Mat dst;
	float knum = 0.0;
	src.copyTo(dst);
	/*Significant Function*/
	for (int i = 0; i < src.rows; i++)
	for (int j = 0; j < src.cols; j++){
		dst.at<uchar>(i, j) = MeanGray.at<uchar>(i, j) * (1 - k*exp(-MeanGradientFinal.at<float>(i, j) / MaxGradient));
		if (src.at<uchar>(i, j) <= dst.at<uchar>(i, j))knum++;
	}

	return knum;
}

void Step3(Mat src, int windowSize){
	Mat MeanGray;
	Mat MeanGradient;
	Mat MeanGradientFinal(src.rows,src.cols,CV_32F);
	Mat dst;

	//float k = 0.05;
	float MaxGradient = -100.0;

	src.copyTo(MeanGradient);
	src.copyTo(dst);

	/*mean filter*/
	blur(src, MeanGray, Size(windowSize, windowSize));

	/*mean gradient*/
	giveMeGradientMap(MeanGradient, windowSize, MeanGradientFinal, MaxGradient);
	
	/*Calculate k1、k2*/
	float k1, k2;
	float ksmall, kbig;
	float tempRk = 0.0;
	float MaxRk, MinRk;
	ksmall = giveMeK(src, MeanGray, MeanGradientFinal, MaxGradient, 0.3);
	
	bool flag = false;
	for (float i = 0.299; i > 0 ; i -= 0.001){//flag is to init the MaxRk MinRk
		kbig = ksmall;
		ksmall = giveMeK(src, MeanGray, MeanGradientFinal, MaxGradient, i);

		tempRk = (ksmall - kbig) / kbig;
		cout << tempRk << endl;
		if (flag == false){
			MaxRk = tempRk;
			MinRk = tempRk;
			flag = true;
			continue;
		}
		
		if (MinRk > tempRk&&tempRk>0){
			MinRk = tempRk;
			k1 = i;
		}
		if (MaxRk < tempRk&&tempRk>0){
			MaxRk = tempRk;
			k2 = i;
		}
	}
	cout << "MaxRk=" << MaxRk << "MinRk=" << MinRk << endl;
	cout << "k1 = " << k1 << " ,k2 = " << k2 << endl;

	/*Significant Function*/
	Mat dst1, dst2;
	src.copyTo(dst1);
	src.copyTo(dst2);
	/*k1*/
	for (int i = 0; i < src.rows; i++)
	for (int j = 0; j < src.cols; j++){
		dst.at<uchar>(i, j) = MeanGray.at<uchar>(i, j) * (1 - k1*exp(-MeanGradientFinal.at<float>(i, j) / MaxGradient));
		if (src.at<uchar>(i, j) <= dst.at<uchar>(i, j))dst1.at<uchar>(i, j) = 0;
		else dst1.at<uchar>(i, j) = 255;
	}
	/*k2*/
	for (int i = 0; i < src.rows; i++)
	for (int j = 0; j < src.cols; j++){
		dst.at<uchar>(i, j) = MeanGray.at<uchar>(i, j) * (1 - k2*exp(-MeanGradientFinal.at<float>(i, j) / MaxGradient));
		if (src.at<uchar>(i, j) <= dst.at<uchar>(i, j))dst2.at<uchar>(i, j) = 0;
		else dst2.at<uchar>(i, j) = 255;
	}

	imshow("outputfinal1", dst1);
	imshow("outputfinal2", dst2);
	waitKey();
}