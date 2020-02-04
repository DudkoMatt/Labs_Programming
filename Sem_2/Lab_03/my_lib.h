//
// Created by dudko on 04.02.2020.
//

#include <iostream>

#ifndef LAB_03_MY_LIB_H
#define LAB_03_MY_LIB_H

// Задание 3

class Point {
public:
    int x, y;

    Point();

    Point(int x, int y);

    Point(const Point &o);
};

double distance_between_points(Point A, Point B);

class Triangle {
public:
    Point A, B, C;
    double area;

    Triangle(Point A, Point B, Point C);

    void print();

    double calc_area();

    void move(Point a);

    bool operator==(Triangle &t) const {
        return area == t.area;
    }

    bool operator<(Triangle &t) const {
        return area < t.area;
    }

    bool operator>(Triangle &t) const {
        return area > t.area;
    }

    bool operator!=(Triangle &t) const {
        return area != t.area;
    }

};


// Задание 7

class DecSet {
    bool is_in_set[10];
public:
    DecSet();

    void print();

    DecSet operator+(const DecSet &d) {
        DecSet new_set;
        for (int i = 0; i < 10; ++i) {
            new_set.is_in_set[i] = is_in_set[i] or d.is_in_set[i];
        }
        return new_set;
    }

    bool operator==(const DecSet &d) const {
        for (int i = 0; i < 10; ++i) {
            if (is_in_set[i] != d.is_in_set[i])
                return false;
        }
        return true;
    }

    bool operator!=(const DecSet &d) const {
        return !(operator==(d));
    }

    void operator+=(int x) {
        if (0 <= x and x < 10)
            is_in_set[x] = true;
    }

    void operator-=(int x) {
        if (0 <= x and x < 10)
            is_in_set[x] = false;
    }
};

#endif //LAB_03_MY_LIB_H
