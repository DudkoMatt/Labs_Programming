#include <stdio.h>
#include "my_lib.h"

int main() {
    Rhombus r;

    // Задание координат точек вершин ромба

    r.a.x = 0;
    r.a.y = 3;

    r.b.x = 4;
    r.b.y = 0;

    r.c.x = 0;
    r.c.y = -3;

    r.d.x = -4;
    r.d.y = 0;

    printf("Perimeter: %lf\n", perimeter(r));
    printf("Area: %lf\n", area(r));

    return 0;
}