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
        return iterator(array_head, 0);
    }

    iterator end() {
        return iterator(array_head, size);
    }

private:
    static const int INIT_CAPACITY = 20;
    int *array_head;
    int capacity;
    int size;
};

template <class T>
class CircleBuffer<T> :: iterator : public std::iterator<std::random_access_iterator_tag, T>{
public:

    typedef T& reference;

//    explicit iterator(int pos = 0): head(nullptr), pos(pos) {}
    explicit iterator(T *head, int pos = 0): head(head), pos(pos) {}

    // Input Iterator Requirements
    // ---------------------------
    bool operator!= (iterator &it) {
        return it.pos != this->pos;
    }

    // Pre-increment
    iterator& operator++() {
        pos++;
        return *this; // ToDO
    }

    // Post-increment
    iterator operator++(int) {
        return iterator(head, pos++);
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
        pos--;
        return *this; // ToDO
    }

    // Post-decrement
    iterator operator--(int) {
        return iterator(head, pos--);
    }

    // Random Access Iterator Requirements
    iterator& operator+(int n) {
        this->pos += n;
        return *this;  // ToDO
    }

    iterator& operator-(int n) {
        this->pos -= n;
        return *this;
    }

    T operator[](int n){
        return *(head + n);
    }

    iterator& operator+=(int n) {
        this->pos += n;
        return *this;
    }

    iterator& operator-=(int n) {
        this->pos -= n;
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
//    template <class A>
//    operator A() {
//        return (A) *(head + pos);
//    }

private:
    int pos;
    T *head;
};

template <class T>
typename CircleBuffer<T>::iterator operator+(int n, typename CircleBuffer<T>::iterator iterator) {
    iterator.pos += n;
    return iterator;
}

template <class T>
std::ostream& operator<<(std::ostream& stream, const typename CircleBuffer<T>::iterator &iterator) {
    stream << iterator;
    return stream;
}

int main() {
    CircleBuffer<int> circleBuffer;
    CircleBuffer<int>::iterator iterator = circleBuffer.begin();
    *iterator = 1;
    int k = iterator;    // ToDO: it's not valid
    int k_ = *iterator;  //  Must be valid
    std::fill(circleBuffer.begin(), circleBuffer.end() + 5, 1);
    std::cout << k;

    std::cout << "";
    return 0;
}
