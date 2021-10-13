#include <stdio.h>
#include <math.h>
#include "map_reduce.h"

// HELPER FUNCTIONS
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

void print_output(MapReduceTuple val) {
    printf("Array Size: %d\n", val->size);
    for (int i = 0; i < val->size; ++i) {
        printf( "\tloc: %p - %.32s\n", (void*)val->data[i], val->data[i]);
    }
}

// NOTE: Map Function...can be customized to achieve different goal
MapReduceTuple map(MapReduceTuple mapData) {
    int i;
    long long dataVal, nth_prime;
    int size = mapData->size;
    char **mapAryPtr = malloc( size * sizeof(char*) );
    char dataValueBuff[DATA_ARRAY_LEN];

    // BEGIN: Customizable Map Code Below...
    for(i = 0; i < size; i++){
        // Map the data value to the Nth Prime represented by the data array
        memcpy(dataValueBuff, &(mapData->data)[i][ETH_ADDRESS_LEN + EXTRA_DATA_LEN], DATA_ARRAY_LEN);
        dataVal = atoll(&dataValueBuff[0]);
        nth_prime = find_nth_prime(&dataVal);
        mapAryPtr[i] = (char *)malloc((TOTAL_DATA_LEN+1) * sizeof(char));
        strncpy(mapAryPtr[i], mapData->data[i], TOTAL_DATA_LEN);
        sprintf(&mapAryPtr[i][ETH_ADDRESS_LEN + EXTRA_DATA_LEN], "%1.8lld", nth_prime);
    }
    // END: Customizable Map Code Above...

    MapReduceTuple ret = malloc(sizeof(MapReduceStruct));
    ret->size = size;
    ret->data = mapAryPtr;
    return ret;
}

// NOTE: Reduce Function...can be customized to achieve different goal
MapReduceTuple reduce(MapReduceTuple reduceData) {
    int i, j, idx, insertIdx, newSize, size;
    size = reduceData->size;
    insertIdx = 0;
    newSize = 0;
    char **reduceAryPtr = calloc(0, sizeof(char *));
    char *dPtr;

    // BEGIN: Customizable Reduce Code Below...
    for(i = 0; i < size; i++){
        dPtr = reduceData->data[i];
        for(j = 0; j < DATA_ARRAY_LEN; j++){
            idx = 31 - j;
            if (dPtr[idx] != '3') {
            continue;
            }
            newSize++;
            reduceAryPtr = realloc(reduceAryPtr, sizeof(char *) * newSize);
            reduceAryPtr[insertIdx] = (char *)malloc((TOTAL_DATA_LEN+1) * sizeof(char));
            strncpy(reduceAryPtr[insertIdx], reduceData->data[i], TOTAL_DATA_LEN);
            insertIdx = newSize;
            break;
        }
    }
    // END: Customizable Reduce Code Above...

    MapReduceTuple ret = malloc(sizeof(MapReduceStruct));
    ret->size = newSize;
    ret->data = reduceAryPtr;
    return ret;
}


int main() {

    /* My First TrueBit Task Program in C */

    // TODO: Read Data for array of Binary32: <20B - Eth Address: 12B value>
    char char_array1[32] = {'2','7','d','c','7','A','F','F','9','3','5','5','9','0','2','3','5','8','c','D','0','0','0','0','0','0','0','0','0','0','2','1'};
    char char_array2[32] = {'1','7','d','c','7','A','F','F','9','3','5','5','9','0','2','3','5','8','c','B','0','0','0','0','0','0','0','0','0','4','0','1'};
    char **dataAryPtr = malloc(sizeof(char *) * 2);
    dataAryPtr[0] = char_array1;
    dataAryPtr[1] = char_array2;
    MapReduceTuple start = malloc(sizeof(MapReduceStruct));
    start->size = 2;
    start->data = dataAryPtr;

    /* Map Call: Find the Nth Prime Number in the Value Range of the Binary32: data[20] - data[31] */
    MapReduceTuple mapValue = map(start);

    /* Reduce Call: Find if there is a '3' in the Value Range of the Binary32: mapped_data[20] - mapped_data[31] */
    MapReduceTuple reduceValue = reduce(mapValue);

    /* Output Result of Map-Reduce to output file */
    print_output(reduceValue);

    return 0;
}
