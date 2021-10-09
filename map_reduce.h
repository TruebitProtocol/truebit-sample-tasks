#pragma once

#include <stdlib.h>
#include <string.h>

// Constants
int TOTAL_DATA_LEN = 32;
int ETH_ADDRESS_LEN = 20;
int EXTRA_DATA_LEN = 4;
int DATA_ARRAY_LEN = 8;

typedef struct {
    int size;
    char **data;
} MapReduceStruct;

typedef MapReduceStruct *MapReduceTuple;

// inline MapReduceTuple* alloc_data(int size, char (*data)[][32]) {
//     MapReduceTuple * ret = malloc(sizeof(MapReduceTuple) + (sizeof(char[32]) * size));
//     if(ret) memcpy( ret,
//                     &(MapReduceTuple const){ .size = size, .data = data },
//                     sizeof(MapReduceTuple));
//     return ret;
// }

MapReduceTuple mr_func(int size, char **data);
