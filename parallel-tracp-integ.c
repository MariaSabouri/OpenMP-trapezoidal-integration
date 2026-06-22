#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "math.h"



#define U(x) sin(x)
#define a 0
#define b acos(-1.0)

/*
If your code includes mathematical functions (like exp, cos, etc.), you need to link to the mathematics 
library libm.so. This is done, just like for serial compiling, by adding -lm to the end of your compile command, 
that is,

gcc -fopenmp parallel-tracp-integ.c -o parallel-tracp-integ -lm
*/
   
void save_results(
    int thead_count,
    int slices,
    double total_area
) {
    FILE *fp = fopen("race-condition_fixed.csv", "a");

    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(fp,
            "%d,%d%.6f\n",
            thead_count,
            slices,
            total_area);

    fclose(fp);
}


void Trap(int n , double* global_result, int thead_count){
    double h, x, my_result, temp_global_result;
    int i;
    
    h = (b - a) / n;
    temp_global_result = *global_result;

    #pragma omp parallel for num_threads(thead_count) default(none) private(i) shared(n, h, temp_global_result)

    for (i = 0; i < n; i++) {
        #pragma omp critical
        temp_global_result += U(a + i*h);
    }

    *global_result = temp_global_result;
    *global_result *= h;

}


int main(int argc, char* argv[]) {
    int thead_count = strtol(argv[1], NULL, 10);
    int n = strtol(argv[2], NULL, 10);
    double global_result = (U(a) + U(b)) / 2;

    Trap(n, &global_result, thead_count);

    printf("area is: %lf\n", global_result);

    save_results(thead_count, n, global_result);

}