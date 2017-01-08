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
 String extention = ".tif";
 String URLfull_in = "/home/aldisio/Dropbox/mestrado/PDI/Bases/BancoImagens_TomCinza/";
 //String name_image = "circulos";
 String name_image = "house";
 String name_image_in = name_image+extention;

 // --------------------------------------------
 // Endereço da imagem de saída
 // --------------------------------------------
 String URLfull_out = "/home/aldisio/Dropbox/mestrado/workspaceC/OpenCV/src/OUT/";
 String name_image_out = name_image;



void help()
{
 cout << "\nThis program demonstrates line finding with the Hough transform.\n"
         "Usage:\n"
         "./houghlines <image_name>, Default is pic1.jpg\n" << endl;
}

int main(int argc, char** argv)
{
 const char* filename = argc >= 2 ? argv[1] : "pic1.jpg";
 // --------------------------------------------
 // Carrega a imagem
 // --------------------------------------------
 Mat img_src = imread(URLfull_in+name_image_in, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);   // Read the file


 Mat src = img_src;//imread(filename, 0);
 if(src.empty())
 {
     help();
     cout << "can not open " << filename << endl;
     return -1;
 }

 Mat dst, cdst;
 Canny(src, dst, 50, 200, 3);
 cvtColor(dst, cdst, CV_GRAY2BGR);

 #if 0
  vector<Vec2f> lines;
  HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );

  for( size_t i = 0; i < lines.size(); i++ )
  {
     float rho = lines[i][0], theta = lines[i][1];
     Point pt1, pt2;
     double a = cos(theta), b = sin(theta);
     double x0 = a*rho, y0 = b*rho;
     pt1.x = cvRound(x0 + 1000*(-b));
     pt1.y = cvRound(y0 + 1000*(a));
     pt2.x = cvRound(x0 - 1000*(-b));
     pt2.y = cvRound(y0 - 1000*(a));
     line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
  }
 #else
  vector<Vec4i> lines;
  HoughLinesP(dst, lines, 1, CV_PI/180, 150, 10, 10 );

  // votos: 10, 100, 150 (100)
  // votos: 10, 100, 150 (10)

//dst: Output of the edge detector. It should be a grayscale image (although in fact it is a binary one)
//lines: A vector that will store the parameters (r,\theta) of the detected lines
//rho : The resolution of the parameter r in pixels. We use 1 pixel.
//theta: The resolution of the parameter \theta in radians. We use 1 degree (CV_PI/180)
//threshold: The minimum number of intersections to “detect” a line
//srn and stn: Default parameters to zero. Check OpenCV reference for more info.

  for( size_t i = 0; i < lines.size(); i++ )
  {
    Vec4i l = lines[i];
    line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
  }
 #endif
 imshow("source", src);
 imwrite(URLfull_out+name_image_out + "_orig_"+ extention, src);
 imshow("detected lines", cdst);
 imwrite(URLfull_out+name_image_out + "_lines_"+ extention, cdst);

 waitKey();

 return 0;
}

/**********************************************/

