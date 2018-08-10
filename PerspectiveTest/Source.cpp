#include <stdio.h>
#include <opencv2/opencv.hpp> 
#include <string>

#ifdef _DEBUG  
#pragma comment(lib, "opencv_core331d.lib")   
#pragma comment(lib, "opencv_imgproc331d.lib")   //MAT processing  
#pragma comment(lib, "opencv_highgui331d.lib")  
#pragma comment(lib, "opencv_calib3d331d.lib") 
#pragma comment(lib, "opencv_imgcodecs331d.lib")
#else  
#pragma comment(lib, "opencv_core331.lib")  
#pragma comment(lib, "opencv_imgproc331.lib")  
#pragma comment(lib, "opencv_highgui331.lib")
#pragma comment(lib, "opencv_calib3d331.lib")
#pragma comment(lib, "opencv_imgcodecs331.lib")
#endif


//#pragma comment(lib, "opencv_objdetect246.lib")  
//#pragma comment(lib, "opencv_features2d246.lib")  


using namespace std;
using namespace cv;


bool setPt(float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, vector< Point2f> &in)
{
	if (in.size() == 4)
	{
		in[0].x = x1;
		in[1].x = x2;
		in[2].x = x3;
		in[3].x = x4;

		in[0].y = y1;
		in[1].y = y2;
		in[2].y = y3;
		in[3].y = y4;
	}
	else {
		return false;
	}

	return true;
}


bool makeRandomPerspectivePoint(const vector< Point2f> &A, vector< Point2f> &B, Size imgSize)
{
	if (A.size() != 4 || B.size() != 4)
	{
		cout << "matrix vector error!" << endl;
		return false;
	}

	if (imgSize.width <= 0 || imgSize.height <= 0)
	{
		cout << "image size error!" << endl;
		return false;
	}

	const float maxRand = 0.4;
	int width = imgSize.width;
	int height = imgSize.height;
	int rndMarginX = width * maxRand;
	int rndMarginY = height * maxRand;

	int rx1 = rand() % rndMarginX;
	int ry1 = rand() % rndMarginY;
	int rM = rand() % 4;

	//minimum value
	if (rx1 == 0)
		rx1 = 1;
	if (ry1 == 0)
		ry1 = 1;

	//4 perspective mode
	if (rM == 0)
	{
		setPt(0, width, width, 0, 0 + ry1, 0, height, height - ry1, B);
	}
	else if (rM == 1)
	{
		setPt(0 + rx1, width - rx1, width, 0, 0, 0, height, height, B);
	}
	else if (rM == 2)
	{
		setPt(0, width, width, 0, 0, 0 + ry1, height - ry1, height, B);
	}
	else {
		setPt(0, width, width - rx1, 0 + rx1, 0, 0, height, height, B);
	}

	return true;
}

bool getPerspectiveImg(const Mat& inImg, Mat& outImg, const vector< Point2f> &A, const vector< Point2f> &B)
{
	if (A.size() != 4 || B.size() != 4)
	{
		cout << "matrix vector error!" << endl;
		return false;
	}

	//B = H*A
	Mat H = findHomography(A, B);
	//perspectiveImg = H * inImg
	warpPerspective(inImg, outImg, H, inImg.size());


	//return
	return true;
}



int main()
{
	
	string fn = "Flag_of_South_Korea.png";
	Mat testImg = imread(fn);
	srand(time(0));
	Size imgSize = testImg.size();
	vector< Point2f > opt(4);
	setPt(0, imgSize.width, imgSize.width, 0, 0, 0, imgSize.height, imgSize.height, opt);
	vector< Point2f > dpt(4);

	while (1)
	{

		if (makeRandomPerspectivePoint(opt, dpt, imgSize) == false)
			break;

		Mat outImg;
		if (getPerspectiveImg(testImg, outImg, opt, dpt)) {
			imshow("origin", testImg);
			imshow("perspective", outImg);
			waitKey(0);
		}
		else {
			cout << "matrix vector error!" << endl;
			break;
		}
	}

}