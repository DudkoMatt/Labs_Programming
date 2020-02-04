//
// Created by dudko on 04.02.2020.
//

#include "my_lib.h"
#include <cstring>

// Конструктор по умолчанию
StringQueue::StringQueue(int MAX_LENGTH) : MAX_LENGTH(MAX_LENGTH) {
    queue = (queueEntry *) std::calloc(sizeof(queueEntry), MAX_LENGTH);
    tail = 0;
    head = 0;
    amount = 0;
}

// Конструктор копирования
StringQueue::StringQueue(const StringQueue &o) : MAX_LENGTH(o.MAX_LENGTH) {
    queue = (queueEntry *) std::calloc(sizeof(queueEntry), MAX_LENGTH);
    tail = o.tail;
    head = o.head;
    amount = o.amount;
    // ToDO: проверить на то, что значение копируется, а не переписывается указатель
    int t = head;
    while (t != tail) {
        strcpy(queue[t], o.queue[t]);
        t = (t + 1) % MAX_LENGTH;
    }
}

int StringQueue::getAmountInQueue() {
    return amount;
}

void StringQueue::push(queueEntry s) {
    if (amount >= MAX_LENGTH)
        return;
    strcpy(*(queue + tail), s);
    tail = (tail + 1) % MAX_LENGTH;
    amount++;
}

char *StringQueue::pop() {
    if (amount <= 0)
        return nullptr;
    amount--;
    int tmp = head;
    head = (head + 1) % MAX_LENGTH;
    return queue[tmp];
}

char *StringQueue::show_last() {
    if (amount <= 0)
        return nullptr;
    return queue[tail - 1 < 0 ? (MAX_LENGTH - 1) : tail - 1];
}

char *StringQueue::show_first() {
    if (amount <= 0)
        return nullptr;
    return queue[head];
}

void StringQueue::print() {
    if (amount <= 0) return;
    int t = head;
    while (t != tail) {
        std::cout << queue[t] << " ";
        t = (t + 1) % MAX_LENGTH;
    }
    std::cout << "\n";
}
