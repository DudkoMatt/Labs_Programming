#include <stdio.h>
#include <malloc.h>

// https://prog-cpp.ru/c-alloc/

int main() {
    // Задание 1:
    // Это и есть создание с использованием указателя?
    char a[5];
    char *p = a;
    for (int i = 0; i < 4; ++i) {
        scanf("%c", &a[i]);
    }
    for (int j = 0; j < 4; ++j) {
        printf("%c ", a[j]);
    }

    char *b;
    b = (char*) calloc(4, sizeof(int));
    for (int k = 0; k < 4; ++k) {
        scanf("%c", (b + k));
    }
    for (int j = 0; j < 4; ++j) {
        printf("%c ", *(b + j));
    }

    free(b);

    // calloc(num, bytes_for_one) - создаёт и обнуляет значения
    // malloc(bytes) - просто создаёт
    // free(ptr) - освобождает
    // realloc(ptr, new_amount_of_bytes)

    return 0;
}