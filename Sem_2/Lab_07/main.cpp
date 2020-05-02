#include <iostream>
#include <algorithm>
#include <vector>

/*
 * Лабораторная работа No7
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
        array_head = (T *) calloc(sizeof(T), init_capacity);
        if (array_head == nullptr)
            throw std::bad_alloc();
    }

    ~CircleBuffer() {
        if (array_head)
            free(array_head);
    }

    class iterator;

    /* Буфер должен обладать следующими возможностями:
    *  1. Вставка и удаление в конец
    *  2. Вставка и удаление в начало
    *  3. Вставка и удаление в произвольное место по итератору
    *  4. Доступ в конец, начало
    *  5. Доступ по индексу
    *  6. Изменение капасити
    */

    iterator begin() {
        return iterator(array_head, size, capacity, head);
    }

    iterator end() {
        return iterator(array_head, size, capacity, tail);
    }

    void push_back(T t) {
        if (tail == head && size == capacity)
            throw std::out_of_range("Buffer overflow");

        *end() = t;
        size++;
        tail = (tail + 1 + capacity) % capacity;
    }

    T pop_back() {
        if (size == 0)
            throw std::out_of_range("Trying to remove when size == 0");

        T value = *(end() - 1);
        size--;
        tail = (tail - 1 + capacity) % capacity;
        return value;
    }

    void push_front(T t) {
        if (head == tail && size == capacity)
            throw std::out_of_range("Buffer overflow");
        *(array_head + (head - 1 + capacity) % capacity) = t;
        size++;
        head = (head - 1 + capacity) % capacity;
    }

    T pop_front() {
        if (size == 0)
            throw std::out_of_range("Trying to remove when size == 0");
        T value = *begin();
        size--;
        head = (head + 1) % capacity;
        return value;
    }

private:
    static const unsigned INIT_CAPACITY = 20;
    T *array_head;
    int head, tail;
    unsigned capacity;
    unsigned size;
};

template <class T>
class CircleBuffer<T> :: iterator {//: public std::iterator<std::random_access_iterator_tag, T>{
public:

//    explicit iterator(int pos = 0): head(nullptr), pos(pos) {}
    explicit iterator(T *head, unsigned size, unsigned capacity, int pos): head(head), size(size), pos(pos), capacity(capacity) {}

    // Input Iterator Requirements
    // ---------------------------
    bool operator!= (iterator &it) {
        return it.pos != this->pos;
    }

    // Pre-increment
    iterator& operator++() {
        pos = (pos + 1) % capacity;
        return *this;
    }

    // Post-increment
    const iterator operator++(int) {
        const iterator old = iterator(head, pos);
        pos = (pos + 1) % capacity;
        return old;
    }

//    T operator*() {
//        return *(head + pos);
//    }

    iterator& operator*() {
        return *this;
    }

//    iterator& operator*(int) {
//        return *this;
//    }

    bool operator==(iterator &it){
        return this->pos == it.pos;
    }
    // ---------------------------

    // Output Iterator Requirements
    // ---------------------------
    // ToDO: ввод через *
    // ---------------------------

    // Forward Iterator Requirements
    // Already satisfied

    // Bidirectional Iterator Requirements
    // Pre-decrement
    iterator& operator--() {
        pos = (pos - 1 + capacity) % capacity;
        return *this;
    }

    // Post-decrement
    const iterator operator--(int) {
        const iterator old(head, pos);
        pos = (pos - 1 + capacity) % capacity;
        return old;
    }

    // Random Access Iterator Requirements
    iterator& operator+(int n) {
        this->pos += n;
        pos %= capacity;
        return *this;
    }

    iterator& operator-(int n) {
        this->pos -= n;
        pos = ((std::abs(pos) + capacity - 1) / capacity * capacity + pos) % capacity;
        return *this;
    }

    T operator[](int n){
        if (n >= size) {
            throw std::out_of_range("Out of bounds exception");
        }
        return *(head + n);
    }

    iterator& operator+=(int n) {
        this->pos += n;
        pos %= capacity;
        return *this;
    }

    iterator& operator-=(int n) {
        this->pos -= n;
        pos = ((std::abs(pos) + capacity - 1) / capacity * capacity + pos) % capacity;
        return *this;
    }

    int operator-(iterator it) {
        return this->pos - it.pos;
    }

    bool operator>(iterator it) {
        return this->pos > it.pos;
    }

    bool operator<(iterator it) {
        return this->pos < it.pos;
    }

    bool operator>=(iterator it) {
        return this->pos >= it.pos;
    }

    bool operator<=(iterator it) {
        return this->pos <= it.pos;
    }

    iterator& operator=(T t){
        *(head + pos) = t;
        return *this;
    }

    // ToDO: conversion only for the reference
    template <class A>
    operator A() {
        return (A) *(head + pos);
    }

private:
    int pos;
    unsigned size;
    unsigned capacity;
    T *head;
};

template <class T>
typename CircleBuffer<T>::iterator operator+(int n, typename CircleBuffer<T>::iterator iterator) {
    iterator.pos += n;
    return iterator;
}

//template <class T>
//std::ostream& operator<<(std::ostream& stream, const typename CircleBuffer<T>::iterator &iterator) {
//    stream << iterator;
//    return stream;
//}

int main() {
    CircleBuffer<int> circleBuffer;
    CircleBuffer<int>::iterator iterator = circleBuffer.begin();
    *iterator = 1;
    int k = iterator;    // ToDO: it's not valid
    int k_ = *iterator;  //  Must be valid
//    std::fill(circleBuffer.begin(), circleBuffer.end() + 5, 1);
    std::cout << k;

    circleBuffer.push_back(1);
    circleBuffer.pop_back();
    circleBuffer.push_back(2);

    circleBuffer.push_back(5);
    circleBuffer.push_front(6);
    circleBuffer.push_front(-1);

    circleBuffer.pop_front();
    circleBuffer.pop_front();
    circleBuffer.pop_front();
    circleBuffer.pop_front();

    std::cout << "";
    return 0;
}
