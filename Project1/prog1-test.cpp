/*
First Program
Name: Ajay Karthik Ganesan (agk130030)
Descripiton: This program displays a continious range of color in the xy and Luv domain
Input parameters: Width, Height
*/

#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	if (argc != 3) {
		cout << argv[0] << ": "
			<< "got " << argc - 1 << " arguments. Expecting two: width height."
			<< endl;
		return(-1);
	}

	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	int** RED1 = new int*[height];
	int** GREEN1 = new int*[height];
	int** BLUE1 = new int*[height];
	int** RED2 = new int*[height];
	int** GREEN2 = new int*[height];
	int** BLUE2 = new int*[height];

	for (int i = 0; i < height; i++) {
		RED1[i] = new int[width];
		GREEN1[i] = new int[width];
		BLUE1[i] = new int[width];
		RED2[i] = new int[width];
		GREEN2[i] = new int[width];
		BLUE2[i] = new int[width];
	}

	double uw = 4 * 0.95 / (0.95 + 15 + 3 * 1.09);
	double vw = 9 / (0.95 + 15 + 3 * 1.09);
	for (int i = 0; i < height; i++)
	for (int j = 0; j < width; j++)
	{
		int r1, g1, b1;
		int r2, g2, b2;

		double x = (double)j / (double)width;
		double y = (double)i / (double)height;
		double Y = 1.0;

		double L = 90;
		double u = x * 512 - 255;
		double v = y * 512 - 255;


		/* Your code should be placed here
		It should translate xyY to byte sRGB
		and Luv to byte sRGB
		*/
		//Converting xyY to XYZ system
		double X = 0;
		double Z = 0;

		if (y != 0)
		{
			X = x / y;     //Y = 1
			Z = (1 - x - y) / y;  // Y = 1
		}

		if (X < 0 || isnan(X)) X = 0;
		if (Z < 0 || isnan(Z)) Z = 0;
		//Convert to linear sRGB
		double Rsrgb = 3.240479*X - 1.53715*Y - 0.498535*Z;
		double Gsrgb = -0.969256*X + 1.875991*Y + 0.041556*Z;
		double Bsrgb = 0.055648*X - 0.204043*Y + 1.057311*Z;
		//clipping
		if (Rsrgb < 0)Rsrgb = 0;
		if (Gsrgb < 0)Gsrgb = 0;
		if (Bsrgb < 0)Bsrgb = 0;
		if (Rsrgb > 1)Rsrgb = 1;
		if (Gsrgb > 1)Gsrgb = 1;
		if (Bsrgb > 1)Bsrgb = 1;
		//Convert to non linear sRGB
		if (Rsrgb < 0.00304)
		{
			Rsrgb = 12.92*Rsrgb;
		}
		else
		{
			Rsrgb = 1.055*pow(Rsrgb, (1.0 / 2.4)) - 0.055;
		}
		if (Gsrgb < 0.00304)
		{
			Gsrgb = 12.92*Gsrgb;
		}
		else
		{
			Gsrgb = 1.055*pow(Gsrgb, (1 / 2.4)) - 0.055;
		}
		if (Bsrgb < 0.00304)
		{
			Bsrgb = 12.92*Bsrgb;
		}
		else
		{
			Bsrgb = 1.055*pow(Bsrgb, (1 / 2.4)) - 0.055;
		}
		//scale to 255
		Rsrgb = Rsrgb * 255;
		Bsrgb = Bsrgb * 255;
		Gsrgb = Gsrgb * 255;
		//round off
		r1 = Rsrgb + 0.5;
		g1 = Gsrgb + 0.5;
		b1 = Bsrgb + 0.5;


		//LUV to XYZ
		double u1 = (u + 13 * uw*L) / (13 * L);
		double v1 = (v + 13 * vw*L) / (13 * L);
		Y = Y = pow(((L + 16) / 116), 3); //L = 90 always
		if (v1 == 0)
		{
			X = 0;
			Z = 0;
		}
		else
		{
			X = Y * 2.25*u1 / v1;
			Z = Y * (3 - 0.75*u1 - 5 * v1) / v1;
		}
		if (X < 0 || isnan(X)) X = 0;
		if (Z < 0 || isnan(Z)) Z = 0;
		//Convert to linear sRGB
		Rsrgb = 3.240479*X - 1.53715*Y - 0.498535*Z;
		Gsrgb = -0.969256*X + 1.875991*Y + 0.041556*Z;
		Bsrgb = 0.055648*X - 0.204043*Y + 1.057311*Z;
		//clipping
		if (Rsrgb < 0)Rsrgb = 0;
		if (Gsrgb < 0)Gsrgb = 0;
		if (Bsrgb < 0)Bsrgb = 0;
		if (Rsrgb > 1)Rsrgb = 1;
		if (Gsrgb > 1)Gsrgb = 1;
		if (Bsrgb > 1)Bsrgb = 1;
		//Convert to non linear sRGB
		if (Rsrgb < 0.00304)
		{
			Rsrgb = 12.92*Rsrgb;
		}
		else
		{
			Rsrgb = 1.055*pow(Rsrgb, (1 / 2.4)) - 0.055;
		}
		if (Gsrgb < 0.00304)
		{
			Gsrgb = 12.92*Gsrgb;
		}
		else
		{
			Gsrgb = 1.055*pow(Gsrgb, (1 / 2.4)) - 0.055;
		}
		if (Bsrgb < 0.00304)
		{
			Bsrgb = 12.92*Bsrgb;
		}
		else
		{
			Bsrgb = 1.055*pow(Bsrgb, (1 / 2.4)) - 0.055;
		}
		//scale to 255
		Rsrgb = Rsrgb * 255;
		Bsrgb = Bsrgb * 255;
		Gsrgb = Gsrgb * 255;
		//round off
		r2 = Rsrgb + 0.5;
		g2 = Gsrgb + 0.5;
		b2 = Bsrgb + 0.5;

		// this is the end of your code

		RED1[i][j] = r1;
		GREEN1[i][j] = g1;
		BLUE1[i][j] = b1;
		RED2[i][j] = r2;
		GREEN2[i][j] = g2;
		BLUE2[i][j] = b2;
	}


	Mat R1(height, width, CV_8UC1);
	Mat G1(height, width, CV_8UC1);
	Mat B1(height, width, CV_8UC1);

	Mat R2(height, width, CV_8UC1);
	Mat G2(height, width, CV_8UC1);
	Mat B2(height, width, CV_8UC1);

	for (int i = 0; i < height; i++)
	for (int j = 0; j < width; j++) {

		R1.at<uchar>(i, j) = RED1[i][j];
		G1.at<uchar>(i, j) = GREEN1[i][j];
		B1.at<uchar>(i, j) = BLUE1[i][j];

		R2.at<uchar>(i, j) = RED2[i][j];
		G2.at<uchar>(i, j) = GREEN2[i][j];
		B2.at<uchar>(i, j) = BLUE2[i][j];
	}

	Mat xyY;
	Mat xyY_planes[] = { B1, G1, R1 };
	merge(xyY_planes, 3, xyY);
	namedWindow("xyY", CV_WINDOW_AUTOSIZE);
	imshow("xyY", xyY);

	Mat Luv;
	Mat Luv_planes[] = { B2, G2, R2 };
	merge(Luv_planes, 3, Luv);
	namedWindow("Luv", CV_WINDOW_AUTOSIZE);
	imshow("Luv", Luv);
	waitKey(0); // Wait for a keystroke
	return(0);
}