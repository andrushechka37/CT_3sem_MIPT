#!/bin/bash

# Переход в нужную директорию
cd /home/vless-user/3sem/fork

# Компиляция программы
clang++ -o sample 6.cpp

# Создание файлов 1 и 2
touch 1 2

echo "execute [./sample 1 2] in that terminal and [./sample 2 1] in another"