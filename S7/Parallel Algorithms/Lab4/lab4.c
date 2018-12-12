#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <sys/timeb.h>
#include <stdbool.h>

const long is_quicksort = true;
const long p;
const long n;
const bool is_verbose;

struct thread_settings {
    int* array;
    long thread_index;
    int left_pipe_read, left_pipe_write, right_pipe_read, right_pipe_write;
};

void merge(int *array, int pos0, int pos1, int pos2) {
    int buffer_size = pos2 - pos0;
    int *buffer = (int*) malloc( buffer_size * sizeof(int) );

    int i = pos0, j = pos1;
    for (int k = 0; k < buffer_size; k++) {
        if (j == pos2 || (i < pos1) && (array[i] < array[j])) {
            buffer[k] = array[i++];
        } else {
            buffer[k] = array[j++];
        }
    }

    // printf("merge: %d %d %d \n", pos0, pos1, pos2);
    // print_array(array + pos0, buffer_size);
     // print_array(buffer, buffer_size);

    for (int k = 0; k < buffer_size; k++) array[pos0 + k] = buffer[k];
    
    // print_array(array + pos0, buffer_size);
    free(buffer);
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
} 

int partition (int array[], int low, int high) {
    int pivot = array[high]; 
    int i = (low - 1);  
 
    for (int j = low; j <= high- 1; j++)
    {
        if (array[j] <= pivot)
        {
            i++; 
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return (i + 1);
}
 
void quicksort(int array[], int pos0, int pos1) {
    if (pos0 < pos1)
    {
        int middle = partition(array, pos0, pos1);
 
        quicksort(array, pos0, middle - 1);
        quicksort(array, middle + 1, pos1);
    }
}

void check_return_code(int code, int correct_code, char* function_name) {
    if (code != correct_code) {
        printf("function call failed: %s return %d\n", function_name, code);
        exit(-1);
    } 
}

void* thread_func(void *arg)
{ 
    struct thread_settings settings = *(struct thread_settings*)arg;
    
    int left_start  = ((settings.thread_index - 1)*n) / p;
    int self_start  = (settings.thread_index*n) / p;
    int self_end    = ((settings.thread_index + 1)*n) / p;    
    int right_end   = ((settings.thread_index + 2)*n) / p;

    //printf("%d\n", settings.thread_index);    
	quicksort(settings.array, self_start, self_end - 1);
    // printf("sorted %d\n", settings.thread_index);
    int dummy = 1;

    for (int i = 0; i < p; i++) {
        // printf("%d\t%d\n", settings.thread_index, i);
        if ((i % 2) == (settings.thread_index % 2)) { 
            // [self][right]
            if (settings.thread_index < p - 1) {
                // if needed: pthread_mutex_lock(settings.self_done);
                read(settings.right_pipe_read, &dummy, sizeof(int)); // right ready                
                merge(settings.array, self_start, self_end, right_end);
                write(settings.right_pipe_write, &dummy, sizeof(int)); // done
            }
        } else {
            // [left][self]
            if (settings.thread_index > 0) {
                write(settings.left_pipe_write, &dummy, sizeof(int)); // i am ready
                read(settings.left_pipe_read, &dummy, sizeof(int)); // left done  
            }
        }
    }
}

void init_array(int **array) {
    time_t t;
    srand((unsigned) time(&t));
    int mod = 10 * n;

    *array = (int*)malloc(n * sizeof(int));    
    for(int i = 0; i < n; i++) {
        (*array)[i] = rand() % mod;
    }
}

void init_pipes(int pipes[][2]) {
    for (int i = 0; i < 2*(p - 1); i++) {
        int result = pipe(pipes[i]);
        check_return_code(result, 0, "pipe");
    }  
}

void print_array(int *array, int length) {
    printf("[ ");
    for(int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("]\n");
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

int main(int argc, char *argv[])
{ 
    parse_args(argc, argv, &n, &is_verbose, &p);
    int *array;
    int pipes[2*(p - 1)][2]; 
    pthread_t threads[p];
    struct thread_settings settings[p];
    
    init_pipes(pipes);
    init_array(&array);

    if (is_verbose) print_array(array, n);

    unsigned long start = currentTime();
    
    if (is_quicksort) {
        // just quicksort
        quicksort(array, 0, n - 1);
    } else  {
        // parallel sort
        for (int i = 0; i < p; i++) {
            settings[i].array = array;
            settings[i].thread_index = i;

            if (i > 0) {
                settings[i].left_pipe_read = pipes[2*(i - 1)][0];
                settings[i].left_pipe_write = pipes[2*(i - 1) + 1][1];
            }
            if (i < p - 1) {
                settings[i].right_pipe_read = pipes[2*i + 1][0];
                settings[i].right_pipe_write = pipes[2*i][1];
            }        
            int result = pthread_create(&threads[i], NULL, thread_func, &settings[i]);
            check_return_code(result, 0, "pthread_create");
        }
    
        for (int i = 0; i < p; i++) {
            int result = pthread_join(threads[i], NULL);
            check_return_code(result, 0, "pthread_join");
        }
    }
    
    unsigned long end = currentTime();    
    double work_time = (double)(end - start) / 1000.0;
    if (is_verbose) {
        print_array(array, n);
        printf("elapsed time:\t%f\n", work_time);
    } else {
        printf("%.10f", work_time);
    }
}