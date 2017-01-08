#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

using namespace cv;
using namespace std;

// --------------------------------------------
 // Endereço da imagem
 // --------------------------------------------
 String extention = ".png";
 String URLfull_in = "/home/aldisio/Dropbox/mestrado/PDI/Bases/BancoImagens_TomCinza/";
 //String name_image = "circulos";
 String name_image = "moedas";
 String name_image_in = name_image+extention;

 // --------------------------------------------
 // Endereço da imagem de saída
 // --------------------------------------------
 String URLfull_out = "/home/aldisio/Dropbox/mestrado/workspaceC/OpenCV/src/OUT/";
 String name_image_out = name_image;


int main(int argc, char** argv)
{
  Mat src, src_gray;

  Mat img_src = imread(URLfull_in+name_image_in, CV_LOAD_IMAGE_ANYCOLOR);   // Read the file
  /// Read the image
  src = img_src;//imread( argv[1], 1 );

  if( !src.data )
    { return -1; }

  if(src.channels()>1)
      cvtColor(src, src_gray, CV_BGR2GRAY);
  else
	  src_gray = src;


  /// Convert it to gray
  //cvtColor( src, src_gray, CV_BGR2GRAY );

  /// Reduce the noise so we avoid false circle detection
  GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

  vector<Vec3f> circles;

  /// Apply the Hough Transform to find the circles
  HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 10, 10, 0, 0 );

  /// Draw the circles detected
  //for( size_t i = 0; i < circles.size(); i++ )
  for( size_t i = 0; i < 2; i++ )
  {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
      circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // circle outline
      circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
   }

  /// Show your results
  namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
  imshow( "Hough Circle Transform Demo", src );
  imwrite(URLfull_out+name_image_out + "_circ_"+ extention, src);

  waitKey(0);
  return 0;
}

