#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "func.hpp"

IplImage* image = 0;
	
//для хранения каналов HSV
IplImage* hsv = 0;
IplImage* h_plane = 0;
IplImage* s_plane = 0;
IplImage* v_plane = 0;
//для хранения каналов HSV после преобразования
IplImage* h_range = 0;
IplImage* s_range = 0;
IplImage* v_range = 0;
//для хранения суммарной картинки
IplImage* hsv_and = 0;

int Hmin = 0;
int Hmax = 256;

int Smin = 0;
int Smax = 256;

int Vmin = 0;
int Vmax = 256;

int HSVmax = 256;

int main(int argc, char* argv[])
{
	//имя картинки задаётся первым параметром
	if ( argc < 2 ) return 1;
	//получаем картинку
	image = cvLoadImage( argv[1], 1 );

	printf("[i] image: %s\n", argv[1]);
	assert( image != 0 );

	//создаём картинки
	hsv = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 3 );
	h_plane = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
	s_plane = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
	v_plane = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
	h_range = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
	s_range = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
	v_range = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
	hsv_and = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
	//конвертируем в HSV 
	cvCvtColor( image, hsv, CV_BGR2HSV ); 
	//разбиваем на отельные каналы
	cvSplit( hsv, h_plane, s_plane, v_plane, 0 );
	//окна для отображения картинки
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("hsv and", CV_WINDOW_AUTOSIZE);
	//ограничиваем светоцветовые параметры
	Hmin = 23;
	Hmax = 36;
	Smin = 53;
	Smax = 256;
	Vmin = 238;
	Vmax = 256;
	cvInRangeS(h_plane, cvScalar(Hmin), cvScalar(Hmax), h_range);
	cvInRangeS(s_plane, cvScalar(Smin), cvScalar(Smax), s_range);
	cvInRangeS(v_plane, cvScalar(Vmin), cvScalar(Vmax), v_range);
	while(1)
	{
		//показываем картинку
		cvShowImage("original", image);

		//складываем 
		cvAnd(h_range, s_range, hsv_and);
		cvAnd(hsv_and, v_range, hsv_and);

		
		
		cvSmooth(hsv_and, hsv_and, CV_MEDIAN, 3, 3);
		cvShowImage( "hsv and", hsv_and );
		char c = cvWaitKey(33);
		//27 символ это "esc"
		if (c == 27) break;
	}
	cvSmooth(hsv_and, hsv_and, CV_MEDIAN, 3, 3);
	int x, y;
	for(y = 0; y < hsv_and->height; y += 2)
	{
		uchar* ptr = (uchar*) (hsv_and->imageData + y * hsv_and->widthStep);
		for(x = 0; x < hsv_and->width; x += 2)
		{
			if(ptr[x] > 127)
				ptr[x] = 255;
			else
				ptr[x] = 0;
			if(ptr[x] != 0 && ptr[x] != 255)
				printf("%d ", ptr[x]);
		}
	}
	cvSaveImage("caski.png", hsv_and, 0);
	work(hsv_and);
	//освобождаем ресурсы
	cvReleaseImage(&image);
	cvReleaseImage(&hsv);
	cvReleaseImage(&h_plane);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
	cvReleaseImage(&h_range);
	cvReleaseImage(&s_range);
	cvReleaseImage(&v_range);
	cvReleaseImage(&hsv_and);
	//удаляем окна
	cvDestroyAllWindows();
	return 0;
}
