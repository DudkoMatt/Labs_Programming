#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char USE_COMPRESSION = 0;
// В теории можно добавить дополнительный аргумент для смены этого флага
// Изначально


FILE* create_template(char*, int);
int read_file_and_write_to_archive(FILE*, char*);
void extract_archive(char*);
void list_files(char*);

int main(int argc, char *argv[]) {

    // Архиватор файлов
    // Limit to file size: MAXIMUM a bit less than 2Gb

    // Часть 1: Разбор аргументов строки

    unsigned char _file = 0;
    char* name_of_file = "";
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
            // Архивирование без сжатия
            FILE* archive = create_template(name_of_file, argc - index_of_first_file);
            for (int i = index_of_first_file; i < argc; ++i) {
                read_file_and_write_to_archive(archive, argv[i]);
            }
            fclose(archive);
        }
    }
    else if (_list) {
        // ToDO: сжатый архив
        // Отобразить список элементов
        list_files(name_of_file);
    }
    else if (_extract) {
        // ToDO: сжатый архив
        // Разархивировать файл
        extract_archive(name_of_file);
    }

    return 0;
}

FILE* create_template(char* name_of_file, int number_of_files){
    // Note: уже файл созданный до этого будет перезаписан
    FILE* archive = fopen(name_of_file, "wb");

    fwrite("ARC", 1, 3, archive);
    fwrite(&USE_COMPRESSION, 1, 1, archive);

    if (USE_COMPRESSION) {
        // ToDO
    }

    // Количество файлов
    fwrite(&number_of_files, sizeof(int), 1, archive);

    return archive;
}

int read_file_and_write_to_archive(FILE* archive, char* name_of_file) {
    FILE* read_file = fopen(name_of_file, "rb");
    if (read_file == NULL) {
        printf("Error when opening file \"%s\", skipping\n", name_of_file);
        return 1;
    }

    // Запись имени файла
    for (unsigned long i = 0; i < strlen(name_of_file); ++i) {
        char buffer = *(name_of_file + i);
        fwrite(&buffer, 1, 1, archive);
    }

    char zero = 0x0;
    unsigned char max_char = 0xFF;
    fwrite(&max_char, 1, 1, archive);

    unsigned long long bytes_written = 0;
    long point_in_file_for_size = ftell(archive);
    fwrite(&zero, sizeof(unsigned long long), 1, archive);

    char buffer;
    while (fread(&buffer, 1, 1, read_file) != 0) {
        fwrite(&buffer, 1, 1, archive);
        bytes_written += 1;
    }

    long last_point_in_file = ftell(archive);
    fseek(archive, point_in_file_for_size, SEEK_SET);
    fwrite(&bytes_written, sizeof(unsigned long long), 1, archive);
    fseek(archive, last_point_in_file, SEEK_SET);

    return 0;
}

void extract_archive(char* name_of_archive){
    FILE* archive = fopen(name_of_archive, "rb");
    char* buffer = calloc(1, 3);
    fread(buffer, 1, 3, archive);
    if (strcmp(buffer, "ARC") != 0) {
        printf("This is not supportable file\n");
        return;
    }
    free(buffer);
    buffer = calloc(1, 1);
    fread(buffer, 1, 1, archive);
    if (*buffer == 1) {
        // ToDO: С использованием сжатия
    } else {
        // Без использования сжатия
        int number_of_files = 0;
        fread(&number_of_files, sizeof(int), 1, archive);
        unsigned long long k;
        char* buffer_name;
        unsigned char buffer_byte;
        FILE* writing_file;
        unsigned long long size_of_file;
        for (int i = 0; i < number_of_files; ++i) {
            k = 1;
            buffer_name = calloc(1, 1);
            *buffer_name = '\0';
            buffer_byte = 0;
            fread(&buffer_byte, 1, 1, archive);
            while (buffer_byte != 0xFF) {
                realloc(buffer_name, k);
                *(buffer_name + k - 1) = buffer_byte;
                fread(&buffer_byte, 1, 1, archive);
                k++;
            }
            *(buffer_name + k - 1) = '\0';
            writing_file = fopen((const char*)buffer_name, "wb");
            free(buffer_name);
            size_of_file = 0;
            fread(&size_of_file, sizeof(unsigned long long), 1, archive);
            for (unsigned long long j = 0; j < size_of_file; ++j) {
                fread(&buffer_byte, 1, 1, archive);
                fwrite(&buffer_byte, 1, 1, writing_file);
            }
            fclose(writing_file);
        }
    }
}

void list_files(char* name_of_archive){
    FILE* archive = fopen(name_of_archive, "rb");
    char* buffer = calloc(1, 3);
    fread(buffer, 1, 3, archive);
    if (strcmp(buffer, "ARC") != 0) {
        printf("This is not supportable file\n");
        return;
    }
    free(buffer);
    buffer = calloc(1, 1);
    fread(buffer, 1, 1, archive);

    printf("List of files:\n");

    if (*buffer == 1) {
        // ToDO: С использованием сжатия
    } else {
        // Без использования сжатия
        int number_of_files = 0;
        fread(&number_of_files, sizeof(int), 1, archive);
        unsigned long long k;
        char* buffer_name;
        unsigned char buffer_byte;
        unsigned long long size_of_file;

        printf("Amount of files: %d\n", number_of_files);
        for (int i = 0; i < number_of_files; ++i) {
            k = 1;
            buffer_name = calloc(1, 1);
            *buffer_name = '\0';
            buffer_byte = 0;
            fread(&buffer_byte, 1, 1, archive);
            while (buffer_byte != 0xFF) {
                realloc(buffer_name, k);
                *(buffer_name + k - 1) = buffer_byte;
                fread(&buffer_byte, 1, 1, archive);
                k++;
            }
            *(buffer_name + k - 1) = '\0';
            printf("%s\n", buffer_name);
            free(buffer_name);
            size_of_file = 0;
            fread(&size_of_file, sizeof(unsigned long long), 1, archive);
            // Note: maximum long
            fseek(archive, size_of_file, SEEK_CUR);
        }
    }
}