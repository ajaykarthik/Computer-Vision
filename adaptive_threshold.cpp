#include <opencv2/opencv.hpp>
#include <iostream>
#include<math.h>

using namespace cv;
using namespace std;

#define DBL_MAX         1.7976931348623158e+308

int main(int argc, char** argv) {

	if (argc < 2) {
		cerr << argv[0] << ": "
			<< "got " << argc - 1 << " arguments. Expecting one: an image."
			<< endl;
		return(-1);
	}
	Mat inputImage = imread("lenna.png", CV_LOAD_IMAGE_UNCHANGED);  // Read the image
	if (inputImage.empty()) {
		cerr << "Could not open or find the image " << argv[1] << endl;
		return(-1);
	}
	namedWindow("input", CV_WINDOW_AUTOSIZE);
	imshow("input", inputImage);
	int rows = inputImage.rows;
	int cols = inputImage.cols;

	// make sure image is gray level
	Mat grayImage(rows, cols, CV_8UC1);
	if (inputImage.type() == CV_8UC1) grayImage = inputImage;
	else {
		if (inputImage.type() == CV_8UC3)
			cvtColor(inputImage, grayImage, CV_BGR2GRAY);
		else {
			cerr << "can't deal with image " << argv[1] << endl;
			return(-1);
		}
	}
	imshow("grayImage", grayImage);

	// compute the histogram
	int hist[256];
	for (int k = 0; k < 256; k++) hist[k] = 0;
	for (int i = 0; i < rows; i++)
	for (int j = 0; j < cols; j++)
		hist[grayImage.at<uchar>(i, j)]++;

	// your code should be placed here.

	long double currentMinError = DBL_MAX;
	int currentThreshold;
	double e;
	double q1, q2;
	long double sum = 0;
	double numerator, denominator;
	for (int t = 0; t < 256; t++) // t is the threshold
	{
		numerator = 0;
		denominator = 0;
		for (int x = 0; x < t; x++)
		{
			numerator += x*hist[x];
			denominator += hist[x];
		}
		q1 = numerator / denominator;
		//reset 
		numerator = 0;
		denominator = 0;
		for (int x = t; x < 256; x++)
		{
			numerator += x*hist[x];
			denominator += hist[x];
		}
		q2 = numerator / denominator;
		//calculate error
		sum = 0;
		for (int x = 0; x < t;x++) 
		{
			sum += ((double)x - q1)*((double)x - q1)*hist[x];

		}
		for (int x = t; x < 256;x++)
		{
			sum += ((double)x - q2)*((double)x - q2)*hist[x];
		}
		if (sum < currentMinError)
		{
			currentMinError = sum;
			currentThreshold = t;
		}
	}


	// end of your code

	cout << "Threshold value is " << currentThreshold << endl;
	Mat thresholdedImage(rows, cols, CV_8UC1);
	threshold(grayImage, thresholdedImage, currentThreshold, 255, THRESH_BINARY);
	imshow("thresholded Image", thresholdedImage);

	waitKey(0); // Wait for a keystroke
	return(0);
}