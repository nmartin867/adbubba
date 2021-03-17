#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */

int main() {
    int p[2];
    pid_t pid;

    char *argv[] = {"adb", "logcat", NULL};

    if(pipe(p) == -1){
        fprintf(stderr, "Error creating pipe\n");
    }
    pid = fork();
    if(pid == -1){
        fprintf(stderr, "Error creating child process.");
    }
    if(pid == 0){
        close(p[0]);
        dup2(p[1], 1);
        execv("/usr/bin/adb",argv);
        fprintf(stderr, "exec failed\n");
    } else {
        close(p[1]);
        char buffer[1];
        while(read(p[0], buffer, 1) > 0) {
            write(1, buffer, 1);
        }
        close(p[0]);
        wait(NULL);
    }
}