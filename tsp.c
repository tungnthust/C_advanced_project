#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define m 20
#define n 11
#define g 5
#define COL 26
#define ROW 60
#define INFINITE 10000

int subset[1000000];
int dist[m * n][m * n];
short memo[m * n][1100000];
int indx = -1;
int matrix[50][50];
int sum(int a[], int size);
int findMinCost(int start, int num_node);
int *findOptimalTour(int start, int num_node);
int min(int a, int b);
void setup(int start, int num_node);
void solve(int start, int num_node);
int notIn(int start, int subset);
int factorial(int a);
void combination(int set, int at, int r, int num_node, int *subset);
int *combinations(int r, int num_node, int size);
void distanceMatrix(int a[m][n]);

int main()
{
    // for(int i = 0; i < m*n; i++) {
    //     memo[i] = malloc(sizeof(int) * 1000);
    // }
    // int* a = combinations(3, 4);
    // for (int i = 0; i < 4; i++) {
    //     printf("%d ", a[i]);
    // }
    int a[20][11] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  
    };
    distanceMatrix(a);
    printf("Distance Matrix of all nodes: \n");

    for (int i = 0; i < m * n; i++)
    {
        for (int j = 0; j < m * n; j++)
            printf("%-10d", dist[i][j]);
        printf("\n");
    }
    int vertices[20] = {0, 96, 19, 58, 119, 189, 153, 78, 123, 204, 62, 42, 174, 69, 112, 35, 140, 85, 199, 100};

    printf("\nFind shortest path to pick all items at nodes:\n3, 5, 26, 23, 12.\n");

    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            matrix[i][j] = dist[vertices[i]][vertices[j]];
        }
    }

    printf("Distance Matrix between 5 nodes:\n3, 5, 26, 23, 12.\n\n");
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
            printf("%-10d", matrix[i][j]);
        printf("\n");
    }

    setup(0, 20);    
    solve(0, 20);
    int *tour = findOptimalTour(0, 20);
    printf("\nPath: \n");
    for (int i = 0; i < 21; i++)
    {
        printf("%d ", vertices[tour[i]]);
    }

    printf("\nMincost: %d", sum(tour, 21));
}

int sum(int a[], int size)
{
    int sum = 0;
    for (int i = 0; i < size - 1; i++)
    {
        sum += matrix[a[i]][a[i + 1]];
    }
    return sum;
}
int findMinCost(int start, int num_node)
{
    int END_STATE = (1 << num_node) - 1;
    int minTourCost = INFINITE;
    int tourCost;
    for (int e = 0; e < num_node; e++)
    {
        if (e == start)
            continue;
        tourCost = memo[e][END_STATE] + matrix[e][start];
        if (tourCost < minTourCost)
            minTourCost = tourCost;
    }
    return minTourCost;
}

int* findOptimalTour(int start, int num_node)
{
    int lastIndex = start;
    int state = (1 << num_node) - 1; //End of state
    int *tour = malloc(sizeof(int) * (num_node + 1));
    int newDist, prevDist;
    int index;
    for (int i = num_node - 1; i >= 1; i--)
    {
        index = -1;
        for (int j = 0; j < num_node; j++)
        {
            if (j == start || notIn(j, state))
                continue;
            if (index == -1)
                index = j;
            prevDist = memo[index][state] + matrix[index][lastIndex];
            newDist = memo[j][state] + matrix[j][lastIndex];
            if (newDist < prevDist)
                index = j;
        }
        tour[i] = index;
        state = state ^ (1 << index);
        lastIndex = index;
    }
    tour[0] = start;
    tour[num_node] = start;
    return tour;
}

int min(int a, int b)
{
    if (a <= b)
        return a;
    else
        return b;
}
void setup(int start, int num_node)
{

    for (int i = 0; i < num_node; i++)
    {
        if (i == start)
            continue;
        memo[i][1 << start | 1 << i] = matrix[start][i];
    }
}

void solve(int start, int num_node)
{
    int size;
    int state;
    int minDist;
    int newDistance;
    for (int r = 3; r <= num_node; r++)
    {
        size = factorial(num_node) / (factorial(r) * factorial(num_node - r));
        // printf("\nr = %d", r);
        // printf("\n------------size=%d", size);
        // printf("creat");
        // int* subset = (int*) malloc(sizeof(int) * size);
        // printf("FAILLLLLL");
        combination(0, 0, r, num_node, subset);
        indx = 0;
        for (int i = 0; i < size; i++)
        {
            // printf("\ni = %d", i);
            if (notIn(start, subset[i]))
                continue;
            for (int next = 0; next < num_node; next++)
            {
                // printf("\nnext = %d", next);
                if (next == start || notIn(next, subset[i]))
                    continue;
                state = subset[i] ^ (1 << next);
                minDist = INFINITE;
                for (int e = 0; e < num_node; e++)
                {
                    // printf("\ne = %d", e);
                    if (e == start || e == next || notIn(e, subset[i]))
                        continue;
                    newDistance = memo[e][state] + matrix[e][next];
                    if (newDistance < minDist)
                        minDist = newDistance;
                }
                memo[next][subset[i]] = minDist;
            }
        }
        // free(subset);
        // subset = NULL;
    }
}

int notIn(int start, int subset)
{
    return ((1 << start) & subset) == 0;
}

int factorial(int a)
{
    if (a == 0)
        return 1;
    else
        return a * factorial(a - 1);
}

void combination(int set, int at, int r, int num_node, int *subset)
{
    if (r == 0)
    {
        subset[++indx] = set;
    }
    else
    {
        for (int i = at; i < num_node; i++)
        {
            set = set | (1 << i);
            combination(set, i + 1, r - 1, num_node, subset);
            set = set & ~(1 << i);
        }
    }
}

// int *combinations(int r, int num_node, int size)
// {
//     printf("creat");
//     int *subset = malloc(sizeof(int) * size);
//     combination(0, 0, r, num_node, subset);
//     indx = 0;
//     return subset;
// }

void distanceMatrix(int a[m][n])
{
    for (int a = 0; a < m; a++)
    {
        for (int b = 0; b < n; b++)
        {
            for (int c = 0; c < m; c++)
            {
                for (int d = 0; d < n; d++)
                {
                    if (b == d)
                    {
                        dist[a * n + b][c * n + d] = abs(a - c) * COL + abs(a / g - c / g) * COL;
                    }
                    else
                    {
                        if (a / g == c / g) {
                            dist[a * n + b][c * n + d] =
                                min(abs(b - d) * ROW + (a + c) * COL + 2, abs(b - d) * ROW + abs(2 * m - a - c) * COL);
                        }
                        else if (a / g < c / g) {
                            dist[a * n + b][c * n + d] = (g - a % g) * COL + (c / g - a / g - 1) * 6 * COL + COL + (c % g) * COL + abs(b - d) * ROW; 
                        }
                        else {
                            dist[a * n + b][c * n + d] = (a % g + 1) * COL + (a / g - c / g - 1) * 6 * COL + (g - c % g) * COL + abs(b - d) * ROW;
                        }
                    }
                }
            }
        }
    }
}
