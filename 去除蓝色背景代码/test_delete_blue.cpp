// test_delete_blue.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
using namespace std;
using namespace cv;//包含cv命名空

int main()
{
    std::cout << "Hello World!\n";
	char *origin = (char*)"Original";
	char *window = (char*)"Image";
	char *str = (char*)"D:\\opencv\\opencv_test_demo\\test_delete_blue\\x64\\Debug\\old.jpg";
	namedWindow(origin, 1);
	namedWindow(window, 1);
	Mat image = imread(str);
	if (!image.data)
	{
		cout << "图像载入出现问题" << endl;
		return 0;
	}
	Mat roi = image(Rect(20, 20, 20, 20));
	Mat hsvImg;
	cvtColor(image, hsvImg, CV_BGR2HSV); //将图像转换到HSV颜色空间
	//分离HSV空间，v[0]为H色调，v[1]为S饱和度，v[2]为v灰度
	vector<Mat> v;
	split(hsvImg, v);
	Mat roiH = v[0](Rect(20, 20, 20, 20));
	Mat roiS = v[1](Rect(20, 20, 20, 20));
	int SumH = 0;
	int SumS = 0;
	int avgH, avgS;//蓝底的平均色调和平均饱和度
	//取一块蓝色背景，计算出它的平均色调和平均饱和度
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			/*SumH=SumH+roiH(i,j);*/
			SumH = int(roiH.at<uchar>(j, i)) + SumH;
			SumS = int(roiS.at<uchar>(j, i)) + SumS;
		}
	}
	avgH = SumH / 400;
	avgS = SumS / 400;
	//遍历整个图像
	int nl = hsvImg.rows;
	int nc = hsvImg.cols;
	int step = 10;
	for (int j = 0; j < nl; j++)
	{
		for (int i = 0; i < nc; i++)
		{
			//以H.S两个通道做阈值分割，把蓝色替换成红色
			if ((v[0].at<uchar>(j, i)) <= (avgH + 5) && v[0].at<uchar>(j, i) >= (avgH - 5)
				&& (v[1].at<uchar>(j, i)) <= (avgS + 40) && v[1].at<uchar>(j, i) >= (avgS - 40))
			{
				//cout<<int(v[0].at<uchar>(j,i))<<endl;
				//红色底
				//v[0].at<uchar>(j,i)=0;
				//白色底
				v[0].at<uchar>(j, i) = 0;
				v[1].at<uchar>(j, i) = 0; //V[0]和V[1]全调成0就是变成白色
				//绿色底
				//v[0].at<uchar>(j,i)=60;
				//蓝色底
				//v[0].at<uchar>(j,i)=120;
				/*cout<<int(v[0].at<uchar>(j,i))<<endl;*/
			}
		}
	}
	Mat finImg;
	merge(v, finImg);
	Mat rgbImg;
	cvtColor(finImg, rgbImg, CV_HSV2BGR); //将图像转换回RGB空间
	imshow(origin, image);
	imshow(window, rgbImg);
	//加个滤波把边缘部分的值滤掉（此处应该用低通滤波器，但感觉不太好，还是不用了。）
	Mat result;
	GaussianBlur(rgbImg, result, Size(3, 3), 0.5);
	imshow(window, result);
	imwrite("new.jpg", result);
	waitKey(0);
	//system("pause");
	return 0;


}
