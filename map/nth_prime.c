#include "map.h"

struct MapReduceTuple mr_func(char data[][32], int size) {
   // int i;
   // char[32] *dPtr;
   // char[32][] mappedData = [];
   // for(i = 0; i < size; i++){
   //    dPtr = &data[i];
   //    *dPtr[31] = 3;
   //    // mappedData[i] = &dPtr;
   // }

   char char_array[32] = {'2','7','d','c','7','A','F','F','9','3','5','5','9','0','2','3','5','8','c','D','0','0','0','0','0','0','0','0','0','0','0','3'};

   struct MapReduceTuple ret; //= { .size = 1, .data = {char_array} };
   return ret;
}
