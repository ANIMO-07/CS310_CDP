#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include "sort.h"


void print_keys_and_records_from_array(rec_t* start, int n){
    printf("\n\nPRINTING THE ARRAY READ FROM THE FILE AFTER SORTING...\n\n");
    
    for(int j = 0;j<n;j++){
        printf("%u\t:", start[j].key);
        for(int i = 0;i<NUMRECS;i++)
            printf("%u ", start[j].record[i]);
        printf("\n");
    }
}

void write_to_file(rec_t* start, char* outpath, int n){
    FILE *fptr = fopen((const char*)outpath, "wb");
    fwrite(start, sizeof(rec_t),n, fptr );
    fclose(fptr);
    return;
}

void print_bin_file_content(char* outpath, int n){
    printf("\n\nREAD FROM THE FILE LOCATED AT \"%s\"...\n\n", outpath);

    FILE *fptr = fopen((const char*)outpath, "rb");

    unsigned int* keyptr = malloc(sizeof(unsigned int));
    unsigned int* recptr_part = malloc(sizeof(unsigned int)*24);

    for(int j = 0;j<n;j++){
        fread(keyptr, sizeof(unsigned int), 1, fptr);
        printf("%u\t:", *keyptr);
        
        fread(recptr_part, sizeof(unsigned int), NUMRECS, fptr);
        for( int i = 0; i<NUMRECS ; i++)
            printf("%u ",recptr_part[i]);

        printf("\n");
    }
}

int sort_comp(const void* a, const void* b){
    if ( ((rec_t*)a)->key >= ((rec_t*)b)->key) return 1;
    else return 0; 
}

int main(int argc, char* argv[]){

    if(argc != 3){
        printf("Inappropriate arguments provided!\n");
        printf("Please run the file in the following syntax \n./<executable_file> -s <location of the file to be sorted>\n");
        return -1;
    }

    

    char* outpath = argv[2];    
    
    // finding the number of 100 byte records in the output file ( we won't be hardcoding n, it will work for any input of n in when generating the out )
    struct stat st;
    stat(outpath, &st);
    int n = st.st_size/100;     // as st.st_size return the size of the file in bytes and each record in the file has size 100 bytes


    // Reading file contents before sorting
    print_bin_file_content(outpath, n);

    FILE *fptr;
    fptr = fopen((const char*)outpath, "rb");

    
    rec_t arr[n];
    int count = 0;


    unsigned int* keyptr = malloc(sizeof(unsigned int));
    unsigned int* recptr_part = malloc(sizeof(unsigned int)*24);

    while(count < n) {
        fread(keyptr, sizeof(unsigned int), 1, fptr);
        arr[count].key = *keyptr;
        fread(recptr_part, sizeof(unsigned int), NUMRECS, fptr);
        for( int i = 0;i<NUMRECS;i++)
            arr[count].record[i] = recptr_part[i];
        count++;
    }

    fclose(fptr);
  
    // sorting the array read from the file
    qsort(arr, n, sizeof(rec_t), sort_comp);

    // Uncomment the line below to print the array after it is sorted
    print_keys_and_records_from_array(arr, n);
    
    // writing the sorted array to the file
    write_to_file(arr, outpath, n);

    // reading the file we just wrote to
    print_bin_file_content(outpath, n);
      
    return 0;
}