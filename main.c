#include <stdio.h>
#include <dlfcn.h>

typedef int (*f_ptr)(int);
typedef f_ptr (*pm)();

f_ptr process_to_function(char* file_name) {

    void *handle;
    int (*func_mapreduce)(int);

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
    int (*func_map)(int);
    int (*func_reduce)(int);

    // TODO: Read Data for array of Binary32: <20B - Eth Address: 12B value>

    /* Map Functionality */
    func_map = process_to_function("./map/lib_nth_prime.so");
    if (!func_map) {
        return 1;
    }

    int mapValue = func_map(12); // TODO: Add the array of Binary32
    fprintf(stdout, "Map: %d\n", mapValue);
    dlclose(handle);

    /* Reduce Functionality */
    func_reduce = process_to_function("./reduce/lib_find_threes.so");
    if (!func_reduce) {
        return 1;
    }

    int reduceValue = func_reduce(mapValue);
    fprintf(stdout, "Reduce: %d\n", reduceValue);
    dlclose(handle);

    return 0;
}
