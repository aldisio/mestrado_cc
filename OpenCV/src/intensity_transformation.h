#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;

Mat negativo(Mat image_orig);
Mat gama(Mat image_orig, float c, float gama);

Mat negativo(Mat image_orig){

	return 255-image_orig;
}

Mat gama(Mat image_orig, float c, float gama){

	return c * image_orig ^ gama;
}
