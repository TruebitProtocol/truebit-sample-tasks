#include "../map_reduce.h"
#include <stdio.h>
#include <math.h>

long long find_nth_prime(long long *rangeNum) {
   long long rangenumber, c = 0, num = 2, i, nth_prime = 0;
   while (c != *rangeNum)  {
    int count = 0;
    for (i = 2; i <= sqrt (num); i++) {
      if (num % i == 0) {
        count++;
        break;
      }
    }
    if (count == 0) {
      c++;
      nth_prime = num;
    }
    num = num + 1;
  }

  return nth_prime;
}

MapReduceTuple mr_func(int size, char **data) {
   int i;
   long long dataVal, nth_prime;
   char **mapAryPtr = malloc( size * sizeof(char*) );
   char dataValueBuff[DATA_ARRAY_LEN];
   for(i = 0; i < size; i++){
      // Map the data value to the Nth Prime represented by the data array
      memcpy(dataValueBuff, &data[i][ETH_ADDRESS_LEN + EXTRA_DATA_LEN], DATA_ARRAY_LEN);
      dataVal = atoll(&dataValueBuff[0]);
      nth_prime = find_nth_prime(&dataVal);
      mapAryPtr[i] = (char *)malloc((TOTAL_DATA_LEN+1) * sizeof(char));
      strncpy(mapAryPtr[i], data[i], TOTAL_DATA_LEN);
      sprintf(&mapAryPtr[i][ETH_ADDRESS_LEN + EXTRA_DATA_LEN], "%1.8lld", nth_prime);
   }

   MapReduceTuple ret = malloc(sizeof(MapReduceStruct));
   ret->size = size;
   ret->data = mapAryPtr;
   return ret;
}
