//
// Created by dudko on 04.02.2020.
//

#ifndef LAB_01_MY_LIB_H
#define LAB_01_MY_LIB_H

// # 5
void frac(double *x);

void frac(double &x);

// # 8
void inverse(double *x);

void inverse(double &x);

// # 13
struct Point {
    int x, y;
};

struct Square {
    Point A, B;
};

struct Vector {
    Point a;
};

void move(Square *s, Vector *v);

void move(Square &s, Vector &v);

void print_square(Square *s);

// # 16
struct MatrixRow {
    int width;
    int *a;
};

struct Matrix {
    int height;
    MatrixRow *a;
};

void swap(Matrix *m, int a, int b);

void swap(Matrix &m, int a, int b);

void init_matrix(Matrix *m, int height);

void init_row(Matrix *m, int index_of_raw, int size_of_row);

void init_rows(Matrix *m, int size_of_rows);

void print_matrix(Matrix *m);

#endif //LAB_01_MY_LIB_H
