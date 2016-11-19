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
	 String extention = ".png";
	 String URLfull_in = "/home/aldisio/Dropbox/mestrado/PDI/Bases/BancoImagens_Binarias/png/";
	 String name_image = "device8-9";
	 String name_image_in = name_image+extention;

	 // --------------------------------------------
	 // Endereço da imagem de saída
	 // --------------------------------------------
	 String URLfull_out = "/home/aldisio/Dropbox/mestrado/workspaceC/OpenCV/src/OUT/";
	 String name_image_out = name_image;

	 Mat img_src, img_dst, warp_rotate_dst;

	 // --------------------------------------------
	 // Carrega a imagem
	 img_src = imread(URLfull_in+name_image_in, CV_LOAD_IMAGE_COLOR);   // Read the file

	 // --------------------------------------------
	 // Inicializa uma matriz que será a imagem resultante da transformação Afim
	 // A imagem resultante terá a mesma dimensão e tipo
	 img_dst = Mat::zeros( img_src.rows, img_src.cols, img_src.type() );


	 namedWindow( "Original", CV_WINDOW_AUTOSIZE );
	 imshow( "Original", img_src );
	 imwrite(URLfull_out+name_image_out+"_orig"+extention, img_src);

	 // --------------------------------------------
	 // Transformação de Translação
	 // --------------------------------------------
	 /*int desl_x = img_src.cols/2;
	 int desl_y = 0;
	 img_dst = geot_translation(img_src, desl_x, desl_y);

	 namedWindow( "Transformação de Translação", CV_WINDOW_AUTOSIZE );
	 imshow( "Transformação de Translação", img_dst );
	 imwrite(URLfull_out+name_image_out+"_tran_x"+std::to_string(desl_x)+'y'+std::to_string(desl_y)+extention, img_dst);
	 */

	 // --------------------------------------------
	 // Transformação de Rotação
	 // --------------------------------------------
	 /*double angle = 270;
	 img_dst = geot_rotation(img_src, img_src.cols/2, img_src.rows/2, angle, 1);

	 namedWindow( "Transformação de Rotação", CV_WINDOW_AUTOSIZE );
	 imshow( "Transformação de Rotação", img_dst );
	 imwrite(URLfull_out+name_image_out+"_rot_270"+extention, img_dst);
	 */

	 // --------------------------------------------
	 // Transformação de Escala
	 // --------------------------------------------
	 double angle = 0;
	 img_dst = geot_rotation(img_src, img_src.cols/2, img_src.rows/2, angle, 1.5);

	 namedWindow( "Transformação de Escala", CV_WINDOW_AUTOSIZE );
	 imshow( "Transformação de Escala", img_dst );
	 imwrite(URLfull_out+name_image_out+"_esc_150perc"+extention, img_dst);






	/// Wait until user exits the program
	waitKey(0);

	return 0;
  }
