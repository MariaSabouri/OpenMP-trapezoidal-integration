#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "math.h"
#include <time.h>


#define U(x) sin(x)
#define a 0
#define b acos(-1.0)
#define EXACT_VALUE 2

enum ScheduleType { STATIC, DYNAMIC, GUIDED };
/*
If your code includes mathematical functions (like exp, cos, etc.), you need to link to the mathematics 
library libm.so. This is done, just like for serial compiling, by adding -lm to the end of your compile command, 
that is,

gcc -fopenmp parallel-tracp-integ.c -o parallel-tracp-integ -lm
*/
   

void save_results(
    int thead_count,
    int slices,
    double total_area,
    double elapsed,
    double exact_value,
    char* schedule,
    int chunk_size
) {
    FILE *fp = fopen("result.csv", "a");

    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(fp,
            "%d,%d,%.6f,%.6f,%.6f,%s,%d\n",
            thead_count,
            slices,
            total_area,
            elapsed,
            exact_value,
            schedule,
            chunk_size);

    fclose(fp);
}


void Trap(int n , 
        double* global_result, 
        int thead_count,
        int schedule,
        int chunk_size){
    double h, x, my_result, temp_global_result;
    int i;

    
    h = (b - a) / n;
    temp_global_result = *global_result;
    
    if (schedule == STATIC) {
            #pragma omp parallel for num_threads(thead_count) default(none) \
            reduction(+: temp_global_result) private(i) shared(n, h, chunk_size) schedule(static,chunk_size)
            for (i = 0; i < n; i++) {
                if (i == 0) printf("number of thead(s) is(are): %d\n", omp_get_num_threads());
                temp_global_result += U(a + i*h);
            }
    }
    else if (schedule == DYNAMIC){
            #pragma omp parallel for num_threads(thead_count) default(none) \
            reduction(+: temp_global_result) private(i) shared(n, h) schedule(dynamic)
            for (i = 0; i < n; i++) {
                if (i == 0) printf("number of thead(s) is(are): %d\n", omp_get_num_threads());
                temp_global_result += U(a + i*h);
            }
    }
    else {        
            #pragma omp parallel for num_threads(thead_count) default(none) \
            reduction(+: temp_global_result) private(i) shared(n, h) schedule(guided)
            for (i = 0; i < n; i++) {
                if (i == 0) printf("number of thead(s) is(are): %d\n", omp_get_num_threads());
                temp_global_result += U(a + i*h);
            }
    }
    *global_result = temp_global_result;
    *global_result *= h;

}


int main(int argc, char* argv[]) {
    int thead_count = strtol(argv[1], NULL, 10);
    int n = strtol(argv[4], NULL, 10);
    char* schedule_str = argv[2];
    int schedule = strtol(argv[2], NULL, 10);
    int chunk_size = strtol(argv[3], NULL, 10);

    double global_result = (U(a) + U(b)) / 2;
    time_t start_time, end_time;

    time(&start_time);
    Trap(n, &global_result, thead_count, schedule, chunk_size);
    time(&end_time);

    double elapsed = difftime(end_time, start_time) * 1000000;

    printf("area is: %lf\n", global_result);

    save_results(thead_count, n, global_result, elapsed, EXACT_VALUE, schedule_str, chunk_size);
}