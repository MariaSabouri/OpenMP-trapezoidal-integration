#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "math.h"

#define U(x) sin(x)
#define a 0
#define b acos(-1.0)


void distribute_local_values(
    int my_rank,
    int thead_count,
    int n,
    int* local_n
){
    /*calculate local_n*/
    *local_n = n / thead_count;
    int remainder = n % thead_count;
    *local_n += (my_rank < remainder) ? 1 : 0;
    
    printf("rank: %d ,local n: %d\n",my_rank, *local_n);
}
   

void Trap(int n , double* global_result){
    double h, x, my_result;
    int my_rank = omp_get_thread_num();
    int thead_count = omp_get_num_threads();
    int local_n;

    h = (b - a) / n;
    distribute_local_values(my_rank, thead_count, n, &local_n); 

}


int main(int argc, char* argv[]) {
    int thead_count = strtol(argv[1], NULL, 10);
    int n = strtol(argv[2], NULL, 10);
    double global_result = (U(a) + U(b)) / 2;

    


}