#include <stdio.h>

int main() {
    int x;                                                 // Число должно быть со знаком?
    printf("Введите число в 10 системе счисления: ");
    scanf("%d", &x);
    printf("Число в 16 системе счисления: %X\n", x);
    printf("Число в 8 системе счисления: %o\n", x);
    printf("Число в 8 системе счисления, сдвинутое на вправо на 1: %o\n", x >> 1);
    printf("Число в 8 системе счисления с битовым отрицанием: %o\n", ~x);
    int y;                                                 // Число должно быть со знаком?
    printf("Введите второе число в 8 системе счисления: ");
    scanf("%o", &y);
    printf("Результат исключающего или для двух чисел: %d\n", x ^ y);
    return 0;
}