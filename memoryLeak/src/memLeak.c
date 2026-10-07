#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "utils.h"

int main(int argc, char **argv) {

  int size, numIterations;

  if(argc == 1) {
    //default
    size = 100000;
    numIterations = 10;
  } else if(argc == 3) {
    size = atoi(argv[1]);
    numIterations = atoi(argv[2]);
  } else {
    fprintf(stderr, "Usage: %s [size of arrays] [number of random lists to generate]\n", argv[0]);
    exit(1);
  }

  //initialize random number generator with current time:
  srand(time(NULL));

  printf("Generating %d arrays of size %d...\n", numIterations, size);

  for(int i=0; i<numIterations; i++) {
    //generate a random array and find the median:
    printf("Generating array %d...\n", (i+1));
    int * randomArray = generateRandomArray(size);
    int medianValue = findMedian(randomArray, size);
    printf("The median element of the randomly generated array is %d\n", medianValue);
    //the randomArray is no longer needed, so clean it up by freeing it
    free(randomArray);
  }

  return 0;
}
