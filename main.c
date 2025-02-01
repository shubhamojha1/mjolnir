#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include "mylib.h"

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

// #define MAX_COMMAND_SIZE 1024
#define READLINE_BUFSIZE 1024
#define TOKEN_BUFSIZE 64
#define TOKEN_DELIMITER " \t\r\n\a"

void print_welcome_art();
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);

char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &shell_cd,
    &shell_help,
    &shell_exit,
};

int shell_num_builtins(){
    return sizeof(builtin_str) / sizeof(char *);
}

int shell_cd(char **args){
    if(args[1] == NULL){
        fprintf(stderr, "mjolnir: expected argument to \"cd\"\n");
    }else {
        if(chdir(args[1]) != 0){
            perror("mjolnir");
        }
    }
    return 1;
}

int shell_help(char **args){
    printf("mjolnir (also called jonathan)\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for(int i=0;i<shell_num_builtins(); i++){
        printf("  %s\n", builtin_str[i]);
    }

    printf("Used the man command for information on other programs.\n");
    return 1;
}

int shell_exit(char **args){
    return 0;
}

int shell_launch(char **args){
    pid_t pid;
    int status;

    pid = fork();
    if(pid == 0){
        // child process
        if(execvp(args[0], args) == -1){
            perror("mjolnir");
        }
        exit(EXIT_FAILURE);
    }else if(pid < 0){
        perror("mjolnir");
    }else{
        // parent process
        do{
            waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    } 
    return 1;
}

int execute(char **args){
    if(args[0] == NULL) {
        return 1;
    }

    for(int i=0;i<shell_num_builtins();i++){
        if(strcmp(args[0], builtin_str[i]) == 0){
            return (*builtin_func[i])(args);
        }
    }
    return shell_launch(args);
}

void print_welcome_art() {
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⣡⡾⠋⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢟⣵⡿⠋⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣱⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡾⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣧⡙⢿⣦⡀⠀⠀⠀⠀⠀⠀⣠⣾⢋⣴⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣤⣬⣿⣶⣿⣿⣦⡀⠀⠀⣠⣾⢟⣵⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣤⣤⣤⣾⣿⣿⣿⣿⣿⣿⣦⣾⣯⣵⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⠟⠉⠉⠉⢩⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⡿⣿⡛⠛⢛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⡏⠙⢷⣌⣻⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣷⣄⣀⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n");
}

char *read_line(void){

    // #ifdef USE_STD_GETLINE

    int bufsize = READLINE_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if(!buffer) {
        fprintf(stderr, "mjolnir: allocation error!\n");
        exit(EXIT_FAILURE);
    }

    while(1) {
        c = getchar();

        if (c == EOF || c == '\n'){
            buffer[position] = '\0';
            return buffer;
        }else {
            buffer[position] = c;
        }

        position++;

        // if bufsize exceeded, reallocate
        if (position >= bufsize) {
            bufsize += READLINE_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "mjolnir: allocation error!\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **split_line(char *line){
    int bufsize = TOKEN_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char * token, **tokens_backup;

    if(!tokens) {
        fprintf(stderr, "mjolnir: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOKEN_DELIMITER);
    while(token != NULL){
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += TOKEN_BUFSIZE;
            tokens_backup = tokens;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if(!tokens) {
                free(tokens_backup);
                fprintf(stderr, "mjolnir: allocation error!");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOKEN_DELIMITER);
    }
    tokens[position] = NULL;
    return tokens;

}

void shell_loop(void){
    char *line;
    char **args;
    int status;

    do {
        printf("> "); 
        // printf("Normal \033[5m\033[31mBlink\033[0m\n");
        line = read_line();
        args = split_line(line);
        status = execute(args);

        free(line);
        free (args);

    } while(status);
}

int main(int argc, char **argv){
    // implementing custom shell in C for windows

    // char command[MAX_COMMAND_SIZE];
    // char *args[10];

    // print_welcome_art();

    // run command loop
    shell_loop();
    // perform shutdown/cleanup

    return EXIT_SUCCESS;
}
