#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum OFFSETS {
    ALL_SIZE = 2,
    WIDTH = 18,
    HEIGHT = 22,
    IMG_SIZE = 34,
    PIXEL_DATA = 62
};

FILE* create_template(char*);

char *input_file_path;
char *output_directory;

int main(int argc, char *argv[]) {

    // Часть 1: разбор аргументов строки

    unsigned int _input = 0;
    unsigned int _output = 0;
    unsigned int _max_iter = 0;
    int max_iter = -1;
    unsigned int _dump_freq = 0;
    int dump_freq = 1;

    unsigned char flag = 0;

    for (int i = 1; i < argc; ++i) {
        if (flag) {
            flag = 0;
            continue;
        }
        if (strlen(argv[i]) > 2 && strncmp(argv[i], "--", 2) == 0) {
            if (strlen(argv[i]) == 7 && strncmp(argv[i], "--input", 7) == 0) {
                _input = 1;
                input_file_path = argv[i+1];
                flag = 1;
            } else if (strlen(argv[i]) == 8 && strncmp(argv[i], "--output", 8) == 0) {
                _output = 1;
                output_directory = argv[i+1];
                flag = 1;
            } else if (strlen(argv[i]) == 10 && strncmp(argv[i], "--max_iter", 10) == 0) {
                _max_iter = 1;
                max_iter = strtosigno(argv[i+1]);
                flag = 1;
            } else if (strlen(argv[i]) == 11 && strncmp(argv[i], "--dump_freq", 11) == 0) {
                _dump_freq = 1;
                dump_freq = strtosigno(argv[i+1]);
                flag = 1;
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

    if (!_input || !_output) {
        printf("Argument --input path_to_file is required\n");
        printf("Program requires at minimum 2 arguments.\nRequired arguments:\n1) --input path_to_file\n2) --output dir_name\n\nOptions:\n");
        printf("1) --max_iter N\n");
        printf("2) --dump_freq N\n");
        printf("Note: pass 'long' values with double quotes. Example: --value \"some long value\"");
        printf("Syntax:\n");
        printf("--input path_to_file --output dir_name [...]");
        return 0;
    }


    // Часть 2: чтение входного файла

    // Часть 3: эмулирование игры и вывод

    // Test
    // FILE *file = create_template("test.bmp");
    // fclose(file);
    return 0;
}

FILE* create_template(char *name){
    FILE* file = fopen(name, "wb");
    unsigned char template[] = {0x42, 0x4D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00,
                                0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
                                0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00};
    fwrite(template, 1, 62, file);
    return file;
}
