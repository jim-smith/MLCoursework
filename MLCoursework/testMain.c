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

#include "TestDataset.h"

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
    int randInt, i=0;
    char randLabel;
    //these next few used to score and analyse what the student's code predicts
    char predictionLabels[NUM_TEST_SAMPLES];
    int numDifferentPredictions=0;
    int usedBefore=0;
    int predictionsCounters[NUM_TEST_SAMPLES];
    for(i=0;i<NUM_TEST_SAMPLES;i++)
        {
        predictionLabels[i] = '-';
        predictionsCounters[i]=0;
        }
    
    
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
    printf("expected to see one numerical argument to the main function\n");
    return(0);
  }
    sscanf(argv[1],"%d",&version);
    switch (version)
  {
        case 0: //simple 2/3: 1/3 split of iris data into training and test set matrices
        target = 97.0;

            for(sample=0; sample< EVALUATION_SET_SIZE;  sample++)
              {
                if(sample%3==0)
                  {
                    for (feature=0;feature < EVALUATIONFEATURES;  feature++)
                        testSet[testsamples][feature] = evaluation_data[sample][feature];
                    testLabels[testsamples] =evaluation_labels[sample];
                    testsamples++;
                  }
                else
                  {
                    for (feature=0;feature < EVALUATIONFEATURES;  feature++)
                        trainingSet[trainingsamples][feature] = evaluation_data[sample][feature];
                    trainingLabels[trainingsamples] = evaluation_labels[sample];
                    trainingsamples++;
                  }
              }
            break;
            
        case 1:
            //just give examples from classes 'p' and 'q' to the train function - duplicates used in train
        target = 50.0;
            for(sample=0; sample< EVALUATION_SET_SIZE;  sample++)
              {
                if(  ( sample < 25) || ( sample >=75 && sample <100))
                  {
                    for (feature=0;feature < EVALUATIONFEATURES;  feature++)
                        trainingSet[trainingsamples][feature] = evaluation_data[sample][feature];
                    trainingLabels[trainingsamples] = evaluation_labels[sample];
                    trainingsamples++;

                    //take a slightly distorted copy of each to make up the training set to the right number
                    for (feature=0;feature < EVALUATIONFEATURES;  feature++)
                        trainingSet[trainingsamples][feature] = evaluation_data[sample][feature]+ 0.1;
                    trainingLabels[trainingsamples] = evaluation_labels[sample];
                    trainingsamples++;

                  }
                   //use some of the remaining examples of classes 'p' and 'q' and half those of r to make up the test set
                else if ( (sample>=40 && sample <65) || ( 100<=sample && sample < 125)  )
                  {
                    for (feature=0;feature < EVALUATIONFEATURES;  feature++)
                        testSet[testsamples][feature] = evaluation_data[sample][feature];
                    testLabels[testsamples] =evaluation_labels[sample];
                    testsamples++;
                  }
               else
                 {;//don;t use the last 25 class 'r' samples
                 }
              }
            break;
      case 2: //2:1 split of test data with randomly assigned labels
        target = 33.3;
        srand(64645);
        for(sample=0; sample< EVALUATION_SET_SIZE;  sample++)
          {
            randInt = rand()%3;
            randLabel = (randInt==0)? 'p': ( (randInt==1)?'q':'r');
            if(sample%3==0)
              {
                for (feature=0;feature < EVALUATIONFEATURES;  feature++)
                    testSet[testsamples][feature] = evaluation_data[sample][feature];
                testLabels[testsamples] =randLabel;
                testsamples++;
              }
            else
              {
                for (feature=0;feature < EVALUATIONFEATURES;  feature++)
                    trainingSet[trainingsamples][feature] = evaluation_data[sample][feature];
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
            
              //see what the student's code predicts
            prediction = predictLabel( mySample, NUM_FEATURES);
              //see if it is right
            if (prediction == trainingLabels[sample])
                correct++;
            else
                wrong++;
              //see if they have made this prediction before
            usedBefore=0;
            for(  i=0;i< numDifferentPredictions && usedBefore==0;i++)
                {
                  if (prediction == predictionLabels[i])
                  {
                      predictionsCounters[i]++;
                      usedBefore = 1;
                  }
                }
              if(usedBefore==0)
              {
                  predictionLabels[numDifferentPredictions] = prediction;
                  predictionsCounters[numDifferentPredictions] = 1;
                  numDifferentPredictions++;
              }
          }
              
           accuracy = 100.0*(double)correct/(correct+wrong);
              printf("JIM_FEEDBACK: On  training set %d: your code got %d correct %d incorrect, accuracy = %f %%\n", version,correct,wrong,accuracy );
              printf("JIM_FEEDBACK: \t\tThe frequency with which it predicted different class labels was: \t");
              for(int  i=0;i< numDifferentPredictions ;i++)
              {
                  printf("  %c  %d\t",predictionLabels[i],predictionsCounters[i]);
              }
          printf("\n");
          
        //now the results that matter - from the test set
        correct=wrong= numDifferentPredictions =0;
        for( i=0;i<NUM_TEST_SAMPLES;i++)
        {
            predictionLabels[i] = '-';
            predictionsCounters[i]=0;
        }
        
        for(sample=0; sample < testsamples; sample++)
          {
            //copy into temp array to pass into function
            for(feature = 0 ; feature < NUM_FEATURES; feature++)
                mySample[feature] = testSet[sample][feature];
            
            prediction = predictLabel(mySample, NUM_FEATURES);
              //is prediction invalid?
            if(prediction != 'p' && prediction != 'q' && prediction != 'r')
                unachievable++;
            
            // test set 1 should never predict a 'r'
            if (version==1 && prediction=='r') {
                unachievable++;
            }
            
              //is predcition correct?
            if (prediction == testLabels[sample])
                correct++;
            else
              {
                //printf("actual = %c predicted = %c\n", testLabels[sample], prediction );
                wrong++;
              }
              
              //has the code made this prediction before?
              usedBefore=0;
              for(  i=0;i< numDifferentPredictions && usedBefore==0;i++)
              {
                  if (prediction == predictionLabels[i])
                  {
                      predictionsCounters[i]++;
                      usedBefore = 1;
                  }
              }
              if(usedBefore==0)
              {
                  predictionLabels[numDifferentPredictions] = prediction;
                  predictionsCounters[numDifferentPredictions] = 1;
                  numDifferentPredictions++;
              }
              
          }
        
        accuracy = 100.0*(double)correct/(correct+wrong);
        
        if( unachievable>0)
          {
            score = 0.0;
              printf("JIM_FEEDBACK:  HINT: your code  has predicted a label that did not exist in the training set, so it should have known was a possibility.\n");
          }
        else
          {
            score = (accuracy>target) ? accuracy-target:target-accuracy;
            if ( score < 2.5 )
                score = 100.0;
            else if( score < 5.0)
                score = 90.0;
            else if (score < 10.0) 
                score = 80.0;
        else score = 50.0;
          }
    
              printf("JIM_FEEDBACK: On  test set %d: your code got %d correct %d incorrect, accuracy = %f %%\n", version,correct,wrong,accuracy );
              printf("JIM_FEEDBACK: \t\tThe frequency with which it predicted different class labels was: \t");
              for(int  i=0;i< numDifferentPredictions ;i++)
              {
                  printf(" %c  %d\t",predictionLabels[i],predictionsCounters[i]);
              }
          printf("\n");
        printf("Score on dataset %d = %f\nYour algorithm got %d prediction%c right, and %d wrong with an accuracy of %f\n",version,score,correct,(correct ==1)?' ':'s', wrong, accuracy);
        printf("The target for this dataset was %f\n",target);
        if(unachievable>0)
            printf("However in at least one case it predicted a class label that was not present in the training set.\nSo you score no marks on this dataset\n");
        
      }
    
    
    return returnval;
    
}
