#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>


static int get_size_of_file(FILE * file) {
    struct stat buff;
    fstat(fileno(file), &buff);
    return buff.st_size;
}


#define IS_NULL_PTR(ptr)           \
    if (ptr == NULL)  {            \
        continue;                  \
    }


int start_messenger(char *argv[]) {
    pid_t pid = fork();

    if (pid == -1) {
        printf("error on fork\n");
        return 0;
    } else if (pid == 0) { // child for writing

        while (true) {

            char buffer[100] = "";
            scanf("%s", buffer);
            FILE * pfile = fopen(argv[1], "w");

            IS_NULL_PTR(pfile);

            fprintf(pfile, "%s", buffer);
            printf("\nwrite %s to file out.txt\n", buffer);
            fclose(pfile);
        }

    } else { // parent for reading

        while(true) {
            char buffer[100] = "";
            sleep(1);
            FILE * pfile = fopen(argv[2], "r");

            IS_NULL_PTR(pfile);

            int size = get_size_of_file(pfile);
            if (size != 0) {
                fread(buffer, 1, size, pfile);
                printf("\nread [%s], size: %d \n", buffer, size);
            }
            fclose(pfile);

            pfile = fopen(argv[2], "w");
            fclose(pfile);

            
        }
    }       
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("not enough arguments, first argument for writer and second for reader\n");
        return 0;
    }

    start_messenger(argv);
    return 0;
}