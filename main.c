#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

#define TOKEN_BUFSIZE 64

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

char **split_line(char *line){
    int bufsize = TOKEN_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));

    if(!tokens) {
        fprintf(stderr, "mjolnir: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(*line) {
        while(*line == ' ' || *line == '\t') line++;
        if (!*line) break;

        if (position >= bufsize) {
            bufsize += TOKEN_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if(!tokens) {
                fprintf(stderr, "mjolnir: allocation error!");
                exit(EXIT_FAILURE);
            }
        }

        tokens[position] = line;
        int in_quote = 0;
        int escape = 0;

        while(*line) {
            if (escape) {
                escape = 0;
                line++;
                continue;
            }

            if (*line == '\\' && !in_quote) {
                escape = 1;
                line++;
                continue;
            }

            if (*line == '"') {
                in_quote = !in_quote;
                line++;
                continue;
            }

            if (*line == '\'' && !in_quote) {
                in_quote = 1;
                line++;
                char *start = line;
                while(*line && *line != '\'') line++;
                if (*line == '\'') *line = '\0';
                line++;
                tokens[position] = start;
                position++;

                while(*line == ' ' || *line == '\t') line++;
                break;
            }

            if (!in_quote && (*line == ' ' || *line == '\t')) {
                *line = '\0';
                line++;
                position++;
                break;
            }

            line++;
        }

        if (!*line && tokens[position] != line) {
            position++;
        }
    }

    tokens[position] = NULL;
    return tokens;
}

#define HISTORY_MAX 50

static char *history[HISTORY_MAX];
static int history_count = 0;

static void add_history_entry(const char *line) {
    if (!line || !*line) return;

    if (history_count > 0 && strcmp(history[history_count - 1], line) == 0) return;

    if (history_count >= HISTORY_MAX) {
        free(history[0]);
        memmove(&history[0], &history[1], sizeof(char *) * (HISTORY_MAX - 1));
        history_count--;
    }

    history[history_count] = strdup(line);
    history_count++;
}

void shell_loop(void){
    char *line = NULL;
    size_t linesize = 0;
    char **args;
    int status = 1;

    while (status) {
        printf("mjolnir> ");
        fflush(stdout);

        if (getline(&line, &linesize, stdin) == -1) {
            printf("\n");
            break;
        }

        if (line[strcspn(line, "\n")] != '\0') {
            line[strcspn(line, "\n")] = '\0';
        }

        if (!*line) continue;

        add_history_entry(line);

        args = split_line(line);
        status = execute(args);

        free(args);
    }

    free(line);
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
