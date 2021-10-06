#pragma once

#include <stdlib.h>
#include <string.h>

typedef struct MapReduceTuple {
    int size;
    char data[][32];
} MapReduceTuple;

inline MapReduceTuple* alloc_data(int size, char data[][32]) {
    MapReduceTuple * ret = calloc(sizeof(MapReduceTuple) + (sizeof(char[32]) * size), 1);
    if(ret) memcpy( ret,
                    &(MapReduceTuple const){ .size = size, .data = data },
                    sizeof(MapReduceTuple));
    return ret;
}

struct MapReduceTuple mr_func(char data[][32], int size);
