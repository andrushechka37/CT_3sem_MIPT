#include <cstddef>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    int fd = 0;
    ssize_t size = 0;
    char string[1200] = "";

    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        /* Если файл открыть не удалось, печатаем об этом сообщение и прекращаем работу */
        printf("Can\'t open file\n");
        exit(-1);
    }

    // Читаем данные
    size = read(fd, string, 1190); // Читаем максимум 119 символов, оставляем место для \0
    printf("size: %lu", size);
    if (size < 0) {
        printf("Error reading file\n");
        close(fd);
        exit(-1);
    }

    // Завершаем строку нулевым символом
    string[size] = '\0';

    // Печатаем прочитанное
    printf("%s\n", string);

    // Записываем прочитанное в out.txt
    FILE *output = fopen("out.txt", "w");
    if (output) {
        int i = fwrite(string, size, 1, output);
        printf("written: %d", i);
        fclose(output);
    } else {
        printf("Can\'t open out.txt for writing\n");
    }

    // Закрываем файл
    if (close(fd) < 0) {
        printf("Can\'t close file\n");
    }

    // Открываем файл out.txt в редакторе nano
    system("nano out.txt");

    return 0;
}