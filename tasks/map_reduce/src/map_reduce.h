#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

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
