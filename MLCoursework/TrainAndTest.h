//
//  TrainAndTest.h
//  MLCoursework
//
//  Created by Jim Smith on 06/02/2017.
//  Copyright © 2017 Jim SmithJim Smith. All rights reserved.
//

#ifndef TrainAndTest_h
#define TrainAndTest_h

#include <stdio.h>
#include "MLCoursework.h"



int  train(float **argv, int numSamples, int numFeatures, char *labels) ;
char  test(float *newSample, int numFeatures);



#endif /* TrainAndTest_h */
