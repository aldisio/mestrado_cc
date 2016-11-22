#include "opencv2/opencv.hpp"

using namespace cv;


int min_matiz = 220;
int max_matiz = 260;
int min_sat = 190;
int max_sat = 255;
int min_value = 0;
int max_value = 255;
Mat frame_rgb,frame_result;
Mat img_dstC3;
Mat frame_hsv;
Mat EE;
String widow_name = "OpenCV - Detecção de Objetos";

void atualiza_param_hsv(int, void*);


int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    namedWindow(widow_name , CV_WINDOW_AUTOSIZE );

    createTrackbar( "H-Min:" , widow_name, &min_matiz, 360, atualiza_param_hsv);
    createTrackbar( "H-Max:" , widow_name, &max_matiz, 360, atualiza_param_hsv);

    createTrackbar( "S-Min:" , widow_name, &min_sat, 255, atualiza_param_hsv);
    createTrackbar( "S-Max:" , widow_name, &max_sat, 255, atualiza_param_hsv);

    createTrackbar( "V-Min:" , widow_name, &min_value, 255, atualiza_param_hsv);
    createTrackbar( "V-Max:" , widow_name, &max_value, 255, atualiza_param_hsv);

    for(;;)
    {
    	// captura do frame
        cap >> frame_rgb;

        // conversão do frame de rgb para hsv
        cvtColor(frame_rgb, frame_hsv, CV_RGB2HSV);

        // conversão do frame de rgb para hsv
        Scalar threshold_min(min_matiz, min_sat, min_value);
        Scalar threshold_max(max_matiz, max_sat, max_value);

        // Filtragem do objetos a partir da manipulação das componentes HSV como limiares da imagem
        // o resultado desta etapa será uma imagem preto e branco com as componentes detectadas
        frame_result = Mat(frame_hsv.rows, frame_hsv.cols, CV_8U);
        cv::inRange( frame_hsv, threshold_min, threshold_max, frame_result);

        // Filtra componentes menores utilizando MM
        EE = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
        morphologyEx(frame_result, frame_result, cv::MORPH_OPEN, EE);
        morphologyEx(frame_result, frame_result, cv::MORPH_CLOSE, EE);

        // -------------------------------------------
		// Identificação e cálculo do centróide na componente identificada na imagem
		// -------------------------------------------
        int cy=0, cx=0;
        int num_pixels = 1;

        for(int i=0; i < frame_rgb.rows; i++) {

				for(int j=0; j < frame_rgb.cols;j++) {

					// captura o pixel da imagem em preto e branco
					uchar pixel = frame_result.at<uchar>(i, j);

					// Na imagem, 0 é o fundo e 255 é o objeto
					if(pixel == 255){
							cx += i;
							cy += j;
							num_pixels++;
					}

				}
		}

    	printf("Centroide: p(%d, %d)\n", cx/num_pixels, cy/num_pixels );

    	// -------------------------------------------
		// Marcação do centróide identificado na imagem
		// -------------------------------------------
    	int tam_centroide = 10;
        for(int i=0; i < frame_rgb.rows; i++) {

        		for(int j=0; j < frame_rgb.cols;j++) {

        					// captura o pixel da imagem colorida
        					Vec3b pixel_colorido = frame_rgb.at<Vec3b>(i, j);

        					// Demarcação do intervalo de pixels para projeção do centróide na imagem original
        					if((i >= cx/num_pixels && i <= cx/num_pixels+tam_centroide)
        					&& (j >= cy/num_pixels && j <= cy/num_pixels+tam_centroide)
        					){
        						pixel_colorido.val[0] = 0;
        						pixel_colorido.val[1] = 255;
        						pixel_colorido.val[2] = 255;

        						frame_rgb.at<Vec3b>(i, j) = pixel_colorido;
        					}


        		}
        	}

        // -------------------------------------------
        // Exibição do resultado
        // -------------------------------------------
        imshow(widow_name+" - Componente Detectada", frame_result);
        imshow(widow_name+" - Original", frame_rgb);
        if(waitKey(30) >= 0) break;
    }

    return 0;
}


void atualiza_param_hsv(int, void*){
	min_matiz = min_matiz;
	max_matiz = max_matiz;

	min_sat = min_sat;
	max_sat = max_sat;

	min_value = min_value;
	max_value = max_value;
}

