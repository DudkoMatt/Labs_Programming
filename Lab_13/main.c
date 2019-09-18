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

int main(int argc, char *argv[]) {
    // Редактор метаинформации mp3-файла

    // Часть 1: разбор аргументов строки

    
    // Часть 2: работа с файлом

    FILE *file;

    // Debug
    file = fopen("test.mp3", "rb+");

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
        show(file, version, length_of_header);

        printf("--------------------\n");
        get(file, version, length_of_header, "TPE2");

    } else {
        printf("It's not a valid ID3v2 mp3 file\n");
    }

    fclose(file);
    return 0;
}

void printnchars(int n, char * ptr){
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

void show(FILE *file, unsigned short version, unsigned int length_of_header){
    fseek(file, 10, SEEK_SET);
    if (version == 2) {
        // ID of frame is 3 chars in length
        Frame_v2 frame;
        fread(frame.bytes, 1, 6, file);
        while (length_of_header > 0 && (frame.info.id[0] != '\000')) {
            unsigned int l = sizeOfFramev2(frame);

            // Do not show image
            if (strcmp(frame.info.id, "APIC") == 0) {
                printf("%s: skipped\n", frame.info.id);
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
                printf("%s: skipped\n", frame.info.id);
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

void get(FILE *file, unsigned short version, unsigned int length_of_header, char* prop){
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