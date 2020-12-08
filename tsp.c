#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define m 5
#define n 6
#define COL 1
#define ROW 3

int dist[m*n][m*n];
int minimum = 999;
int matrix[25][25], visited_cities[10], limit, cost = 0;

int sum(int a[], int size){
    int sum = 0;
    for(int i = 0; i < size - 1; i++){
        sum += matrix[a[i]][a[i + 1]];
    }
    return sum;
}

void swap(int* a, int* b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void permutation(int a[], int size, int l, int r, int* vertices, int* res) {
    if (l == r){
        if (sum(a, size) < minimum) {
            minimum = sum(a, size);
            for (int i = 0; i < size; i++) {
                res[i] = a[i];
            }
        } 
    } else {
            for (int i = l; i <= r; i++) {
                swap(&a[l], &a[i]);
                permutation(a, size, l + 1, r, vertices, res);
                swap(&a[l], &a[i]);
            }
    }  

}



int tsp(int c)
{
    int count, nearest_city = 999;
    int minimum = 999, temp;
    for (count = 0; count < limit; count++)
    {
        if ((matrix[c][count] != 0) && (visited_cities[count] == 0))
        {
            if (matrix[c][count] + matrix[count][c] < minimum)
            {
                minimum = matrix[count][c] + matrix[c][count];
            }
            temp = matrix[c][count];
            nearest_city = count;
        }
    }
    if (minimum != 999)
    {
        cost = cost + temp;
    }
    return nearest_city;
}

void minimum_cost(int city)
{
    int nearest_city;
    visited_cities[city] = 1;
    printf("%d ", city + 1);
    nearest_city = tsp(city);
    if (nearest_city == 999)
    {
        nearest_city = 0;
        printf("%d", nearest_city + 1);
        cost = cost + matrix[city][nearest_city];
        return;
    }
    minimum_cost(nearest_city);
}

int min(int a, int b) {
    if (a <= b) return a;
    else return b;
}

void distanceMatrix(int a[m][n]){
    for(int a = 0; a < m; a++){
        for (int b = 0; b < n; b++) {
            for (int c = 0; c < m; c++) {
                for (int d = 0; d < n; d++){
                    if (b == d) {
                        dist[a*n + b][c*n + d] = abs(a - c) * COL;
                    } else {
                        dist[a*n + b][c*n + d] =
                        min(abs(b - d) * ROW + (a + c) * COL + 2, abs(b - d) * ROW + abs(2*m - a - c) * COL);
                    } 
                }
            }
        }
    }
}


int main()
{
    /*
    int i, j;
    printf("Enter Total Number of Cities:\t");
    scanf("%d", &limit);
    printf("\nEnter Cost Matrix\n");
    for (i = 0; i < limit; i++)
    {
        printf("\nEnter %d Elements in Row[%d]\n", limit, i + 1);
        for (j = 0; j < limit; j++)
        {
            scanf("%d", &matrix[i][j]);
        }
        visited_cities[i] = 0;
    }
    printf("\nEntered Cost Matrix\n");
    for (i = 0; i < limit; i++)
    {
        printf("\n");
        for (j = 0; j < limit; j++)
        {
            printf("%d ", matrix[i][j]);
        }
    }
    printf("\n\nPath:\t");
    minimum_cost(0);
    printf("\n\nMinimum Cost: \t");
    printf("%d\n", cost);
    return 0;
    */
    
    int a[5][6] = { {0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0},
                };
    distanceMatrix(a);
    printf("Distance Matrix of all nodes: \n");

    for(int i = 0; i < m*n; i++){
        for(int j = 0; j < m*n; j++)
            printf("%-3d", dist[i][j]);
        printf("\n");
    }
    limit = 5;
    int vertices[5] = {3, 5, 26, 23, 12};
    
    printf("\nFind shortest path to pick all items at nodes:\n3, 5, 26, 23, 12.\n");
    
    for (int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++) {
            matrix[i][j] = dist[vertices[i]][vertices[j]];
        }
    }

    printf("Distance Matrix between 5 nodes:\n3, 5, 26, 23, 12.\n\n");
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++)
            printf("%-3d", matrix[i][j]);
        printf("\n");
    }
    int b[5] = {0, 1, 2, 3, 4};
    int res[5];
    printf("\nPath: \n");
    permutation(b, 5, 0, 4, vertices, res);
    for (int i = 0; i < 5; i++) {
        printf("%-3d", vertices[res[i]]);
    }

    printf("\nMin cost: %-3d", minimum);
}