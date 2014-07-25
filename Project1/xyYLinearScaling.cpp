/*
Fourth Program
Name: Ajay Karthik Ganesan (agk130030)
Descripiton: This program gets an input color image, performs liner scaling in the xyY domain on the given window
Ymin and Ymax values are calculated in the specified window
linear scaling is performed on the whole image
values lower than Ymin are set to Ymin
values greater than Ymax are set to Ymax
Input parameters: Window top left (W1,H1) Window bottom right (W2,H2) Input image, output image name
*/
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <iostream>
using namespace cv;
using namespace std;

void runOnWindow(int W1, int H1, int W2, int H2, Mat inputImage, char *outName) {
	int rows = inputImage.rows;
	int cols = inputImage.cols;

	vector<Mat> i_planes;
	split(inputImage, i_planes);
	Mat iB = i_planes[0];
	Mat iG = i_planes[1];
	Mat iR = i_planes[2];

	// dynamically allocate RGB arrays of size rows x cols
	int** R = new int*[rows];
	int** G = new int*[rows];
	int** B = new int*[rows];
	for (int i = 0; i < rows; i++) {
		R[i] = new int[cols];
		G[i] = new int[cols];
		B[i] = new int[cols];
	}

	for (int i = 0; i < rows; i++)
	for (int j = 0; j < cols; j++) {
		R[i][j] = iR.at<uchar>(i, j);
		G[i][j] = iG.at<uchar>(i, j);
		B[i][j] = iB.at<uchar>(i, j);
	}


	//	   The transformation should be based on the
	//	   historgram of the pixels in the W1,W2,H1,H2 range.
	//	   The following code goes over these pixels
	double minY = 1000000;
	double maxY = 0;
	for (int i = H1; i <= H2; i++)
	for (int j = W1; j <= W2; j++) {
		double r = R[i][j];
		double g = G[i][j];
		double b = B[i][j];
		int gray = (int)(0.3*r + 0.6*g + 0.1*b + 0.5);
		//convert non linear Srgb (input image format) to XYZ

		//normailze
		r /= 255;
		g /= 255;
		b /= 255;
		//make linear
		if (r < 0.03928)
		{
			r = r / 12.92;
		}
		else
		{
			r = pow(((r + 0.055) / 1.055), 2.4);
		}
		if (g < 0.03928)
		{
			g = g / 12.92;
		}
		else
		{
			g = pow(((g + 0.055) / 1.055), 2.4);
		}
		if (b < 0.03928)
		{
			b = b / 12.92;
		}
		else
		{
			b = pow(((b + 0.055) / 1.055), 2.4);
		}
		//convert to XYZ
		double X = 0.412453*r + 0.35758*g + 0.180423*b;
		double Y = 0.212671*r + 0.71516*g + 0.072169*b;
		double Z = 0.019334*r + 0.119193*g + 0.950227*b;

		//get min Y and max Y values
		minY = min(Y, minY);
		maxY = max(Y, maxY);
	}

	Mat oR(rows, cols, CV_8UC1);
	Mat oG(rows, cols, CV_8UC1);
	Mat oB(rows, cols, CV_8UC1);
	for (int i = 0; i < rows; i++)
	for (int j = 0; j < cols; j++) {

		double r = R[i][j];
		double g = G[i][j];
		double b = B[i][j];
		//convert non linear Srgb (input image format) to XYZ

		//normailze
		r /= 255;
		g /= 255;
		b /= 255;
		//make linear inverse gamma
		if (r < 0.03928)
		{
			r = r / 12.92;
		}
		else
		{
			r = pow(((r + 0.055) / 1.055), 2.4);
		}
		if (g < 0.03928)
		{
			g = g / 12.92;
		}
		else
		{
			g = pow(((g + 0.055) / 1.055), 2.4);
		}
		if (b < 0.03928)
		{
			b = b / 12.92;
		}
		else
		{
			b = pow(((b + 0.055) / 1.055), 2.4);
		}
		//convert to XYZ
		double X = 0.412453*r + 0.35758*g + 0.180423*b;
		double Y = 0.212671*r + 0.71516*g + 0.072169*b;
		double Z = 0.019334*r + 0.119193*g + 0.950227*b;
		//convert to xyY
		double x = X / (X + Y + Z);
		double y = Y / (X + Y + Z);

		//perform linear scaling (from minY , MaxY)(a,b) to [0,1][A,B]
		if (Y < minY)
		{
			Y = 0;
		}
		else if (Y > maxY)
		{
			Y = 1;
		}
		else
		{
			Y = (Y - minY)*(1) / (maxY - minY);
		}
		//now convert back to XYZ
		//xyY to XYZ
		X = x*Y / y;
		Z = (1 - x - y)*Y / y;
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
		r = Rsrgb + 0.5;
		g = Gsrgb + 0.5;
		b = Bsrgb + 0.5;

		oR.at<uchar>(i, j) = r;
		oG.at<uchar>(i, j) = g;
		oB.at<uchar>(i, j) = b;

	}

	Mat o_planes[] = { oB, oG, oR };
	Mat outImage;
	merge(o_planes, 3, outImage);

	namedWindow("output", CV_WINDOW_AUTOSIZE);
	imshow("output", outImage);
	imwrite(outName, outImage);
}

int main(int argc, char** argv) {
	if (argc != 7) {
		cerr << argv[0] << ": "
			<< "got " << argc - 1
			<< " arguments. Expecting six: w1 h1 w2 h2 ImageIn ImageOut."
			<< endl;
		cerr << "Example: proj1b 0.2 0.1 0.8 0.5 fruits.jpg out.bmp" << endl;
		system("pause");
		return(-1);
	}
	double w1 = atof(argv[1]);
	double h1 = atof(argv[2]);
	double w2 = atof(argv[3]);
	double h2 = atof(argv[4]);
	char *inputName = argv[5];
	char *outputName = argv[6];

	if (w1<0 || h1<0 || w2 <= w1 || h2 <= h1 || w2>1 || h2>1) {
		cerr << " arguments must satisfy 0 <= w1 < w2 <= 1"
			<< " ,  0 <= h1 < h2 <= 1" << endl;
		return(-1);
	}

	Mat inputImage = imread(inputName, CV_LOAD_IMAGE_UNCHANGED);
	if (inputImage.empty()) {
		cout << "Could not open or find the image " << inputName << endl;
		return(-1);
	}

	string windowInput("input: ");
	windowInput += inputName;

	namedWindow(windowInput, CV_WINDOW_AUTOSIZE);
	imshow(windowInput, inputImage);

	if (inputImage.type() != CV_8UC3) {
		cout << inputName << " is not a standard color image  " << endl;
		return(-1);
	}

	int rows = inputImage.rows;
	int cols = inputImage.cols;
	int W1 = (int)(w1*(cols - 1));
	int H1 = (int)(h1*(rows - 1));
	int W2 = (int)(w2*(cols - 1));
	int H2 = (int)(h2*(rows - 1));

	runOnWindow(W1, H1, W2, H2, inputImage, outputName);

	waitKey(0); // Wait for a keystroke
	return(0);
}