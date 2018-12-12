#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/timeb.h>
#include <stdbool.h>

const long n;  // terms count
const long p;  // process count
const bool is_verbose;

double compute_pi_part(long n_from, long n_to) {
    double res = 0;
    int sign = n_from  % 2 == 0 ? 1 : -1; 
    for (int i = n_from; i < n_to; i++) {
        res += sign * 4.0 / (2.0*i + 1.0); 
        sign = -sign;
    }
    return res;
}

unsigned long currentTime() {
    struct timeb tmb;  
    ftime(&tmb);
    return (unsigned long) tmb.time * 1000 + tmb.millitm;
}

void parse_args(int argc, char *argv[], long *n, bool *is_verbose, long *p) {
    if(argc == 4) {
        *n = atoi(argv[1]);
        *is_verbose = atoi(argv[2]);
        *p = atoi(argv[3]);
        
    } else {
        printf("Invalid arguments!\n");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    parse_args(argc, argv, &n, &is_verbose, &p);

    int pipes[p][2];    
    double subprocess_res;

    unsigned long start = currentTime();
    // spam processes
    for (int i = 0; i < p; i++) {
        if (pipe(pipes[i]) < 0) {
            printf("function call failed: pipe\n");
            return 1;
        }

        pid_t pid;
        if ((pid = fork()) < 0) {
            printf("function call failed: fork\n");
            return 1;
        } else if (pid == 0) {              
            subprocess_res = compute_pi_part((n*i) / p, (n*(i + 1)) / p);
            
            close(pipes[i][0]);
            write(pipes[i][1], &subprocess_res, sizeof(subprocess_res));
            return 0;
        }
    }

    double res = 0;
    for (int i = 0; i < p; i++) {
        close(pipes[i][1]);
        read(pipes[i][0], &subprocess_res, sizeof(subprocess_res));
        res += subprocess_res;
    }    

    unsigned long end = currentTime();    
    double work_time = (double)(end - start) / 1000.0;
    if (is_verbose) {
        printf("For n = %d\tp = %d\n", n, p);
        printf("pi = %.20f \t time = %f s\n", res, work_time);
    } else {
        printf("%f", work_time);
    }
}