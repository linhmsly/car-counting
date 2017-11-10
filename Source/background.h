/*
* background.h
*
*  Created on: 17/10/30
*      Author: LinhVQ
*/

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include <stdio.h>
#include <opencv\cv.h>
#include <opencv2\highgui\highgui.hpp>
#include <ctype.h>

/*!
 * @brief make background image
 *
 * @param[in]  frame_gray_now
 * @param[out]  frame_bkg
 * @param[in]  frame_gray_pass
 * @param[in]  frame_count
 * @param[in]  StopReflesh
 * @return  IplImage
 */
IplImage *Background(IplImage *frame_gray_now, IplImage *frame_bkg,
                     IplImage *frame_gray_pass, int frame_count,
                     int StopReflesh);

#endif
