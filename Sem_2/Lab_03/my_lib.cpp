//
// Created by dudko on 04.02.2020.
//

#include "my_lib.h"
#include <cmath>

Point::Point() {
    x = 0;
    y = 0;
}

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

Point::Point(const Point &o) {
    x = o.x;
    y = o.y;
}

double distance_between_points(Point A, Point B){
    return sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
}

Triangle::Triangle(Point A, Point B, Point C) {
    this->A = Point(A);
    this->B = Point(B);
    this->C = Point(C);
    area = calc_area();
}

double Triangle::calc_area() {
    double a = distance_between_points(A, B);
    double b = distance_between_points(A, C);
    double c = distance_between_points(B, C);
    double p = (a + b + c) / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}

// Считаем, что задан радиус вектор (начало в 0, и точка)
void Triangle::move(Point a) {
    A.x += a.x;
    B.x += a.x;
    C.x += a.x;
    A.y += a.y;
    B.y += a.y;
    C.y += a.y;
}

void Triangle::print() {
    printf("Point 1: (%d; %d)\n", A.x, A.y);
    printf("Point 2: (%d; %d)\n", B.x, B.y);
    printf("Point 3: (%d; %d)\n", C.x, C.y);
}

DecSet::DecSet() {
    for (bool & i : is_in_set) {
        i = false;
    }
}

void DecSet::print() {
    for (int i = 0; i < 10; ++i) {
        std::cout << i << " ";
    }
    std::cout << "\n";
    for (bool i : is_in_set) {
        std::cout << (i ? "+" : "-") << " ";
    }
    std::cout << "\n";
}
