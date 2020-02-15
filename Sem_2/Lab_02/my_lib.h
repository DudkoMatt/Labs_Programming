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

    // Конструктор по умолчанию + максимальной длины
    explicit StringQueue(int MAX_LENGTH = 255);

    // Конструктор копирования очереди строк
    StringQueue(const StringQueue &o);

    int getAmountInQueue();

    void push(const queueEntry s);

    char *pop();

    char *show_last();

    char *show_first();

    void print();
};

#endif //LAB_02_MY_LIB_H
