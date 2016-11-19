#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "highgui.h"
#include "geometric_transformation.h"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

const int num_morph_operation = 2;
const int num_morph_ee = 3;

Mat img_src, img_dst, EE;
int sel_ee = 1;
int sel_morph_operation = 0;
int morph_ee = 1;
int morph_ee_size = 3;

// parametros para salvar a imagem
int sel_save = 0;

// --------------------------------------------
// Endereço da imagem
// --------------------------------------------
String extention = ".png";
String URLfull_in = "/home/aldisio/Dropbox/mestrado/PDI/Bases/BancoImagens_TomCinza/";
String name_image = "mdb075_roi";
String name_image_in = name_image+extention;

// --------------------------------------------
// Endereço da imagem de saída
// --------------------------------------------
String URLfull_out = "/home/aldisio/Dropbox/mestrado/workspaceC/OpenCV/src/OUT/";
String name_image_out = name_image;

// --------------------------------------------
// Funções
// --------------------------------------------
void morph_erosion();
void morph_dilatation();
void SelectOp( int, void* );
void img_save( int, void*);
const std::string currentDateTime();

/** @function main */
int main( int argc, char** argv )
{

	/// Create windows
	  namedWindow( "Operação Morfológica", CV_WINDOW_AUTOSIZE );
	  //cvMoveWindow( "Operação Morfológica", 900, 500 );

	 /// Load an image
	 img_src = imread(URLfull_in+name_image_in, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);   // Read the file
	 //img_src = geot_rotation(img_src, img_src.cols/2, img_src.rows/2, 0, 1.50);

	 createTrackbar( "Operação Morfológica:\n 0:Erosão \n 1: Dilatação", "Operação Morfológica", &sel_morph_operation, num_morph_operation-1, SelectOp);

	 createTrackbar( "Elemento Estruturante:\n 0: Quadrado \n 1: Cruz \n 2: Circulo", "Operação Morfológica", &sel_ee, num_morph_ee-1, SelectOp );

	 createTrackbar( "Tamanho (2n+1):\n", "Operação Morfológica", &morph_ee_size, 15, SelectOp );

	 createTrackbar( "Salva Imagem(n=1):\n", "Operação Morfológica", &sel_save, 1, img_save );


	 /// Default start
	 SelectOp( 0 ,0 );

	  waitKey(0);
	  return 0;
}

void SelectOp( int, void*){

	switch(sel_ee){
			case 0: morph_ee = MORPH_RECT; break;
			case 1: morph_ee = MORPH_CROSS; break;
			case 2: morph_ee = MORPH_ELLIPSE; break;
	}

	EE = getStructuringElement( morph_ee, Size( 2*morph_ee_size + 1, 2*morph_ee_size+1 ), Point( morph_ee_size, morph_ee_size ) );

	switch(sel_morph_operation){
		case 0: morph_erosion(); break;
		case 1: morph_dilatation(); break;
	}

}

void morph_erosion( ){
  erode( img_src, img_dst, EE );
  imshow( "Operação Morfológica", img_dst );
}

void morph_dilatation( ){
  dilate( img_src, img_dst, EE );
  imshow( "Operação Morfológica", img_dst );
}

void img_save( int, void*){
  if(sel_save == 1){
	  imwrite( URLfull_out+name_image_out+currentDateTime()+extention, img_dst );
  }
  sel_save = 0;
}

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}
