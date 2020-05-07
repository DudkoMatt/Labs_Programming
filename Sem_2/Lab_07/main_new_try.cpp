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
        T* array_head_ptr;
        int head, tail;
        unsigned size;
        unsigned capacity;
        explicit Position(T* p, T* array_head_ptr, int head, int tail, unsigned capacity, unsigned size)
            : p(p), array_head_ptr(array_head_ptr), head(head), tail(tail), capacity(capacity), size(size) {}

        T& dereference() const { return *p; } // Получение текущего элемента.
        bool equal(const Position& other) const { return p == other.p; } // Проверка на равенство.
        void increment() { // Перемещение вперед.
            if (p == array_head_ptr + (head + size - 1) % capacity) {
                // Выводим за пределы - после выделенной памяти
                p = array_head_ptr + tail;
            } else if (p == array_head_ptr + tail && tail != capacity) {
                p = array_head_ptr + capacity;
            } else if (p >= array_head_ptr + capacity) {
                ++p;
            } else if (p == array_head_ptr - 1) {
                // Если за пределами слева
                p = array_head_ptr + head;
            } else if (p < array_head_ptr) {
                ++p;
            } else {
                // ToDO:
                if (p - array_head_ptr == capacity - 1)
                    p = array_head_ptr;
                else
                    ++p;
            }
        }

        void decrement() { // Перемещение назад.
            if (p == array_head_ptr + head) {
                // Выводим за пределы - сразу перед выделенной памятью
                p = array_head_ptr - 1;
            } else if (p < array_head_ptr) {
                --p;
            } else if (p == array_head_ptr + capacity) {
                p = array_head_ptr + (head + size - 1) % capacity;
            } else if (p > array_head_ptr + capacity) {
                --p;
            } else {
                if (p == array_head_ptr)
                    p = array_head_ptr + capacity - 1;
                else
                    --p;
            }
        }

        // ToDO:
        void move_forward(int n) { // Перемещение на "n" элементов.
            if (n > 0)
                for (int i = 0; i < n; ++i) {
                    this->increment();
                }
            else
                for (int j = 0; j < -n; ++j) {
                    this->decrement();
                }

//            if (array_head_ptr + tail < p) {
//                if (((array_head_ptr + tail + capacity) - p) % capacity >= n) {
//                    // Хватает элементов
//                    p = array_head_ptr + (p - array_head_ptr + n) % capacity;
//                } else {
//                    // Не хватает
//                    p = array_head_ptr + capacity + ((array_head_ptr + tail + capacity) - p) % capacity - n;
//                    //                            ^ -- сколько элементов не хватило
//                }
//            } else {
//                // p <= tail
//                if (tail - (p - array_head_ptr) >= n) {
//                    // Хватает элементов
//                    p = array_head_ptr + (p - array_head_ptr + n) % capacity;
//                } else {
//                    // Не хватает
//                    p = array_head_ptr + capacity + ((array_head_ptr + tail + capacity) - p) % capacity - n;
//                    //                            ^ -- сколько элементов не хватило
//                }
//            }
        }

        int distance_to(const Position& other) const { // Расстояние до другой позиции.
            // ToDO

            int other_idx;
            if (other.p >= array_head_ptr + capacity) {
                other_idx = other.p - (array_head_ptr + capacity) + (size - 1);
            } else if (other.p < array_head_ptr) {
                other_idx = -(array_head_ptr - other.p);
            } else {
                // Внутри массива
                other_idx = other.p - (array_head_ptr + head);
            }

            int this_idx;
            if (p >= array_head_ptr + capacity) {
                this_idx = p - (array_head_ptr + capacity) + (size - 1);
            } else if (p < array_head_ptr) {
                this_idx = -(array_head_ptr - p);
            } else {
                // Внутри массива
                this_idx = p - (array_head_ptr + head);
            }

            return other_idx - this_idx;

//            if (other.p < p)
//                return (other.p + capacity - p) % capacity;
//            else
//                return other.p - p;
        }
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
        iterator& operator++() { pos.increment(); return *this; }
        iterator operator++(int) { auto old = *this; ++(*this); return old; }

        // Операции, необходимые для InputIterator.
        T* operator->() const;

        // Операции, необходимые для BidirectionalIterator.
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
        iterator operator+(int n) { iterator new_it(this->pos); new_it.operator+=(n); return new_it; }

        // ToDO:
        //        iterator operator+(int n, iterator it) { return it + n; }

        iterator operator-(int n) { iterator new_it(this->pos); new_it.operator-=(n); return new_it; }

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
        return iterator(Position(array_head_ptr + head, array_head_ptr, head, tail, capacity, size));
    }

    iterator end() {
        return iterator(Position(array_head_ptr + tail, array_head_ptr, head, tail, capacity, size));
    }

    void push_back(T t) {
        if (size == capacity || tail >= capacity)
            throw std::out_of_range("Buffer overflow");

        *this->end() = t;
        size++;
        tail = (tail + 1 + capacity) % capacity;
        if (tail == head) {
            tail = capacity;
        }
    }

    T pop_back() {
        // ToDO
        if (size == 0)
            throw std::out_of_range("Trying to remove when size == 0");

        if (tail == capacity) {
            // size == capacity
            tail = (head + size - 1) % capacity;
        } else {
            --tail;
        }

        T value = *(array_head_ptr + tail);
        size--;
        return value;
    }

    void push_front(T t) {
        // ToDO
        if (size == capacity)
            throw std::out_of_range("Buffer overflow");

        *(array_head_ptr + (head - 1 + capacity) % capacity) = t;
        head = (head - 1 + capacity) % capacity;
        size++;
        if (size == capacity)
            tail = capacity;
    }

    T pop_front() {
        // ToDO
        if (size == 0)
            throw std::out_of_range("Trying to remove when size == 0");

        T value = *begin();
        head = (head + 1) % capacity;
        size--;

        if (tail == capacity)
            tail = (head + size) % capacity;

        return value;
    }

    void insert(iterator it, const T& val) {
        if (size == capacity)
            throw std::out_of_range("Buffer overflow");
        if (it > end() && it < begin())
            throw std::out_of_range("Index out of range");

        iterator back_iter = end();
        while (back_iter != it) {
            T value = *(back_iter - 1);
            *back_iter = value;
            --back_iter;
        }
        *it = val;

        size++;
        tail = (tail + 1) % capacity;
        if (size == capacity)
            tail = capacity;
    }

    void erase(iterator it) {
        if (size == 0)
            throw std::out_of_range("Trying to remove when size == 0");

        while (it != end()) {
            *it = *(it + 1);
            ++it;
        }

        if (tail == capacity)
            tail = (head + size - 1) % capacity;
        else
            tail = (tail - 1 + capacity) % capacity;
        size--;
    }

    T& front() {
        return *(array_head_ptr + head);
    }

    T& back() {
        return *(array_head_ptr + (head + size - 1) % capacity);
    }

    T& at(int idx) {
        return *(array_head_ptr + idx);
    }

    void change_capacity(unsigned new_capacity) {
        T* new_array_ptr = (T*) calloc(sizeof(T), new_capacity);
        if (!new_array_ptr)
            throw std::bad_alloc();

        int k = 0;
        iterator it = this->begin();
        while (it != this->end()) {
            *(new_array_ptr + k) = *it;
            ++k; ++it;
        }

        free(array_head_ptr);
        array_head_ptr = new_array_ptr;
        capacity =  new_capacity;
        head = 0;

        if (size > new_capacity) {
            size = new_capacity;
            tail = new_capacity;
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
//    buffer.pop_back();
//    buffer.push_back(0);
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
//    buffer.pop_back();
//    buffer.push_back(3);
    buffer.pop_front();
//    buffer.pop_front();
//    buffer.pop_front();
//    buffer.pop_front();
    buffer.push_back(4);
    buffer.push_back(5);


    // ToDO: try insert
    // ToDO: try erase
    // ToDO: try change_capacity

    buffer.erase(buffer.begin());
    buffer.insert(buffer.begin() + 1, -1);
    buffer.change_capacity(2);

    int get_back = buffer.back();


    auto iterator1 = buffer.begin();
    *iterator1 = -5;

    auto it_begin = buffer.begin();
    auto it_end = buffer.end();

    for (auto iterator = buffer.begin(); iterator != buffer.end(); ++iterator) {
        std::cout << *iterator << " ";
    }

    std::cout << "\n\n";
    std::cout << "\n\n";
    std::fill(buffer.begin(), buffer.end(), 1);

    for (auto iterator = buffer.begin(); iterator != buffer.end(); ++iterator) {
        std::cout << *iterator << " ";
    }
//
//    std::cout << "\n";

    return 0;
}
