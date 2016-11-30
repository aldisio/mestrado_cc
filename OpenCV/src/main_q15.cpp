#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#include <opencv/cxcore.h>
#include <opencv/ml.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/core/core.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/photo.hpp>
#include <stdlib.h>
#include <stdio.h>
using namespace cv;
using namespace std;

// Filter type
#define NONE 0    // no filter
#define HARD 1    // hard shrinkage
#define SOFT 2    // soft shrinkage
#define GARROT 3  // garrot filter

//--------------------------------
// signum
//--------------------------------
float sgn(float x)
{
    float res=0;
    if(x==0)
    {
        res=0;
    }
    if(x>0)
    {
        res=1;
    }
    if(x<0)
    {
        res=-1;
    }
    return res;
}
//--------------------------------
// Soft shrinkage
//--------------------------------
float soft_shrink(float d,float T)
{
    float res;
    if(fabs(d)>T)
    {
        res=sgn(d)*(fabs(d)-T);
    }
    else
    {
        res=0;
    }
    
    return res;
}
//--------------------------------
// Hard shrinkage
//--------------------------------
float hard_shrink(float d,float T)
{
    float res;
    if(fabs(d)>T)
    {
        res=d;
    }
    else
    {
        res=0;
    }
    
    return res;
}
//--------------------------------
// Garrot shrinkage
//--------------------------------
float Garrot_shrink(float d,float T)
{
    float res;
    if(fabs(d)>T)
    {
        res=d-((T*T)/d);
    }
    else
    {
        res=0;
    }
    
    return res;
}
//--------------------------------
// Wavelet transform
//--------------------------------
static void cvHaarWavelet(Mat &src,Mat &dst,int NIter)
{
    float c,dh,dv,dd;
    assert( src.type() == CV_32FC1 );
    assert( dst.type() == CV_32FC1 );
    int width = src.cols;
    int height = src.rows;
    for (int k=0;k<NIter;k++)
    {
        for (int y=0;y<(height>>(k+1));y++)
        {
            for (int x=0; x<(width>>(k+1));x++)
            {
                c=(float)((src.at<float>(2*y,2*x)+src.at<float>(2*y,2*x+1)+src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5);
                dst.at<float>(y,x)=c;
                
                dh=(float)((src.at<float>(2*y,2*x)+src.at<float>(2*y+1,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x+1))*0.5);
                dst.at<float>(y,x+(width>>(k+1)))=dh;
                
                dv=(float)((src.at<float>(2*y,2*x)+src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)-src.at<float>(2*y+1,2*x+1))*0.5);
                dst.at<float>(y+(height>>(k+1)),x)=dv;
                
                dd=(float)((src.at<float>(2*y,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5);
                dst.at<float>(y+(height>>(k+1)),x+(width>>(k+1)))=dd;
            }
        }
        dst.copyTo(src);
    }
}
//--------------------------------
//Inverse wavelet transform
//--------------------------------
static void cvInvHaarWavelet(Mat &src,Mat &dst,int NIter, int SHRINKAGE_TYPE=0, float SHRINKAGE_T=50)
{
    float c,dh,dv,dd;
    assert( src.type() == CV_32FC1 );
    assert( dst.type() == CV_32FC1 );
    int width = src.cols;
    int height = src.rows;
    //--------------------------------
    // NIter - number of iterations
    //--------------------------------
    for (int k=NIter;k>0;k--)
    {
        for (int y=0;y<(height>>k);y++)
        {
            for (int x=0; x<(width>>k);x++)
            {
                c=src.at<float>(y,x);
                dh=src.at<float>(y,x+(width>>k));
                dv=src.at<float>(y+(height>>k),x);
                dd=src.at<float>(y+(height>>k),x+(width>>k));
                
                // (shrinkage)
                switch(SHRINKAGE_TYPE)
                {
                    case HARD:
                        dh=hard_shrink(dh,SHRINKAGE_T);
                        dv=hard_shrink(dv,SHRINKAGE_T);
                        dd=hard_shrink(dd,SHRINKAGE_T);
                        break;
                    case SOFT:
                        dh=soft_shrink(dh,SHRINKAGE_T);
                        dv=soft_shrink(dv,SHRINKAGE_T);
                        dd=soft_shrink(dd,SHRINKAGE_T);
                        break;
                    case GARROT:
                        dh=Garrot_shrink(dh,SHRINKAGE_T);
                        dv=Garrot_shrink(dv,SHRINKAGE_T);
                        dd=Garrot_shrink(dd,SHRINKAGE_T);
                        break;
                }
                
                //-------------------
                dst.at<float>(y*2,x*2)=(float)(0.5*(c+dh+dv+dd));
                dst.at<float>(y*2,x*2+1)=(float)(0.5*(c-dh+dv-dd));
                dst.at<float>(y*2+1,x*2)=(float)(0.5*(c+dh-dv-dd));
                dst.at<float>(y*2+1,x*2+1)=(float)(0.5*(c-dh-dv+dd));
            }
        }
        Mat C=src(Rect(0,0,width>>(k-1),height>>(k-1)));
        Mat D=dst(Rect(0,0,width>>(k-1),height>>(k-1)));
        D.copyTo(C);
    }
}
//--------------------------------
//
//--------------------------------
int main()
{
    int n = 0;
    const int NIter=3;
    
	/*
    // --------------------------------------------
    // Endereço da imagem
    // --------------------------------------------
    //String extention = ".tif";
    String extention = ".pgn";
    String URLfull_in = "/home/aldisio/Dropbox/mestrado/PDI/Bases/BancoImagens_TomCinza/";
    //String URLfull_in ="/home/aldisio/Dropbox/mestrado/PDI/Bases/BancoImagens_Binarias/png/";
    String name_image = "mdb075_roi";
    String name_image_in = name_image+extention;
*/

    Mat frame = imread("digital.png",1);
    Mat GrayFrame = Mat(frame.rows, frame.cols, CV_8UC1);
    Mat Src  = Mat(frame.rows, frame.cols, CV_32FC1);
    Mat Dst  = Mat(frame.rows, frame.cols, CV_32FC1);
    Mat Temp = Mat(frame.rows, frame.cols, CV_32FC1);
    Mat Filtered = Mat(frame.rows, frame.cols, CV_32FC1);
    
    Dst=0;
    cvtColor(frame, GrayFrame, CV_BGR2GRAY);
    GrayFrame.convertTo(Src,CV_32FC1);
    cvHaarWavelet(Src,Dst,NIter);
    
    Dst.copyTo(Temp);
    
	cvInvHaarWavelet(Temp,Filtered,NIter,GARROT,30);
    
    double M=0,m=0;
    //----------------------------------------------------
    // Normalização
    //----------------------------------------------------
    
    minMaxLoc(Dst,&m,&M);
    if((M-m)>0) {Dst=Dst*(1.0/(M-m))-m/(M-m);}
    
    minMaxLoc(Filtered,&m,&M);
    if((M-m)>0) {Filtered=Filtered*(1.0/(M-m))-m/(M-m);}        
    
    imshow("Imagem_Original", frame);
	imwrite("Imagem_Original.png", frame);

    imshow("Decomposicao", Dst);
	imwrite("Decomposicao.jpg", Dst);

    imshow("Imagem_filtrada", Filtered);
	imwrite("Imagem_filtrada.png", Filtered);
    waitKey(0);
}
