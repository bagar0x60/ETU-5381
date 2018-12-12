#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <sys/sem.h>
#include <stdbool.h>

const long m;
const long n;
const long p;
const bool is_by_lines = false;
const bool is_verbose;

void compute_lines(long line_from, long line_to, double (*matrix)[n], double *vector, double *result) {
    double res;
    for (int i = line_from; i < line_to; i++) {
        res = 0;
        for (int j = 0; j < n; j++) {
            res += matrix[i][j] * vector[j];
        }
        result[i] = res;
    }
}

void compute_rows(long row_from, long row_to, double (*matrix)[n], double *vector, double *result, int semafor_id) {
    for (int i = 0; i < m; i++) {
        double res = 0;
        for (int j = row_from; j < row_to; j++) {
            res += matrix[i][j] * vector[j];
        }

        if (res != 0) {
            semafor(semafor_id, -1, i);
            result[i] += res;
            semafor(semafor_id, 1, i);
        }
    }
}

void semafor(int semafor_id, int n, int index) {
    struct sembuf buf;
    buf.sem_num = index;
    buf.sem_op = n;
    buf.sem_flg = 0;
    int status = semop(semafor_id, &buf, 1);
    if (status < 0) {
        printf("function call failed: semop\n");
        exit(-1);
    }
}

void print_matrix(double (*matrix)[n]) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void print_vector(double *vector, long size) {
    for (int i = 0; i < size; i++) {
        printf("%.2f ", vector[i]);
    }
    printf("\n");
} 

void fill_data(double (*matrix)[n], double *vector) {
    // fill matrix
    for (long i = 0; i < m; i++) {
        for (long j = 0; j < n; j++) {
            matrix[i][j] = 1;
        }
    }

    // fill vector
    for (long j = 0; j < n; j++) {
        vector[j] = j;
    }
}

unsigned long currentTime() {
    struct timeb tmb;  
    ftime(&tmb);
    return (unsigned long) tmb.time * 1000 + tmb.millitm;
}

void parse_args(int argc, char *argv[], long *m, long *n, bool *is_verbose, long *p) {
    if(argc == 5) {
        *m = atoi(argv[1]);
        *n = atoi(argv[2]);
        *is_verbose = atoi(argv[3]);
        *p = atoi(argv[4]);        
    } else {
        printf("Invalid arguments!\n");
        exit(1);
    }
}

int main(int argc, char *argv[]) {    
    parse_args(argc, argv, &m, &n, &is_verbose, &p);

    int semafor_id = semget(IPC_PRIVATE, m, 0666 | IPC_CREAT);
    
    for (int i = 0; i < m; i++) {
        semafor(semafor_id, 1, i);
    }

    int matrix_id = shmget( IPC_PRIVATE, sizeof(double[m][n]), 0666 | IPC_CREAT | IPC_EXCL );
    int vector_id = shmget( IPC_PRIVATE, sizeof(double[n]), 0666 | IPC_CREAT | IPC_EXCL );
    int result_id = shmget( IPC_PRIVATE, sizeof(double[m]), 0666 | IPC_CREAT | IPC_EXCL );

    double (*matrix)[n] = shmat(matrix_id, NULL, 0);   // matrix m*n
    double *vector = shmat(vector_id, NULL, 0);        // vector n*1
    double *result = shmat(result_id, NULL, 0);        // result vector m*1

    fill_data(matrix, vector);

    if (is_verbose) {
        printf("\nmatrix: \n");
        print_matrix(matrix);
        printf("\nvector: \n");
        print_vector(vector, n);
    }
    
    long start_time = currentTime();
    for (long i = 0; i < p; i++) {
        int status = fork();
        if (status == 0) {
            if (is_by_lines) {
                compute_lines((i * m) / p, ((i + 1)*m) / p, matrix, vector, result);
            } else {
                compute_rows((i * n) / p, ((i + 1)*n) / p, matrix, vector, result, semafor_id);
            }
            return 0;
        } else if (status < 0) {
            printf("function call failed: fork\n");
            exit(-1);
        }
    }

    while(wait(NULL) > 0);

    long end_time = currentTime();
    double work_time = (double)(end_time - start_time) / 1000.0;
    

    if (is_verbose) {
        printf("elapsed time:\t%f\n", work_time);

        printf("\nresult: \n");
        print_vector(result, m);
    } else {
        printf("%f", work_time);
    }
    
    shmctl(matrix_id, IPC_RMID, NULL);
    shmctl(vector_id, IPC_RMID, NULL);
    shmctl(result_id, IPC_RMID, NULL);
    semctl(semafor_id, 0, IPC_RMID, NULL);
}