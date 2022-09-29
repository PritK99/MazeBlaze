#ifndef LINE_FOLLOWING_H 
#define LINE_FOLLOWING_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

float bound(float val, float min, float max) ;//To bound a certain value in range MAX to MIN 
void calculate_error() ;
void calculate_correction() ;
void line_follow_task(void* arg) ;
void circular_defn(int change_in_dir) ;

#endif
