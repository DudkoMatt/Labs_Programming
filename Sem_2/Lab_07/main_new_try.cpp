#include <iostream>
#include <algorithm>

/*
 * Лабораторная работа №7
 * Реализовать кольцевой буфер в виде stl-совместимого
 * контейнера (например, может быть использован с стандартными
 * алгоритмами), обеспеченного итератором произвольного доступа.
 * Реализация не должна использовать ни одни из контейнеров STL.
 *
 * Буфер должен обладать следующими возможностями:
 * 1. Вставка и удаление в конец
 * 2. Вставка и удаление в начало
 * 3. Вставка и удаление в произвольное место по итератору
 * 4. Доступ в конец, начало
 * 5. Доступ по индексу
 * 6. Изменение капасити
 */


template <class T>
class CircleBuffer {
public:
    explicit CircleBuffer(int init_capacity = INIT_CAPACITY) {
        capacity = init_capacity;
        head = 0;
        tail = 0;
        size = 0;
        array_head_ptr = (T *) calloc(sizeof(T), init_capacity);
        if (array_head_ptr == nullptr)
            throw std::bad_alloc();
    }

    ~CircleBuffer() {
        if (array_head_ptr)
            free(array_head_ptr);
    }

    struct Position {
        T* p;
        explicit Position(T* p) : p(p) {}

        T& dereference() const { return *p; } // Получение текущего элемента.
        bool equal(const Position& other) const { return p == other.p; } // Проверка на равенство.
        void increment() { ++p; } // Перемещение вперед.
        void decrement() { --p; } // Перемещение назад.
        void move_forward(int n) { p += n; } // Перемещение на "n" элементов.
        int distance_to(const Position& other) const { return other.p - p; } // Расстояние до другой позиции.
    };

    struct iterator : std::iterator<std::random_access_iterator_tag, T> {
        // Вложенный объект Position, и конструктор для него.
        Position pos;
        iterator(Position pos) : pos(pos) {}

        // Операции, необходимые для всех категорий итераторов.
        iterator() = default;
        iterator(const iterator&) = default;
        iterator& operator=(const iterator&) = default;
        ~iterator() = default;
        T& operator*() const { return pos.dereference(); }

        // ToDO:
        iterator& operator++() { pos.increment(); return *this; }
        iterator operator++(int) { auto old = *this; ++(*this); return old; }

        // Операции, необходимые для InputIterator.
        T* operator->() const;

        // Операции, необходимые для BidirectionalIterator.
        // ToDO:
        iterator& operator--() { pos.decrement(); return *this; }
        iterator operator--(int) { auto old = *this; --(*this); return old; }

        // Операции, необходимые для RandomAccessIterator.
        T& operator[](int n) const { auto tmp = *this; tmp += n; return *tmp; }
        iterator& operator+=(int n) { pos.move_forward(n); return *this; }
        iterator& operator-=(int n) { return *this += -n; }


        // ---------------------------------------------------------------

        // Операции, необходимые для InputIterator.
        bool operator==(const iterator& rhs) const { return this->pos.equal(rhs.pos); }
        bool operator!=(const iterator& rhs) const { return !(this->operator==(rhs)); }

        // Операции, необходимые для RandomAccessIterator.
        bool operator<(const iterator& rhs) const { return this->pos.distance_to(rhs.pos) > 0; }

        bool operator>(const iterator& rhs) const { return rhs.operator<(*this); }
        bool operator<=(const iterator& rhs) const { return !(rhs.operator>(*this)); }
        bool operator>=(const iterator& rhs) const { return !(this->operator<(rhs)); }
        iterator operator+(int n) { this->operator+=(n); return *this; }

        // ToDO:
        //        iterator operator+(int n, iterator it) { return it + n; }

        iterator operator-(int n) { this->operator-=(n); return *this; }

        // ToDO:
        //        int operator-(const iterator& lhs, const iterator& rhs) { return rhs.pos.distance_to(lhs.pos); }

    };

    // Операции, необходимые для всех категорий итераторов.
    void swap(iterator& a, iterator& b) { std::swap(a.pos, b.pos); }

    /* Буфер должен обладать следующими возможностями:
    *  1. Вставка и удаление в конец
    *  2. Вставка и удаление в начало
    *  3. Вставка и удаление в произвольное место по итератору
    *  4. Доступ в конец, начало
    *  5. Доступ по индексу
    *  6. Изменение капасити
    */

    iterator begin() {
        return iterator(Position(array_head_ptr + head));
    }

    iterator end() {
        return iterator(Position(array_head_ptr + size));
    }

    void push_back(T t) {
        // ToDO
        if (tail == head && size != 0)
            throw std::out_of_range("Buffer overflow");

        *this->end() = t;
        size++;
        tail = (tail + 1 + capacity) % capacity;
    }

    T pop_back() {
        // ToDO
        if (size == 0)
            throw std::out_of_range("Trying to remove when size == 0");

        T value = *(end() - 1);
        tail = (tail - 1 + capacity) % capacity;
        size--;
        return value;
    }

    void push_front(T t) {
        // ToDO
        if (head == tail && size != 0)
            throw std::out_of_range("Buffer overflow");

        *(array_head_ptr + (head - 1 + capacity) % capacity) = t;
        head = (head - 1 + capacity) % capacity;
        size++;
    }

    T pop_front() {
        // ToDO
        if (size == 0)
            throw std::out_of_range("Trying to remove when size == 0");

        T value = *begin();
        head = (head + 1) % capacity;
        size--;
        return value;
    }

    void insert(const iterator it, const T& val) {
        // ToDO
        if (head == tail && size != 0)
            throw std::out_of_range("Buffer overflow");
        if (it > end() && it < begin())
            throw std::out_of_range("Index out of range");

        iterator back_iter = end();
        while (back_iter != it) {
            *back_iter = *(back_iter - 1);
            --back_iter;
        }
        *it = val;

        size++;
        tail = (tail + 1) % capacity;
    }

    void erase(iterator it) {
        // ToDO
        if (size == 0)
            throw std::out_of_range("Trying to remove when size == 0");

        while (it != end()) {
            *it = *(it + 1);
            ++it;
        }

        tail = (tail - 1 + capacity) % capacity;
        size--;
    }

    T& front() {
        return *(array_head_ptr[head]);
    }

    T& back() {
        return *(array_head_ptr[tail - 1]);
    }

    T& at(int idx) {
        return *(array_head_ptr[idx]);
    }

    void change_capacity(unsigned new_capacity) {
        T* new_array_ptr = (T*) calloc(sizeof(T), new_capacity);
        if (!new_array_ptr)
            throw std::bad_alloc();

        int k = 0;
        iterator it = this->begin();
        while (it != this->end()) {
            *(new_array_ptr + k) = *it;
            ++k;
        }

        free(array_head_ptr);
        array_head_ptr = new_array_ptr;
        capacity =  new_capacity;
        head = 0;

        if (size > new_capacity) {
            size = new_capacity;
            tail = 0;
        } else {
            tail = size;
        }
    }

private:
    static const unsigned INIT_CAPACITY = 20;
    T *array_head_ptr;
    int head, tail;
    unsigned size;
    unsigned capacity;
};

template<class T>
typename CircleBuffer<T>::iterator operator+(int n, typename CircleBuffer<T>::iterator it) { return it + n; }

template<class T>
int operator-(const typename CircleBuffer<T>::iterator& lhs, const typename CircleBuffer<T>::iterator& rhs) { return rhs.pos.distance_to(lhs.pos); }

int main() {
    CircleBuffer<int> buffer = CircleBuffer<int>(5);
    buffer.push_back(0);
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    buffer.push_back(4);

    for (auto iterator = buffer.begin(); iterator != buffer.end(); ++iterator) {
        std::cout << *iterator << " ";
    }

    std::fill(buffer.begin(), buffer.end(), 1);

    for (auto iterator = buffer.begin(); iterator != buffer.end(); ++iterator) {
        std::cout << *iterator << " ";
    }

    std::cout << "\n";

    return 0;
}
