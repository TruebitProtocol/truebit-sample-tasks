#include "../map_reduce.h"

struct MapReduceTuple mr_func(int size, char **data) {
   int i;
   char *dPtr;
   char **mapAryPtr = malloc(sizeof(char *) * size);
   for(i = 0; i < size; i++){
      dPtr = data[i];
      // TODO: Map Process Here!
      mapAryPtr[i] = dPtr;
   }

   // REMOVE: For Testing
   mapAryPtr[0][31] = '3';
   mapAryPtr[1][31] = '5';
   /// END REMOVE

   struct MapReduceTuple ret = { .size = 2, .data = mapAryPtr };
   return ret;
}
