#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/wait.h> 
#include <errno.h> 

#define FIFO_NAME "canal" 
#define LOG_FILE  "log.txt" 
#define BUF_SIZE  4096 
#define MAX_CMDS  100 
#define MAX_ARGS  64 

void parse_args(char *cmd, char *args[]) { 
int i = 0; 
char *token = strtok(cmd, " "); 
    while (token && i < MAX_ARGS - 1) { 
        args[i++] = token; 
        token = strtok(NULL, " "); 
    } 
    args[i] = NULL; 
} 
 
int main() { 
    if (mkfifo(FIFO_NAME, 0666) == -1 && errno != EEXIST) { 
        perror("erro ao criar FIFO com mkfifo"); 
        exit(1); 
    } 
 
    while (1) { 
        int fd = open(FIFO_NAME, O_RDONLY); 
        if (fd == -1) { 
            perror("erro ao abrir FIFO para leitura"); 
            continue; 
        } 
 
        char buffer[BUF_SIZE]; 
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1); 
        if (n == -1) { 
            perror("erro ao ler do FIFO"); 
            close(fd); 
            continue; 
        } 
 
        close(fd); 
 
        if (n <= 0) continue; 
        buffer[n] = '\0'; 
 
        char *cmd_originals[MAX_CMDS]; 
        int count = 0; 
 
        char *line = strtok(buffer, "\n"); 
        while (line && count < MAX_CMDS) { 
            cmd_originals[count++] = strdup(line); 
            line = strtok(NULL, "\n"); 
        } 
 
        pid_t pids[MAX_CMDS]; 
 
        for (int i = 0; i < count; i++) { 
            pids[i] = fork(); 
            if (pids[i] == -1) { 
                perror("erro ao criar processo com fork"); 
                continue; 
            } 
 
            if (pids[i] == 0) { 
                char *args[MAX_ARGS]; 
                parse_args(cmd_originals[i], args); 
                execvp(args[0], args); 
                perror("erro ao executar comando com execvp"); 
                _exit(1); 
            } 
        } 
 
        for (int i = 0; i < count; i++) { 
            int status; 
            waitpid(pids[i], &status, 0); 
 
            int log_fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644); 
            if (log_fd == -1) { 
                perror("erro ao abrir ficheiro de log"); 
            } else { 
                int exit_val = WIFEXITED(status) ? WEXITSTATUS(status) : -1; 
                dprintf(log_fd, "%s; exit status: %d\n", cmd_originals[i], exit_val); 
                close(log_fd); 
            } 
 
            free(cmd_originals[i]); 
        } 
    } 
 
    return 0; 
}
