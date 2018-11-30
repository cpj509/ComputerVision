//소프트웨어학부 20133273 최필준
#include "opencv2/opencv.hpp" 
#include <iostream>  

using namespace cv;
using namespace std;

int main()
{
	int na = 0, cir = 0, rec = 0, tri = 0;
	Mat img, dst, img_gray, bin_noisy;
	//이미지파일을 로드하여 image에 저장  
	img = imread("c:/opencv/shape5.bmp", IMREAD_COLOR);
	if (img.empty())
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	imshow("input", img);
	cvtColor(img, img_gray, COLOR_BGR2GRAY);
	//이미지를 이진화
	threshold(img_gray, img_gray, 125, 255, THRESH_BINARY_INV | THRESH_OTSU);
	imshow("binary", img_gray);
	for (int i = 1; i < 4; i = i + 2)
	{
		medianBlur(img_gray, bin_noisy, i);
	}
	imshow("noise", bin_noisy);
	//contour를 탐색
	vector<vector<Point> > contours;
	findContours(bin_noisy, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	vector<Point2f> approx;
	dst = img.clone();
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.017, true);
		if (fabs(contourArea(Mat(approx))) > 100)
		{
			int size = approx.size();
			Rect r = boundingRect(contours[i]);
			Point pt(r.x + ((r.width) / 2), r.y + ((r.height) / 2));
			//선과 중심점을 그림
			if (fabs(contourArea(Mat(approx))) > 1500) {
				if (size == 3) {
					line(dst, approx[0], approx[approx.size() - 1], CV_RGB(255, 0, 0), 2);
					for (int k = 0; k < size - 1; k++) {
						line(dst, approx[k], approx[k + 1], CV_RGB(255, 0, 0), 2);
					}
					circle(dst, pt, 5, CV_RGB(255, 0, 0), CV_FILLED);
					tri++;
				}
				else if (size == 4) {
					line(dst, approx[0], approx[approx.size() - 1], CV_RGB(0, 255, 0), 2);
					for (int k = 0; k < size - 1; k++) {
						line(dst, approx[k], approx[k + 1], CV_RGB(0, 255, 0), 2);
					}
					circle(dst, pt, 5, CV_RGB(0, 255, 0), CV_FILLED);
					rec++;
				}
				else {
					line(dst, approx[0], approx[approx.size() - 1], CV_RGB(0, 0, 255), 2);
					for (int k = 0; k < size - 1; k++) {
						line(dst, approx[k], approx[k + 1], CV_RGB(0, 0, 255), 2);
					}
					circle(dst, pt, 5, CV_RGB(0, 0, 255), CV_FILLED);
					cir++;
				}
			}
			else {
				line(dst, approx[0], approx[approx.size() - 1], CV_RGB(255, 255, 255), 2);
				for (int k = 0; k < size - 1; k++) {
					line(dst, approx[k], approx[k + 1], CV_RGB(255, 255, 255), 2);
				}
				circle(dst, pt, 5, CV_RGB(255, 255, 255), CV_FILLED);
				na++;
			}
		}
	}
	imshow("result", dst);
	cout << "nail: " << na << endl;
	cout << "circle: " << cir << endl;
	cout << "rectangle: " << rec << endl;
	cout << "triangle: " << tri << endl;
	waitKey(0);
	return 0;
}