#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"

/*
- search for startup files
- user specific configuration
- establishes environment variables
- defines executable search paths
- sets up command aliases
- prepares cli
- sets up prompt
- enables shell specific features

. fork() to create a new process
. exec() to replace the process with a new program
. wait() to manage child process completion
 */

#define MAX_COMMAND_SIZE 1024

void print_welcome_art();



int main(int argc, char **argv){
    // implementing custom shell in C for windows

    char command[MAX_COMMAND_SIZE];
    char *args[10];

    print_welcome_art();
    
}

void print_welcome_art(){
    
}