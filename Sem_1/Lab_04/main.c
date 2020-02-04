#include <stdio.h>

int main() {
    int x;
    scanf("%d", &x);

    // Задание 1:
    // Диапазон [-3; 6]
    printf("%d\n", (x >= -3) && (x <= 6));

    // Задание 2:
    scanf("%d", &x);
    printf("%d", (x >> 5) & 1);

    return 0;
}