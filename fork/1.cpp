/* Программа для распечатки аргументов командной строки и параметров среды текущего процесса */
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
    int i;

    for(i = 0; envp[i] != NULL; i++) {
        printf("envp[%d]: %s\n", i, envp[i]);
    }

    for(i = 0; i < argc - 1; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

return 0;

} 