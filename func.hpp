#ifndef "func.hpp"
#define "func.hpp"

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>

void EnclosingCircle(IplImage* _image, IplImage *dem)
{
        assert(_image != 0);

        IplImage* bin = cvCreateImage( cvGetSize(_image), IPL_DEPTH_8U, 1);

        // конвертируем в градации серого
        cvConvertImage(_image, bin, CV_BGR2GRAY);
        // находим границы
        cvCanny(bin, bin, 100, 200);

        //cvNamedWindow( "bin", 1 );
        //cvShowImage("bin", bin);

        // хранилище памяти для контуров
        CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* contours = 0;

        // находим контуры
        int contoursCont = cvFindContours( bin, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

        assert(contours != 0);

        CvPoint2D32f center;
        float radius = 0;
        // обходим все контуры
        for( CvSeq* current = contours; current != NULL; current = current->h_next )
        {
                // находим параметры окружности
                cvMinEnclosingCircle(current, &center, &radius);
        
                // рисуем
                cvCircle(dem, cvPointFrom32f(center), radius, CV_RGB(0, 255, 0), 1, 1);
        }

        // освобождаем ресурсы
        cvReleaseMemStorage(&storage);
        cvReleaseImage(&bin);
}

int work(IplImage *src)
{
        IplImage *dst = 0;

        // получаем картинку
        IplImage *dem = cvLoadImage("caski.png", 1);

        assert( src != 0 );

        // покажем изображение
        //cvNamedWindow( "original", 1 );
        //cvShowImage( "original", src );

        dst = cvCloneImage(src);

        // показываем
        EnclosingCircle(dst, dem);

	cvNamedWindow( "circles", 1 );
	cvShowImage( "circles", dem);

        // ждём нажатия клавиши
        cvWaitKey(0);
        cvSaveImage("caskiX.png", dem, 0);
        // освобождаем ресурсы
        cvReleaseImage(&dem);
        cvReleaseImage(&dst);
        // удаляем окна
        cvDestroyAllWindows();
        return 0;
}

#endif
