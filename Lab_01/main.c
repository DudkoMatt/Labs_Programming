#include <stdio.h>

int main() {
    char s[50];
    int i;
//    printf("Введите строку и число: ");
    // В GitHub сказано, что нужно делать отчёт и не писать вводных фраз. Это правда?
    scanf("%s %d", &s, &i);
    printf("%s %d", s, i);
    return 0;
}