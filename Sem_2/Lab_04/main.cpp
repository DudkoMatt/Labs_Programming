#include <iostream>
#include <cmath>
#include <vector>

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
        std::cin >> x >> y;
    }

    double distance_to_point(Point A) {
        return sqrt((x - A.x) * (x - A.x) + (y - A.y) * (y - A.y));
    }

    void print() {
        std::cout << "(" << x << "; " << y << ")\n";
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
//        std::cout << "Координаты первой точки: (" << X.x << "; " << X.y << ")\n";
//        std::cout << "Координаты второй точки: (" << Y.x << "; " << Y.y << ")\n";

        std::cout << "Координаты первой точки:";
        X.print();
        std::cout << "Координаты второй точки:";
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
public:
    virtual void initFromDialog() = 0;
};

// Интерфейс "Класс"
class BaseObject : public IGeoFig, public IDialogInitiable, public IPrintable {
public:
// Имя класса (типа данных).
    virtual const char *classname() = 0;

// Размер занимаемой памяти.
    virtual unsigned int size() = 0;
};

class Hexagon : public BaseObject {
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
        std::cout << "Центр шестиугольника: ";
        center.print();
        std::cout << "Длина основания: " << length << "\n";
    }

    const char *classname() override {
        return "Hexagon";
    }

    unsigned int size() override {
        return sizeof(*this);
    }

    void initFromDialog() override {
        std::cout << "Инициализация\n";
        std::cout << "Введите координаты точки центра через пробел: ";
        center.initFromDialog();
        std::cout << "Введите длину ребра шестиугольника: ";
        std::cin >> length;
    }
};

class IsoscelesTrapezium : public BaseObject {
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
        std::cout << "Первое основание трапеции\n";
        a.print();
        std::cout << "\nВторое основание трапеции\n";
        b.print();
    }

    const char *classname() override {
        return "IsoscelesTrapezium";
    }

    unsigned int size() override {
        return sizeof(*this);
    }

    void initFromDialog() override {
        std::cout << "Инициализация\n";
        std::cout << "Первое основание трапеции\n";
        std::cout << "Введите координаты первой точки через пробел: ";
        a.X.initFromDialog();

        std::cout << "Введите координаты второй точки через пробел: ";
        a.Y.initFromDialog();

        std::cout << "\nВторое основание трапеции\n";
        std::cout << "Введите координаты первой точки через пробел: ";
        b.X.initFromDialog();

        std::cout << "Введите координаты второй точки через пробел: ";
        b.Y.initFromDialog();
    }
};


int main() {

    /*

    // Тестирование классов

    // Шестиугольник
    std::cout << "Шестиугольник\n-------------\n";

    Hexagon h;
    h.initFromDialog();

    std::cout << "Площадь: " << h.square() << "\n";
    std::cout << "Периметр: " << h.perimeter() << "\n";

    std::cout << "Имя класса: " << h.classname() << "\n";
    std::cout << "Размер: " << h.size() << "\n";

    h.draw();
    std::cout << "\n";

    // Равнобедренная трапеция
    std::cout << "\nРавнобедренная трапеция\n-----------------------\n";

    IsoscelesTrapezium trapezium;
    trapezium.initFromDialog();

    std::cout << "Площадь: " << trapezium.square() << "\n";
    std::cout << "Периметр: " << trapezium.perimeter() << "\n";

    std::cout << "Имя класса: " << trapezium.classname() << "\n";
    std::cout << "Размер: " << trapezium.size() << "\n";

    trapezium.draw();

     */

    // Хранение множества фигур
    std::vector<BaseObject*> geoObjects;

    std::cout << "Введите количество хранимых объектов: ";

    int k;
    std::cin >> k;

    if (k <= 0) return 0;

    // Добавление пользователем через консоль
    for (int i = 0; i < k; ++i) {
        int type;
        std::cout << "\nТипы объектов: \n";
        std::cout << "1: Шестиугольник\n";
        std::cout << "2: Равнобедренная трапеция\n";
        do {
            std::cout << "Введите тип объекта: ";
            std::cin >> type;
        }
        while (!(type == 1 || type == 2));

        if (type == 1)
            geoObjects.push_back(new Hexagon);
        else
            geoObjects.push_back(new IsoscelesTrapezium);

        geoObjects.back()->initFromDialog();
    }

    std::cout << "\n\nОтражение всех фигур:\n";

    // Отражение всех фигур
    int count = 0;
    for (auto i : geoObjects) {
        std::cout << count++ << "): " << i->classname() << "\n";
        i->draw();
        std::cout << "\n";
    }


    // Суммарная площадь всех фигур

    std::cout << "\nСуммарная площадь всех фигур: ";

    double s = 0;
    for (auto i : geoObjects) {
        s += i->square();
    }

    std::cout << s << "\n";


    // Суммарный периметр всех фигур

    std::cout << "Суммарный периметр всех фигур: ";

    s = 0;
    for (auto i : geoObjects) {
        s += i->square();
    }

    std::cout << s << "\n";


    // Память, занимаемая всеми экземплярами классов
    std::cout << "Память, занимаемая всеми экземплярами классов: ";

    unsigned int r = 0;
    for (auto i : geoObjects) {
        r += i->size();
    }

    std::cout << r << "\n";

    return 0;
}
