//
// Created by dudko on 11.05.2020.
//

#ifndef LAB_08_EDGE_H
#define LAB_08_EDGE_H

#include "Ceil.h"
#include <iostream>
#include <vector>

class Edge {
public:
    std::vector<std::vector<Ceil>> matrix;

    explicit Edge(int color) {
        matrix = std::vector<std::vector<Ceil>>(3, std::vector<Ceil>(3, Ceil(color)));
    }
    Edge(const Edge& edge) = default;

    void set_ceil(int i, int j, int color) {
        matrix[i][j].set_color(color);
    }

    int get_ceil_color(int i, int j) const {
        return matrix[i][j].get_color();
    }

    void print() const {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                std::cout << matrix[i][j].get_color() << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    void rotate(bool clockwise = true) {
        if (clockwise) {
            // Углы
            int tmp = get_ceil_color(0, 0);
            set_ceil(0, 0, get_ceil_color(2, 0));
            set_ceil(2, 0, get_ceil_color(2, 2));
            set_ceil(2, 2, get_ceil_color(0, 2));
            set_ceil(0, 2, tmp);

            // Центры боковых линий
            tmp = get_ceil_color(0, 1);
            set_ceil(0, 1, get_ceil_color(1, 0));
            set_ceil(1, 0, get_ceil_color(2, 1));
            set_ceil(2, 1, get_ceil_color(1, 2));
            set_ceil(1, 2, tmp);
        } else {
            // Углы
            int tmp = get_ceil_color(0, 0);
            set_ceil(0, 0, get_ceil_color(0, 2));
            set_ceil(0, 2, get_ceil_color(2, 2));
            set_ceil(2, 2, get_ceil_color(2, 0));
            set_ceil(2, 0, tmp);

            // Центры боковых линий
            tmp = get_ceil_color(0, 1);
            set_ceil(0, 1, get_ceil_color(1, 2));
            set_ceil(1, 2, get_ceil_color(2, 1));
            set_ceil(2, 1, get_ceil_color(1, 0));
            set_ceil(1, 0, tmp);
        }
    }

    void write_to_file(FILE *file) const {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                fprintf(file, "%i ", matrix[i][j].get_color());
            }
            fprintf(file, "\n");
        }
        fprintf(file, "\n");
    }

    void read_from_file(FILE *file) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                int color_;
                fscanf(file, "%i ", &color_);
                matrix[i][j].set_color(color_);
            }
            fscanf(file, "\n");
        }
        fscanf(file, "\n");
    }
};

#endif //LAB_08_EDGE_H
