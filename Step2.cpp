#include"Niblack.h"

/*�פ妡(6)*/
float giveMeTotalSD(Mat src,Mat OtsuMap,int windowSize){
	int count = 0;
	float TotalSD = 0.0;

	for (int i = windowSize; i < src.rows - windowSize; i++)
	for (int j = windowSize; j < src.cols - windowSize; j++){
		if ((int)OtsuMap.at<uchar>(i, j) == 0){
			Mat tempROI = src(Rect(j - windowSize / 2, i - windowSize / 2, windowSize, windowSize));
			Mat tempMean, tempStd;

			meanStdDev(tempROI, tempMean, tempStd);
			float StandardDeviation = (float)tempStd.at<double>(0, 0);
			TotalSD += StandardDeviation;

			count++;
		}
	}
	TotalSD /= count;
	return TotalSD;
}

int Step2(Mat src, Mat &OtsuMap){
	int windowSize=1;
	float IR = 5.0;
	float SDsmall = 0.0;
	float SDbig = 0.0;

	/*find fitest window size*/
	while (1){
		/*init*/
		if (windowSize == 1){
			windowSize = 3;//start from 3
			SDbig = giveMeTotalSD(src, OtsuMap, windowSize);
			continue;
		}

		else{
			SDsmall = SDbig;
			SDbig = giveMeTotalSD(src, OtsuMap, windowSize+2);
			IR = (SDbig - SDsmall) / windowSize;
			cout << "WinSize = " << windowSize << ", IR = " << IR << endl;
			if (IR <= 0.01)break;
			windowSize += 2;
		}
	}

	cout << windowSize << endl;
	return windowSize;
}