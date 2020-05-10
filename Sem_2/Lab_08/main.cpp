#include <iostream>
#include <vector>
#include <set>
#include <ctime>

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
    Ceil(const Ceil& ceil) = default;

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
    Cube (const Cube& cube) = default;

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

    bool is_correct() {
        // Цвет -> Положение [Центр ребра / Угол / Центр грани] -> Число
        std::vector<std::vector<int>> count_colors =
                std::vector<std::vector<int>>(
                        6, std::vector<int>(
                                3, 0
                        )
                );

        for (int k = 0; k < 6; ++k) {
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    int curr_color = edges[k].get_ceil_color(i, j);
                    if (curr_color < 1 || curr_color > 6)
                        return false;
                    if (i == 1 && j == 1)
                        count_colors[curr_color - 1][2] += 1;
                    else if (i == 1 || j == 1)
                        count_colors[curr_color - 1][0] += 1;
                    else
                        count_colors[curr_color - 1][1] += 1;
                }
            }
        }


        for (int curr_color = 0; curr_color < 6; ++curr_color) {
            if (count_colors[curr_color][0] != 4 || count_colors[curr_color][1] != 4 || count_colors[curr_color][2] != 1)
                return false;
        }


        return true;
    }

    long long hash() {
        const int X_POWER = 3947;
        const long long K_MOD = 20999999;
        long long ans = 0;

        for (int k = 0; k < 6; ++k) {
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    ans = (ans * X_POWER + (edges[k].get_ceil_color(i, j))) % K_MOD;
                }
            }
        }

        return (K_MOD + (ans % K_MOD)) % K_MOD;
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
                edges[4].matrix[2 - i][2] = edges[2].matrix[i][0];
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

    void shuffle() {
        std::srand(unsigned(std::time(nullptr)));

        // 1 .. 20
        int random_length = 1 + std::rand() % 20;
        for (int i = 0; i < random_length; ++i) {
            int rand_int_ = std::rand();
            bool direction = std::rand() % 2 == 0;
            switch (rand_int_ % 9) {
                case 0:
                    F(direction);
                    break;
                case 1:
                    R(direction);
                    break;
                case 2:
                    L(direction);
                    break;
                case 3:
                    B(direction);
                    break;
                case 4:
                    U(direction);
                    break;
                case 5:
                    D(direction);
                    break;
                case 6:
                    M(direction);
                    break;
                case 7:
                    S(direction);
                    break;
                case 8:
                default:
                    E(direction);
                    break;
            }
        }
    }

    void interactive_mode() {
        int x = 1;
        do {
            std::cout << "Select command:" << std::endl;
            std::cout << "1: F" << std::endl;
            std::cout << "2: R" << std::endl;
            std::cout << "3: L" << std::endl;
            std::cout << "4: B" << std::endl;
            std::cout << "5: U" << std::endl;
            std::cout << "6: D" << std::endl;
            std::cout << "7: M" << std::endl;
            std::cout << "8: S" << std::endl;
            std::cout << "9: E" << std::endl;
            std::cout << "10: print" << std::endl;
            std::cout << "0: exit" << std::endl << std::endl << ":";
            std::cin >> x;
            std::cout << std::endl;

            bool dir = false;
            if (1 <= x && x <= 9) {
                while (true) {
                    char y;
                    std::cout << "Direction [True(T) / False(F)]" << std::endl << ":";
                    std::cin >> y;
                    if (y == 't' || y == 'T') {
                        dir = true;
                        break;
                    } else if (y == 'f' || y == 'F') {
                        dir = false;
                        break;
                    }
                }
            }

            switch (x) {
                case 1:
                    F(dir);
                    break;
                case 2:
                    R(dir);
                    break;
                case 3:
                    L(dir);
                    break;
                case 4:
                    B(dir);
                    break;
                case 5:
                    U(dir);
                    break;
                case 6:
                    D(dir);
                    break;
                case 7:
                    M(dir);
                    break;
                case 8:
                    S(dir);
                    break;
                case 9:
                    E(dir);
                    break;
                case 10:
                    print();
                    break;
                default:
                    x = 0;
                    break;
            }

        } while (x != 0);
    }

    bool is_solved() {
        for (int k = 0; k < 6; ++k) {
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (edges[k].matrix[i][j].get_color() != k + 1) return false;
                }
            }
        }
        return true;
    }

    /*
     * F - 1
     * R - 2
     * L - 3
     * B - 4
     * U - 5
     * D - 6
     *
     * F' - 7
     * R' - 8
     * L' - 9
     * B' - 10
     * U' - 11
     * D' - 12
     */

//#define MAX_DEPTH__ 20
#define MAX_DEPTH__ 5

    bool dfs1(Cube& cube, int curr_depth, int max_depth, std::vector<int>& moves, std::set<long long> &visited) {
//        long long curr_hash = cube.hash();
//        if (visited.find(curr_hash) != visited.end())
//            return false;
//        visited.insert(curr_hash);
        if (cube.is_solved()) return true;
        if (curr_depth >= max_depth)
            return dfs2(cube, 0, MAX_DEPTH__ - max_depth, moves, visited);

        for (int i = 1; i <= 12; ++i) {
            moves.push_back(i);
            switch (i) {
                case 1:
                    F(true);
                    break;
                case 2:
                    R(true);
                    break;
                case 3:
                    L(true);
                    break;
                case 4:
                    B(true);
                    break;
                case 5:
                    U(true);
                    break;
                case 6:
                    D(true);
                    break;
                case 7:
                    F(false);
                    break;
                case 8:
                    R(false);
                    break;
                case 9:
                    L(false);
                    break;
                case 10:
                    B(false);
                    break;
                case 11:
                    U(false);
                    break;
                case 12:
                default:
                    D(false);
                    break;
            }

//            visited.insert(curr_hash);
            if (dfs2(cube, MAX_DEPTH__ - (curr_depth + 1), max_depth, moves, visited) || dfs1(cube, curr_depth + 1, max_depth, moves, visited))
                return true;
//            visited.erase(curr_hash);

            moves.pop_back();
            switch (i) {
                case 1:
                    F(false);
                    break;
                case 2:
                    R(false);
                    break;
                case 3:
                    L(false);
                    break;
                case 4:
                    B(false);
                    break;
                case 5:
                    U(false);
                    break;
                case 6:
                    D(false);
                    break;
                case 7:
                    F(true);
                    break;
                case 8:
                    R(true);
                    break;
                case 9:
                    L(true);
                    break;
                case 10:
                    B(true);
                    break;
                case 11:
                    U(true);
                    break;
                case 12:
                default:
                    D(true);
                    break;
            }
        }

//        visited.erase(curr_hash);
        return false;
    }

    /*
     * U - 5
     * D - 6
     *
     * U' - 11
     * D' - 12
     *
     * F2 - 13
     * R2 - 14
     * L2 - 15
     * B2 - 16
     */
    bool dfs2(Cube& cube, int curr_depth, int max_depth, std::vector<int>& moves, std::set<long long> &visited) {
//        long long curr_hash = cube.hash();
//        if (visited.find(curr_hash) != visited.end())
//            return false;
//        visited.insert(curr_hash);
        if (cube.is_solved()) return true;
        if (curr_depth >= max_depth) {
            return cube.is_solved();
        }

        for (int i = 0; i < 8; ++i) {
            switch (i) {
                case 0:
                    U(true);
                    moves.push_back(5);
                    break;
                case 1:
                    D(true);
                    moves.push_back(6);
                    break;
                case 2:
                    F(true);
                    F(true);
                    moves.push_back(13);
                    break;
                case 3:
                    R(true);
                    R(true);
                    moves.push_back(14);
                    break;
                case 4:
                    L(true);
                    L(true);
                    moves.push_back(15);
                    break;
                case 5:
                    D(true);
                    D(true);
                    moves.push_back(16);
                    break;
                case 6:
                    U(false);
                    moves.push_back(11);
                    break;
                case 7:
                default:
                    D(false);
                    moves.push_back(12);
                    break;
            }

//            visited.insert(curr_hash);
            if (dfs2(cube, curr_depth + 1, max_depth, moves, visited))
                return true;
//            visited.erase(curr_hash);

            moves.pop_back();
            switch (i) {
                case 0:
                    U(false);
                    break;
                case 1:
                    D(false);
                    break;
                case 2:
                    F(false);
                    F(false);
                    break;
                case 3:
                    R(false);
                    R(false);
                    break;
                case 4:
                    L(false);
                    L(false);
                    break;
                case 5:
                    D(false);
                    D(false);
                    break;
                case 6:
                    U(true);
                    break;
                case 7:
                default:
                    D(true);
                    break;
            }
        }

//        visited.erase(curr_hash);
        return false;
    }

    void solve() {
        // Max is 20 moves
        std::vector<int> moves;
        std::set<long long> visited;
        bool found = false;
        for (int length_first = 0; length_first <= 10; ++length_first) {
            moves.clear();
            if (dfs1(*this, 0, length_first, moves, visited)) {
                found = true;
                break;
            }
        }

        if (found) {
            std::cout << "Count: " << moves.size() << std::endl;
            for (int move : moves) {
                std::cout << move << " ";
            }
        } else {
            std::cout << "NO";
        }
    }

private:
    std::vector<Edge> edges;
};

int main() {
    Cube cube;
//    cube.shuffle();
//    cube.print_as_file(); std::cout << std::flush;
//    std::cout << (cube.is_correct() ? "T" : "F") << std::endl;
    cube.R();
//    cube.U();
//    cube.D();
//    cube.L();
    cube.solve();
    return 0;
}
