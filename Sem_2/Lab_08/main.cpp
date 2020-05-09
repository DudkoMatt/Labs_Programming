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
 * Реализовать графический интерфейс приложения, с использование OpenGL Utility
 * Toolkit
 * https://en.wikipedia.org/wiki/OpenGL_Utility_Toolkit
 */

class Ceil {
public:
    explicit Ceil(int color): color_(color) {}

    int get_color() {
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
    explicit Edge(int color) {
        matrix = std::vector<std::vector<Ceil>>(3, std::vector<Ceil>(3, Ceil(color)));
    }

    void set_ceil(int i, int j, int color) {
        matrix[i][j].set_color(color);
    }

    int get_ceil_color(int i, int j) {
        return matrix[i][j].get_color();
    }

    void print() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                std::cout << matrix[i][j].get_color() << " ";
            }
            std::cout << "\n";
        }
    }

private:
    std::vector<std::vector<Ceil>> matrix;
};

class Cube {
public:
    Cube() {
        edges = std::vector<Edge>(6);
        for (int i = 0; i < 6; ++i) {
            edges[i] = Edge(i + 1);
        }
    }

    void print() {
        for (int i = 0; i < 6; ++i) {
            edges[i].print();
        }
    }

private:
    std::vector<Edge> edges;
};

int main() {

    return 0;
}
