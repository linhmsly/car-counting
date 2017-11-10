/*
* foreground.c
*
*  Created on: 17/10/30
*      Author: LinhVQ
*/

#include "foreground.h"

/*FUNCTION**********************************************************************
 *
 * Function Name : Foreground
 * Description   :
 *END**************************************************************************/
PointSeqList Foreground(IplImage *frame_gray_now, IplImage *frame_bkg, IplImage *final,
                        int frame_count, double *averagethreshold)
{
    const int CONTOUR_MAX_AREA = 4900; /* Bo xung */
    //const int CONTOUR_MAX_AREA = 600; //orignal

    const double SPEED_LIMIT = 80.00;
    CvMemStorage *storage;
    CvSeq *cont;
    PointNode *Centroids;
    PointSeqList Head = NULL;
    double threshold_now;

    IplImage *pyr = cvCreateImage(cvSize((frame_gray_now->width & -2) / 2, (frame_gray_now->height & -2) / 2), 8, 1);
    //cvShowImage("debug 0", pyr);
    IplImage *temp = cvCreateImage(cvSize(frame_gray_now->width, frame_gray_now->height), 8, 1);
    temp->origin = 1;

    CvScalar *mean = (CvScalar *)malloc(sizeof(CvScalar));
    CvScalar *std_dev = (CvScalar *)malloc(sizeof(CvScalar));

    /* Get the difference of the current image and the background*/
    cvAbsDiff(frame_gray_now, frame_bkg, frame_gray_now);

    /* Get the standard deviation of the difference image*/
    cvAvgSdv(frame_gray_now, mean, std_dev, NULL);

    /* Accumulate the threshold to get the average threshold*/
    threshold_now = 2.3 * std_dev->val[0];
    //if (frame_count < 55) { //original
    if (frame_count < 25)
    {
        /* Initialize the average threshold*/
        if (*averagethreshold < threshold_now)
        {
            *averagethreshold = threshold_now;
        }
    }
    else if (threshold_now < *averagethreshold)
    {
        threshold_now = *averagethreshold;
    }
    else
    {
        /* Update the average threshold */
        *averagethreshold = ((frame_count - 1) * (*averagethreshold) + threshold_now) / frame_count;
    }

    /* Reduce the noise */
    cvErode(frame_gray_now, frame_gray_now, NULL, 1);
    cvDilate(frame_gray_now, frame_gray_now, NULL, 3);
    cvErode(frame_gray_now, frame_gray_now, NULL, 1);

    cvSmooth(frame_gray_now, temp, CV_GAUSSIAN, 5, 3, 0);
    cvThreshold(temp, temp, 0, 255, CV_THRESH_BINARY);
    //cvShowImage( "background", temp );

    cvPyrDown(temp, pyr, CV_GAUSSIAN_5x5);       // lay mau duoi
    cvDilate(pyr, pyr, 0, 1);
    cvPyrUp(pyr, temp, CV_GAUSSIAN_5x5);         // lay mau tren
    cvSmooth(temp, temp, CV_GAUSSIAN, 5, 3, 0);
    cvAvgSdv(temp, mean, std_dev, NULL);
    cvThreshold(temp, temp, mean->val[0], 255, CV_THRESH_BINARY);

    cvDilate(temp, temp, NULL, 1);
    cvErode(temp, temp, NULL, 1);

    cvShowImage("temp", temp);
    //cvShowImage("pyr", pyr);
    //************************************************************
    storage = cvCreateMemStorage(0);
    cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), storage);

    /* Find the external contours of the object */
    cvFindContours(temp, storage, &cont, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

    cvZero(frame_gray_now);

    for (; cont; cont = cont->h_next)
    {
        /* Number point must be more than or equal to 20 */
        if (cont->total < 20 )
        {
            continue;
        }
        CvRect r = ((CvContour*)cont)->rect;
         /* Remove the smaller areas */
        if (r.height * r.width > CONTOUR_MAX_AREA)
        {
            /* Ve duong vien hien tai*/
            cvDrawContours(frame_gray_now, cont, CV_RGB(255,0,0),CV_RGB(255,255,255), 0, CV_FILLED, 8, cvPoint(0,0));
            /* Lay trong tam doi tuong */
            CvMoments moments;
            cvMoments(cont, &moments, 0);
            CvPoint Center = cvPoint(cvRound(moments.m10 / moments.m00), cvRound(moments.m01 / moments.m00));
            //if virtual line
            //if (Center.x > 20 && Center.x < 570 && Center.y > 450 && Center.y < 600 or Center.x > 690 && Center.x < 1280 && Center.y > 450 && Center.y < 600)
            if (Center.x > 0 && Center.x <= WIDTH && Center.y > 210 && Center.y <= 420)
            {
                cvRectangleR(final, r, CV_RGB(255, 255, 0), 2);
                cvCircle(final, Center, 2, CV_RGB(255, 255, 0), -1, 4, 0);
                // make a list of the centroidste
                /*Tao danh sach cac trong tam cua xe */
                Centroids = (PointNode *)malloc(sizeof(PointNode));
                Centroids->Point = Center;
                Centroids->ID = 0;
                /* Phan loai xe */
                Centroids->contourArea = r.height * r.width;
                Centroids->next = Head;
                Head = Centroids;
                if (Head->next)
                {
                    Head->next->pre = Head;
                }
            }
        }
    }
    cvReleaseMemStorage(&storage);
    cvReleaseImage(&pyr);
    cvReleaseImage(&temp);
    delete[] mean;
    //free(mean);
    delete[] std_dev;
    //free(std_dev);
    return Head;
}
