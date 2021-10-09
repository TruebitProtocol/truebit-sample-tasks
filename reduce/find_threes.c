#include "../map_reduce.h"

MapReduceTuple mr_func(int size, char **data) {
   int i, j, idx, insertIdx, newSize;
   insertIdx = 0;
   newSize = 0; 
   char **reduceAryPtr = calloc(0, sizeof(char *));
   char *dPtr;
   for(i = 0; i < size; i++){
      dPtr = data[i];
      for(j = 0; j < DATA_ARRAY_LEN; j++){
         idx = 31 - j;
         if (dPtr[idx] != '3') {
            continue;
         }
         newSize++;
         reduceAryPtr = realloc(reduceAryPtr, sizeof(char *) * newSize);
         reduceAryPtr[insertIdx] = (char *)malloc((TOTAL_DATA_LEN+1) * sizeof(char));
         strncpy(reduceAryPtr[insertIdx], data[i], TOTAL_DATA_LEN);
         insertIdx = newSize;
         break;
      }
   }

   MapReduceTuple ret = malloc(sizeof(MapReduceStruct));
   ret->size = newSize;
   ret->data = reduceAryPtr;
   return ret;
}
