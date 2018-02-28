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
static    double myModel[NUM_TRAINING_SAMPLES][ NUM_FEATURES];
//even if each item in the training set is from a diffferent class we know how many there are
static char myModelLabels[NUM_TRAINING_SAMPLES];

static int trainingSetSize=0;

int  train( double **trainingSamples, char *trainingLabels, int numSamples, int numFeatures)
{
    int returnval = 1;
    int sample, feature;
    
  
    
    //clean the model because C leaves whatever is in the memory
    for (sample=0; sample < NUM_TRAINING_SAMPLES; sample++) {
        for (feature=0; feature<NUM_FEATURES; feature++) {
            myModel[sample][feature] = 0.0;
        }
    }
    
    //sanity checking
    if ( numFeatures > NUM_FEATURES || numSamples > NUM_TRAINING_SAMPLES) {
        fprintf(stdout,"error: called train with data set larger than spaced allocated to store it");
        returnval=0;
    }
    

    
    //make a simple copy of the data we are being passed but don't do anything with it
     
     if(returnval==1) {
        //store the labels and the feature values
        trainingSetSize = numSamples;
        int index,feature;
        for (index=0; index < numSamples; index++) {
            myModelLabels[index] = trainingLabels[index];
            for (feature=0; feature < numFeatures; feature++) {
                myModel[index][feature] = trainingSamples[index][feature];
            }
        }
    }//end else
    
    
    return returnval;
}

char  predictLabel(double *sample, int numFeatures)
{
    
       //this is a silly trivial test function
       // obviously you need to replace this with something that uses the model you byilt in your train() function
        char prediction = 'c';
    //    return prediction;
  
    float BIGNUM = 9999e45;
    float  thisdist, diff, nearestDistSoFar = BIGNUM;
    int trainingSample=0, feature=0,closest = 0;
    
    //loop over every stored exemplar in the model
    for (trainingSample=0; trainingSample < trainingSetSize; trainingSample++) {
        //set distance to zero
        thisdist=0.0f;
        //loop over every feature (or until current min distacne exceeded
        for(feature = 0; feature < numFeatures && thisdist <nearestDistSoFar;feature++)
          {
            //adding the squared difference between the newsample and this training instance from our model
            diff = sample[feature] - myModel[trainingSample][feature];
            thisdist += diff* diff;
            
          }
        

        //if this is a  new closest save its index so we can refer ot it later
        if (thisdist < nearestDistSoFar) {
            closest = trainingSample;
            nearestDistSoFar = thisdist;
        }//endif
        
    }//end loop over exemplars in the models
     //this is 1-Nearet neighour so just get the label of the closest
    prediction = myModelLabels[closest];
   
   
 
    return prediction;
    
}


