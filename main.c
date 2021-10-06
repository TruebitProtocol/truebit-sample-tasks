#include <stdio.h>
#include <dlfcn.h>

typedef int (*f_ptr)(char[32]);
typedef f_ptr (*pm)();

struct MapReduceTuple {
    int size;
    char[][32] data
};

f_ptr process_to_function(char* file_name) {

    void *handle;
    MapReduceTuple (*func_mapreduce)(char[][32], int);

    handle = dlopen(file_name, RTLD_LAZY);
    if (!handle) {
        /* fail to load the library */
        fprintf(stderr, "Error: %s\n", dlerror());
        return NULL;
    }

    *(void**)(&func_mapreduce) = dlsym(handle, "main");
    if (!func_mapreduce) {
        /* no such symbol */
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return NULL;
    }

    return func_mapreduce;
}

int main() {

    /* My First TrueBit Task Program in C */
    void *handle;
    MapReduceTuple (*func_map)(char[][32], int);
    MapReduceTuple (*func_reduce)(char[][32], int);

    // TODO: Read Data for array of Binary32: <20B - Eth Address: 12B value>
    char data[][32] = ["27dc7AFF9355902358cD000000000001"];

    /* Map Functionality */
    func_map = process_to_function("./map/lib_nth_prime.so");
    if (!func_map) {
        return 1;
    }

    MapReduceTuple mapValue = func_map(data, 1); // TODO: Add the array of Binary32
    fprintf(stdout, "Map: %d\n", mapValue);
    dlclose(handle);

    /* Reduce Functionality */
    func_reduce = process_to_function("./reduce/lib_find_threes.so");
    if (!func_reduce) {
        return 1;
    }

    MapReduceTuple reduceValue = func_reduce(mapValue.data, mapValue.size);
    fprintf(stdout, "Reduce: %d\n", reduceValue);
    dlclose(handle);

    return 0;
}
