#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <string>
#include "geometric_transformation.h"
#include "intensity_transformation.h"

using namespace cv;
using namespace std;

/// Global variables
char* source_window = "Source image";
char* warp_window = "Warp";
char* warp_rotate_window = "Warp + Rotate";

/** @function main */
 int main( int argc, char** argv )
 {
	 // --------------------------------------------
	 // Endereço da imagem
	 // --------------------------------------------
	 String extention = ".tif";
	 String URLfull_in = "/home/aldisio/Dropbox/mestrado/PDI/Bases/BancoImagens_TomCinza/";
	 String name_image = "woman_blonde";
	 String name_image_in = name_image+extention;

	 // --------------------------------------------
	 // Endereço da imagem de saída
	 // --------------------------------------------
	 String URLfull_out = "/home/aldisio/Dropbox/mestrado/workspaceC/OpenCV/src/OUT/";
	 String name_image_out = name_image;

	 Mat img_src, img_dst, warp_rotate_dst;

	 // --------------------------------------------
	 // Carrega a imagem
	 img_src = imread(URLfull_in+name_image_in, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);   // Read the file

	 // --------------------------------------------
	 // Inicializa uma matriz que será a imagem resultante da transformação Afim
	 // A imagem resultante terá a mesma dimensão e tipo
	 img_dst = Mat::zeros( img_src.rows, img_src.cols, img_src.type() );


	 namedWindow( "Original", CV_WINDOW_AUTOSIZE );
	 imshow( "Original", img_src );
	 imwrite(URLfull_out+name_image_out+"_orig"+extention, img_src);

	// ---------------------------------------------------------
	// NEGATIVO
	// ---------------------------------------------------------
	 /*img_dst =  negativo(img_src);

	 namedWindow( "Negativo", CV_WINDOW_AUTOSIZE );
	 imshow( "Negativo", img_dst );
	 imwrite(URLfull_out+name_image_out+"_negativo"+extention, img_dst);
	 */

	// ---------------------------------------------------------
	// GAMA
	// ---------------------------------------------------------
	 img_dst =  gama(img_src,3,0.6);
	 namedWindow( "Gama", CV_WINDOW_AUTOSIZE );
	 imshow( "Gama", img_dst );
	 imwrite(URLfull_out+name_image_out+"_gama3x06"+extention, img_dst);

	 /// Wait until user exits the program
	 waitKey(0);

	return 0;
  }
