#include <stdio.h>
#include <math.h>

int main() {
    // Or we should use double?
    float x;

    printf("Введите точку первой функции: ");
    scanf("%f", &x);

    // Guess that it's radians
    // printf("cos(%f) = %f", x, cos(x));

    // Convert to double?
    printf("Значение первой функции: %f", 1 - sin(2 * x) * sin(2 * x) / 4 + cos(2 * x));
    printf("\nВведите точку второй функции: ");
    scanf("%f", &x);
    printf("Значение второй функции: %f", cos(x) * cos(x) + cos(x) * cos(x) * cos(x) * cos(x));
    return 0;
}