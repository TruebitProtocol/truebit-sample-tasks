#include "../map_reduce.h"
#include <stdio.h>

int DATA_ARRAY_SIZE = 12;

MapReduceTuple mr_func(int size, char **data) {
   int i, j, idx, insertIdx, newSize;
   insertIdx = 0;
   newSize = 0; 
   char **reduceAryPtr = calloc(0, sizeof(char *));
   char *dPtr;
   for(i = 0; i < size; i++){
      dPtr = data[i];
      for(j = 0; j < DATA_ARRAY_SIZE; j++){
         idx = 31 - j;
         if (dPtr[idx] != '3') {
            continue;
         }
         newSize++;
         reduceAryPtr = realloc(reduceAryPtr, sizeof(char *) * newSize);
         reduceAryPtr[insertIdx] = (char *)malloc(33 * sizeof(char));
         strncpy(reduceAryPtr[insertIdx], data[i], 32);
         insertIdx = newSize;
         break;
      }
   }

   MapReduceTuple ret = malloc(sizeof(MapReduceStruct));
   ret->size = newSize;
   ret->data = reduceAryPtr;
   return ret;
}
