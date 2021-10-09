#include "../map_reduce.h"

MapReduceTuple mr_func(int size, char **data) {
   int i;
   char **mapAryPtr = malloc( size * sizeof(char*) ); 
   for(i = 0; i < size; i++){
      // TODO: Map Process Here!
      mapAryPtr[i] = (char *)malloc(33 * sizeof(char));
      strncpy(mapAryPtr[i], data[i], 32);
   }

   // REMOVE: For Testing
   mapAryPtr[0][31] = '3';
   mapAryPtr[1][31] = '5';
   // END REMOVE

   MapReduceTuple ret = malloc(sizeof(MapReduceStruct));
   ret->size = size;
   ret->data = mapAryPtr;
   return ret;
}
