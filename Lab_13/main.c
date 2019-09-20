#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef union {
    unsigned char bytes[10];
    struct {
        unsigned char id3[3];
        unsigned char version[2];

        // Флаги
        // NB: для разных машин может быть неодинаковым!
        unsigned : 5;          // Младший бит
        unsigned flag_experimental: 1;
        unsigned flag_extended_header: 1;
        unsigned flag_unsync: 1; // Старший бит

        unsigned char size[4];
    } info;
} Header;

typedef union {
    unsigned char bytes[6];
    struct {
        unsigned char id[3];
        unsigned char size[3];
    } info;
} Frame_v2;

typedef union {
    unsigned char bytes[10];
    struct {
        unsigned char id[4];
        unsigned char size[4];
        unsigned char flags[2];
    } info;
} Frame;

void printnchars(int, char *);
unsigned int sizeOfMainHeader(Header);
unsigned int sizeOfFrame(Frame);
unsigned int sizeOfFramev2(Frame_v2);
void show(FILE*, unsigned short, unsigned int);
void get(FILE*, unsigned short, unsigned int, char*);
void set(FILE*, unsigned short, unsigned int, char*, char*);

char *file_path;

int main(int argc, char *argv[]) {
    // Редактор метаинформации mp3-файла

    // Часть 1: разбор аргументов строки

    unsigned int _show = 0;
    unsigned int _get = 0;
    char get_prop[5] = {};
    unsigned int _set = 0;
    char set_prop[5] = {};
    unsigned int _value = 0;
    char *set_val;
    unsigned int _path = 0;

    for (int i = 1; i < argc; ++i) {
        if (strlen(argv[i]) > 2 && strncmp(argv[i], "--", 2) == 0) {
            // Debug
            //  printf("%s\n", argv[i]);
            if (strlen(argv[i]) > 6 && strncmp(argv[i], "--get=", 6) == 0) {
                _get = 1;
                sscanf(argv[i], "--get=%s", get_prop);
            } else if (strlen(argv[i]) == 6 && strncmp(argv[i], "--show", 6) == 0) {
                _show = 1;
            } else if (strlen(argv[i]) > 6 && strncmp(argv[i], "--set=", 6) == 0) {
                _set = 1;
                sscanf(argv[i], "--set=%s", set_prop);
            } else if (strlen(argv[i]) > 8 && strncmp(argv[i], "--value=", 8) == 0) {
                // Mistake
                _value = 1;
                set_val = (char *) calloc(strlen(argv[i]) - 8, 1);
                sscanf(argv[i], "--value=%[^\t\n]", set_val);
            } else if (strlen(argv[i]) > 11 && strncmp(argv[i], "--filepath=", 11) == 0) {
                // Mistake
                _path = 1;
                file_path = (char *)calloc(strlen(argv[i]) - 11, 1);
                sscanf(argv[i], "--filepath=%[^\t\n]", file_path);
            } else {
                printf("Invalid argument: %s\n", argv[i]);
                return 0;
            }
        } else {
            printf("Invalid argument: %s\n", argv[i]);
            printf("If you want to pass a 'long' value use double quotes. Example:\n\"--value=some long value\"");
            return 0;
        }

    }

    if (!_path){
        printf("Argument --filepath=path_to_file is required\n");
        printf("Program requires at minimum 2 arguments.\nRequired argument: --filepath=path_to_file\nOptions:\n");
        printf("1) --show\n");
        printf("2) --get=prop\n");
        printf("3) --set=prop --value=prop_value\n");
        printf("Note: pass 'long' values with double quotes. Example:\"--value=some long value\"\n\n");
        printf("Syntax:\n");
        printf("--filepath=path_to_file [one_of_the_options]");
        return 0;
    }

    if (_show) {
        if (_get || _set || _value) {
            printf("Too many arguments\n");
            return 0;
        }
    } else if (_get) {
        if (_set || _value) {
            printf("Too many arguments\n");
            return 0;
        }
    } else if (_set) {
        if (!_value) {
            printf("Argument --set requires argument --value");
            return 0;
        }
    } else if (_value) {
        printf("Argument --value requires argument --set");
        return 0;
    } else {
        printf("Program requires at minimum 2 arguments.\nRequired argument: --filepath=path_to_file\nOptions:\n");
        printf("1) --show\n");
        printf("2) --get=prop\n");
        printf("3) --set=prop --value=prop_value\n\n");
        printf("Syntax:\n");
        printf("--filepath=path_to_file [one_of_the_options]");
    }

    // Часть 2: работа с файлом

    FILE *file;

    // Debug
    //  file = fopen("test.mp3", "rb+");
    file = fopen(file_path, "rb");

    // Читаем первые 10 байт файла
    // Если первые 3 - ID3
    // То это то, что нам нужно

    // Массив для первых 10 байт
    Header header;
    fread(header.bytes, 1, 10, file);

    // Заведомо обнуляем строку
    char s[4] = {};
    strncpy(s, header.info.id3, 3);

    unsigned int length_of_header = sizeOfMainHeader(header);

    // Пропуск Extended Header
    // Вопрос: Он входит в length_of_header?
    // Ответ: Да.
    if (header.info.flag_extended_header) {
        fread(header.bytes, 1, 10, file);
        length_of_header -= 10;
    }

//    Debug
//    int x = strcmp(s, "ID3");
//    printf("Compare: %d; header: %s; str: %s", x, header.info.id3, s);

    if (strcmp(s, "ID3") == 0) {
        // Parsing version
        int version = (int) header.info.version[0];

        // Debug
        printf("Version: %d\n", version);
//        show(file, version, length_of_header);
//
//        printf("--------------------\n");
//        get(file, version, length_of_header, "TPE2");

        if (_show) {
            show(file, version, length_of_header);
        } else if (_get) {
            get(file, version, length_of_header, get_prop);
        } else if (_set) {
            set(file, version, length_of_header, set_prop, set_val);
        }

    } else {
        printf("It's not a valid ID3v2 mp3 file\n");
    }

    fclose(file);
    return 0;
}

void printnchars(int n, char* ptr){
    for (int i = 0; i < n; ++i) {
        printf("%c", *(ptr + i));
    }
}

unsigned int sizeOfMainHeader(Header header){
    unsigned int x = 0;
    for (int i = 0; i < 4; ++i) {
        x = (x << 7) | (unsigned int)header.info.size[i];
        // То, что сдвиг на 7 - в спецификации
    }
    return x;
}

unsigned int sizeOfFrame(Frame frame){
    unsigned int x = 0;
    for (int i = 0; i < 4; ++i) {
        x = (x << 8) | frame.info.size[i];
    }
    return x;
}

unsigned int sizeOfFramev2(Frame_v2 frame){
    unsigned int x = 0;
    for (int i = 0; i < 3; ++i) {
        x = (x << 8) | frame.info.size[i];
    }
    return x;
}

void show(FILE* file, unsigned short version, unsigned int length_of_header){
    fseek(file, 10, SEEK_SET);
    if (version == 2) {
        // ID of frame is 3 chars in length
        Frame_v2 frame;
        fread(frame.bytes, 1, 6, file);
        while (length_of_header > 0 && (frame.info.id[0] != '\000')) {
            unsigned int l = sizeOfFramev2(frame);

            // Do not show image
            if (strcmp(frame.info.id, "APIC") == 0) {
                printf("%s: picture is skipped\n", frame.info.id);
                fseek(file, l, SEEK_CUR);
                length_of_header -= l + 6;
                fread(frame.bytes, 1, 6, file);
                continue;
            }

            char *data;
            data = calloc(l, sizeof(char));
            fread(data, 1, l, file);

            printf("%s: ", frame.info.id);
            printnchars(l, data);
            printf("\n");
            free(data);

            length_of_header -= l + 6;
            fread(frame.bytes, 1, 6, file);
        }
    } else if (version == 3 || version == 4) {
        // ID of frame is 4 chars in length
        Frame frame;
        fread(frame.bytes, 1, 10, file);
        while (length_of_header > 0 && (frame.info.id[0] != '\000')) {
            unsigned int l = sizeOfFrame(frame);

            // Do not show image
            if (strcmp(frame.info.id, "APIC") == 0) {
                printf("%s: picture is skipped\n", frame.info.id);
                fseek(file, l, SEEK_CUR);
                length_of_header -= l + 10;
                fread(frame.bytes, 1, 10, file);
                continue;
            }

            char *data;
            data = calloc(l, sizeof(char));
            fread(data, 1, l, file);

            printf("%s: ", frame.info.id);
            printnchars(l, data);
            printf("\n");
            free(data);

            length_of_header -= l + 10;
            fread(frame.bytes, 1, 10, file);
        }
    } else {
        printf("It's not a valid ID3v2 mp3 file");
    }
}

void get(FILE* file, unsigned short version, unsigned int length_of_header, char* prop){
    fseek(file, 10, SEEK_SET);
    if (version == 2){
        Frame_v2 frame;
        fread(frame.bytes, 1, 6, file);
        while (length_of_header > 0 && (frame.info.id[0] != '\000')) {
            unsigned int l = sizeOfFramev2(frame);

            // ToDo: seek if not
            if (strcmp(frame.info.id, prop) == 0){
                char *data;
                data = calloc(l, sizeof(char));
                fread(data, 1, l, file);
                printf("%s: ", frame.info.id);
                printnchars(l, data);
                printf("\n");
                free(data);
                return;
            } else {
                fseek(file, l, SEEK_CUR);
            }

            length_of_header -= l + 6;
            fread(frame.bytes, 1, 6, file);
        }
    } else if (version == 3 || version == 4) {
        Frame frame;
        fread(frame.bytes, 1, 10, file);
        while (length_of_header > 0 && (frame.info.id[0] != '\000')) {

            unsigned int l = sizeOfFrame(frame);

            // ToDo: seek if not
            if (strcmp(frame.info.id, prop) == 0){
                char *data;
                data = calloc(l, sizeof(char));
                fread(data, 1, l, file);
                printf("%s: ", frame.info.id);
                printnchars(l, data);
                printf("\n");
                free(data);
                return;
            } else {
                fseek(file, l, SEEK_CUR);
            }

            length_of_header -= l + 10;
            fread(frame.bytes, 1, 10, file);
        }
    } else {
        printf("It's not a valid ID3v2 mp3 file");
    }
}

void set(FILE* file, unsigned short version, unsigned int length_of_header, char* prop, char* value){
    char* tmp_filename;
    tmp_filename = (char *)calloc(strlen(file_path) + 4, 1);
    strcat(tmp_filename, file_path);
    strcat(tmp_filename, ".tmp");
    FILE* tmp_file = fopen(tmp_filename, "wb+");

    // Debug
    printf("Temp filename: %s\n", tmp_filename);

    printf("Prop: %s\n", prop);
    printf("Value: %s\n", value);

    // Writing header
    fseek(file, 0, SEEK_SET);
    Header header;
    fread(header.bytes, 1, 10, file);
    fwrite(header.bytes, 1, 10, tmp_file);

    unsigned long bytes_written = 0; // excluding main header

    unsigned short modified = 0;

    if (version == 2){
        Frame_v2 frame;
        fread(frame.bytes, 1, 6, file);

        while (length_of_header > 0 && (frame.info.id[0] != '\000')) {
            unsigned int l = sizeOfFramev2(frame);

            if (strcmp(frame.info.id, prop) == 0){
                // Replace prop
                modified = 1;
                // Write header
                bytes_written += 3;
                fwrite(frame.info.id, 1, 3, tmp_file);

                // Write new size
                unsigned long len = strlen(value);
                char write_size[] = {len & 0xff0000, len & 0xff00, len & 0xff};
                bytes_written += 3;
                fwrite(write_size, 1, 3, tmp_file);

                // Write value
                bytes_written += len;
                fwrite(value, 1, len, tmp_file);

                fseek(file, l, SEEK_CUR);
            } else {
                // Just write
                fwrite(frame.bytes, 1, 6, tmp_file);
                bytes_written += 6;
                char *data;
                data = calloc(l, sizeof(char));
                fread(data, 1, l, file);
                fwrite(data, 1, l, tmp_file);
                bytes_written += l;
                free(data);
            }

            length_of_header -= l + 6;
            fread(frame.bytes, 1, 6, file);
        }

        // Если такого значения не было, создадим его
        if (!modified) {
            // Write new prop
            bytes_written += 3;
            fwrite(prop, 1, 3, tmp_file);

            // Write size
            unsigned long len = strlen(value);
            char write_size[] = {len & 0xff0000, len & 0xff00, len & 0xff};
            bytes_written += 3;
            fwrite(write_size, 1, 3, tmp_file);

            // Write value
            bytes_written += len;
            fwrite(value, 1, len, tmp_file);
        }

        // Дописываем остальную часть
        int c;
        while (!feof(file)){                        // while not end of file
            c = fgetc(file);                        // get a byte from the file
            fputc(c, tmp_file);
        }

        // Completed writing file

        // Calculating new size
        fseek(tmp_file, 6, SEEK_SET);

        char header_size[] = {
                ((bytes_written << 3) & 0x7f000000),
                ((bytes_written << 2) & 0x7f0000),
                ((bytes_written << 1) & 0x7f00),
                (bytes_written & 0x7f)
        };

        fwrite(header_size, 1, 4,tmp_file);


    } else if (version == 3 || version == 4) {


        Frame frame;
        fread(frame.bytes, 1, 10, file);
        while (length_of_header > 0 && (frame.info.id[0] != '\000')) {
            unsigned int l = sizeOfFrame(frame);

            if (strcmp(frame.info.id, prop) == 0){
                // Replace prop
                modified = 1;
                // Write header
                bytes_written += 4;
                fwrite(frame.info.id, 1, 4, tmp_file);

                // Write new size
                unsigned long len = strlen(value);
                char write_size[] = {len & 0xff000000, len & 0xff0000, len & 0xff00, len & 0xff};
                bytes_written += 4;
                fwrite(write_size, 1, 4, tmp_file);

                // Write flags
                char flags[] = {0x0, 0x0};
                bytes_written += 2;
                fwrite(flags, 1, 2, tmp_file);

                // Write value
                bytes_written += len;
                fwrite(value, 1, len, tmp_file);

                fseek(file, l, SEEK_CUR);
            } else {
                // Just write
                fwrite(frame.bytes, 1, 10, tmp_file);
                bytes_written += 10;
                char *data;
                data = calloc(l, sizeof(char));
                fread(data, 1, l, file);
                fwrite(data, 1, l, tmp_file);
                bytes_written += l;
                free(data);
            }

            length_of_header -= l + 10;
            fread(frame.bytes, 1, 10, file);
        }

        // Если такого значения не было, создадим его
        if (!modified) {
            // Write new prop
            bytes_written += 4;
            fwrite(prop, 1, 4, tmp_file);

            // Write size
            unsigned long len = strlen(value);
            char write_size[] = {len & 0xff000000, len & 0xff0000, len & 0xff00, len & 0xff};
            fwrite(write_size, 1, 4, tmp_file);
            bytes_written += 4;

            // Write flags
            char flags[] = {0x0, 0x0};
            bytes_written += 2;
            fwrite(flags, 1, 2, tmp_file);

            // Write value
            bytes_written += len;
            fwrite(value, 1, len, tmp_file);
        }

        // Дописываем остальную часть

        int c;
        while (!feof(file)){                        // while not end of file
            c = fgetc(file);                        // get a byte from the file
            fputc(c, tmp_file);
        }


        // Completed writing header

        // Calculating new size

        char header_size[] = {
                ((bytes_written << 3) & 0x7f000000),
                ((bytes_written << 2) & 0x7f0000),
                ((bytes_written << 1) & 0x7f00),
                (bytes_written & 0x7f)
        };

        fwrite(header_size, 1, 4,tmp_file);


    } else {
        printf("It's not a valid ID3v2 mp3 file");
    }


    fclose(file);
    fclose(tmp_file);
    remove(file_path);
    rename(tmp_filename, file_path);
}
