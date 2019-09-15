#include <stdio.h>
#include <string.h>

// К заданию 1:
int gcd(int, int);

int lcm(int, int);

// К заданию 2:
void clean(char *, unsigned long, char *, unsigned long);

unsigned long count_double_quote = 0;
char *last_point;

int main() {
    // Задание 1:
    // Реализовать функции нахождения наибольшего общего делителя и наименьшего общего
    // кратного двух натуральных чисел.
    int x, y;
    scanf("%d %d", &x, &y);
    printf("GCD: %d\nLCM: %d\n", gcd(x, y), lcm(x, y));

    // Задание 5:
    // Разработать рекурсивную функцию, удаляющую из строки все лишние пробелы. Пробелы считаются лишними, если:
    // 1) их подряд идет более двух;
    // 2) они стоят в конце строки после последней точки;
    // 3) стоят после открывающегося парного знака препинания.
    char str1[50] = {};
    char result_string[50] = {};
    // scanf("%s", str1);
    char c;
    // Пропуск символов, чтобы корректно работала функция gets()
    while((c= getchar()) != '\n' && c != EOF)
        /* discard */ ;
    gets(str1);
    last_point = strrchr(str1, '.');
    // Проверка, что после последней точки идут только пробелы
    // NULL - после точки не только пробелы
    if (last_point != NULL) {
        for (unsigned long i = 0; i < strlen(str1) - (last_point - str1) - 1; ++i) {
            if (*(last_point + i + 1) != ' ') {
                last_point = NULL;
                break;
            }
        }
    }
    clean(str1, strlen(str1), result_string, 0);
    // Test:
    // "abc   def"  abc" abcd[   adgr]  .    "
    printf("%s", result_string);
    return 0;
}

// К заданию 1:
int gcd(int x, int y) {
    while (x * y != 0) {
        if (x > y) {
            x %= y;
        } else {
            y %= x;
        }
    }
    return x + y;
}

int lcm(int x, int y) {
    return x * y / gcd(x, y);
}

// К заданию 2:
void clean(char *p, unsigned long length, char *result, unsigned long k) {
    if (length <= 0) return;
    if (last_point != NULL){
        if (p - 1 == last_point) return;
    }
    if (*(p) != ' ' || k == 0) {
        if (*p == '"') count_double_quote += 1;
        strncpy(result, p, 1);
        clean(p + 1, length - 1, result + 1, k + 1);
    } else {
        // Рассматриваемый символ - пробел

        if (*(result - 1) == ' ') {
            // Если предыдущий символ пробел - скипаем
            clean(p + 1, length - 1, result, k);
        } else if (*(result - 1) == '(' || *(result - 1) == '{' || *(result - 1) == '[') {
            // Если предыдущий символ - двойной открывающий знак препинания
            // Пробелы после них - скипаем
            // Пусть ' - не двойной символ
            // Пусть < - не двойной символ
            clean(p + 1, length - 1, result, k);
        } else if (*(result - 1) == '"') {
            // Отдельно рассмотрим "
            if (count_double_quote % 2 == 1) {
                // Если нечётное количество, то это открывающий символ
                // Скипаем
                clean(p + 1, length - 1, result, k);
            } else {
                // В другом случае добавляем
                strncpy(result, p, 1);
                clean(p + 1, length - 1, result + 1, k + 1);
            }
        } else {
            // Если ничего из этого, то добавляем пробел
            strncpy(result, p, 1);
            clean(p + 1, length - 1, result + 1, k + 1);
        }
    }
}