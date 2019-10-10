#include <stdio.h>
#include <string.h>

unsigned char USE_COMPRESSION = 0;
// В теории можно добавить дополнительный аргумент для смены этого флага
// Изначально

int main(int argc, char *argv[]) {

    // Архиватор файлов

    // Часть 1: Разбор аргументов строки

    unsigned char _file = 0;
    char* name_of_file;
    unsigned char _create = 0;
    int index_of_first_file = 0;
    unsigned char _extract = 0;
    unsigned char _list = 0;

    unsigned char flag = 0;

    for (int i = 1; i < argc; ++i) {
        if (flag == 2) {
            if (strlen(argv[i]) >= 2 && strncmp(argv[i], "--", 2) == 0) {
                printf("After --create key only names of files are acceptable\n");
                printf("Note: Files with names starting with \"--\" are considered as keys to program\n");
                return 0;
            }
            continue;
        }
        else if (flag) {
            flag = 0;
            continue;
        }
        if (strlen(argv[i]) > 2 && strncmp(argv[i], "--", 2) == 0) {
            if (strlen(argv[i]) == 6 && strncmp(argv[i], "--file", 6) == 0) {
                _file = 1;
                name_of_file = argv[i+1];
                flag = 1;
            } else if (strlen(argv[i]) == 8 && strncmp(argv[i], "--create", 8) == 0) {
                _create = 1;
                index_of_first_file = i + 1;
                flag = 2;
            } else if (strlen(argv[i]) == 9 && strncmp(argv[i], "--extract", 9) == 0) {
                _extract = 1;
            } else if (strlen(argv[i]) == 6 && strncmp(argv[i], "--list", 6) == 0) {
                _list = 1;
            } else {
                printf("Invalid argument: %s\n", argv[i]);
                return 0;
            }
        } else {
            printf("Invalid argument: %s\n", argv[i]);
            printf("If you want to pass a 'long' value use double quotes. Example:\n--value \"some long value\"");
            return 0;
        }

    }

    if (!_file) {
        printf("Argument --file is required\n");
        printf("Options:\n");
        printf("1) --create <list_of_files>\n");
        printf("2) --extract\n");
        printf("3) --list\n");
        printf("Note: pass 'long' values with double quotes. Example: --value \"some long value\"\n\n");
        printf("Syntax:\n");
        printf("--file FILE [...]\n");
        return 0;
    }

    if (_create + _extract + _list != 1) {
        printf("Wrong number of arguments\n");
        return 0;
    }

    // Часть 2: выбор команды
    if (_create) {
        // Чтение файлов и архивирование
        if (USE_COMPRESSION) {
            // ToDO
            // Использование сжатия
        } else {
            // ToDO
            // Архивирование без сжатия
        }
    }
    else if (_list) {
        // ToDO
        // Отобразить список элементов
    }
    else if (_extract) {
        // ToDO
        // Разархивировать файл
    }

    return 0;
}