//
//  exampleMain.c
//  MLCoursework
//
//  Created by Jim Smith on 23/02/2017.
//  Copyright © 2017 Jim SmithJim Smith. All rights reserved.
//

#include <stdio.h>

#include "MLCoursework.h"
#include "TrainAndTest.h"

#include "IrisData.h"

#include <stdlib.h>



//FIXME you'll need to change the name of this function to main()
int main(int argc, const char * argv[])
{
    
    int sample, trainingsamples=0,testsamples=0,feature=0;
    int ok=1;
    int correct=0, wrong=0, unachievable=0;
    char prediction;
    int returnval=0;
    double   mySample[NUM_FEATURES];
    int version;
    double accuracy, score, target;
    int randInt;
    char randLabel;
    
    
    //allocate space for data storage
    double **trainingSet = calloc(NUM_TRAINING_SAMPLES , sizeof(double *));
    for(sample=0;sample < NUM_TRAINING_SAMPLES; sample++)
        trainingSet[sample] = calloc(NUM_FEATURES, sizeof(double));
    
                                  
    char *trainingLabels = calloc(NUM_TRAINING_SAMPLES , sizeof(char));
    
    double **testSet = calloc(NUM_TEST_SAMPLES , sizeof(double *));
    for(sample=0;sample < NUM_TEST_SAMPLES; sample++)
        testSet[sample] = calloc(NUM_FEATURES, sizeof(double));

    char * testLabels = calloc(NUM_TEST_SAMPLES, sizeof(char));
    
if(argc!=2)
  {
    printf("expected to see one numerical argument ot the main function\n");
    return(0);
  }
    sscanf(argv[1],"%d",&version);
    switch (version)
  {
        case 0: //simple 2/3: 1/3 split of iris data into training and test set matrices
        target = 97.0;
            for(sample=0; sample< IRIS_SET_SIZE;  sample++)
              {
                if(sample%3==0)
                  {
                    for (feature=0;feature < IRISFEATURES;  feature++)
                        testSet[testsamples][feature] = iris_data[sample][feature];
                    testLabels[testsamples] =iris_labels[sample];
                    testsamples++;
                  }
                else
                  {
                    for (feature=0;feature < IRISFEATURES;  feature++)
                        trainingSet[trainingsamples][feature] = iris_data[sample][feature];
                    trainingLabels[trainingsamples] = iris_labels[sample];
                    trainingsamples++;
                  }
              }
            break;
            
        case 1:
            //just give examples from classes 'a' and 'b' to the train function - duplicates used in train
        target = 50.0;
            for(sample=0; sample< IRIS_SET_SIZE;  sample++)
              {
                if(  ( sample < 25) || ( sample >=75 && sample <100))
                  {
                    for (feature=0;feature < IRISFEATURES;  feature++)
                        trainingSet[trainingsamples][feature] = iris_data[sample][feature];
                    trainingLabels[trainingsamples] = iris_labels[sample];
                    trainingsamples++;

                    //take a slightly distorted copy of each to make up the training set to the right number
                    for (feature=0;feature < IRISFEATURES;  feature++)
                        trainingSet[trainingsamples][feature] = iris_data[sample][feature]+ 0.1;
                    trainingLabels[trainingsamples] = iris_labels[sample];
                    trainingsamples++;

                  }
                   //use some of the remaining examples of classes 'a' and 'b' and half those of c to make up the test set
                else if ( (sample>=40 && sample <65) || ( 100<=sample && sample < 125)  )
                  {
                    for (feature=0;feature < IRISFEATURES;  feature++)
                        testSet[testsamples][feature] = iris_data[sample][feature];
                    testLabels[testsamples] =iris_labels[sample];
                    testsamples++;
                  }
               else
                 {;//don;t use the last 25 class 'c' samples
                 }
              }
            break;
      case 2: //2:1 split of iris data with randomly assigned labels
        target = 33.3;
        srand(64645);
        for(sample=0; sample< IRIS_SET_SIZE;  sample++)
          {
            randInt = rand()%3;
            randLabel = (randInt==0)? 'a': ( (randInt==1)?'b':'c');
            if(sample%3==0)
              {
                for (feature=0;feature < IRISFEATURES;  feature++)
                    testSet[testsamples][feature] = iris_data[sample][feature];
                testLabels[testsamples] =randLabel;
                testsamples++;
              }
            else
              {
                for (feature=0;feature < IRISFEATURES;  feature++)
                    trainingSet[trainingsamples][feature] = iris_data[sample][feature];
                trainingLabels[trainingsamples] = randLabel;
                trainingsamples++;
              }
          }
        break;

        
        default:
        printf("I haven't decided what to use for the fourth data set yet\n");
        exit(0);
            break;
  }
    
    
    //call the train function
    ok= train(  &trainingSet[0], trainingLabels ,trainingsamples, NUM_FEATURES);
    if( ok!= 1)
      {
        printf("there was a problem running the train() function\n");
        returnval=0;
      }
    
    else
      {
        correct = wrong = 0;
        //print the results from the training set for information
          for(sample=0; sample < trainingsamples; sample++)
          {
            
            // Make a copy of the sample ot be classified because it is faster to access a local copy
            //and pass a pointer to it, and also  I don't want to overwrite it by accident
            for(feature = 0 ; feature < NUM_FEATURES; feature++)
                mySample[feature] = trainingSet[sample][feature];
            
            prediction = predictLabel( mySample, NUM_FEATURES);
            if (prediction == trainingLabels[sample])
                correct++;
            else
                wrong++;
          }
           accuracy = 100.0*(double)correct/(correct+wrong);
        //printf("On  training set %d: %d correct %d incorrect, accuracy = %f %%\n", version,correct,wrong,accuracy );
        

        //now the results that matter - from the test set
        correct=wrong=0;
        for(sample=0; sample < testsamples; sample++)
          {
            //copy into temp array ot pass into fiunction
            for(feature = 0 ; feature < NUM_FEATURES; feature++)
                mySample[feature] = testSet[sample][feature];
            
            prediction = predictLabel(mySample, NUM_FEATURES);
            
            //if(sample==0)
            //    prediction = 't';
            
            if(prediction != 'a' && prediction != 'b' && prediction != 'c')
                unachievable++;
            
            // test set 1 should nrever predict a 'c'
            if (version==1 && prediction=='c') {
                unachievable++;
            }
            
            if (prediction == testLabels[sample])
                correct++;
            else
              {
                //printf("actual = %c predicted = %c\n", testLabels[sample], prediction );
                wrong++;
              }
          }
        
        accuracy = 100.0*(double)correct/(correct+wrong);
        
        if( unachievable>0)
          {
            score = 0.0;
          }
        else
          {
            score = (accuracy>target) ? accuracy-target:target-accuracy;
            if ( score < 2.5 )
                score = 100.0;
            else if( score < 5.0)
                score = 90.0;
            else if (version <2 && score < 10.0) //shouldnt be ale ot get more than +/-5 of 1/3 for random guessing)
                score = 80.0;
        else score = 50.0;
          }
    
            
        printf("Score on dataset %d = %f\nYour algorithm got %d prediction%c right, and %d wrong with an accuracy of %f\n",version,score,correct,(correct ==1)?' ':'s', wrong, accuracy);
        printf("The target for this dataset was %f\n",target);
        if(unachievable>0)
            printf("However in at least one case it predicted a class label that was not present in the training set.\nSo you score no marks on this dataset\n");
        
      }
    
    
    return returnval;
    
}