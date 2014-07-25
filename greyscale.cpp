#include <opencv2\opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main() {

  int rows = 200;
  int cols = 500;
  double currentValue; //compute value at each pixel before assigning
 
  // gray level image
  Mat OutputGray(rows, cols, CV_8UC1);
  for(int i = 0 ; i < rows ; i++)
    for(int j = 0 ; j < cols ; j++) {
      currentValue = (255*j/499.0);
	  currentValue += 0.5;
	  currentValue = floor(currentValue); // round to nearest integer
      OutputGray.at<uchar>(i,j) = currentValue;
    }
  imshow("Output",OutputGray);
  waitKey(0); // Wait for a keystroke
  return(0);
}