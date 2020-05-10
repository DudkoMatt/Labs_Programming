#include <iostream>
#include <vector>

/*
 * Лабораторная работа No8. Кубик Рубика
 * Спроектировать и реализовать программу, имитирующую сборку Кубика Рубика 3x3.
 * https://ru.wikipedia.org/wiki/%D0%9A%D1%83%D0%B1%D0%B8%D0%BA_%D0%A0%D1%83%D0%B1%D0%B8%D0%BA%D0%B0
 *
 * К программе предъявляются следующие функциональные требования:
 * - Сохранение и чтение состояния кубика рубика из файла
 * - Проверка корректности текущего состояния (инвариант состояний кубика)
 * - Вывод в консоль текущего состояния
 * - Вращение граней кубика рубика с помощью вводимых команд
 * - Генерация случайного состояния Кубика Рубика, корректного с точки зрения
 * инварианта состояний
 * - Нахождения “решения” для текущего состояния в виде последовательности
 * поворотов граней
 * Нефункциональные требования:
 * - Программа должна быть спроектирована, с использованием ОПП
 * - Логические сущности должны быть выделены в отдельный классы
 *
 * Критерии оценки:
 * - Логично выстроенная архитектура приложения
 * - Применение возможностей языка программирования С++ включая стандартную
 * библиотеку
 *
 * Дополнительно (за дополнительные баллы):
 * Реализовать графический интерфейс приложения, с использованием OpenGL Utility
 * Toolkit
 * https://en.wikipedia.org/wiki/OpenGL_Utility_Toolkit
 */

class Ceil {
public:
    explicit Ceil(int color) : color_(color) {}
    Ceil() {color_ = 0;}

    int get_color() const {
        return color_;
    }

    void set_color(int color) {
        color_ = color;
    }

private:
    int color_;
};

class Edge {
public:
    std::vector<std::vector<Ceil>> matrix;

    explicit Edge(int color) {
        matrix = std::vector<std::vector<Ceil>>(3, std::vector<Ceil>(3, Ceil(color)));
    }

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

/*
 * Нумерация граней - по номеру цвета. Исходное состояние:
 * Смотрим с первой грани
 *
 *         | 5 5 5 |
 *         | 5 5 5 |
 *         | 5 5 5 |
 * | 4 4 4 | 1 1 1 | 2 2 2 | 3 3 3 |
 * | 4 4 4 | 1 1 1 | 2 2 2 | 3 3 3 |
 * | 4 4 4 | 1 1 1 | 2 2 2 | 3 3 3 |
 *         | 6 6 6 |
 *         | 6 6 6 |
 *         | 6 6 6 |
 */
class Cube {
public:
    Cube() {
        for (int i = 0; i < 6; ++i) {
            edges.emplace_back(i + 1);
        }
    }

    void print_as_file() const {
        for (int i = 0; i < 6; ++i) {
            edges[i].print();
        }
    }

    void print() const {
        for (int i = 0; i < 3; ++i) {
            printf("        %i %i %i\n", edges[4].get_ceil_color(i, 0), edges[4].get_ceil_color(i, 1), edges[4].get_ceil_color(i, 2));
        }

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j)
                printf("%i ", edges[3].get_ceil_color(i, j));
            printf("| ");
            for (int j = 0; j < 3; ++j)
                printf("%i ", edges[0].get_ceil_color(i, j));
            printf("| ");
            for (int j = 0; j < 3; ++j)
                printf("%i ", edges[1].get_ceil_color(i, j));
            printf("| ");
            for (int j = 0; j < 3; ++j)
                printf("%i ", edges[2].get_ceil_color(i, j));
            printf("\n");
        }

        for (int i = 0; i < 3; ++i) {
            printf("        %i %i %i\n", edges[5].get_ceil_color(i, 0), edges[5].get_ceil_color(i, 1), edges[5].get_ceil_color(i, 2));
        }
    }

    void write_to_file(const char *file_name = "output.txt") const {
        FILE *file = fopen(file_name, "w");
        for (int i = 0; i < 6; ++i) {
            edges[i].write_to_file(file);
        }
        fclose(file);
    }

    void read_from_file(const char *file_name = "input.txt") {
        FILE *file = fopen(file_name, "r");
        for (int i = 0; i < 6; ++i) {
            edges[i].read_from_file(file);
        }
        fclose(file);
    }

    bool is_correct() const {
        // Цвет -> Положение(x, y)
        std::vector<std::vector<std::vector<int>>>
                count_colors =
                std::vector<std::vector<std::vector<int>>>(
                        6, std::vector<std::vector<int>>(
                                3, std::vector<int>(
                                        3, 0
                                )
                        )
                );

        for (int k = 0; k < 6; ++k) {
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    int curr_color = edges[k].get_ceil_color(i, j);
                    if (curr_color < 1 || curr_color > 6)
                        return false;
                    count_colors[curr_color - 1][i][j] += 1;
                }
            }
        }


        for (int curr_color = 0; curr_color < 6; ++curr_color) {
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (count_colors[curr_color][i][j] != 1)
                        return false;
                }
            }
        }


        return true;
    }

    // Rotations
    void F(bool clockwise = true) {
        // Фронтальная грань
        edges[0].rotate(clockwise);

        if (clockwise) {
            // Прилегающие грани
            std::vector<Ceil> tmp_v = edges[4].matrix[2];
            // 3 -> 4
            for (int i = 0; i < 3; ++i) {
                edges[4].matrix[2][2 - i] = edges[3].matrix[i][2];
            }

            // 5 -> 3
            for (int i = 0; i < 3; ++i) {
                edges[3].matrix[i][2] = edges[5].matrix[0][i];
            }

            // 1 -> 5
            for (int i = 0; i < 3; ++i) {
                edges[5].matrix[0][i] = edges[1].matrix[2 - i][0];
            }

            // tmp_v (4) -> 1
            for (int i = 0; i < 3; ++i) {
                edges[1].matrix[i][0] = tmp_v[i];
            }

        } else {
            // Прилегающие грани
            std::vector<Ceil> tmp_v = edges[4].matrix[2];
            // 1 -> 4
            for (int i = 0; i < 3; ++i) {
                edges[4].matrix[2][i] = edges[1].matrix[i][0];
            }

            // 5 -> 1
            for (int i = 0; i < 3; ++i) {
                edges[1].matrix[i][0] = edges[5].matrix[0][2 - i];
            }

            // 3 -> 5
            for (int i = 0; i < 3; ++i) {
                edges[5].matrix[0][2 - i] = edges[3].matrix[2 - i][2];
            }

            // tmp_v (4) -> 3
            for (int i = 0; i < 3; ++i) {
                edges[3].matrix[2 - i][2] = tmp_v[i];
            }
        }
    }
    void R(bool clockwise = true) {
        edges[1].rotate(clockwise);
        if (clockwise) {
            std::vector<Ceil> tmp_v;
            tmp_v.resize(3);

            // 4 -> tmp_v
            for (int i = 0; i < 3; ++i) {
                tmp_v[i] = edges[4].matrix[2 - i][2];
            }

            // 0 -> 4
            for (int i = 0; i < 3; ++i) {
                edges[4].matrix[i][2] = edges[0].matrix[i][2];
            }

            // 5 -> 0
            for (int i = 0; i < 3; ++i) {
                edges[0].matrix[i][2] = edges[5].matrix[i][2];
            }

            // 2 -> 5
            for (int i = 0; i < 3; ++i) {
                edges[5].matrix[i][2] = edges[2].matrix[2 - i][0];
            }

            // tmp_v -> 2
            for (int i = 0; i < 3; ++i) {
                edges[2].matrix[i][0] = tmp_v[i];
            }

        } else {
            std::vector<Ceil> tmp_v;
            tmp_v.resize(3);

            // 4 -> tmp_v
            for (int i = 0; i < 3; ++i) {
                tmp_v[i] = edges[4].matrix[2 - i][2];
            }

            // 2 -> 4
            for (int i = 0; i < 3; ++i) {
                edges[4].matrix[2 - i][2] = edges[2].matrix[i][2];
            }

            // 5 -> 2
            for (int i = 0; i < 3; ++i) {
                edges[2].matrix[i][0] = edges[5].matrix[2 - i][2];
            }

            // 5 -> 0
            for (int i = 0; i < 3; ++i) {
                edges[5].matrix[i][2] = edges[0].matrix[i][2];
            }

            // tmp_v -> 0
            for (int i = 0; i < 3; ++i) {
                edges[0].matrix[i][2] = tmp_v[2 - i];
            }

        }
    }
    void L(bool clockwise = true) {
        edges[3].rotate(clockwise);
        if (clockwise) {
            std::vector<Ceil> tmp_v;
            tmp_v.resize(3);

            // 4 -> tmp_v
            for (int i = 0; i < 3; ++i) {
                tmp_v[i] = edges[4].matrix[i][0];
            }

            // 2 -> 4
            for (int i = 0; i < 3; ++i) {
                edges[4].matrix[i][0] = edges[2].matrix[2 - i][2];
            }

            // 5 -> 2
            for (int i = 0; i < 3; ++i) {
                edges[2].matrix[i][2] = edges[5].matrix[2 - i][0];
            }

            // 0 -> 5
            for (int i = 0; i < 3; ++i) {
                edges[5].matrix[i][0] = edges[0].matrix[i][0];
            }

            // tmp_v -> 0
            for (int i = 0; i < 3; ++i) {
                edges[0].matrix[i][0] = tmp_v[i];
            }
        } else {
            std::vector<Ceil> tmp_v;
            tmp_v.resize(3);

            // 4 -> tmp_v
            for (int i = 0; i < 3; ++i) {
                tmp_v[i] = edges[4].matrix[i][0];
            }

            // 0 -> 4
            for (int i = 0; i < 3; ++i) {
                edges[4].matrix[i][0] = edges[0].matrix[i][0];
            }

            // 5 -> 0
            for (int i = 0; i < 3; ++i) {
                edges[0].matrix[i][0] = edges[5].matrix[i][0];
            }

            // 2 -> 5
            for (int i = 0; i < 3; ++i) {
                edges[5].matrix[i][0] = edges[2].matrix[2 - i][2];
            }

            // tmp_v -> 2
            for (int i = 0; i < 3; ++i) {
                edges[2].matrix[2 - i][2] = tmp_v[i];
            }
        }
    }
    void B(bool clockwise = true) {
        edges[2].rotate(clockwise);
        if (clockwise) {
            std::vector<Ceil> tmp_v;
            tmp_v.resize(3);

            // 4 -> tmp_v
            for (int i = 0; i < 3; ++i) {
                tmp_v[i] = edges[4].matrix[0][2 - i];
            }

            // 1 -> 4
            for (int i = 0; i < 3; ++i) {
                edges[4].matrix[0][2 - i] = edges[1].matrix[2 - i][2];
            }

            // 5 -> 1
            for (int i = 0; i < 3; ++i) {
                edges[1].matrix[i][2] = edges[5].matrix[2][2 - i];
            }

            // 3 -> 5
            for (int i = 0; i < 3; ++i) {
                edges[5].matrix[2][2 - i] = edges[3].matrix[2 - i][0];
            }

            // tmp_v -> 0
            for (int i = 0; i < 3; ++i) {
                edges[3].matrix[i][0] = tmp_v[i];
            }
        } else {
            std::vector<Ceil> tmp_v;
            tmp_v.resize(3);

            // 4 -> tmp_v
            for (int i = 0; i < 3; ++i) {
                tmp_v[i] = edges[4].matrix[0][2 - i];
            }

            // 3 -> 4
            for (int i = 0; i < 3; ++i) {
                edges[4].matrix[0][2 - i] = edges[3].matrix[i][0];
            }

            // 3 -> 5
            for (int i = 0; i < 3; ++i) {
                edges[3].matrix[i][0] = edges[5].matrix[2][i];
            }

            // 1 -> 5
            for (int i = 0; i < 3; ++i) {
                edges[5].matrix[2][i] = edges[1].matrix[2 - i][2];
            }

            // tmp_v -> 1
            for (int i = 0; i < 3; ++i) {
                edges[1].matrix[i][2] = tmp_v[2 - i];
            }
        }
    }
    void U(bool clockwise = true) {
        edges[4].rotate(clockwise);
        if (clockwise) {
            std::vector<Ceil> tmp_v;
            tmp_v.resize(3);

            // 2 -> tmp_v
            for (int i = 0; i < 3; ++i) {
                tmp_v[i] = edges[2].matrix[0][2 - i];
            }

            // 3 -> 2
            for (int i = 0; i < 3; ++i) {
                edges[2].matrix[0][2 - i] = edges[3].matrix[0][2 - i];
            }

            // 0 -> 3
            for (int i = 0; i < 3; ++i) {
                edges[3].matrix[0][2 - i] = edges[0].matrix[0][2 - i];
            }

            // 1 -> 0
            for (int i = 0; i < 3; ++i) {
                edges[0].matrix[0][2 - i] = edges[1].matrix[0][2 - i];
            }

            // tmp_v -> 1
            for (int i = 0; i < 3; ++i) {
                edges[1].matrix[0][2 - i] = tmp_v[i];
            }
        } else {
            std::vector<Ceil> tmp_v;
            tmp_v.resize(3);

            // 2 -> tmp_v
            for (int i = 0; i < 3; ++i) {
                tmp_v[i] = edges[2].matrix[0][2 - i];
            }

            // 1 -> 2
            for (int i = 0; i < 3; ++i) {
                edges[2].matrix[0][2 - i] = edges[1].matrix[0][2 - i];
            }

            // 0 -> 1
            for (int i = 0; i < 3; ++i) {
                edges[1].matrix[0][2 - i] = edges[0].matrix[0][2 - i];
            }

            // 3 -> 0
            for (int i = 0; i < 3; ++i) {
                edges[0].matrix[0][2 - i] = edges[3].matrix[0][2 - i];
            }

            // tmp_v -> 3
            for (int i = 0; i < 3; ++i) {
                edges[3].matrix[0][2 - i] = tmp_v[i];
            }
        }
    }
    void D(bool clockwise = true) {
        edges[5].rotate(clockwise);
        if (clockwise) {
            std::vector<Ceil> tmp_v;
            tmp_v.resize(3);

            // 0 -> tmp_v
            for (int i = 0; i < 3; ++i) {
                tmp_v[i] = edges[0].matrix[2][i];
            }

            // 3 -> 0
            for (int i = 0; i < 3; ++i) {
                edges[0].matrix[2][i] = edges[3].matrix[2][i];
            }

            // 2 -> 3
            for (int i = 0; i < 3; ++i) {
                edges[3].matrix[2][i] = edges[2].matrix[2][i];
            }

            // 1 -> 2
            for (int i = 0; i < 3; ++i) {
                edges[2].matrix[2][i] = edges[1].matrix[2][i];
            }

            // tmp_v -> 1
            for (int i = 0; i < 3; ++i) {
                edges[1].matrix[2][i] = tmp_v[i];
            }
        } else {
            std::vector<Ceil> tmp_v;
            tmp_v.resize(3);

            // 0 -> tmp_v
            for (int i = 0; i < 3; ++i) {
                tmp_v[i] = edges[0].matrix[2][i];
            }

            // 1 -> 0
            for (int i = 0; i < 3; ++i) {
                edges[0].matrix[2][i] = edges[1].matrix[2][i];
            }

            // 2 -> 1
            for (int i = 0; i < 3; ++i) {
                edges[1].matrix[2][i] = edges[2].matrix[2][i];
            }

            // 3 -> 2
            for (int i = 0; i < 3; ++i) {
                edges[2].matrix[2][i] = edges[3].matrix[2][i];
            }

            // tmp_v -> 3
            for (int i = 0; i < 3; ++i) {
                edges[3].matrix[2][i] = tmp_v[i];
            }
        }
    }
    void M(bool down = true) {
        std::vector<Ceil> tmp_v;
        tmp_v.resize(3);

        for (int i = 0; i < 3; ++i) {
            tmp_v[i] = edges[0].matrix[i][1];
        }

        if (down) {
            // 4 -> 0
            for (int i = 0; i < 3; ++i) {
                edges[0].matrix[i][1] = edges[4].matrix[i][1];
            }

            // 2 -> 4
            for (int i = 0; i < 3; ++i) {
                edges[4].matrix[i][1] = edges[2].matrix[2 - i][1];
            }

            // 5 -> 2
            for (int i = 0; i < 3; ++i) {
                edges[2].matrix[2 - i][1] = edges[5].matrix[i][1];
            }

            // tmp_v -> 5
            for (int i = 0; i < 3; ++i) {
                edges[5].matrix[i][1] = tmp_v[i];
            }
        } else {
            // 5 -> 0
            for (int i = 0; i < 3; ++i) {
                edges[0].matrix[i][1] = edges[5].matrix[i][1];
            }

            // 2 -> 5
            for (int i = 0; i < 3; ++i) {
                edges[5].matrix[i][1] = edges[2].matrix[2 - i][1];
            }

            // 4 -> 2
            for (int i = 0; i < 3; ++i) {
                edges[2].matrix[2 - i][1] = edges[4].matrix[i][1];
            }

            // tmp_v -> 4
            for (int i = 0; i < 3; ++i) {
                edges[4].matrix[i][1] = tmp_v[i];
            }
        }
    }
    void S(bool clockwise = true) {
        std::vector<Ceil> tmp_v;
        tmp_v.resize(3);
        // 4 -> tmp_v
        for (int i = 0; i < 3; ++i) {
            tmp_v[i] = edges[4].matrix[1][i];
        }

        if (clockwise) {
            // 3 -> 4
            for (int i = 0; i < 3; ++i) {
                edges[4].matrix[1][i] = edges[3].matrix[2 - i][1];
            }

            // 5 -> 3
            for (int i = 0; i < 3; ++i) {
                edges[3].matrix[2 - i][1] = edges[5].matrix[1][2 - i];
            }

            // 1 -> 5
            for (int i = 0; i < 3; ++i) {
                edges[5].matrix[1][2 - i] = edges[1].matrix[i][1];
            }

            // tmp_v -> 1
            for (int i = 0; i < 3; ++i) {
                edges[1].matrix[i][1] = tmp_v[i];
            }
        } else {
            // 1 -> 4
            for (int i = 0; i < 3; ++i) {
                edges[4].matrix[1][i] = edges[1].matrix[i][1];
            }

            // 5 -> 1
            for (int i = 0; i < 3; ++i) {
                edges[1].matrix[i][1] = edges[5].matrix[1][2 - i];
            }

            // 3 -> 5
            for (int i = 0; i < 3; ++i) {
                edges[5].matrix[1][2 - i] = edges[3].matrix[2 - i][1];
            }

            // tmp_v -> 3
            for (int i = 0; i < 3; ++i) {
                edges[3].matrix[2 - i][1] = tmp_v[i];
            }
        }
    }
    void E(bool clockwise = true) {
        std::vector<Ceil> tmp_v;
        tmp_v.resize(3);
        // 0 -> tmp_v
        for (int i = 0; i < 3; ++i) {
            tmp_v[i] = edges[0].matrix[1][i];
        }

        if (clockwise) {
            // 3 -> 0
            for (int i = 0; i < 3; ++i) {
                edges[0].matrix[1][i] = edges[3].matrix[1][i];
            }

            // 2 -> 3
            for (int i = 0; i < 3; ++i) {
                edges[3].matrix[1][i] = edges[2].matrix[1][i];
            }

            // 1 -> 2
            for (int i = 0; i < 3; ++i) {
                edges[2].matrix[1][i] = edges[1].matrix[1][i];
            }

            // tmp_v -> 1
            for (int i = 0; i < 3; ++i) {
                edges[1].matrix[1][i] = tmp_v[i];
            }
        } else {
            // 1 -> 0
            for (int i = 0; i < 3; ++i) {
                edges[0].matrix[1][i] = edges[1].matrix[1][i];
            }

            // 2 -> 1
            for (int i = 0; i < 3; ++i) {
                edges[1].matrix[1][i] = edges[2].matrix[1][i];
            }

            // 3 -> 2
            for (int i = 0; i < 3; ++i) {
                edges[2].matrix[1][i] = edges[3].matrix[1][i];
            }

            // tmp_v -> 3
            for (int i = 0; i < 3; ++i) {
                edges[3].matrix[1][i] = tmp_v[i];
            }
        }
    }

private:
    std::vector<Edge> edges;
};

int main() {
    Cube cube;
    cube.read_from_file();
    cube.print();
    std::cout << std::endl;
    cube.E(true);
    cube.print();
    std::cout << std::endl;
    cube.E(false);
    cube.print();
    std::cout << std::endl;
    return 0;
}
