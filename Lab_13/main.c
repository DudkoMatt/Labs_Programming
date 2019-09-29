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
        // Not used
        unsigned : 5;          // Младший бит
        unsigned flag_experimental: 1;
        unsigned flag_extended_header: 1;
        unsigned flag_unsync: 1; // Старший бит

        unsigned char size[4];
    } info;
} Header;

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

void show(FILE *, unsigned int);

void get(FILE *, unsigned int, char *);

void set(FILE *, unsigned int, char *, char *);

unsigned long calcPlayCount(unsigned int, char*);

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
            if (strlen(argv[i]) > 6 && strncmp(argv[i], "--get=", 6) == 0) {
                _get = 1;
                sscanf(argv[i], "--get=%s", get_prop);
            } else if (strlen(argv[i]) == 6 && strncmp(argv[i], "--show", 6) == 0) {
                _show = 1;
            } else if (strlen(argv[i]) > 6 && strncmp(argv[i], "--set=", 6) == 0) {
                _set = 1;
                sscanf(argv[i], "--set=%s", set_prop);
            } else if (strlen(argv[i]) > 8 && strncmp(argv[i], "--value=", 8) == 0) {
                _value = 1;
                set_val = (char *) calloc(strlen(argv[i]) - 8, 1);
                sscanf(argv[i], "--value=%[^\t\n]", set_val);
            } else if (strlen(argv[i]) > 11 && strncmp(argv[i], "--filepath=", 11) == 0) {
                _path = 1;
                file_path = (char *) calloc(strlen(argv[i]) - 11, 1);
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

    if (!_path) {
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

    // Check if file opens
    file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Cannot open file\n");
        return 0;
    }

    // Читаем первые 10 байт файла
    // Если первые 3 - ID3
    // То это то, что нам нужно

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

    if (strcmp(s, "ID3") == 0) {
        // Parsing version
        int version = (int) header.info.version[0];

        if (version != 3) {
            printf("This is ID3v2 file, but not ID3v2.3");
            return 0;
        }

        if (_show) {
            show(file, length_of_header);
        } else if (_get) {
            get(file, length_of_header, get_prop);
        } else if (_set) {
            set(file, length_of_header, set_prop, set_val);
        }

    } else {
        printf("It's not a valid ID3v2 mp3 file\n");
    }

    fclose(file);
    return 0;
}

void printnchars(int n, char *ptr) {
    for (int i = 0; i < n; ++i) {
        printf("%c", *(ptr + i));
    }
}

unsigned int sizeOfMainHeader(Header header) {
    unsigned int x = 0;
    for (int i = 0; i < 4; ++i) {
        x = (x << 7u) | (unsigned int) header.info.size[i];
        // То, что сдвиг на 7 - в спецификации
    }
    return x;
}

unsigned int sizeOfFrame(Frame frame) {
    unsigned int x = 0;
    for (int i = 0; i < 4; ++i) {
        x = (x << 8u) | frame.info.size[i];
    }
    return x;
}

void show(FILE *file, unsigned int length_of_header) {
    fseek(file, 10, SEEK_SET);

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

        // Handling frames

        if (strcmp(frame.info.id, "UFID") == 0) {
            // Unique file identifier
            long len_first_part = data - strchr(data, '\0');
            printf("\n|\n---> Owner identifier: ");
            printnchars(len_first_part, data);
            printf("\n|\n---> Identifier: ");
            printnchars(l - len_first_part - 1, data + len_first_part + 2);
            printf("\n");
        } else if (strncmp(frame.info.id, "T", 1) == 0) {
            // Text info
            if (strcmp(frame.info.id, "TXXX") != 0) {

                if ((int) data[0] == 0) {
                    printf("ISO-8859-1: ");
                } else {
                    printf("Unicode: ");
                }
                data++;
                // Skip first byte encoding
                printnchars(l - 1, data);
                printf("\n");

            } else {
                short _unicode = 0;
                // User defined text info
                if ((int) data[0] == 0) {
                    printf("ISO-8859-1: ");
                } else {
                    printf("Unicode: ");
                    _unicode = 1;
                }
                data++;
                // Skip first byte encoding
                long len_first_part = data - strchr(data, '\0');
                printf("\n|\n---> Description: ");
                printnchars(len_first_part, data);
                printf("\n|\n---> Value: ");
                printnchars(l - len_first_part - 1 - _unicode, data + len_first_part + 1 + _unicode);
                printf("\n");
            }
        } else if (strncmp(frame.info.id, "W", 1) == 0) {
            if (strcmp(frame.info.id, "WXXX") != 0) {
                printnchars(l, data);
            } else {
                short _unicode = 0;
                if ((int) data[0] == 0) {
                    printf("ISO-8859-1: ");
                } else {
                    printf("Unicode: ");
                    _unicode = 1;
                }
                data++;
                // Skip first byte encoding
                long len_first_part = data - strchr(data + 1, '\0');
                printf("\n|\n---> Description: ");
                printnchars(len_first_part, data);
                printf("\n|\n---> URL: ");
                printnchars(l - len_first_part - 1 - _unicode, data + len_first_part + 1 + _unicode);
                printf("\n");
            }
        } else if (strcmp(frame.info.id, "IPLS") == 0) {
            short _unicode = 0;
            if ((int) data[0] == 0) {
                printf("ISO-8859-1: ");
            } else {
                printf("Unicode: ");
                _unicode = 1;
            }
            data++;
            // Skip first byte encoding
            printnchars(l - 1, data);
            printf("\n");
        } else if (strcmp(frame.info.id, "MCDI") == 0) {
            printf("CD TOC <binary data, skipped>\n");
        } else if (strcmp(frame.info.id, "ETCO") == 0) {
            printf("Event timing codes <skipped>\n");
        } else if (strcmp(frame.info.id, "MLLT") == 0) {
            printf("Location lookup table <skipped>\n");
        } else if (strcmp(frame.info.id, "SYTC") == 0) {
            printf("Synchronised tempo codes <binary data, skipped>\n");
        } else if (strcmp(frame.info.id, "USLT") == 0) {
            printf("Unsynchronised lyrics/text transcription <skipped>\n");
        } else if (strcmp(frame.info.id, "SYLT") == 0) {
            printf("Synchronised lyrics/text <skipped>\n");
        } else if (strcmp(frame.info.id, "COMM") == 0) {
            // _ToDO - done?
            if ((int) data[0] == 0) {
                printf("ISO-8859-1: ");
                data++;
                printf("\n|\n---> Language: %c%c%c", data[0], data[1], data[2]);
                data += 3;
                printf("\n|\n---> Content: ");
                printnchars(l - 4, data);
                printf("\n");
            } else {
                printf("Unicode, skipping\n");
            }
        } else if (strcmp(frame.info.id, "RVAD") == 0) {
            printf("Relative volume adjustment <skipped>\n");
        } else if (strcmp(frame.info.id, "EQUA") == 0) {
            printf("Equalisation <skipped>\n");
        } else if (strcmp(frame.info.id, "RVRB") == 0) {
            // _ToDo?
            printf("Reverb <skipped>\n");
        } else if (strcmp(frame.info.id, "APIC") == 0) {
            // Already skipped in the beginning
        } else if (strcmp(frame.info.id, "GEOB") == 0) {
            printf("General encapsulated object <skipped>\n");
        } else if (strcmp(frame.info.id, "PCNT") == 0) {
            // ToDO!
            // ToDO: check if it works
            printf("Play count: %ld", calcPlayCount(l, data));
        } else if (strcmp(frame.info.id, "POPM") == 0) {
            printf("Popularimeter <skipped>\n");
        } else if (strcmp(frame.info.id, "RBUF") == 0) {
            printf("Recommended buffer size <skipped>\n");
        } else if (strcmp(frame.info.id, "AENC") == 0) {
            printf("Audio encryption <skipped>\n");
        } else if (strcmp(frame.info.id, "LINK") == 0) {
            // Hm
            printf("Linked information: ");
            printnchars(l, data);
        } else if (strcmp(frame.info.id, "POSS") == 0) {
            printf("Position synchronisation <skipped>\n");
        } else if (strcmp(frame.info.id, "USER") == 0) {
            printf("Terms of use frame: ");
            printnchars(l - 4, data + 4);
            printf("\n");
        } else if (strcmp(frame.info.id, "OWNE") == 0) {
            printf("Ownership frame: ");
            printnchars(l - 1, data + 1);
            printf("\n");
        } else if (strcmp(frame.info.id, "COMR") == 0) {
            printf("Commercial frame <skipped>\n");
        } else if (strcmp(frame.info.id, "ENCR") == 0) {
            printf("Encryption method registration <skipped>\n");
        } else if (strcmp(frame.info.id, "GRID") == 0) {
            printf("Group ID registration <binary data, skipped>\n");
        } else if (strcmp(frame.info.id, "PRIV") == 0) {
            printf("Private frame <binary data, skipped>\n");
        }
        else {

            printnchars(l, data);
            printf("\n");

        }

        free(data);
        length_of_header -= l + 10;
        fread(frame.bytes, 1, 10, file);
    }
}

void get(FILE *file, unsigned int length_of_header, char *prop) {
    fseek(file, 10, SEEK_SET);
    Frame frame;
    fread(frame.bytes, 1, 10, file);
    while (length_of_header > 0 && (frame.info.id[0] != '\000')) {

        unsigned int l = sizeOfFrame(frame);

        // Print if it is prop
        // Seek if not
        if (strcmp(frame.info.id, prop) == 0) {
            char *data;
            data = calloc(l, sizeof(char));
            fread(data, 1, l, file);
            printf("%s: ", frame.info.id);


            if (strcmp(prop, "UFID") == 0) {
                // Unique file identifier
                long len_first_part = data - strchr(data, '\0');
                printf("\n|\n---> Owner identifier: ");
                printnchars(len_first_part, data);
                printf("\n|\n---> Identifier: ");
                printnchars(l - len_first_part - 1, data + len_first_part + 2);
                printf("\n");
            } else if (strncmp(prop, "T", 1) == 0) {
                // Text info
                if (strcmp(prop, "TXXX") != 0) {

                    if ((int) data[0] == 0) {
                        printf("ISO-8859-1: ");
                    } else {
                        printf("Unicode: ");
                    }
                    data++;
                    // Skip first byte encoding
                    printnchars(l - 1, data);
                    printf("\n");

                } else {
                    short _unicode = 0;
                    // User defined text info
                    if ((int) data[0] == 0) {
                        printf("ISO-8859-1: ");
                    } else {
                        printf("Unicode: ");
                        _unicode = 1;
                    }
                    data++;
                    // Skip first byte encoding
                    long len_first_part = data - strchr(data, '\0');
                    printf("\n|\n---> Description: ");
                    printnchars(len_first_part, data);
                    printf("\n|\n---> Value: ");
                    printnchars(l - len_first_part - 1 - _unicode, data + len_first_part + 1 + _unicode);
                    printf("\n");
                }
            } else if (strncmp(prop, "W", 1) == 0) {
                if (strcmp(prop, "WXXX") != 0) {
                    printnchars(l, data);
                } else {
                    short _unicode = 0;
                    if ((int) data[0] == 0) {
                        printf("ISO-8859-1: ");
                    } else {
                        printf("Unicode: ");
                        _unicode = 1;
                    }
                    data++;
                    // Skip first byte encoding
                    long len_first_part = data - strchr(data + 1, '\0');
                    printf("\n|\n---> Description: ");
                    printnchars(len_first_part, data);
                    printf("\n|\n---> URL: ");
                    printnchars(l - len_first_part - 1 - _unicode, data + len_first_part + 1 + _unicode);
                    printf("\n");
                }
            } else if (strcmp(prop, "IPLS") == 0) {
                short _unicode = 0;
                if ((int) data[0] == 0) {
                    printf("ISO-8859-1: ");
                } else {
                    printf("Unicode: ");
                    _unicode = 1;
                }
                data++;
                // Skip first byte encoding
                printnchars(l - 1, data);
                printf("\n");
            } else if (strcmp(prop, "MCDI") == 0) {
                printf("CD TOC <binary data, skipped>\n");
            } else if (strcmp(prop, "ETCO") == 0) {
                printf("Event timing codes <skipped>\n");
            } else if (strcmp(prop, "MLLT") == 0) {
                printf("Location lookup table <skipped>\n");
            } else if (strcmp(prop, "SYTC") == 0) {
                printf("Synchronised tempo codes <binary data, skipped>\n");
            } else if (strcmp(prop, "USLT") == 0) {
                printf("Unsynchronised lyrics/text transcription <skipped>\n");
            } else if (strcmp(prop, "SYLT") == 0) {
                printf("Synchronised lyrics/text <skipped>\n");
            } else if (strcmp(prop, "COMM") == 0) {
                // _ToDO - done?
                if ((int) data[0] == 0) {
                    printf("ISO-8859-1: ");
                    data++;
                    printf("\n|\n---> Language: %c%c%c", data[0], data[1], data[2]);
                    data += 3;
                    printf("\n|\n---> Content: ");
                    printnchars(l - 4, data);
                    printf("\n");
                } else {
                    printf("Unicode, skipping\n");
                }
            } else if (strcmp(prop, "RVAD") == 0) {
                printf("Relative volume adjustment <skipped>\n");
            } else if (strcmp(prop, "EQUA") == 0) {
                printf("Equalisation <skipped>\n");
            } else if (strcmp(prop, "RVRB") == 0) {
                // _ToDo?
                printf("Reverb <skipped>\n");
            } else if (strcmp(prop, "APIC") == 0) {
                // Already skipped in the beginning
            } else if (strcmp(prop, "GEOB") == 0) {
                printf("General encapsulated object <skipped>\n");
            } else if (strcmp(prop, "PCNT") == 0) {
                // ToDO: check if it works
                printf("Play count: %ld", calcPlayCount(l, data));
            } else if (strcmp(prop, "POPM") == 0) {
                printf("Popularimeter <skipped>\n");
            } else if (strcmp(prop, "RBUF") == 0) {
                printf("Recommended buffer size <skipped>\n");
            } else if (strcmp(prop, "AENC") == 0) {
                printf("Audio encryption <skipped>\n");
            } else if (strcmp(prop, "LINK") == 0) {
                // Hm
                printf("Linked information: ");
                printnchars(l, data);
            } else if (strcmp(prop, "POSS") == 0) {
                printf("Position synchronisation <skipped>\n");
            } else if (strcmp(prop, "USER") == 0) {
                printf("Terms of use frame: ");
                printnchars(l - 4, data + 4);
                printf("\n");
            } else if (strcmp(prop, "OWNE") == 0) {
                printf("Ownership frame: ");
                printnchars(l - 1, data + 1);
                printf("\n");
            } else if (strcmp(prop, "COMR") == 0) {
                printf("Commercial frame <skipped>\n");
            } else if (strcmp(prop, "ENCR") == 0) {
                printf("Encryption method registration <skipped>\n");
            } else if (strcmp(prop, "GRID") == 0) {
                printf("Group ID registration <binary data, skipped>\n");
            } else if (strcmp(prop, "PRIV") == 0) {
                printf("Private frame <binary data, skipped>\n");
            }
            else {
                printnchars(l, data);
                printf("\n");
            }
            

            free(data);
            return;
        } else {
            fseek(file, l, SEEK_CUR);
        }

        length_of_header -= l + 10;
        fread(frame.bytes, 1, 10, file);
    }
    printf("Prop: %s not found", prop);
}

void set(FILE *file, unsigned int length_of_header, char *prop, char *value) {
    char *tmp_filename;
    tmp_filename = (char *) calloc(strlen(file_path) + 4, 1);
    strcat(tmp_filename, file_path);
    strcat(tmp_filename, ".tmp");
    FILE *tmp_file = fopen(tmp_filename, "wb+");

    // Debug
    // ToDo: remove
    printf("Temp filename: %s\n", tmp_filename);
    printf("Prop: %s\n", prop);
    printf("Value: %s\n", value);

    // Writing main header
    fseek(file, 0, SEEK_SET);
    Header header;
    fread(header.bytes, 1, 10, file);
    fwrite(header.bytes, 1, 10, tmp_file);

    unsigned long bytes_written = 0; // excluding main header

    unsigned short modified = 0;

    Frame frame;
    fread(frame.bytes, 1, 10, file);
    while (length_of_header > 0 && (frame.info.id[0] != '\000')) {
        unsigned int l = sizeOfFrame(frame);

        if (strcmp(frame.info.id, prop) == 0) {
            // Replace prop
            modified = 1;
            // Write header
            bytes_written += 4;
            fwrite(frame.info.id, 1, 4, tmp_file);

            // Write new size
            unsigned long len = strlen(value);
            unsigned long _len = len;
            // Separate each header
            // Specify each size

            if (strncmp(prop, "T", 1) == 0) {
                // Text info
                if (strcmp(prop, "TXXX") != 0) {
                    len += 2;
                } else {
                    // Do not do anything
                    printf("Trying to wtite TXXX: do not do anything\n");
                    return;
                }
            } else if (strncmp(prop, "W", 1) == 0) {
                if (strcmp(prop, "WXXX") != 0) {
                    len += 1;
                } else {
                    printf("Trying to write WXXX: do not do anything\n");
                    return;
                }
            } else if (strcmp(prop, "IPLS") == 0) {
                len += 2;
            } else if (strcmp(prop, "COMM") == 0) {
                len += 5 + 13;
                // len += 5 - "\0ENG <...> \0"
                // len += 13 - "USER COMMENT\0"
            } else if (strcmp(prop, "PCNT") == 0) {
                // _ToDO?
                printf("PCNT - not implemented\n");
                return;
            } else {
                printf("Trying to write %s - not implemented", prop);
                return;
            }



            unsigned char write_size[] = {(len & 0xff000000u) >> 24u, (len & 0xff0000u) >> 16u, (len & 0xff00u) >> 8u,
                                          len & 0xffu};
            bytes_written += 4;
            fwrite(write_size, 1, 4, tmp_file);

            // Write flags
            char flags[] = {0x0, 0x0};
            bytes_written += 2;
            fwrite(flags, 1, 2, tmp_file);

            // Restore previous value
            len = _len;

            // Write new values

            if (strncmp(prop, "T", 1) == 0) {
                fwrite("\0", 1, 1, tmp_file);
                bytes_written += 1;
                fwrite(value, 1, len, tmp_file);
                bytes_written += len;
                fwrite("\0", 1, 1, tmp_file);
                bytes_written += 1;
            } else if (strncmp(prop, "W", 1) == 0) {
                fwrite(value, 1, len, tmp_file);
                bytes_written += len;
                fwrite("\0", 1, 1, tmp_file);
                bytes_written += 1;
            } else if (strcmp(prop, "IPLS") == 0) {
                fwrite("\0", 1, 1, tmp_file);
                bytes_written += 1;
                fwrite(value, 1, len, tmp_file);
                bytes_written += len;
                fwrite("\0", 1, 1, tmp_file);
                bytes_written += 1;
            } else if (strcmp(prop, "COMM") == 0) {
                fwrite("\0ENG", 1, 4, tmp_file);
                fwrite("USER COMMENT\0", 1, 13, tmp_file);
                bytes_written += 17;
                fwrite(value, 1, len, tmp_file);
                bytes_written += len;
                fwrite("\0", 1, 1, tmp_file);
                bytes_written += 1;
            }


            /*
             * Already done above
            // Write value
            bytes_written += len;
            fwrite(value, 1, len, tmp_file);
             */

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

        if (strncmp(prop, "T", 1) == 0) {
            // Text info
            if (strcmp(prop, "TXXX") != 0) {
                len += 2;
            } else {
                // Do not do anything
                printf("Trying to wtite TXXX: do not do anything\n");
                return;
            }
        } else if (strncmp(prop, "W", 1) == 0) {
            if (strcmp(prop, "WXXX") != 0) {
                len += 1;
            } else {
                printf("Trying to write WXXX: do not do anything\n");
                return;
            }
        } else if (strcmp(prop, "IPLS") == 0) {
            len += 2;
        } else if (strcmp(prop, "COMM") == 0) {
            len += 5 + 13;
            // len += 5 - "\0ENG <...> \0"
            // len += 13 - "USER COMMENT\0"
        } else if (strcmp(prop, "PCNT") == 0) {
            // _ToDO?
            printf("PCNT - not implemented\n");
            return;
        } else {
            printf("Trying to write %s - not implemented", prop);
            return;
        }

        unsigned char write_size[] = {(len & 0xff000000u) >> 24u, (len & 0xff0000u) >> 16u, (len & 0xff00u) >> 8u,
                                      len & 0xffu};
        fwrite(write_size, 1, 4, tmp_file);
        bytes_written += 4;

        // Write flags
        char flags[] = {0x0, 0x0};
        bytes_written += 2;
        fwrite(flags, 1, 2, tmp_file);

        // Write values
        if (strncmp(prop, "T", 1) == 0) {
            fwrite("\0", 1, 1, tmp_file);
            bytes_written += 1;
            fwrite(value, 1, len, tmp_file);
            bytes_written += len;
            fwrite("\0", 1, 1, tmp_file);
            bytes_written += 1;
        } else if (strncmp(prop, "W", 1) == 0) {
            fwrite(value, 1, len, tmp_file);
            bytes_written += len;
            fwrite("\0", 1, 1, tmp_file);
            bytes_written += 1;
        } else if (strcmp(prop, "IPLS") == 0) {
            fwrite("\0", 1, 1, tmp_file);
            bytes_written += 1;
            fwrite(value, 1, len, tmp_file);
            bytes_written += len;
            fwrite("\0", 1, 1, tmp_file);
            bytes_written += 1;
        } else if (strcmp(prop, "COMM") == 0) {
            fwrite("\0ENG", 1, 4, tmp_file);
            fwrite("USER COMMENT\0", 1, 13, tmp_file);
            bytes_written += 17;
            fwrite(value, 1, len, tmp_file);
            bytes_written += len;
            fwrite("\0", 1, 1, tmp_file);
            bytes_written += 1;
        }

        // Previous version
        /*
        // Write value
        bytes_written += len;
        fwrite(value, 1, len, tmp_file);
        */
    }


    // Дописываем остальную часть
    int c;
    while (!feof(file)) {                       // while not end of file
        c = fgetc(file);                        // get a byte from the file
        fputc(c, tmp_file);
    }


    // Completed writing header and the rest of the file
    // Calculating new size

    unsigned char header_size[] = {
            (((bytes_written << 3u) & 0x7f000000u) >> 24u),
            (((bytes_written << 2u) & 0x7f0000u) >> 16u),
            (((bytes_written << 1u) & 0x7f00u) >> 8u),
            (bytes_written & 0x7fu)
    };

    fwrite(header_size, 1, 4, tmp_file);

    fclose(file);
    fclose(tmp_file);
    remove(file_path);
    rename(tmp_filename, file_path);
}

unsigned long calcPlayCount(unsigned int l, char *data) {
    // l - number of bytes
    unsigned long x = 0;
    for (int i = 0; i < l; ++i) {
        x = (x << 8u) | data[i];
    }
    return x;
}
