#include <iostream>
#include "my_lib.h"

using namespace std;

/*
 * Лабораторная работа #3. Перегрузка операторов.
 *
 * Согласно варианту описать указанные типы данных и поместить их в отдельный заголовочный файл, в нем же описать
 * операторы, указанные в варианте. Реализацию функций поместить в отдельный cpp файл. Написать программу, проверяющую
 * правильность работы – для наглядности и лучшего усвоения материала использовать как явный, так и не явный метод
 * вызова функций операторов (см. пример в конце задания).
 *
 *
 * Вариант 5:
 *
 *
 * Задание 3:
 * Треугольник на плоскости (самостоятельно выбирайте необходимые данные).
 *
 * Операторы:
 * Равенство площадей треугольников (перегрузите операции ==, !=, <, >) – для вычисления площади можете использовать,
 * например, формулу Герона (зависит от тех данных, что используются для хранения треугольника). Прибавление вектора
 * (смещение треугольника на указанный вектор).
 *
 *
 * Задание 7:
 * Подмножество множества целых чисел от нуля до девяти: { 0, 1, 2, ... 9 }.
 *
 * Операторы:
 * Объединение двух множеств (operator+).
 * Сравнение (== и !=).
 * Добавление числа в множество (operator+=).
 * Изъятие числа из множества (operator-=).
 *
 */

int main() {
    // Задание 3
    Triangle t1(Point(0, 0), Point(3, 0), Point(0, 4));
    Triangle t2(Point(0, 0), Point(4, 0), Point(0, 3));
    Triangle t3(Point(-10, 0), Point(4, 0), Point(0, 3));

    cout << "Area t1: " << t1.area << "\n";
    cout << "Area t2: " << t2.area << "\n";
    cout << "Area t3: " << t3.area << "\n\n";

    cout << "t1 == t2: " << (t1 == t2 ? "true" : "false") << "\n";
    cout << "t1 == t3: " << (t1 == t3 ? "true" : "false") << "\n";
    cout << "t1 != t3: " << (t1 != t3 ? "true" : "false") << "\n\n";

    cout << "Print t1:\n";
    t1.print();
    t1.move(Point(1, 1));
    cout << "\nMoved on (1; 1)\n";
    t1.print();
    cout << "Area: " << t1.area << "\n\n";


    // Задание 7
    DecSet d1;
    DecSet d2;
    DecSet d3;
    DecSet d4;

    d1 += 1;
    d1 += 2;
    d1 += 3;

    d2 += 8;
    d2 += 9;

    d3 = d1 + d2;
    d4 = d1 + d2;

    cout << "Print d1:\n";
    d1.print();
    cout << "\n";

    cout << "Print d2:\n";
    d2.print();
    cout << "\n";

    cout << "Print d3:\n";
    d3.print();
    cout << "\n";

    cout << "Print d4:\n";
    d4.print();
    cout << "\n";

    cout << "d1 == d2: " << (d1 == d2 ? "true" : "false") << "\n";
    cout << "d1 != d2: " << (d1 != d2 ? "true" : "false") << "\n";
    cout << "d3 == d4: " << (d3 == d4 ? "true" : "false") << "\n";
}