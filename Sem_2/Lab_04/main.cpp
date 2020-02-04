#include <iostream>
#include <cmath>

using namespace std;

/*
 * Лабораторная работа #4. “Виртуальные функции”.
 * Реализовать все указанные интерфейсы (абстрактные базовые классы) для классов (согласно варианту):
 * A. Круг
 * B. Отрезок
 * C. Равносторонний треугольник
 * D. Прямоугольник
 * E. Шестиугольник
 * F. Параллелограмм
 * G. Равнобедренная трапеция
 * H. Эллипс (периметр можно считать по любой приближенной формуле: см. интернет, справочники и т.п.).
 *
 * Функционал системы:
 * - Хранение множества фигур
 * - Динамическое добавление фигур пользователем. (через консоль)
 * - Отобразить все фигуры.
 * - Суммарная площадь всех фигур.
 * - Суммарный периметр всех фигур.
 * - Центр масс всей системы.
 * - Память, занимаемая всеми экземплярами классов.
 * - Сортировка фигур между собой по массе.
 *
 * Вопросы для обдумывания:
 * - Есть ли необходимость делать методы сравнения по массе виртуальными?
 * - Получится ли также перегрузить операторы сравнения для интерфейса
 * BaseCObject чтобы сравнивать объекты по объему занимаемой памяти?
 * - Предположите, что в дальнейшем придется изменить код таким образом,
 * чтобы фигуры (оставаясь сами по себе плоскими) задавались уже не в
 * двумерном, а в трехмерном пространстве. Укажите как бы вы действовали?
 * Что пришлось бы изменить?
 *
 * Вариант 5:
 * E. Шестиугольник
 * G. Равнобедренная трапеция
 *
 */

// Интерфейс "Геометрическая фигура".
class IGeoFig {
public:
// Площадь.
    virtual double square() = 0;

// Периметр.
    virtual double perimeter() = 0;
};

class Point {
public:
    double x, y;

    void initFromDialog() {
        cin >> x >> y;
    }

    double distance_to_point(Point A) {
        return sqrt((x - A.x) * (x - A.x) + (y - A.y) * (y - A.y));
    }

    void print() {
        cout << "(" << x << "; " << y << ")\n";
    }

};

// Note: Переопределено по сравнению с условием
class Vector2D {
public:
    Point X, Y;

    Point middle() {
        return {(X.x + Y.x) / 2.0, (X.y + Y.y) / 2.0};
    }

    double length() {
        return X.distance_to_point(Y);
    }

    void print() {
//        cout << "Координаты первой точки: (" << X.x << "; " << X.y << ")\n";
//        cout << "Координаты второй точки: (" << Y.x << "; " << Y.y << ")\n";

        cout << "Координаты первой точки:";
        X.print();
        cout << "Координаты второй точки:";
        Y.print();
    }
};

// Интерфейс "Отображаемый"
class IPrintable {
public:
// Отобразить на экране
// (выводить в текстовом виде параметров фигуры).
    virtual void draw() = 0;
};

// Интерфейс для классов, которые можно задать через диалог с пользователем.
class IDialogInitiable {
// Задать параметры объекта с помощью диалога с пользователем.
    virtual void initFromDialog() = 0;
};

// Интерфейс "Класс"
class BaseObject {
public:
// Имя класса (типа данных).
    virtual const char *classname() = 0;

// Размер занимаемой памяти.
    virtual unsigned int size() = 0;
};

class Hexagon : public IGeoFig, public BaseObject, public IDialogInitiable, public IPrintable {
public:

    Point center{0, 0};
    double length = 0;

    double square() override {
        return 6 * length * length * sqrt(3) / 4.0;
    }

    double perimeter() override {
        return 6 * length;
    }

    void draw() override {
        cout << "Центр шестиугольника: ";
        center.print();
        cout << "Длина основания: " << length;
    }

    const char *classname() override {
        return "Hexagon";
    }

    unsigned int size() override {
        return sizeof(Hexagon);
    }

    void initFromDialog() override {
        cout << "Инициализация\n";
        cout << "Введите координаты точки центра через пробел: ";
        center.initFromDialog();
        cout << "Введите длину ребра шестиугольника: ";
        cin >> length;
    }
};

class IsoscelesTrapezium : public IGeoFig, public BaseObject, public IDialogInitiable, public IPrintable {
public:

    // Пусть трапеция задана через два сонаправленных вектора, обозначающих основания трапеции
    Vector2D a{{0, 0},
               {0, 0}};

    Vector2D b{{0, 0},
               {0, 0}};

    double square() override {
        return a.middle().distance_to_point(b.middle()) * (a.length() + b.length()) / 2.0;
    }

    double perimeter() override {
        // Note: Векторы сонаправлены
        return a.length() + b.length() + a.X.distance_to_point(b.X) + a.Y.distance_to_point(b.Y);
    }

    void draw() override {
        cout << "Первое основание трапеции\n";
        a.print();
        cout << "\nВторое основание трапеции\n";
        b.print();
    }

    const char *classname() override {
        return "IsoscelesTrapezium";
    }

    unsigned int size() override {
        return sizeof(IsoscelesTrapezium);
    }

    void initFromDialog() override {
        cout << "Инициализация\n";
        cout << "Первое основание трапеции\n";
        cout << "Введите координаты первой точки через пробел: ";
        a.X.initFromDialog();

        cout << "Введите координаты второй точки через пробел: ";
        a.Y.initFromDialog();

        cout << "\nВторое основание трапеции\n";
        cout << "Введите координаты первой точки через пробел: ";
        b.X.initFromDialog();

        cout << "Введите координаты второй точки через пробел: ";
        b.Y.initFromDialog();
    }
};


int main() {

    // Шестиугольник
    cout << "Шестиугольник\n-------------\n";

    Hexagon h;
    h.initFromDialog();

    cout << "Площадь: " << h.square() << "\n";
    cout << "Периметр: " << h.perimeter() << "\n";

    cout << "Имя класса: " << h.classname() << "\n";
    cout << "Размер: " << h.size() << "\n";

    h.draw();
    cout << "\n";

    // Равнобедренная трапеция
    cout << "\nРавнобедренная трапеция\n-----------------------\n";

    IsoscelesTrapezium trapezium;
    trapezium.initFromDialog();

    cout << "Площадь: " << trapezium.square() << "\n";
    cout << "Периметр: " << trapezium.perimeter() << "\n";

    cout << "Имя класса: " << trapezium.classname() << "\n";
    cout << "Размер: " << trapezium.size() << "\n";

    trapezium.draw();

    return 0;
}
