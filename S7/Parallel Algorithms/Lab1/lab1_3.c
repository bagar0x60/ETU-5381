#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

void print_pid(int id) {
    pid_t pid, ppid;
    pid  = getpid();
	ppid = getppid();

    printf("П%d:\tpid  = %d\tppid = %d\n", id, pid, ppid);
}

void process_1() {
    print_pid(1);

    // First fork П1 -> П2
	if ( fork() == 0 ) {
        // П2
        process_2();
        return;
    }

    // Second fork П1 -> П3
    if ( fork() == 0 ) {
        // П3
        process_3();
        return;
    } 
	
    // П1
	wait(NULL);
    wait(NULL);
}

void process_2() {
    print_pid(2);
}

void process_3() {
    print_pid(3);

    // Third fork П3 -> П4
    if ( fork() == 0 ) {
        // П4
        process_4();
        return;
    }

    wait(NULL);
}

void process_4() {
    print_pid(4);
}


int main() {
	process_1();
	return 0;
}