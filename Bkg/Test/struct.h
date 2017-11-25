#ifndef STRUCT_H_
#define STRUCT_H_

#include <stdio.h>
#include <opencv\cv.h>
#include <opencv2\highgui\highgui.hpp>
#include <ctype.h>

#define WIDTH       1280



typedef struct PointSeq {
    CvPoint Point;
    int ID;
    int contourArea;
    int jenis;  // type car
    PointSeq *pre;
    PointSeq *next;
} PointSeq, *PointSeqList;

typedef struct KalmanPoint {
    int Loss;
    CvPoint firstPoint;
    CvPoint Point_now;
    CvPoint Point_pre;
    CvKalman *Kalman;
    int ID;
    int contourArea;
    int jenis;
    int firstFrame;
    int lastFrame;
    KalmanPoint *pre;
    KalmanPoint *next;
} KalmanPoint, *filterList;

//extern int counterMob;
//extern int counterTruk;
//extern int counterMob2;
//extern int counterTruk2;

#endif /* STRUCT_H_ */
