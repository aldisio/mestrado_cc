#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>


using namespace cv;
using namespace std;

int x_sel=0, y_sel=0;
Mat img_src, mask;
std::vector< int > valores_regiao;

// --------------------------------------------
 // Endereço da imagem
 // --------------------------------------------
 //String extention = ".tif";
 String extention = ".png";
 //String URLfull_in = "/home/aldisio/Dropbox/mestrado/PDI/Bases/BancoImagens_TomCinza/";
 String URLfull_in = "/home/aldisio/Dropbox/mestrado/PDI/Bases/BancoImagens_Binarias/png/";
 //String name_image = "peppers_gray";
 String name_image = "blob";
 String name_image_in = name_image+extention;

 // --------------------------------------------
 // Endereço da imagem de saída
 // --------------------------------------------
 String URLfull_out = "/home/aldisio/Dropbox/mestrado/workspaceC/OpenCV/src/OUT/";
 String name_image_out = name_image;


int calc_dp(){
	float var = 0;
	float media = 0;
	float dp = 0;
	int soma = 0;

	int N = valores_regiao.size();

	cout << "Numero de Elementos:"<< N;

	for(int i = 0; i < N; i++)
		soma+=valores_regiao.at(i);

	media = (soma)/N;

	cout << "Soma:" << soma;
	cout << "Media:" << media;

	for(int i = 0; i < N; i++)
	{

		/*var(x) = Sn ((xi - média)^2 ) /N*/
		var = (var + (pow(valores_regiao.at(i) - media,2))/N);
	}

	cout << "Var:" << var;
	dp = sqrt(var);

	cout << "DP:" << dp;
	return dp;
}

int calc_mean(){
	float media = 0;
	int soma = 0;

	int N = valores_regiao.size();

	cout << "Numero de Elementos:"<< N;

	for(int i = 0; i < N; i++)
		soma+=valores_regiao.at(i);

	media = (soma)/N;

	return media;
}

/***************************************************************
-- imfilter_mean(Mat *im, int dimMask)
----------------------------------------------------------------
-- Descrição: Aplica o filtro da média
-- Exemplo das coordenadas de uma máscara 3x3:
		(i-1,j-1)	, (i-1,j)	, (i-1,j+1)
		(i,j-1)		, (i,j)		, (i,j+1)
		(i+1,j-1)	, (i+1,j)	, (i+1,j+1)
***************************************************************/

Mat region_growing(Mat image, Mat mask, int x, int y){
		Mat image_out = Mat::zeros(image.cols, image.rows, image.type());
		Mat image_flag = Mat::zeros(image.cols, image.rows, image.type());

		// Declarando variaveis de manipulação da imagem
		int height = image.rows;
		int width = image.cols;
		int m_i = 0, m_j = 0; // Índices para varredura da imagem considerando as dimensões da máscara
		int m_x = 0, m_y = 0; // Índices para varredura da mascara
		int a = (mask.cols - 1)/2; // Isso porque a mascará será uma matriz quadrada e com dimensões de valores ímpares, número ímpar = 2a+1
		int i=0;
		int j=0;

		// Declarando variaveis para calculo da media
		int num_px_regiao = 0;
		bool incrementou_regiao;

		// Adiciona o pixel selecionado no click na região
		image_out.at<uchar>(x,y) = 255;
		image_flag.at<uchar>(i,j) = 1;
		valores_regiao.push_back(image.at<uchar>(x,y));
		num_px_regiao++;

		// ----------------------------------------------------------
		// Percorre as linhas i da imagem
		for(i=0; i<height; i++){

			// ----------------------------------------------------------
			// Percorre as colunas j da imagem
			for(j=0; j<width; j++){

					// pixel da imagem de entrada
					uchar pixel_imagem = image.at<uchar>(i,j);
					uchar pixel_imagem_out = image_out.at<uchar>(i,j);
					uchar pixel_flag = image_flag.at<uchar>(i,j);

					if(pixel_flag == 0 && pixel_imagem_out == 255){
						// ----------------------------------------------------------
						// Marca pixel como visitado
						incrementou_regiao = false;
						// ----------------------------------------------------------
						// Percorre as linhas m_i da mascara correlacionando com o pixel p(i,j) da imagem
						for(m_i = i-a , m_y = 0; m_i <= i+a && m_y < mask.cols; m_i++,m_y++){
							//printf("\n");
							for(m_j = j-a, m_x = 0; m_j <= j+a && m_x < mask.rows; m_j++, m_x++){

								// ----------------------------------------------------------
								// Desconsidera na contagem os pixels que estão foram da imagem
								if(!(m_i<0 ||m_j<0 || m_i>=height || m_j >= width)){

									// pixel da imagem de entrada
									uchar pixel_mask = image.at<uchar>(m_i,m_j);
									uchar pixel_mask_out = image_out.at<uchar>(m_i,m_j);
									uchar pixel_flag = image_flag.at<uchar>(m_i,m_j);

									// verifica se ele pertence a região de crescimento
									//if(abs(pixel_imagem-pixel_mask) < calc_dp()){
									//cout << "I: "<<i<<" J: "<<j<< "M_I: "<< m_i << "M_J" << m_j;
									if(abs(pixel_imagem-pixel_mask) == 0){
									//if(abs(pixel_mask) <= calc_mean()){

										// não visitado
										if(pixel_flag == 0){
											image_out.at<uchar>(m_i,m_j) = 255;
											num_px_regiao++;
											incrementou_regiao = true;
										}

										valores_regiao.push_back(image.at<uchar>(m_i,m_j));
										cout << "\nIncrementando Região: "<<valores_regiao.size();

										//if(num_px_regiao > 30000){
										//	return image_out;
										//}
										//printf("\nRegião: %d",num_px_regiao);
									}//end if

								}//end for

							}//end for

						}// end if

						// Marca como visitado
						image_flag.at<uchar>(i,j) = 1;

						if(incrementou_regiao){
							incrementou_regiao = false;
							cout << "\nIndex:" << i << " Iteração: " << num_px_regiao;

							i = 0;
							j = 0;


						}


				}


					/*
					printf("\nSoma Pixels validos: %d ",sum_mask);
					printf("\nNum Pixels validos: %d ",quant_px_val);
					printf("p(%d,%d) <- %d / %d = %d", i, j,sum_product_px, sum_mask, sum_product_px / sum_mask );
					*/
				//} // if para limitar no debug a faixa a ser processada

				// ----------------------------------------------------------
				// Atribui o valor calculado na vizinhança ao pixel processado, soma do produto pixel-a-pixel correspondente dividido pela soma da mascara
				//image.at<uchar>(i,j) = sum_product_px / sum_mask;
			}
		}

		return image_out;
}





void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

          // preenche os valores das coordenadas no evento do click
          x_sel = x;
          y_sel = y;

          x_sel = 179;
          y_sel = 168;


          std::ostringstream ss;
          ss <<URLfull_out;
          ss <<name_image_out;
          ss << "X_";
          ss << x_sel;
          ss << "Y_";
          ss << y_sel;
          ss<<extention;

          Mat i = region_growing(img_src, mask, y_sel, x_sel);
          //imwrite(URLfull_out+name_image_out + "X_" + std::to_string(x) + "Y_"+ std::to_string(y) + extention, i);
          imwrite(ss.str(), i);
         imshow( "Resultado", i );
     }
     else if  ( event == EVENT_RBUTTONDOWN )
     {
          cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }



}

/** @function main */
 int main( int argc, char** argv )
 {

	 // ---------------------------------------------------------
	 	// Definindo mascara
	 	mask = (Mat_<uchar>(3,3) << 1, 1, 1,
	 									1, 1, 1,
	 									1, 1, 1);
	 	/*Mat mask = (Mat_<uchar>(3,3) << 1, 2, 1,
	 									2, 4, 2,
	 									1, 2, 1);*/
	 	/*Mat mask = (Mat_<uchar>(5,5) <<1, 1, 1, 1, 1,
	 									1, 1, 1, 1, 1,
	 									1, 1, 1, 1, 1,
	 									1, 1, 1, 1, 1,
	 									1, 1, 1, 1, 1);*/
	 	/*Mat mask = (Mat_<uchar>(7,7) <<1, 1, 1, 1, 1, 1, 1,
	 									 1, 1, 1, 1, 1, 1, 1,
	 									 1, 1, 1, 1, 1, 1, 1,
	 									 1, 1, 1, 1, 1, 1, 1,
	 									 1, 1, 1, 1, 1, 1, 1,
	 									 1, 1, 1, 1, 1, 1, 1,
	 									 1, 1, 1, 1, 1, 1, 1);*/




	 Mat img_dst, warp_rotate_dst;

	 // --------------------------------------------
	 // Carrega a imagem
	 // --------------------------------------------
	 //img_src = imread(URLfull_in+name_image_in, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);   // Read the file
	 img_src = imread(URLfull_in+name_image_in, CV_LOAD_IMAGE_GRAYSCALE);   // Read the file

	 // --------------------------------------------
	 // Inicializa uma matriz que será a imagem resultante da transformação Afim
	 // A imagem resultante terá a mesma dimensão e tipo
	 img_dst = Mat::zeros( img_src.rows, img_src.cols, img_src.type() );

	 namedWindow( "Original", CV_WINDOW_AUTOSIZE );

	 //set the callback function for any mouse event
	 setMouseCallback("Original", CallBackFunc, NULL);

	 imshow( "Original", img_src );
	 //imwrite(URLfull_out+name_image_out+"_orig"+extention, img_src);

	 /// Wait until user exits the program
	 waitKey(0);

	return 0;
  }
