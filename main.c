#include <stdio.h>
#include <dlfcn.h>

int main() {
    /* My First TrueBit Task Program in C */
    void *handle;
    int (*func_map)();
    int (*func_reduce)(int);

    /* Map Functionality */
    handle = dlopen("./libmap.so", RTLD_LAZY);
    if (!handle) {
        /* fail to load the library */
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    *(void**)(&func_map) = dlsym(handle, "main");
    if (!func_map) {
        /* no such symbol */
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    int mapValue = func_map();
    fprintf(stdout, "Map: %d\n", mapValue);
    dlclose(handle);

    /* Reduce Functionality */
    handle = dlopen("./libreduce.so", RTLD_LAZY);
    if (!handle) {
        /* fail to load the library */
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    *(void**)(&func_reduce) = dlsym(handle, "main");
    if (!func_reduce) {
        /* no such symbol */
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    int reduceValue = func_reduce(mapValue);
    fprintf(stdout, "Reduce: %d\n", reduceValue);
    dlclose(handle);

    return 0;
}
