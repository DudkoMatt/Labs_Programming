//
// Created by dudko_mv on 15.09.2019.
//

#ifndef LAB_11_MY_LIB_H
#define LAB_11_MY_LIB_H

typedef struct Point {
    int x, y;
} Point;

typedef struct Rhombus {
    Point a, b, c, d;
} Rhombus;

int sqr(int x);
double distance(struct Point, struct Point);
double perimeter(struct Rhombus);
double area(struct Rhombus);

#endif //LAB_11_MY_LIB_H
