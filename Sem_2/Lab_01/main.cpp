#include <iostream>
#include "my_lib.h"
using namespace std;

/*
 * Лабораторная #1. Передача значения по ссылке.
 *
 * 1. Объявите (в отельном заголовочном файле) и реализуйте (в другом файле)
 *    процедуры (они не возвращают значений!) согласно варианту.
 * 2. Все процедуры должны быть написаны в двух вариантах – один вариант использует
 *    указатели, второй вариант – ссылки.
 * 3. Напишите программу, проверяющую и демонстрирующую правильность работы
 *    процедур.
 *
 * Вариант 5:
 *
 * Процедура:
 * 5:  Отбрасывает от вещественного числа его целую часть.
 * 8:  Изменяют вещественную переменную на обратное к ней число.
 * 13: Передвигает квадрат на заданный вектор.
 * 16: Меняет в матрице местами две указанные строчки.
 *
 */

int main() {
    // # 5
    cout << "-------------------\nProcedure number 5:\n-------------------\n";

    double x = 1.23;

    // Через указатель:
    cout << "Before function: " << x << "\n";
    frac(&x);
    cout << "After function: " << x << "\n\n";

    x = 1.23;

    // Через ссылку:
    cout << "Before function: " << x << "\n";
    frac(x);
    cout << "After function: " << x << "\n\n";


    // # 8
    cout << "-------------------\nProcedure number 8:\n-------------------\n";

    x = 1.23;

    // Через указатель:
    cout << "Before function: " << x << "\n";
    inverse(&x);
    cout << "After function: " << x << "\n";
    cout << "Multiply on 1.23: " << x * 1.23 << "\n\n";

    x = 1.23;

    // Через ссылку:
    cout << "Before function: " << x << "\n";
    inverse(x);
    cout << "After function: " << x << "\n";
    cout << "Multiply on 1.23: " << x * 1.23 << "\n\n";

    // # 13
    cout << "--------------------\nProcedure number 13:\n--------------------\n";

    Square square = {{0, 0}, {1,1}};
    Vector v = {{1,1}};

    // Через указатель:
    cout << "Before function:\n";
    print_square(&square);
    move(&square, &v);
    cout << "After function:\n";
    print_square(&square);
    cout << "\n";

    // Через ссылку:
    cout << "Before function:\n";
    print_square(&square);
    move(square, v);
    cout << "After function:\n";
    print_square(&square);
    cout << "\n";

    // # 16
    cout << "--------------------\nProcedure number 16:\n--------------------\n";

    Matrix matrix;
    init_matrix(&matrix, 3);
    init_rows(&matrix, 2);

    matrix.a[0].a[0] = 1;
    matrix.a[0].a[1] = 2;

    matrix.a[1].a[0] = 3;
    matrix.a[1].a[1] = 4;

    matrix.a[2].a[0] = 5;
    matrix.a[2].a[1] = 6;

    // Через указатель:
    cout << "Before function:\n";
    print_matrix(&matrix);
    swap(&matrix, 0, 2);
    cout << "After function:\n";
    print_matrix(&matrix);
    cout << "\n";

    // Через ссылку:
    cout << "Before function:\n";
    print_matrix(&matrix);
    swap(matrix, 0, 1);
    cout << "After function:\n";
    print_matrix(&matrix);
    cout << "\n";

}
