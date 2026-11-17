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
void generateAdjMatrix(int n, float p_edge, float p_loop, int** adj) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                adj[i][j] = ((float)rand() / RAND_MAX < p_loop) ? 1 : 0;
            }
            else if (i < j) {
                int val = ((float)rand() / RAND_MAX < p_edge) ? 1 : 0;
                adj[i][j] = val;
                adj[j][i] = val;
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
        if (adj[i][i] == 1) size++;
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
        if (adj[i][i] == 1) step++;

        printf("Вершина %d: степень %d, соседей %d", i + 1, step, neighbors);
        if (neighbors == 0) printf(" (изолированная)");
        if (neighbors == 1) printf(" (концевая)");
        if (neighbors == n - 1) printf(" (доминирующая)");
        printf("\n");
    }
}
void analyzeByInc(int n, int edges, int** inc) {
    printf("\nАнализ вершин:\n");
    for (int i = 0; i < n; i++) {
        int step = 0;
        int neighbors[20] = { 0 };
        int countNeighbors = 0;

        for (int j = 0; j < edges; j++) {
            if (inc[i][j] == 1) {
                step++;
                for (int k = 0; k < n; k++) {
                    if (k != i && inc[k][j] == 1 && !neighbors[k]) {
                        neighbors[k] = 1;
                        countNeighbors++;
                    }
                }
            }
            else if (inc[i][j] == 2) {
                step += 2;
            }
        }
        printf("Вершина %d: степень %d, соседей %d", i + 1, step, countNeighbors);
        if (countNeighbors == 0) printf(" (изолированная)");
        if (countNeighbors == 1) printf(" (концевая)");
        if (countNeighbors == n - 1) printf(" (доминирующая)");
        printf("\n");
    }
}
int inputGraphParams(int* n, float* p_edge, float* p_loop) {
    printf("Введите количество вершин: ");
    scanf("%d", n);
    if (*n <= 0) {
        printf("Ошибка: количество вершин должно быть больше 0.\n");
        return 0;
    }
    printf("Введите вероятность появления обычного ребра (0.0 - 1.0): ");
    scanf("%f", p_edge);
    if (*p_edge < 0.0 || *p_edge > 1.0) {
        printf("Ошибка: вероятность должна быть от 0.0 до 1.0.\n");
        return 0;
    }
    printf("Введите вероятность появления петли (0.0 - 1.0): ");
    scanf("%f", p_loop);
    if (*p_loop < 0.0 || *p_loop > 1.0) {
        printf("Ошибка: вероятность должна быть от 0.0 до 1.0.\n");
        return 0;
    }
    return 1;
}
int main() {
    int n = 0;
    float p_edge, p_loop;
    int** mat = NULL;
    int max_cols = 0;
    int adj_generated = 0; // флаг: сгенерирована ли матрица смежности
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
        // ЗАДАНИЕ 1
        if (choice == 1) {
            printf("\n--- Задание 1 ---\n");

            freeMatrix(mat, n);
            mat = NULL;
            adj_generated = 0;

            if (!inputGraphParams(&n, &p_edge, &p_loop)) continue;

            int max_edges = n * (n + 1) / 2;
            max_cols = (max_edges > n) ? max_edges : n;

            mat = createMatrix(n, max_cols);
            generateAdjMatrix(n, p_edge, p_loop, mat);
            adj_generated = 1;

            printMatrix(mat, n, n, "Матрица смежности");
            printf("\nРазмер графа (количество рёбер): %d\n", getSizeFromAdj(n, mat));
            analyzeByAdj(n, mat);
        }
        // ЗАДАНИЕ 2
        else if (choice == 2) {
            printf("\n--- Задание 2 ---\n");
            if (!adj_generated) {
                printf("Сначала выполните Задание 1 (матрица смежности не сгенерирована)!\n");
                continue;
            }
            // 1) Копируем смежность в правую часть строк
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    mat[i][max_cols - n + j] = mat[i][j];
                }
            }
            // 2) Обнуляем левую часть (где будет инцидентность)
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < max_cols - n; j++) {
                    mat[i][j] = 0;
                }
            }
            // 3) Строим инцидентность в левой части, читая смежность из правой
            int edges = 0;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    int a = mat[i][max_cols - n + j];
                    if (a == 1 && i == j) {
                        mat[i][edges] = 2;
                        edges++;
                    }
                    else if (a == 1 && i < j) {
                        mat[i][edges] = 1;
                        mat[j][edges] = 1;
                        edges++;
                    }
                }
            }
            printMatrix(mat, n, edges, "Матрица инцидентности");
            printf("Размер графа (по матрице инцидентности): %d\n", edges);
            analyzeByInc(n, edges, mat);

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    mat[i][j] = mat[i][max_cols - n + j];
                }
            }
        }
        else {
            printf("Неверный пункт меню!\n");
        }
    }
    freeMatrix(mat, n);
    return 0;
}