#include "../map_reduce.h"

struct MapReduceTuple mr_func(int size, char **data) {
   int i, insertIdx;
   insertIdx = 0; 
   char **reduceAryPtr = malloc(sizeof(char *) * size);
   char *dPtr;
   for(i = 0; i < size; i++){
      dPtr = data[i];
      if (dPtr[31] == 3) {
         reduceAryPtr[insertIdx] = dPtr;
         insertIdx++;
      }
   }

   // reduce the side of the allocation
   int newSize = insertIdx+1;
   if (newSize < size ) {
      reduceAryPtr = realloc(reduceAryPtr, sizeof(char *) * newSize);
   }

   struct MapReduceTuple ret = { .size = newSize, .data = reduceAryPtr };
   return ret;
}
