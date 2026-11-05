#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** createMatrix(int n, int m) {
    int** mat = (int**)malloc(n * sizeof(int*));
    if (mat == NULL) {
        printf("Ошибка выделения памяти!\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        mat[i] = (int*)calloc(m, sizeof(int));
        if (mat[i] == NULL) {
            printf("Ошибка выделения памяти!\n");
            exit(1);
        }
    }
    return mat;
}
void freeMatrix(int** mat, int n) {
    if (mat == NULL) return;
    for (int i = 0; i < n; i++) free(mat[i]);
    free(mat);
}
void generateAdjMatrix(int n, float p, int** adj) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                // Петля
                adj[i][j] = ((float)rand() / RAND_MAX < p) ? 1 : 0;
            }
            else if (i < j) {
                // Обычное ребро
                int val = ((float)rand() / RAND_MAX < p) ? 1 : 0;
                adj[i][j] = val;
                adj[j][i] = val; // симметрия
            }
        }
    }
}
void printMatrix(int** mat, int n, int m, const char* title) {
    printf("\n%s:\n", title);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}
int getSizeFromAdj(int n, int** adj) {
    int size = 0;
    for (int i = 0; i < n; i++) {
        if (adj[i][i] == 1) size++; // петля
        for (int j = i + 1; j < n; j++) {
            if (adj[i][j] == 1) size++;
        }
    }
    return size;
}
void analyzeByAdj(int n, int** adj) {
    printf("\nАнализ вершин:\n");
    for (int i = 0; i < n; i++) {
        int step = 0;
        int neighbors = 0;

        for (int j = 0; j < n; j++) {
            if (adj[i][j] == 1) {
                step++;
                if (i != j) neighbors++;
            }
        }
        if (adj[i][i] == 1) step++; // петля

        printf("Вершина %d: степень %d", i + 1, step);
        if (step == 0) printf(" (изолированная)");
        if (step == 1) printf(" (концевая)");
        if (neighbors == n - 1) printf(" (доминирующая)");
        printf("\n");
    }
}
int buildIncMatrix(int n, int** adj, int** inc) {
    int edges = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (adj[i][j] == 1 && i == j) {
                inc[i][edges] = 2; // петля
                edges++;
            }
            else if (adj[i][j] == 1 && i < j) {
                inc[i][edges] = 1;
                inc[j][edges] = 1;
                edges++;
            }
        }
    }
    return edges;
}
void analyzeByInc(int n, int edges, int** inc) {
    printf("\nАнализ вершин:\n");

    for (int i = 0; i < n; i++) {
        int step = 0;
        int neighbors[20] = { 0 }; // массив-флаг: был ли сосед
        int countNeighbors = 0;
        // Считаем степень и ищем соседей
        for (int j = 0; j < edges; j++) {
            if (inc[i][j] == 1) {
                step++;
                // Ищем, с какой ещё вершиной соединено это ребро
                for (int k = 0; k < n; k++) {
                    if (k != i && inc[k][j] == 1 && !neighbors[k]) {
                        neighbors[k] = 1;
                        countNeighbors++;
                    }
                }
            }
            else if (inc[i][j] == 2) {
                step += 2; // петля
            }
        }
        printf("Вершина %d: степень %d", i + 1, step);
        if (step == 0) printf(" (изолированная)");
        if (step == 1) printf(" (концевая)");
        if (countNeighbors == n - 1) printf(" (доминирующая)");
        printf("\n");
    }
}
int inputGraphParams(int* n, float* p) {
    printf("Введите количество вершин: ");
    scanf("%d", n);
    if (*n <= 0) {
        printf("Ошибка: количество вершин должно быть больше 0.\n");
        return 0;
    }
    printf("Введите вероятность появления ребра (0.0 - 1.0): ");
    scanf("%f", p);
    if (*p < 0.0 || *p > 1.0) {
        printf("Ошибка: вероятность должна быть от 0.0 до 1.0.\n");
        return 0;
    }
    return 1;
}
int main() {
    int n = 0;
    float p;
    int** adj = NULL;
    int choice;

    srand(time(NULL));

    while (1) {
        printf("\n===== МЕНЮ =====\n");
        printf("1 - Задание 1 (матрица смежности)\n");
        printf("2 - Задание 2 (матрица инцидентности)\n");
        printf("0 - Выход\n");
        printf("Выберите пункт: ");
        scanf("%d", &choice);

        if (choice == 0) {
            printf("Выход из программы.\n");
            break;
        }
        if (choice == 1 || choice == 2) {
            freeMatrix(adj, n);
            adj = NULL;

            if (!inputGraphParams(&n, &p)) continue;

            adj = createMatrix(n, n);
            generateAdjMatrix(n, p, adj);
        }
        // ЗАДАНИЕ 1
        if (choice == 1) {
            printf("\n--- Задание 1 ---\n");
            printMatrix(adj, n, n, "Матрица смежности");
            printf("\nРазмер графа (количество рёбер): %d\n", getSizeFromAdj(n, adj));
            analyzeByAdj(n, adj);
        }
        // ЗАДАНИЕ 2
        else if (choice == 2) {
            printf("\n--- Задание 2 ---\n");

            int max_edges = n * (n + 1) / 2;
            int** inc = createMatrix(n, max_edges);

            int edges = buildIncMatrix(n, adj, inc);

            printMatrix(inc, n, edges, "Матрица инцидентности");
            printf("Размер графа (по матрице инцидентности): %d\n", edges);
            analyzeByInc(n, edges, inc);

            freeMatrix(inc, n);
        }
        else {
            printf("Неверный пункт меню!\n");
        }
    }
    freeMatrix(adj, n);

    return 0;
}