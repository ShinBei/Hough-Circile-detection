#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv/highgui.h>
#include <cv.h>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

int ***create_3D_Array(int a, int b, int c)
{
	int ***A = new int**[a];
	for (int i = 0; i < a; i++)
	{
		A[i] = new int*[b];
		for (int j = 0; j < b; j++)
		{
			A[i][j] = new int[c];
		}
	}
	for (int i = 0; i < a; i++)
		for (int j = 0; j < b; j++)
			for (int k = 0; k < c; k++)
				A[i][j][k] = 0;
	return A;
}

int main()
{
	//源图像
	Mat scr = imread("circle.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat out = imread("circle.bmp", CV_LOAD_IMAGE_COLOR);;
	Mat cannyimg = Mat(scr.rows, scr.cols, CV_8UC1);
	int edgeThresh = 50;
	Canny(scr, cannyimg, edgeThresh, edgeThresh * 3, 3);

	imshow("canny", cannyimg);
	int ***circle = create_3D_Array(scr.cols, scr.rows, scr.rows);
	int max = 0;
	int ax = 0;
	int by = 0;
	int aa[8], bb[8], rr[8];
	int count = 0;
	for (int y = 0; y < scr.rows; y++)//heigh
	{
		for (int x = 0; x < scr.cols; x++)//width
		{
			if (cannyimg.at<unsigned char>(y, x) == 255)
			{
				for (int b = 50; b < scr.rows-50; b=b+1)//heigh
				{
					for (int a = 50; a < scr.cols-50; a=a+1)//width
					{
						int r;
						r = sqrt((x - a)*(x - a) + (y - b)*(y - b));
						//cout << r << endl;
						if (20 < r && r < 300)
						{
							circle[a][b][r] = circle[a][b][r] + 1;
							int threshold;
							if(r<80)
							{
								threshold = (double)0.9 * 3.14 * r;
							}
							else
							{
								threshold = (double)1.2 * 3.14 * r;
							}
							if (circle[a][b][r]>threshold)
							{
								
								if(abs(ax-a)>5 && abs(by - b)>5)
								{
									ax = a;
									by = b;
									aa[count] = a;
									bb[count] = b;
									rr[count] = r;
									if (count == 0)
									{
										count++;
									}
										
									int k = 0;
									int same = 0;
									while (k < count)
									{
										if (abs(a - aa[k]) < 5 && abs(b - bb[k]) < 5) 
										{
											same++;
											break;
										}
										k++;
									}
									if (same == 0)
									{
										count++;
									}
								}
							}
						}
						
					}
				}
			}
		}
	}
	int a, b, r;
	for (int i = 0; i < 7; i++)
	{
		a = aa[i];
		b = bb[i];
		r = rr[i];
		for (int s = -2; s < 2; s++)
		{
			for (int q = -2; q < 2; q++)
			{
				out.at<Vec3b>(b + s, a + q)[2] = 191;//Red
				out.at<Vec3b>(b + s, a + q)[0] = 255;//Blue
				out.at<Vec3b>(b + s, a + q)[1] = 62;//Green
			}
		}

		for (int y = 0; y < scr.rows; y++)//heigh
		{
			for (int x = 0; x < scr.cols; x++)//width
			{
				int s = (int)sqrt((x - a)*(x - a) + (y - b)*(y - b));
				if (s == r)
				{
					out.at<Vec3b>(y, x)[2] = 191;//Red
					out.at<Vec3b>(y, x)[0] = 255;//Blue
					out.at<Vec3b>(y, x)[1] = 62;//Green
					
				}
			}
		}
	}
	

	imshow("out", out);
	imwrite("out.bmp", out);
	
	waitKey(0);
	return 0;
}