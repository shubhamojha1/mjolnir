#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "exec.h"

int shell_launch(char **args){
    pid_t pid;
    int status;

    pid = fork();
    if(pid == 0){
        if(execvp(args[0], args) == -1){
            perror("mjolnir");
        }
        exit(EXIT_FAILURE);
    }else if(pid < 0){
        perror("mjolnir");
    }else{
        do{
            waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}
