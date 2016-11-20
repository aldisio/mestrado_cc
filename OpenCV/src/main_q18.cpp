#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "highgui.h"
#include "geometric_transformation.h"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

const int num_morph_operation = 4;
const int num_morph_ee = 3;

Mat img_src, img_dst, EE;
cv::Mat img_dstC3;
int sel_ee = 1;
int sel_morph_operation = 0;
int morph_ee = 1;
int morph_ee_size = 3;

// parametros para salvar a imagem
int sel_save = 0;

// --------------------------------------------
// Endereço da imagem
// --------------------------------------------
//String extention = ".tif";
String extention = ".tif";
String URLfull_in = "/home/aldisio/Dropbox/mestrado/PDI/Bases/BancoImagens_TomCinza/";
//String URLfull_in ="/home/aldisio/Dropbox/mestrado/PDI/Bases/BancoImagens_Binarias/png/";
String name_image = "woman_blonde";
//String name_image = "confusions";
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
void morph_gradiente();
void morph_compconected();
void FindBlobs(const cv::Mat &binary, std::vector < std::vector<cv::Point2i> > &blobs);


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
	 img_dstC3 = cv::Mat::zeros(img_src.size(), CV_8UC3);
	 //img_src = geot_rotation(img_src, img_src.cols/2, img_src.rows/2, 0, 1.50);

	 createTrackbar( "Operação Morfológica:\n 0:Erosão "
			 "\n 1: Dilatação \n 2: Gradiente \n 3: Compo.Conectados"
			 , "Operação Morfológica", &sel_morph_operation, num_morph_operation-1, SelectOp);

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
			case 0: morph_ee = MORPH_RECT; 		break;
			case 1: morph_ee = MORPH_CROSS; 	break;
			case 2: morph_ee = MORPH_ELLIPSE; 	break;
	}

	EE = getStructuringElement( morph_ee, Size( 2*morph_ee_size + 1, 2*morph_ee_size+1 ), Point( morph_ee_size, morph_ee_size ) );

	switch(sel_morph_operation){
		case 0: morph_erosion(); 		break;
		case 1: morph_dilatation(); 	break;
		case 2: morph_gradiente(); 		break;
		case 3: morph_compconected(); 	break;
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

void morph_gradiente( ){
	Mat img_dilate = Mat::zeros(img_src.cols, img_src.rows, img_src.type());
	Mat img_erode = Mat::zeros(img_src.cols, img_src.rows , img_src.type());
	Mat img_gradiente = Mat::zeros(img_src.cols, img_src.rows , img_src.type());

	dilate( img_src, img_dilate, EE );
	erode( img_src, img_erode, EE );

	img_dst = img_dilate - img_erode;
	imshow( "Operação Morfológica", img_dst );
}

void img_save( int, void*){
  if(sel_save == 1){
	  if(sel_morph_operation == 3)
		  imwrite( URLfull_out+name_image_out+currentDateTime()+extention, img_dstC3 );
	  else
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

void morph_compconected(){
		cv::Mat binary;
	    std::vector < std::vector<cv::Point2i > > blobs;

	    cv::threshold(img_src, binary, 0.0, 1.0, cv::THRESH_BINARY+cv::THRESH_OTSU);

	    FindBlobs(binary, blobs);

	    // Randomy color the blobs
	    for(size_t i=0; i < blobs.size(); i++) {
	        unsigned char r = 255 * (rand()/(1.0 + RAND_MAX));
	        unsigned char g = 255 * (rand()/(1.0 + RAND_MAX));
	        unsigned char b = 255 * (rand()/(1.0 + RAND_MAX));

	        for(size_t j=0; j < blobs[i].size(); j++) {
	            int x = blobs[i][j].x;
	            int y = blobs[i][j].y;

	            img_dstC3.at<cv::Vec3b>(y,x)[0] = b;
	            img_dstC3.at<cv::Vec3b>(y,x)[1] = g;
	            img_dstC3.at<cv::Vec3b>(y,x)[2] = r;
	        }
	    }

	    //cv::imshow("binary", img);
	    //cv::imshow("labelled", output);
	    imshow( "Operação Morfológica", img_dstC3 );
}

void FindBlobs(const cv::Mat &binary, std::vector < std::vector<cv::Point2i> > &blobs){
    blobs.clear();

    // Fill the label_image with the blobs
    // 0  - background
    // 1  - unlabelled foreground
    // 2+ - labelled foreground

    cv::Mat label_image;
    binary.convertTo(label_image, CV_32SC1);

    int label_count = 2; // starts at 2 because 0,1 are used already

    for(int y=0; y < label_image.rows; y++) {
        int *row = (int*)label_image.ptr(y);
        for(int x=0; x < label_image.cols; x++) {
            if(row[x] != 1) {
                continue;
            }

            cv::Rect rect;
            cv::floodFill(label_image, cv::Point(x,y), label_count, &rect, 0, 0, 4);

            std::vector <cv::Point2i> blob;

            for(int i=rect.y; i < (rect.y+rect.height); i++) {
                int *row2 = (int*)label_image.ptr(i);
                for(int j=rect.x; j < (rect.x+rect.width); j++) {
                    if(row2[j] != label_count) {
                        continue;
                    }

                    blob.push_back(cv::Point2i(j,i));
                }
            }

            blobs.push_back(blob);

            label_count++;
        }
    }
}

