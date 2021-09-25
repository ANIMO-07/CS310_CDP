#include <stdlib.h>
#include <stdio.h>


// Recursive factorial function
int factorial(int n) {
    printf("Address of local variable for n = %d is : %p \n", n, &n);

    if (n == 0)
        return 1;

    return n * factorial(n - 1);
}

// Global variables
int a = 10;
int b = 4;
int c;


int main(int argc, char** argv ) {
    char* ptr = (char*)malloc(a * sizeof(int));
    char* ptr1 = (char*)malloc(b * sizeof(int));

    int num;

    if(argc > 1)
        num = atoi(argv[1]);
    else
        num = 4;

    // Addressess for each
    printf("Address of main: %lx \n", ((long unsigned int) &main));
    printf("Address of factorial function: %lx \n", ((long unsigned int) &factorial));
    printf("Address of 1st dynamically allocated variable: %p \n", &ptr);
    printf("Address of 2nd dynamically allocated variable: %p \n", &ptr1);
    printf("Address of initialized global variable is : %p \n",&a);
    printf("Address of initialized global variable is : %p \n",&b);
    printf("Address of uninitialized global variable is : %p \n",&c);

    int result = factorial(num);
    printf("Factorial: %d \n", result);

    free(ptr);
    free(ptr1);


    return 0;
}