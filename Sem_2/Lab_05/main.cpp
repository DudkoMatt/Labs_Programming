#include <iostream>

using namespace std;

/*
 * Лабораторная работа #5. Шаблоны, исключения.
 * Написать шаблонную функцию или класс согласно варианту. Описать класс-исключение или иерархию классов-исключений.
 * Генерировать исключения в соответствующих исключительных ситуациях. Если у вас есть другие предложения
 * по исключительным ситуациям – используйте их.
 *
 * Задания A и F:
 *
 * A: Функция
 * ----------
 * Описание:
 * Поиск максимального элемента в массиве.
 *
 * Параметры шаблона:
 * Тип элементов массива
 *
 * F: Класс
 * --------
 * Описание:
 * Массив из N элементов типа T
 *
 * Параметры шаблона:
 * int N, class T
 *
 * Исключение:
 * Обращение по несуществующему индексу
 */

// Задание A
template<class T>
T *find_max(T *array, size_t n) {
    T *max = array;
    for (int i = 0; i < n; ++i) {
        if (*max < *(array + i))
            max = array + i;
    }
    return max;
}

// Задание F

class ArrayBasicException : public exception {
public:
    explicit ArrayBasicException(const string &message = "") {
        cerr << message;
    }
};

class ArrayOutOfBoundsException : public ArrayBasicException {
public:
    explicit ArrayOutOfBoundsException(int index, size_t max_size) :
        ArrayBasicException("Index out of bounds: " + to_string(index) +
            + "\nMax allowed size: " + to_string(max_size)) {}
};

template<int N, class T>
class Array {
public:
    T *array;

    Array() {
        array = (T *) calloc(sizeof(T), N);
    }

    void setElement(size_t index, T element) {
        if (index >= 0 and index < N)
            *(array + index) = element;
        else
            throw ArrayOutOfBoundsException(index, N);
    }

    T getElement(size_t index) {
        if (index >= 0 and index < N)
            cout << *(array + index);
        else
            throw ArrayOutOfBoundsException(index, N);
    }
};

int main() {
    int a[] = {1, 4, 3, 5};
    cout << "Максимум в массиве: " << *find_max(a, 4) << "\n\n";

    Array<10, int> array{};
    for (int i = 0; i < 10; ++i) {
        array.setElement(i, i);
    }

    cout << "Массив: ";
    for (int i = 0; i < 10; ++i) {
        array.getElement(i);
        cout << " ";
    }
    cout << "\n";

    cout << "Попытка обратиться за пределы массива\n";
    array.getElement(-1);
}
