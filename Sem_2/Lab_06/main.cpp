#include <iostream>
#include <set>
#include <vector>
using namespace std;

/*
 * Лабораторная работа #6
 *
 * Требуется реализовать следующие обобщенные алгоритмы.
 * 1. all_of - возвращает true, если все элементы диапазона удовлетворяют
 * некоторому предикату. Иначе false
 * 2. any_of - возвращает true, если хотя бы один из элементов диапазона
 * удовлетворяет некоторому предикату. Иначе false
 * 3. none_of - возвращает true, если все элементы диапазона не удовлетворяют
 * некоторому предикату. Иначе false
 * 4. one_of - возвращает true, если ровно один элемент диапазона удовлетворяет
 * некоторому предикату. Иначе false
 * 5. is_sorted - возвращает true, если все элементы диапазона находятся в
 * отсортированном порядке относительно некоторого критерия
 * 6. is_partitioned - возвращает true, если в диапазоне есть элемент, делящий все
 * элементы на удовлетворяющие и не удовлетворяющие некоторому предикату.
 * Иначе false.
 * 7. find_not - находит первый элемент, не равный заданному
 * 8. find_backward - находит первый элемент, равный заданному, с конца
 * 9. is_palindrome - возвращает true, если заданная последовательность является
 * палиндромом относительно некоторого условия. Иначе false.
 *
 * Каждый алгоритм должен быть выполнен в виде шаблонной функции, позволяющей взаимодействовать со стандартными
 * контейнерами STL с помощью итераторов. Предикаты, условия, операторы сравнения должны быть параметризованы.
 *
 * При сдаче работы требуется продемонстрировать работу алгоритмов как на стандартных, так и на пользовательских
 * типах данных, например CPoint, CRational, далее работает ваша индивидуальная (не "коллективная") фантазия.
 *
 * Вариант 5:
 * Задания 2, 5, 9
 */

class IsEven {
public:
    bool operator() (int x) {
        return x % 2 == 0;
    }
};

// Задание 2
template <class T, class Predicate>
bool any_of(T obj, Predicate p) {
    typename T::const_iterator it = obj.begin();
    while (it != obj.end()) {
        if (p(*it))
            return true;
        ++it;
    }
    return false;
}

// Задание 5
template <class T, class U>
bool is_sorted(T obj, bool (*compare)(U o1, U o2)) {
    typename T::const_iterator it = obj.begin();
    typename T::const_iterator it_next = it + 1;
    if (it == obj.end() or it_next == obj.end()) return true;
    while (it_next != obj.end()) {
        if (!compare(*it, *it_next))
            return false;
        ++it;
        ++it_next;
    }
    return true;
}

// Задание 9
template <class T, class U>
bool is_palindrome(T obj, bool (*compare)(U o1, U o2)) {
    typename T::const_iterator it = obj.begin();
    typename T::const_iterator it_end = obj.end() - 1;
    while (it < it_end) {
        if (!compare(*it, *it_end)) {
            return false;
        }
        ++it;
        --it_end;
    }
    return true;
}

class Point {
private:
    double x, y;
public:
    Point(double x, double y) : x(x), y(y) {}

    double getX() const {
        return x;
    }

    double getY() const {
        return y;
    }

    bool operator==(const Point &p) const {
        return (x == p.getX()) and (y == p.getY());
    }
};

class Comparator {
public:
    bool operator()(Point p) {
        return p == Point(0, 0);
    }
};

template <class T>
bool is_less(T x, T y) {
    return x < y;
}

template <class T>
bool is_equals(T x, T y) {
    return x == y;
}

int main() {

    std::set<int> mySet, mySet1;
    mySet.insert(8);
    mySet.insert(3);
    mySet.insert(-4);
    mySet.insert(9);
    mySet.insert(2);

    mySet1.insert(3);

    cout << "Is any of mySet even?: " << (any_of(mySet, IsEven()) ? "true" : "false") << "\n";
    cout << "Is any of mySet1 even?: " << (any_of(mySet1, IsEven()) ? "true" : "false") << "\n";

    vector<Point> s;
    s.push_back(Point(0, 0));
    s.push_back(Point(0, 1));
    s.push_back(Point(1, 1));

    cout << "Is any of s is Point(0;0)?: " << (any_of(s, Comparator()) ? "true" : "false") << "\n";

    vector<int> p, p1, p2;
    p.push_back(1);
    p1.push_back(1);
    p1.push_back(2);
    p1.push_back(1);
    p2.push_back(1);
    p2.push_back(2);
    p2.push_back(3);

    cout << "Is p sorted?: " << (is_sorted(p, is_less<int>) ? "true" : "false") << "\n";
    cout << "Is p1 sorted?: " << (is_sorted(p1, is_less<int>) ? "true" : "false") << "\n";
    cout << "Is p2 sorted?: " << (is_sorted(p2, is_less<int>) ? "true" : "false") << "\n";

    cout << "Is p a palindrome?: " << (is_palindrome(p, is_equals<int>) ? "true" : "false") << "\n";
    cout << "Is p1 a palindrome?: " << (is_palindrome(p1, is_equals<int>) ? "true" : "false") << "\n";
    cout << "Is p2 a palindrome?: " << (is_palindrome(p2, is_equals<int>) ? "true" : "false") << "\n";
    
    vector<Point> p3, p4;
    p3.push_back(Point(0, 0));
    p3.push_back(Point(0, 1));
    p3.push_back(Point(1, 1));
    p3.push_back(Point(0, 1));
    p3.push_back(Point(0, 0));

    p4.push_back(Point(0, 0));
    p4.push_back(Point(0, 1));
    p4.push_back(Point(1, 1));
    p4.push_back(Point(0, 1));
    p4.push_back(Point(0, 0));
    p4.push_back(Point(0, 0));

    cout << "Is p3 a palindrome?: " << (is_palindrome(p3, is_equals<Point>) ? "true" : "false") << "\n";
    cout << "Is p4 a palindrome?: " << (is_palindrome(p4, is_equals<Point>) ? "true" : "false") << "\n";

    return 0;
}
