/*
* foreground.h
*
*  Created on: 17/10/30
*      Author: LinhVQ
*/

#ifndef FOREGROUND_H_
#define FOREGROUND_H_

#include <stdio.h>
#include <opencv\cv.h>
#include <opencv2\highgui\highgui.hpp>
#include <ctype.h>
#include "struct.h"
/*!
 * @brief extract the centroid of the moved object of the image
 *
 * @param[in]  frame_gray_now
 * @param[out]  frame_bkg
 * @param[in]  final
 * @param[in]  frame_count
 * @param[in]  averagethreshold
 * @return  IplImage
 */
PointSeqList Foreground(IplImage *frame_gray_now, IplImage *frame_bkg, IplImage *final,
                        int frame_count, double *averagethreshold);

#endif   /* FOREGROUND_H_ */
