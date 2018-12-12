#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/timeb.h>
#include <stdbool.h>
#include <stdlib.h>

const long n;  // terms count
const long p;  // process count
const bool is_verbose;
const bool is_reduction = false;

unsigned long currentTime() {
    struct timeb tmb;  
    ftime(&tmb);
    return (unsigned long) tmb.time * 1000 + tmb.millitm;
}

void parse_args(int argc, char *argv[], long *n, bool *is_verbose, long *p) {
    if(argc == 4) {
        *n = atol(argv[1]);
        *is_verbose = atoi(argv[2]);
        *p = atoi(argv[3]);
        
    } else {
        printf("Invalid arguments!\n");
        exit(1);
    }
}

double compute_pi_reduction() {
    double pi = 0;
    double sign;
    
    #pragma omp parallel for reduction(+:pi) private(sign) 
    for (long i = 0; i < n; i++) {
        sign = (i % 2) ? -1 : 1;
        pi += sign / (2.0*i + 1.0); 
    }  
    return pi*4.0;
}

double compute_pi_no_reduction() {
    double pi = 0;
    #pragma omp parallel
    {
        int thread_number = omp_get_thread_num();
        long n_from = (long long)thread_number*(long long)n / (long long)p;
        long n_to = (long long)(thread_number + 1)*(long long)n / (long long)p;

        double thread_result = 0;
        int sign = n_from  % 2 == 0 ? 1 : -1; 
        for (long i = n_from; i < n_to; i++) {
            thread_result += sign / (2.0*i + 1.0); 
            sign = -sign;
        }

        #pragma omp atomic
        pi += thread_result;
    }
    return pi*4.0;
}

int main(int argc, char *argv[]) {
    parse_args(argc, argv, &n, &is_verbose, &p);
    omp_set_num_threads(p);

    unsigned long start = currentTime();   
    double pi = is_reduction ? compute_pi_reduction() : compute_pi_no_reduction();    
    unsigned long end = currentTime();

    double work_time = (double)(end - start) / 1000.0;
    if (is_verbose) {
        printf("For n = %lld\tp = %d\n", n, p);
        printf("pi = %.20f \t time = %f s\n", pi, work_time);
    } else {
        printf("%f", work_time);
    }
}