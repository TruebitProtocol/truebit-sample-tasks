#include <stdio.h>
#include <dlfcn.h>
#include "map_reduce.h"

typedef MapReduceTuple (*f_ptr)(int, char**);
typedef f_ptr (*pm)();

f_ptr process_to_function(char* file_name) {

    void *handle;
    MapReduceTuple (*func_mapreduce)(int, char**);

    handle = dlopen(file_name, RTLD_LAZY);
    if (!handle) {
        /* fail to load the library */
        fprintf(stderr, "Error: %s\n", dlerror());
        return NULL;
    }

    *(void**)(&func_mapreduce) = dlsym(handle, "mr_func");
    if (!func_mapreduce) {
        /* no such symbol */
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return NULL;
    }

    return func_mapreduce;
}

void print_output(MapReduceTuple val) {
    printf("Array Size: %d\n", val->size);
    for (int i = 0; i < val->size; ++i) {
        printf( "\tloc: %p - %.32s\n", (void*)val->data[i], val->data[i]);
    }
}

int main() {

    /* My First TrueBit Task Program in C */
    void *handle;
    MapReduceTuple (*func_map)(int, char**);
    MapReduceTuple (*func_reduce)(int, char**);

    // TODO: Read Data for array of Binary32: <20B - Eth Address: 12B value>
    char char_array1[32] = {'2','7','d','c','7','A','F','F','9','3','5','5','9','0','2','3','5','8','c','D','0','0','0','0','0','0','0','0','0','0','2','1'};
    char char_array2[32] = {'1','7','d','c','7','A','F','F','9','3','5','5','9','0','2','3','5','8','c','B','0','0','0','0','0','0','0','0','0','4','0','1'};
    char **dataAryPtr = malloc(sizeof(char *) * 2);
    dataAryPtr[0] = char_array1;
    dataAryPtr[1] = char_array2;
    MapReduceTuple start = malloc(sizeof(MapReduceStruct));
    start->size = 2;
    start->data = dataAryPtr;
    printf("Data:\n");
    print_output(start);


    /* Map Functionality */
    func_map = process_to_function("lib_nth_prime.so");
    if (!func_map) {
        return 1;
    }

    MapReduceTuple mapValue = func_map(2, dataAryPtr);
    printf("Map:\n");
    print_output(mapValue);
    dlclose(handle);

    /* Reduce Functionality */
    func_reduce = process_to_function("lib_find_threes.so");
    if (!func_reduce) {
        return 1;
    }

    MapReduceTuple reduceValue = func_reduce(mapValue->size, mapValue->data);
    printf("Reduce:\n");
    print_output(reduceValue);
    dlclose(handle);

    return 0;
}
