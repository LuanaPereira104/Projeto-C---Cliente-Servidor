#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <string.h> 
 
#define FIFO_NAME "canal" 
#define MAX_BUF 4096 
 
int main(int argc, char *argv[]) { 
    if (argc < 2) { 
        const char msg[] = "Uso: ./cliente \"comando1\" \"comando2 args\" ...\n"; 
        write(STDERR_FILENO, msg, sizeof(msg) - 1); 
        return 1; 
    } 
 
    int fd = open(FIFO_NAME, O_WRONLY); 
    if (fd == -1) { 
        perror("erro ao abrir FIFO para escrita"); 
        return 1; 
    } 
 
    char buffer[MAX_BUF]; 
    int offset = 0; 
    memset(buffer, 0, MAX_BUF); 
 
    for (int i = 1; i < argc; i++) { 
        int len = strlen(argv[i]); 
        if (offset + len + 1 < MAX_BUF) { 
            memcpy(buffer + offset, argv[i], len); 
            offset += len; 
buffer[offset++] = '\n'; 
} 
} 
if (write(fd, buffer, offset) == -1) { 
perror("erro ao escrever no FIFO"); 
} 
close(fd); 
return 0; 
}
