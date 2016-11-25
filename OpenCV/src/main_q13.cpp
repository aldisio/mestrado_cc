#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#include <opencv/cxcore.h>
#include <opencv/ml.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;
using namespace std;

// --------------------------------------------
// Endereço da imagem
// --------------------------------------------
//String extention = ".tif";
String extention = ".png";
String URLfull_in = "/home/aldisio/Dropbox/mestrado/PDI/Bases/BancoImagens_TomCinza/";
//String URLfull_in ="/home/aldisio/Dropbox/mestrado/PDI/Bases/BancoImagens_Binarias/png/";
String name_image = "mdb075_roi";
//String name_image = "confusions";
String name_image_in = name_image+extention;

String URLfull_out = "/home/aldisio/Dropbox/mestrado/workspaceC/OpenCV/src/OUT/";
//String URLfull_in ="/home/aldisio/Dropbox/mestrado/PDI/Bases/BancoImagens_Binarias/png/";
//String name_image = "lena_color_256";
//String name_image = "confusions";
//String name_image_in = name_image+extention;

Mat Float2Uchar(Mat spectrum){
    Mat spectrum_uchar(spectrum.size(),CV_8UC1);
    int ma,me;
    for (int x=0; x<spectrum.rows; x++) {
        for (int y=0; y<spectrum.cols; y++) {
            if (x==0 && y==0) {
                ma=spectrum.at<float>(x,y);
                me=spectrum.at<float>(x,y);
            }
            else{
                if (ma<spectrum.at<float>(x,y)) {
                    ma=spectrum.at<float>(x,y);
                }
                if (me>spectrum.at<float>(x,y)) {
                    me=spectrum.at<float>(x,y);
                }
            }

        }
    }
    for (int x=0; x<spectrum.rows; x++) {
        for (int y=0; y<spectrum.cols; y++) {
            spectrum_uchar.at<uchar>(x,y)=255*((spectrum.at<float>(x,y)-me)/(ma-me));
        }
    }
    return spectrum_uchar;
}
void shiftDFT(Mat& fImage ){
    Mat tmp, q0, q1, q2, q3;

    // first crop the image, if it has an odd number of rows or columns

    fImage = fImage(Rect(0, 0, fImage.cols & -2, fImage.rows & -2));

    int cx = fImage.cols/2;
    int cy = fImage.rows/2;

    // rearrange the quadrants of Fourier image
    // so that the origin is at the image center

    q0 = fImage(Rect(0, 0, cx, cy));
    q1 = fImage(Rect(cx, 0, cx, cy));
    q2 = fImage(Rect(0, cy, cx, cy));
    q3 = fImage(Rect(cx, cy, cx, cy));

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}
Mat create_spectrum_magnitude_display(Mat& complexImg, bool rearrange){
    Mat planes[2];

    // compute magnitude spectrum (N.B. for display)
    // compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))

    split(complexImg, planes);
    magnitude(planes[0], planes[1], planes[0]);

    Mat mag = (planes[0]).clone();
    mag += Scalar::all(1);
    log(mag, mag);

    if (rearrange)
    {
        // re-arrange the quaderants
        shiftDFT(mag);
    }

    normalize(mag, mag, 0, 1, CV_MINMAX);

    return mag;

}
void create_butterworth_lowpass_filter(Mat &dft_Filter, int D, int n){
    Mat tmp = Mat(dft_Filter.rows, dft_Filter.cols, CV_32F);

    Point centre = Point(dft_Filter.rows / 2, dft_Filter.cols / 2);
    double radius;

    for(int i = 0; i < dft_Filter.rows; i++){
        for(int j = 0; j < dft_Filter.cols; j++){
            radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
            tmp.at<float>(i,j) = (float)( 1 / (1 + pow((double) (radius /  D), (double) (2 * n))));
        }
    }
    Mat toMerge[] = {tmp, tmp};
    merge(toMerge, 2, dft_Filter);
}
void create_butterworth_highpass_filter(Mat &dft_Filter, int D, int n){
    Mat tmp = Mat(dft_Filter.rows, dft_Filter.cols, CV_32F);

    Point centre = Point(dft_Filter.rows / 2, dft_Filter.cols / 2);
    double radius;

    for(int i = 0; i < dft_Filter.rows; i++){
        for(int j = 0; j < dft_Filter.cols; j++){
            radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
            tmp.at<float>(i,j) = (float)(1 /(1+pow((double) (D/radius), (double) (2 * n))));
        }
    }
    Mat toMerge[] = {tmp, tmp};
    merge(toMerge, 2, dft_Filter);
}
Mat MadeFilterLowPass(Mat filter,int raio){
    Mat mask(filter.rows,filter.cols,CV_32F);
    int dist;
    for (int x=0; x<filter.rows; x++)
    {
        for (int y=0; y<filter.cols; y++)
        {
            dist = sqrt( pow(filter.cols/2-y,2)+pow(filter.rows/2-x,2) );
            if(dist<=raio)
            {
                mask.at<float>(x,y)=1;
            }
            else
            {
                mask.at<float>(x,y)=0;
            }
        }
    }
    Mat toMerge[] = {mask, mask};
    merge(toMerge, 2, mask);
    return mask;
}
Mat MadeFilterHighPass(Mat filter,int raio){
    Mat mask(filter.rows,filter.cols,CV_32F);
    int dist;
    for (int x=0; x<filter.rows; x++)
    {
        for (int y=0; y<filter.cols; y++)
        {
            dist = sqrt( pow(filter.cols/2-y,2)+pow(filter.rows/2-x,2) );
            if(dist<=raio)
            {
                mask.at<float>(x,y)=0;
            }
            else
            {
                mask.at<float>(x,y)=1;
            }
        }
    }
    Mat toMerge[] = {mask, mask};
    merge(toMerge, 2, mask);
    return mask;
}

int main(){

    Mat img=imread(URLfull_in+name_image_in,CV_LOAD_IMAGE_GRAYSCALE);

    Mat imgOutput, padded,complexImg, filter, filter_spectrum,filter_space, filterOutput, mag;
    Mat planes[2];

    int N, M;

    int radius = 80;
    int order = 2;

    stringstream convert; // stringstream used for the conversion

    convert << radius;

    URLfull_out = URLfull_out+convert.str();

    M = getOptimalDFTSize( img.rows );
    N = getOptimalDFTSize( img.cols );

    copyMakeBorder(img, padded, 0, M - img.rows, 0,N - img.cols, BORDER_CONSTANT, Scalar::all(0));
    planes[0] = Mat_<float>(padded);
    planes[1] = Mat::zeros(padded.size(), CV_32F);

    merge(planes, 2, complexImg);

    dft(complexImg, complexImg);

    Mat spectrum=complexImg;
    spectrum=create_spectrum_magnitude_display(spectrum, true);

    // construct the filter
    filter = complexImg.clone();
    //create_butterworth_lowpass_filter(filter, radius, order); // Filtro Butterworth LowPass
    create_butterworth_highpass_filter(filter, radius, order); // Filtro Butterworth HighPass
    //filter=MadeFilterLowPass(filter,radius); // Filtro LowPass
    //filter=MadeFilterHighPass(filter,radius); // Filtro HighPass


    // apply filter
    shiftDFT(complexImg);
    mulSpectrums(complexImg, filter, complexImg, 0);
    shiftDFT(complexImg);
    // create magnitude spectrum for display

    mag = create_spectrum_magnitude_display(complexImg, true);
    filter_spectrum = create_spectrum_magnitude_display(filter, true);
    // do inverse DFT on filtered image

    idft(complexImg, complexImg);

    // split into planes and extract plane 0 as output image

    split(complexImg, planes);
    normalize(planes[0], imgOutput, 0, 1, CV_MINMAX);

    // do the same with the filter image

    split(filter, planes);
    normalize(planes[0], filterOutput, 0, 1, CV_MINMAX);

    idft(filter, filter);

        // split into planes and extract plane 0 as output image

        split(filter, planes);
        normalize(planes[0], filter_space, 0, 1, CV_MINMAX);

    //Original Image
    imshow("Imagem original", img);
    imwrite(URLfull_out+"_original.jpg", img);

    //Spectrum Original Image
    imshow("Imagem original no domínio da frequencia - Magnitude", spectrum);
    spectrum=Float2Uchar(spectrum);
    imwrite(URLfull_out+"original_espectro.jpg", spectrum);

    //Filtered Image
    imshow("Imagem Filtrada", imgOutput);
    imgOutput=Float2Uchar(imgOutput);
    imwrite(URLfull_out+"filtrada.jpg", imgOutput);

    //Spectrum Filtered Image
    imshow("Imagem filtrada no domínio da frequencia - Magnitude", mag);
    mag=Float2Uchar(mag);
    imwrite(URLfull_out+"filtrada_espectro.jpg", mag);

    //Mask
    imshow("Filtro no dominio da frequencia", filterOutput);
    filterOutput=Float2Uchar(filterOutput);
    imwrite(URLfull_out+"filtro_espectro.jpg", filterOutput);

    imshow("Filtro Original", filter_space);
    filter_space=Float2Uchar(filter_space);
	imwrite(URLfull_out+"filtro_original.jpg", filter_space);


    waitKey();
}
