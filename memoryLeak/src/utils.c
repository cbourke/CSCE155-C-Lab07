#include <stdlib.h>

#include "utils.h"

int cmpInt(const void *a, const void *b) {
  int x = *((const int *)a);
  int y = *((const int *)b);
  if(x < y) {
    return -1;
  } else if(x > y) {
    return 1;
  } else {
    return 0;
  }
}

int findMedian(const int *array, int size) {
  //make a copy since the passed array is read-only
  int *copy = deepCopy(array, size);
  //sort the copy
  qsort(copy, size, sizeof(int), cmpInt);
  //find the median
  int medianValue = copy[size/2];
  return medianValue;
}

int * generateRandomArray(int size) {
  int * randomArr = malloc(sizeof(int) * size);
  for(int i=0; i<size; i++) {
    randomArr[i] = rand();
  }
  return randomArr;
}

int * deepCopy(const int *array, int size) {
  int *copy = malloc(sizeof(int) * size);
  for(int i=0; i<size; i++) {
    copy[i] = array[i];
  }
  return copy;
}
