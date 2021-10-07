#pragma once

#include <stdlib.h>
#include <string.h>

typedef struct MapReduceTuple {
    int size;
    char **data;
} MapReduceTuple;

// inline MapReduceTuple* alloc_data(int size, char (*data)[][32]) {
//     MapReduceTuple * ret = malloc(sizeof(MapReduceTuple) + (sizeof(char[32]) * size));
//     if(ret) memcpy( ret,
//                     &(MapReduceTuple const){ .size = size, .data = data },
//                     sizeof(MapReduceTuple));
//     return ret;
// }

struct MapReduceTuple mr_func(int size, char **data);
