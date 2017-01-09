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
 String name_image = "okarate";
 String name_image_in = name_image+extention;

 // --------------------------------------------
 // Endereço da imagem de saída
 // --------------------------------------------
 String URLfull_out = "/home/aldisio/Dropbox/mestrado/workspaceC/OpenCV/src/OUT/";
 String name_image_out = name_image;


#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;
// static void help()
// {
//     cout << "\nThis program demonstrates kmeans clustering.\n"
//             "It generates an image with random points, then assigns a random number of cluster\n"
//             "centers and uses kmeans to move those cluster centers to their representitive location\n"
//             "Call\n"
//             "./kmeans\n" << endl;
// }

int main() {
    Mat src;

    src = imread(URLfull_in+name_image_in, CV_LOAD_IMAGE_ANYCOLOR);   // Read the file
    imshow("original", src);

    blur(src, src, Size(15,15));
    imshow("blurred", src);

    Mat p = Mat::zeros(src.cols*src.rows, 5, CV_32F);
    Mat bestLabels, centers, clustered;
    vector<Mat> bgr;
    cv::split(src, bgr);
    // i think there is a better way to split pixel bgr color
    for(int i=0; i<src.cols*src.rows; i++) {
        p.at<float>(i,0) = (i/src.cols) / src.rows;
        p.at<float>(i,1) = (i%src.cols) / src.cols;
        p.at<float>(i,2) = bgr[0].data[i] / 255.0;
        p.at<float>(i,3) = bgr[1].data[i] / 255.0;
        p.at<float>(i,4) = bgr[2].data[i] / 255.0;
    }

    int K = 16;
    cv::kmeans(p, K, bestLabels,
            TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0),
            3, KMEANS_PP_CENTERS, centers);

    int colors[K];
    for(int i=0; i<K; i++) {
        colors[i] = 255/(i+1);
    }
    // i think there is a better way to do this mayebe some Mat::reshape?
    clustered = Mat(src.rows, src.cols, CV_32F);
    for(int i=0; i<src.cols*src.rows; i++) {
        clustered.at<float>(i/src.cols, i%src.cols) = (float)(colors[bestLabels.at<int>(0,i)]);
//      cout << bestLabels.at<int>(0,i) << " " <<
//              colors[bestLabels.at<int>(0,i)] << " " <<
//              clustered.at<float>(i/src.cols, i%src.cols) << " " <<
//              endl;
    }

    clustered.convertTo(clustered, CV_8U);
    imshow("clustered", clustered);

    imwrite(URLfull_out+name_image_out + "_clust16"+extention, clustered);
    imwrite(URLfull_out+name_image_out + "_orig"+extention, src);

    waitKey();
    return 0;
}

