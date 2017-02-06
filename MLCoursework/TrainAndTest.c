//
//  TrainAndTest.c
//  MLCoursework
//
//  This is a fairly inefficient implentation that does not use any dynamic memory allocation
// because that wouldnot be safe on the DEWIS marking system
//
//  Created by Jim Smith on 06/02/2017.
//  Copyright © 2017 Jim SmithJim Smith. All rights reserved.
//

#include "TrainAndTest.h"


//declare this array as static but make it available to any function in this file
//in case we want to store the training examples  and use them later
static    int myModel[TRAINING_SET_SIZE][ MAX_NUM_FEATURES];
//even if each item in the training set is from a diffferent class we know how many there are
static char myModelLabels[TRAINING_SET_SIZE];

int  train(float **argv, int numSamples, int numFeatures, char *labels)
{
 
                

    //this is a silly trivial train()_ function
    fprintf(stdout,"no ML algorithm implemented yet\n");
    
    return 1;
}

char  test(float *newSample, int numFeatures)
{
    
    //this is a silly trivial test function
    
    return 'a';
    
    
    
}