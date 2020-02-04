#include <stdio.h>
#include <string.h>

int main() {
    // Задание 1:
    // Подсчитать количество символов цифр, а также символов строчных и прописных букв
    // английского алфавита в строке, введённой с консоли. Результат вывести на консоль.
    char str[50];
    gets(str);
    int count_numbers = 0;
    int count_letters_lower = 0;
    int count_letters_upper = 0;
    for (int i = 0; i < strlen(str); ++i) {
        if (str[i] >= 'a' && str[i] <= 'z'){
            count_letters_lower += 1;
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
            count_letters_upper += 1;
        } else if (str[i] >= '0' && str[i] <= '9') {
            count_numbers += 1;
        }
    }
    printf("Numbers: %d\n", count_numbers);
    printf("Letters lower: %d\n", count_letters_lower);
    printf("Letters upper: %d\n", count_letters_upper);

    // Задание 5:
    // Вывести на консоль состояние банковского счёта по истечении каждого полного месяца,
    // при нахождении средств на счёте в течение введённого с консоли количества месяцев.
    // Годовую процентную ставку и начальную сумму на счёте ввести с консоли. Моделируемый
    // счёт является счётом с ежемесячной выплатой процентов и последующей капитализацией,
    // а также не предусматривает частичное снятие средств.

    double remaining;
    int n;
    double p;

    scanf("%lf %d %lf", &remaining, &n, &p);
    for (int j = 0; j < n; ++j) {
        remaining += remaining * (p / 12) / 100;
        printf("%d: %.2lf\n", j + 1, remaining);
    }

    return 0;
}