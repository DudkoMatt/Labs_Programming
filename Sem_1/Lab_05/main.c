#include <stdio.h>

int main() {
    int a[] = {99, 88, 77, 66, 55, 44, 33, 22, 11};
    for (int i = 0; i < 9; ++i) {
        printf("%d ", a[i]);
        if ((i + 1) % 3 == 0){
            printf("\n");
        }
    }

    // Или заполнение через двойной цикл?
    int b[2][2] = {{1, 1}, {1, 4}};
    int c[2][2] = {{1, 0}, {0, 1}};

    for (int i = 0; i < 2; ++i) {
        for (int k = 0; k < 2; ++k) {
            int tmp = 0;
            for (int j = 0; j < 2; ++j) {
                tmp += b[i][j] * c[j][k];
            }
            printf("%d ", tmp);
        }
        printf("\n");
    }

    return 0;
}