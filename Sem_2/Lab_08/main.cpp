#include <iostream>
#include <vector>
#include <set>
#include <ctime>

#include "Ceil.h"
#include "Edge.h"

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

    std::vector<int> moves;

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
    void F(bool clockwise = true, bool write_to_move = true) {
        // Фронтальная грань
        edges[0].rotate(clockwise);

        if (clockwise) {
            if (write_to_move) moves.push_back(1);

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
            if (write_to_move) moves.push_back(7);

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
    void R(bool clockwise = true, bool write_to_move = true) {
        edges[1].rotate(clockwise);
        if (clockwise) {
            if (write_to_move) moves.push_back(2);

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
            if (write_to_move) moves.push_back(8);

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
    void L(bool clockwise = true, bool write_to_move = true) {
        edges[3].rotate(clockwise);
        if (clockwise) {
            if (write_to_move) moves.push_back(3);

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
            if (write_to_move) moves.push_back(9);

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
    void B(bool clockwise = true, bool write_to_move = true) {
        edges[2].rotate(clockwise);
        if (clockwise) {
            if (write_to_move) moves.push_back(4);

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
            if (write_to_move) moves.push_back(10);

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
    void U(bool clockwise = true, bool write_to_move = true) {
        edges[4].rotate(clockwise);
        if (clockwise) {
            if (write_to_move) moves.push_back(5);

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
            if (write_to_move) moves.push_back(11);

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
    void D(bool clockwise = true, bool write_to_move = true) {
        edges[5].rotate(clockwise);
        if (clockwise) {
            if (write_to_move) moves.push_back(6);

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
            if (write_to_move) moves.push_back(12);

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
                    F(direction, false);
                    break;
                case 1:
                    R(direction, false);
                    break;
                case 2:
                    L(direction, false);
                    break;
                case 3:
                    B(direction, false);
                    break;
                case 4:
                    U(direction, false);
                    break;
                case 5:
                    D(direction, false);
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

private:
    // Проверка для нижней грани (5).
    bool is_cross_solved() {
        bool a = true;
        for (int i = 0; i < 2; ++i) {
            if (edges[5].matrix[1][i] != edges[5].matrix[1][i + 1])
                return false;
        }
        for (int i = 0; i < 2; ++i) {
            if (edges[5].matrix[i][1] != edges[5].matrix[i + 1][1])
                return false;
        }

        // Крест собран, проверка цветов
        return (edges[0].matrix[2][1] == edges[0].matrix[1][1])
               && (edges[1].matrix[2][1] == edges[1].matrix[1][1])
               && (edges[2].matrix[2][1] == edges[2].matrix[1][1])
               && (edges[3].matrix[2][1] == edges[3].matrix[1][1]);
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

    int solve_step_1_calc_case() {
        if (edges[0].matrix[1][1] == edges[0].matrix[0][1] && edges[4].matrix[2][1] == edges[5].matrix[1][1]) {
            return 1;
        } else if (edges[1].matrix[1][1] == edges[1].matrix[0][1] && edges[4].matrix[1][2] == edges[5].matrix[1][1]) {
            return 2;
        } else if (edges[3].matrix[1][1] == edges[3].matrix[0][1] && edges[4].matrix[1][0] == edges[5].matrix[1][1]) {
            return 3;
        } else if (edges[2].matrix[1][1] == edges[2].matrix[0][1] && edges[4].matrix[0][1] == edges[5].matrix[1][1]) {
            return 4;
        } else if (edges[0].matrix[1][1] == edges[4].matrix[2][1] && edges[0].matrix[0][1] == edges[5].matrix[1][1]) {
            return 5;
        } else if (edges[1].matrix[1][1] == edges[4].matrix[1][2] && edges[1].matrix[0][1] == edges[5].matrix[1][1]) {
            return 6;
        } else if (edges[3].matrix[1][1] == edges[4].matrix[1][0] && edges[3].matrix[0][1] == edges[5].matrix[1][1]) {
            return 7;
        } else if (edges[2].matrix[1][1] == edges[4].matrix[0][1] && edges[2].matrix[0][1] == edges[5].matrix[1][1]) {
            return 8;
        } else if (edges[0].matrix[1][2] == edges[5].matrix[1][1]) {
            return 9;
        } else if (edges[1].matrix[1][2] == edges[5].matrix[1][1]) {
            return 10;
        } else if (edges[3].matrix[1][2] == edges[5].matrix[1][1]) {
            return 11;
        } else if (edges[2].matrix[1][2] == edges[5].matrix[1][1]) {
            return 12;
        } else if (edges[0].matrix[1][0] == edges[5].matrix[1][1]) {
            return 13;
        } else if (edges[1].matrix[1][0] == edges[5].matrix[1][1]) {
            return 14;
        } else if (edges[3].matrix[1][0] == edges[5].matrix[1][1]) {
            return 15;
        } else if (edges[2].matrix[1][0] == edges[5].matrix[1][1]) {
            return 16;
        } else if (edges[0].matrix[2][1] == edges[5].matrix[1][1]) {
            return 17;
        } else if (edges[1].matrix[2][1] == edges[5].matrix[1][1]) {
            return 18;
        } else if (edges[3].matrix[2][1] == edges[5].matrix[1][1]) {
            return 19;
        } else if (edges[2].matrix[2][1] == edges[5].matrix[1][1]) {
            return 20;
        } else if (edges[5].matrix[1][1] == edges[5].matrix[0][1] && edges[0].matrix[2][1] != edges[0].matrix[1][1]) {
            return 21;
        } else if (edges[5].matrix[1][1] == edges[5].matrix[1][2] && edges[1].matrix[2][1] != edges[1].matrix[1][1]) {
            return 22;
        } else if (edges[5].matrix[1][1] == edges[5].matrix[1][0] && edges[3].matrix[2][1] != edges[3].matrix[1][1]) {
            return 23;
        } else if (edges[5].matrix[1][1] == edges[5].matrix[2][1] && edges[2].matrix[2][1] != edges[2].matrix[1][1]) {
            return 24;
        } else {
            return 25;
        }
    }

    // Цвет нижней грани (номер 5) определен. Собираем крест на нижней грани
    void solve_step_1() {
        int k = 0;
        while (k < 4 && !is_cross_solved()) {
            switch (solve_step_1_calc_case()) {
                case 1:
                    F(); F();
                    break;
                case 2:
                    R(); R();
                    break;
                case 3:
                    L(); L();
                    break;
                case 4:
                    B(); B();
                    break;
                case 5:
                    U(false);
                    R(false);
                    F(true);
                    R(true);
                    break;
                case 6:
                    U(false);
                    B(false);
                    R(true);
                    B(true);
                    break;
                case 7:
                    U(false);
                    F(false);
                    L(true);
                    F(true);
                    break;
                case 8:
                    U(false);
                    L(false);
                    B(true);
                    L(true);
                    break;
                case 9:
                    F(false);
                    U(false);
                    F(true);
                    break;
                case 10:
                    R(false);
                    U(false);
                    R(true);
                    break;
                case 11:
                    L(false);
                    U(false);
                    L(true);
                    break;
                case 12:
                    B(false);
                    U(false);
                    B(true);
                    break;
                case 13:
                    F(true);
                    U(false);
                    F(false);
                    break;
                case 14:
                    R(true);
                    U(false);
                    R(false);
                    break;
                case 15:
                    L(true);
                    U(false);
                    L(false);
                    break;
                case 16:
                    B(true);
                    U(false);
                    B(false);
                    break;
                case 17:
                case 21:
                    F(); F();
                    U(false);
                    F(); F();
                    break;
                case 18:
                case 22:
                    R(); R();
                    U(false);
                    R(); R();
                    break;
                case 19:
                case 23:
                    L(); L();
                    U(false);
                    L(); L();
                    break;
                case 20:
                case 24:
                    B(); B();
                    U(false);
                    B(); B();
                    break;
                case 25:
                default:
                    U(true);
                    k++;
                    break;
            }
        }
    }

    void solve_step_2() {

    }

    void solve_step_3() {

    }

    void solve_step_4() {

    }

    void solve_step_5() {

    }

    void solve_step_6() {

    }

    void solve_step_7() {

    }

public:
    void solve() {
        moves = std::vector<int>();  // init
        solve_step_1();
        solve_step_2();
        solve_step_3();
        solve_step_4();
        solve_step_5();
        solve_step_6();
        solve_step_7();
    }

private:
    std::vector<Edge> edges;
};

int main() {
    Cube cube;
    cube.shuffle();
//    cube.shuffle();
//    cube.print_as_file(); std::cout << std::flush;
    std::cout << (cube.is_solved() ? "T" : "F") << std::endl;
//    cube.R();
//    cube.U();
//    cube.D();
//    cube.L();
//    cube.interactive_mode();
    cube.print();
    return 0;
}
