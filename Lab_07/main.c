#include <stdio.h>
#include <math.h>

// К заданию 1:
typedef enum {
    CLASSICAL,
    POP,
    ROCK,
    RAP,
    NEW_AGE,
    ELECTRONIC
} Style;

// К заданию 2:
typedef struct {
    int x;
    int y;
} Point;

// Другой вариант объявления
// Без typedef
// typedef позволяет не писать 10 раз struct <имя_структуры> <имя_переменной>
// Вместо этого: <имя_typedef> <имя_переменной>
struct LineSegment {
    Point a;
    Point b;
};

void printLength(struct LineSegment);

int sqr(int x){
    return x * x;
}

// К заданию 3:
union ADSL {
    unsigned short X;   // 2 байта
    struct {
        unsigned short DSL: 1;  // Нумерация с младшего бита, так как система little endian
        unsigned short PPP: 1;
        unsigned short Link: 1;
    } Bits;  // Объявление переменной сразу
};

int main() {

    // Задание 1:
    Style a = ROCK;
    printf("Rock in enum is: %d\n", a);

    // Задание 2:
    // Разработать структуру хранения отрезка прямой
    struct LineSegment lineSegment = {{0,0}, {10,0}};
    printLength(lineSegment);

    // Задание 3:
    union ADSL adsl;
    scanf("%hX", &adsl.X);

    printf("DSL: ");
    adsl.Bits.DSL ? printf("On") : printf("Off");
    printf("\n");

    printf("PPP: ");
    adsl.Bits.PPP ? printf("On") : printf("Off");
    printf("\n");

    printf("Link: ");
    adsl.Bits.Link ? printf("On") : printf("Off");
    printf("\n");

    return 0;
}

void printLength(struct LineSegment lineSegment){
    printf("Length of line segment: %lf\n", sqrt(sqr(lineSegment.a.x - lineSegment.b.x) + sqr(lineSegment.a.y - lineSegment.b.y)));
}