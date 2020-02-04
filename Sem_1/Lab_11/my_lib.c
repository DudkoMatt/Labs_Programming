//
// Created by dudko_mv on 15.09.2019.
//

#include "my_lib.h"
#include <math.h>

int sqr(int x){
    return x * x;
}

double distance(Point a, Point b){
    return sqrt(sqr(a.x - b.x) + sqr(a.y - b.y));
}

double perimeter(Rhombus rhombus){
    return distance(rhombus.a, rhombus.b) + distance(rhombus.b, rhombus.c) + distance(rhombus.c, rhombus.d) +
    distance(rhombus.d, rhombus.a);
}

double area(Rhombus rhombus){
    return distance(rhombus.a, rhombus.c) * distance(rhombus.b, rhombus.d) / 2;
}