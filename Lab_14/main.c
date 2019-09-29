#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

enum OFFSETS {
    ALL_SIZE = 2,
    WIDTH = 18,
    HEIGHT = 22,
    IMG_SIZE = 34,
    PIXEL_DATA = 62
};

FILE* create_template(char*, long);
long get_height();
long get_width();
void write_row(unsigned char *row_array, long width, long number, FILE*);
void read_row(unsigned char*, long, long);
void update_bmp_metadata(FILE*, long, long);
int count_alive_near(unsigned long x, unsigned long y, unsigned long height, unsigned long width, unsigned char a[][width]);

char *input_file_path;
FILE *input_file;
char *output_directory;

unsigned long MAX_ITERATIONS = 1000;

long row_size = 0;

int main(int argc, char *argv[]) {

    // Часть 1: Разбор аргументов строки

    unsigned char _input = 0;
    unsigned char _output = 0;
    unsigned char _max_iter = 0;
    int max_iter = -1;
    unsigned char _dump_freq = 0;
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
        printf("Arguments --input path_to_file and --output dir_name are required\n");
        printf("Options:\n");
        printf("1) --max_iter N\n");
        printf("2) --dump_freq N\n");
        printf("Note: pass 'long' values with double quotes. Example: --value \"some long value\"");
        printf("Syntax:\n");
        printf("--input path_to_file --output dir_name [...]");
        return 0;
    }

    // Часть 2: Проверка существования папки и открытие входного файла
    mkdir(output_directory, 0777);
    input_file = fopen(input_file_path, "rb");

    // Часть 3: чтение входного файла
    long height = get_height();
    long width = get_width();

    row_size = ((width + 31) / 32) * 4;

    unsigned char pixel_array[height][width];
    unsigned char new_pixel_array[height][width];

    for (long i = 0; i < height; ++i) {
        read_row(pixel_array[i], width, i);
    }

    fclose(input_file);

    // Часть 4: эмулирование игры и вывод

    // Смена директории
    chdir(output_directory);

    unsigned long current_iteration = 0;

    while (current_iteration <= MAX_ITERATIONS && (current_iteration < max_iter || !_max_iter)) {

        // Эмулирование

        // ToDo

        // Вывод
        if (current_iteration + 1 % dump_freq == 0 || !_dump_freq) {

            // Create new file and write
            char name[4] = {};
            sprintf(name, "%lu", current_iteration);
            FILE *output_file = create_template(strcat(name, ".bmp"), height);
            for (long i = 0; i < height; ++i) {
                write_row(pixel_array[i], width, i, output_file);
            }
            update_bmp_metadata(output_file, height, width);
            fclose(output_file);
        }
        current_iteration++;
    }

    // Возврат в исходную директорию
    chdir("..");
    return 0;
}

FILE* create_template(char *name, long height){
    FILE* file = fopen(name, "wb");
    unsigned char template[] = {0x42, 0x4D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00,
                                0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
                                0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00};
    fwrite(template, 1, 62, file);
    for (long i = 0; i < row_size * height; ++i) {
        fwrite("\0", 1, 1, file);
    }
    return file;
}

long get_height(){
    long x = 0;
    fseek(input_file, HEIGHT, SEEK_SET);
    fread(&x, 4, 1, input_file);
    return x;
}

long get_width(){
    long x = 0;
    fseek(input_file, WIDTH, SEEK_SET);
    fread(&x, 4, 1, input_file);
    return x;
}

void read_row(unsigned char *row_array, long width, long row_number) {
    unsigned char *data = calloc(1, row_size);
    fseek(input_file, PIXEL_DATA + row_number * row_size, SEEK_SET);
    fread(data, 1, row_size, input_file);

    // Debug
//    for (long i = 0; i < row_size; ++i) {
//        printf("%02X ", data[i]);
//    }
//    printf("\n");

    for (unsigned long i = 0; i < width; ++i) {
        *(row_array + i) = (data[i / 8] & (0x1u << (7u - i % 8))) >> (7u - i % 8);
    }
    free(data);
}

void write_row(unsigned char *row_array, long width, long row_number, FILE *output_file){
    unsigned char *data = calloc(1, row_size);
    for (unsigned long i = 0; i < width; ++i) {
        data[i / 8] = data[i / 8] | ( *(row_array + i) << (7u - i % 8));
    }
    fseek(output_file, PIXEL_DATA + row_number * row_size, SEEK_SET);
    fwrite(data, 1, row_size, output_file);
}

void update_bmp_metadata(FILE *output_file, long height, long width){
    fseek(output_file, ALL_SIZE, SEEK_SET);
    unsigned long size = 14 + 40 + 8 + row_size * height;
    fwrite(&size, 4, 1, output_file);

    fseek(output_file, WIDTH, SEEK_SET);
    fwrite(&width, 4, 1, output_file);

    fseek(output_file, HEIGHT, SEEK_SET);
    fwrite(&height, 4, 1, output_file);

    fseek(output_file, IMG_SIZE, SEEK_SET);
    size = size - PIXEL_DATA;
}

int count_alive_near(unsigned long x, unsigned long y, unsigned long height, unsigned long width, unsigned char a[][width]){
    // Поле зацикловано
    unsigned long up = (x - 1) < 0 ? height - 1 : x - 1;
    unsigned long down = (x + 1) >= height ? 0 : x + 1;
    unsigned long left = (y - 1) < 0 ? width - 1 : y - 1;
    unsigned long right = (y + 1) >= width ? 0 : y + 1;
    return a[up][left] + a[up][y] + a[up][right] + a[x][left] + a[x][right] + a[down][left] + a[down][y] + a[down][right];
}