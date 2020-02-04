//
// Created by dudko on 04.02.2020.
//

#include <iostream>
#include "my_lib.h"

// # 5
void frac(double *x) {
    *x = *x - (int) *x;
}

void frac(double &x) {
    x = x - (int) x;
}

// # 8
void inverse(double *x) {
    *x = 1 / *x;
}

void inverse(double &x) {
    x = 1 / x;
}

// # 13
void move(Square *s, Vector *v) {
    s->A.x += v->a.x;
    s->A.y += v->a.y;
    s->B.x += v->a.x;
    s->B.y += v->a.y;
}

void move(Square &s, Vector &v) {
    s.A.x += v.a.x;
    s.A.y += v.a.y;
    s.B.x += v.a.x;
    s.B.y += v.a.y;
}

void print_square(Square *s) {
    printf("Point 1: (%d; %d)\n", s->A.x, s->A.y);
    printf("Point 2: (%d; %d)\n", s->B.x, s->B.y);
}

// # 16
void swap(Matrix *m, int a, int b) {
    MatrixRow tmp = m->a[a];
    m->a[a] = m->a[b];
    m->a[b] = tmp;
}

void swap(Matrix &m, int a, int b) {
    MatrixRow tmp = m.a[a];
    m.a[a] = m.a[b];
    m.a[b] = tmp;
}

void init_matrix(Matrix *m, int height) {
    m->height = height;
    m->a = (MatrixRow *) calloc(sizeof(MatrixRow), m->height);
}

void init_row(Matrix *m, int index_of_raw, int size_of_row) {
    m->a[index_of_raw].width = size_of_row;
    m->a[index_of_raw].a = (int *) calloc(sizeof(int), size_of_row);
}

void init_rows(Matrix *m, int size_of_rows) {
    for (int i = 0; i < m->height; ++i) {
        init_row(m, i, size_of_rows);
    }
}

void print_matrix(Matrix *m) {
    for (int i = 0; i < m->height; ++i) {
        for (int j = 0; j < m->a[i].width; ++j) {
            std::cout << m->a[i].a[j] << " ";
        }
        std::cout << "\n";
    }
}