#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include "sort.h"

void createArray(unsigned int *rArray, char* infile, int n) {
    FILE *file_ptr;
    file_ptr = fopen("outfile", "rb");

    rec_t r;
    for(int i = 0; i < n; i++) {
        fread(&r, sizeof(rec_t), 1, file_ptr);
        rArray[i] = r.key;
    }

    fclose(file_ptr);
}

void print_data(unsigned int *rArray, int n) {
    for(int i = 0; i < 100; i++) {
        printf("%d ", rArray[i]);
    }

    printf(" DONE PRINTING \n");
}

void createOutputFile(unsigned int* start, char* outpath, int n){
    FILE *file_ptr = fopen((const char*)outpath, "wb");
    fwrite(start, sizeof(unsigned int), n, file_ptr);

    fclose(file_ptr);
}

int cmp (const void * a, const void * b) {
   return ( *(unsigned int *)a > *(unsigned int *)b );
}

int main(int argc, char* args[]) {

    if (argc != 3) {
        printf("Please also enter 2 arguments \n");
        printf("Use this as follows ./sort2 <input-file> <output-sorted-file-name> \n");
        printf("For example ./sort2 outfile output_file \n");

        return 1;
    }

    // Reading input and outputfile
    char* infile = args[1];
    char* outfile = args[2];

    printf("Input file: %s \n", infile);
    printf("Output file: %s \n", outfile);

    // Finding the size of the input file
    // We asked sir before and he said to make the file
    // for different n, like sir used n = 100, but this can allow
    // to work for multiple values of n
    struct stat st;
    stat(infile, &st);
    int n = st.st_size / 100;


    // creating the array of keys
    unsigned int rArray[n];
    createArray(rArray, infile, n);

    print_data(rArray, n);

    // using quick sort
    qsort(rArray, 100, sizeof(unsigned int), cmp);

    print_data(rArray, n);

    // // Now store in the output file
    createOutputFile(rArray, outfile, n);

    return 0;
}