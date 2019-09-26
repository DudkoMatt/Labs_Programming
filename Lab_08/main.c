#include <stdio.h>
#include <string.h>

int main() {
    char str1[50] = "string1";
    char str2[50] = "hello";

    // ToDo: Заменить весь ввод строк на ввод с консоли

    // Задания 5, 7, 9, 11, 13
    // Задание 5:
    // Осуществить копирование одной строки в другую строку.

    // Ввод данных с консоли
    // gets(str1);
    // gets(str2);

    strcpy(str1, str2);
    printf("String 1: %s\n", str1);
    printf("String 2: %s\n", str2);

    // Задание 7:
    // Определить длину строки.
    printf("Length of string: \"%s\" = %lu\n", str1, strlen(str1));

    // Задание 9:
    // Осуществить поиск в строке последнего вхождения указанного символа.
    char *p = strrchr(str1, 'e');

    // Расчёт дистанции от начала строки
    // printf("%d", p - str1);

    // Задание 11:
    // Определить длину отрезка одной строки, содержащего символы из множества символов,
    // входящих во вторую строку.
    gets(str1);
    gets(str2);
    unsigned long max = 0;
    unsigned long tmp = 0;
    short is_found = 0;
    for (unsigned long i = 0; i < strlen(str1); ++i) {
        is_found = 0;
        for (unsigned long j = 0; j < strlen(str2); ++j) {
            if (str1[i] == str2[j]) {
                tmp += 1;
                is_found = 1;
                break;
            }
        }
        if (!is_found) {
            max = tmp > max ? tmp : max;
            tmp = 0;
        }
    }
    max = tmp > max ? tmp : max;
    printf("Длина наибольшего отрезка: %lu\n", max);

    // Задание 13:
    // Выделить из одной строки лексемы (кусочки), разделенные любым из множества
    // символов (разделителей), входящих во вторую строку.
    gets(str1);
    gets(str2);
    for (unsigned long j = 0; j < strlen(str1); ++j) {
        if (strchr(str2, str1[j]) != NULL){
            printf("\n");
        } else {
            printf("%c", str1[j]);
        }
    }

    return 0;
}