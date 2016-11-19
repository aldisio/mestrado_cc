#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;

Mat geot_translation(Mat img, int x, int y);
Mat geot_rotation(Mat img_in, Point2f center, double angle, double scale);


Mat geot_translation(Mat img_in, int x, int y){
	// --------------------------------------------
	// Variáveis
	// --------------------------------------------
	Mat img_out = Mat::zeros( img_in.rows, img_in.cols, img_in.type());

	// Define a operação, o deslocamento em pixels será feito conforme parametros x e y
	Mat matrix_t = (Mat_<float>(2,3) <<  1, 0, x,
		 								0, 1, y);

	// Aplica a transformação afim a partir da definição da matriz matrix_t
	warpAffine( img_in, img_out, matrix_t, img_out.size() );

	return img_out;

}

Mat geot_rotation(Mat img_in, int p_center_x, int p_center_y, double angle, double scale){
	// --------------------------------------------
	// Variáveis
	// --------------------------------------------
	Mat img_out = Mat::zeros( img_in.rows, img_in.cols, img_in.type());
	Point center = Point( p_center_x, p_center_y );

	// Define a operação, o deslocamento em pixels será feito conforme parametros x e y
	Mat matrix_t = getRotationMatrix2D( center, angle, scale );

	// Aplica a transformação afim a partir da definição da matriz matrix_t
	warpAffine( img_in, img_out, matrix_t, img_out.size() );

	return img_out;

}
