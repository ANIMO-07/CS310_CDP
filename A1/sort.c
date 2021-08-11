#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "sort.h"
#include <sys/types.h>
#include <sys/stat.h>

void createRectArray(FILE* file_ptr, rec_t* rArray) {
    rec_t r;
    for(int i = 0; i < 100; i++) {
        fread(&r, sizeof(rec_t), 1, file_ptr);
        rArray[i] = r;
    }
}

void print_data(rec_t *rArray) {
    for(int i = 0; i < 100; i++) {
        fprintf(stderr, "%d : ", rArray[i].key);

        for(int j = 0; j < 24; j++) {
            fprintf(stderr, "%d ", rArray[i].record[j]);
        }

        fprintf(stderr, "\n");
    }

    fprintf(stderr, " DONE PRINTING \n");
}

void createOutputFile(rec_t *rArray) {
    char *outFile = "sorted_output";

    // open and create output file
	int fd = open(outFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}

	for (int i = 0; i < 100; i++)
	{
		int rc = write(fd, &rArray[i], sizeof(rec_t));
		if (rc != sizeof(rec_t))
		{
			perror("write");
			exit(1);
			// should probably remove file here but ...
		}
	}

	// ok to ignore error code here, because we're done anyhow...
	(void)close(fd);
}

int cmpfunc (const void * a, const void * b) {
   return ( ((rec_t *)a) -> key > ((rec_t *)b) -> key );
}

int main () {

    // Opening the file
    FILE *file_ptr;
    file_ptr = fopen("outfile", "rb");

    // creating the array of struct rec_t
    rec_t rArray[100];
    createRectArray(file_ptr, rArray);

    print_data(rArray);

    // sorting by key using the comparator function
    // using quick sort
    qsort(rArray, 100, sizeof(rec_t), cmpfunc);

    print_data(rArray);

    // Now store in the output file
    createOutputFile(rArray);


    return 0;
}