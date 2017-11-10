/*
* struct.h
*
*  Created on: 17/10/30
*      Author: LinhVQ
*/

#ifndef STRUCT_H_
#define STRUCT_H_

#include <stdio.h>
#include <opencv\cv.h>
#include <opencv2\highgui\highgui.hpp>
#include <ctype.h>

#define WIDTH       1280
#define HEIGHT      720


/*!
 * @brief
 */
typedef struct PointNode {
    CvPoint Point;
    int ID;
    int contourArea;
    int typeCar;
    PointNode *prev;
    PointNode *next;
} PointNode, *PointSeqList;

typedef struct KalmanPointNode {
    int Loss;
    CvPoint firstPoint;
    CvPoint Point_now;
    CvPoint Point_pre;
    CvKalman *Kalman;
    int ID;
    int contourArea;
    int typeCar;
    int firstFrame;
    int lastFrame;
    KalmanPointNode *prev;
    KalmanPointNode *next;
} KalmanPointNode, *filterList;

#endif /* STRUCT_H_ */
