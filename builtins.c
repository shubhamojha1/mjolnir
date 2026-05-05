#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "builtins.h"
#include "exec.h"

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

int shell_num_builtins(void){
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
