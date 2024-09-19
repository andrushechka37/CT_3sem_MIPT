#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 65535

int main()
{
    char *memory;  // Указатель на разделяемую память
    char *p;       // Временный указатель для записи фрагментов файла
    int shmid;     // IPC дескриптор для области разделяемой памяти
    int fd, nread; // Переменные для файлового дескриптора и количества прочитанных байт
    char pathname[] = "hui.txt"; // Имя файла
    key_t key;

    // Генерируем IPC ключ из имени файла и номера экземпляра области разделяемой памяти
    if((key = ftok(pathname, 0)) < 0)
    {
        printf("Can't generate key\n");
        exit(-1);
    }

    // Пытаемся создать разделяемую память для сгенерированного ключа
    if((shmid = shmget(key, SIZE, 0666|IPC_CREAT)) < 0)
    {
        printf("Can't create shared memory\n");
        exit(-1);
    }

    // Пытаемся отобразить разделяемую память в адресное пространство текущего процесса
    if((memory = (char *)shmat(shmid, NULL, 0)) == (char *)(-1))
    {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    // Открываем файл только на чтение
    if((fd = open(pathname, O_RDONLY)) < 0)
    {
        printf("Can't open source file\n");
        (void)shmdt(memory);
        exit(-1);
    }

    // Инициализируем указатель p началом области разделяемой памяти
    p = memory;

    // Читаем файл порциями по 1 KB
    while((nread = read(fd, p, 1024)) > 0)
    {
        p += nread; // Сдвигаем указатель для записи следующей порции данных
    }

    // Закрываем файл
    (void)close(fd);

    // Если возникла ошибка - завершаем работу
    if(nread < 0)
    {
        printf("Error reading source file\n");
        (void)shmdt(memory);
        exit(-1);
    }

    // Вставляем признак конца строки
    *p = '\0';

    // Печатаем содержимое буфера
    printf("%s\n", memory);

    // Отсоединяем разделяемую память
    if(shmdt(memory) < 0)
    {
        printf("Can't detach shared memory\n");
        exit(-1);
    }

    return 0;
}