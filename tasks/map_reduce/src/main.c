#include "map_reduce.h"

// HELPER FUNCTIONS
MapReduceTuple read_input_file() {
    int insertIdx = 0, lineSize = 0;
    char ch;
    FILE *fp;
    char buffer[TOTAL_DATA_LEN+1];
    char **dataAryPtr = (char **) calloc(0, sizeof(char *));

    fp = fopen("input.data", "r"); // read mode

    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    while(fgets(buffer, TOTAL_DATA_LEN+1, fp)) {
        lineSize++;
        dataAryPtr = (char **) realloc(dataAryPtr, sizeof(char *) * lineSize);
        dataAryPtr[insertIdx] = (char *)malloc((TOTAL_DATA_LEN+1) * sizeof(char));
        strncpy(dataAryPtr[insertIdx], buffer, TOTAL_DATA_LEN);
        insertIdx = lineSize;
    }

    MapReduceTuple data = malloc(sizeof(MapReduceStruct));
    data->size = lineSize;
    data->data = dataAryPtr;

    fclose(fp);

    return data;
}

void print_output(MapReduceTuple val) {
    FILE *fp;
    fp = fopen("output.data", "w");
    int i, j;
    char *dPtr;
    int size = val->size;

    if (fp != NULL) {
        for(i = 0; i < size; i++){
            dPtr = val->data[i];
            for(j = 0; j < TOTAL_DATA_LEN; j++) {
                fputc (dPtr[j], fp);
            }
            fputc ('\n', fp);
        }
        fclose(fp);
    }
}

/**
 * find_nth_prime: takes in a value in and finds the corresponding n^th prime
 * i.e. 1 => 2 (1st prime); 9 => 19 (9th prime)
 * @param rangeNum - [long long *] The value for the n^th prime number to find
 */
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

/**
 * map - This is the Map Function that transform the input data
 * * NOTE: Map Function...can be customized to achieve different goal
 * @param mapData - [MapReduceTuple] Tuple that contains the 'size' and 'data' (char **) of the data to map
 * @return - [MapReduceTuple] Tuple that contains the 'size' and 'data' (char **) of the data thats been mapped at new memory space
 */
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

/**
 * Main Program:
 * * This will reading in the input data and output the map-reduced data
**/
int main() {

    /* Read the input file called 'input.data' of line-seperated Binary32 strings */
    MapReduceTuple inputData = read_input_file();

    /* Map Call: Find the Nth Prime Number in the Value Range of the Binary32: data[20] - data[31] */
    MapReduceTuple mappedData = map(inputData);

    /* Reduce Call: Find if there is a '3' in the Value Range of the Binary32: mapped_data[20] - mapped_data[31] */
    MapReduceTuple reducedData = reduce(mappedData);

    /* Output Result of Map-Reduce to output file called 'output.data' */
    print_output(reducedData);

    /* Memory cleanup */
    free(inputData->data);
    free(inputData);
    free(mappedData->data);
    free(mappedData);
    free(reducedData->data);
    free(reducedData);

    exit(EXIT_SUCCESS);
}
