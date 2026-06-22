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
   

void Trap(int n , double* global_result, int thead_count){
    double h, x, my_result, temp_global_result;
    int i;

    // int my_rank = omp_get_thread_num();
    // int thead_count = omp_get_num_threads();
    
    h = (b - a) / n;
    temp_global_result = *global_result;

    #pragma omp parallel for num_threads(thead_count) default(none) reduction(+: temp_global_result) private(i) shared(n, h)

    for (i = 0; i < n; i++) {
        if (i == 0) printf("number of thead(s) is(are): %d\n", omp_get_num_threads());
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
}