//
// Created by dudko on 04.02.2020.
//

#ifndef LAB_02_MY_LIB_H
#define LAB_02_MY_LIB_H

#include <iostream>

typedef char queueEntry[256];

class StringQueue {
    const int MAX_LENGTH;
    queueEntry *queue;
    int tail, head;
    int amount;
public:

    // Конструктор по умолчанию (максимальной длины?)
    StringQueue(int MAX_LENGTH);

    // Конструктор копирования (очереди строк?)
    StringQueue(const StringQueue &o);

    // ToDO: Выяснить:
    /* 1) В чем отличие "по умолчанию" и "максимальной длины"
     * 2) Мы же пользуемся const? И мы же можем объявлять вот так, как здесь? (см. файл .cpp)
     * 3) В чем отличие "копирования" от "очереди строк"?
     */

    int getAmountInQueue();

    void push(queueEntry s);

    char *pop();

    char *show_last();

    char *show_first();

    void print();
};

#endif //LAB_02_MY_LIB_H
