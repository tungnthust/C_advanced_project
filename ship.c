#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>
enum{FAIL,SUCCESS, MAXLEN=1000};

#define INFINITE 10000
#define GRAPH_SIZE 24

void readDistance(char filename[]);
int readDemand(float a[], char filename[]);
float dist[40][40];
int subset[11000000];
float memo[GRAPH_SIZE][17000000];
int indx = -1;
float matrix[GRAPH_SIZE][GRAPH_SIZE];
float sum(int a[], int size);
float findMinCost(int start, int num_node);
int *findOptimalTour(int start, int num_node);
void setup(int start, int num_node);
void solve(int start, int num_node);
int notIn(int start, int subset);
int factorial(int a);
void combination(int set, int at, int r, int num_node, int *subset);
int *combinations(int r, int num_node, int size);

int main()
{
    
    float q[GRAPH_SIZE];
    readDemand(q, "node_data.txt");
    
    int vertices[GRAPH_SIZE];
    for (int i = 0; i < GRAPH_SIZE; i++) {
        vertices[i] = i;
    }

    for (int i = 0; i < GRAPH_SIZE; i++) {
        printf("%3d ", vertices[i]);
    }
    printf("\n");

    printf("\nFind shortest path to pick all items at nodes\n");

    readDistance("distanceMatrix.txt");

    for (int i = 0; i < 40; i++)
    {
        for (int j = 0; j < 40; j++)
            printf("%.2f ", dist[i][j]);
        printf("\n");
    }

    for (int i = 0; i < GRAPH_SIZE; i++)
    {
        for (int j = 0; j < GRAPH_SIZE; j++)
        {
            matrix[i][j] = dist[vertices[i]][vertices[j]];
        }
    }
    printf("Distance Matrix\n\n");
    for (int i = 0; i < GRAPH_SIZE; i++)
    {
        for (int j = 0; j < GRAPH_SIZE; j++)
            printf("%.2f ", matrix[i][j]);
        printf("\n");
    }

    setup(0, GRAPH_SIZE);    
    solve(0, GRAPH_SIZE);
    int *tour = findOptimalTour(0, GRAPH_SIZE);
    printf("\nPath: \n");
    for (int i = 0; i < GRAPH_SIZE + 1; i++)
    {
        printf("%d ", vertices[tour[i]]);
    }

    printf("\nMincost: %.2f\n", sum(tour, GRAPH_SIZE + 1));
    printf("\nDemands: \n");
    for(int i = 0; i < GRAPH_SIZE + 1; i++) {
        printf("%.2f ", q[vertices[tour[i]]]);
    }
    printf("\nPath\n");
    float Q = 10;
    float load = 0;
    for(int i = 0; i < GRAPH_SIZE + 1; i++) {
        if (load + q[vertices[tour[i]]] > Q) {
            printf("0 Subtotal load: %.2f kg\n0   ", load);
            i--;
            load = 0;
        } else {
            printf("%-3d ", vertices[tour[i]]);
            load += q[vertices[tour[i]]];
        }
        if (i == GRAPH_SIZE - 3) load -= 1.5;
    }
    printf("Subtotal load: %.2f kg", load);
    
}

float sum(int a[], int size)
{
    float sum = 0;
    for (int i = 0; i < size - 1; i++)
    {
        sum += matrix[a[i]][a[i + 1]];
    }
    return sum;
}
float findMinCost(int start, int num_node)
{
    int END_STATE = (1 << num_node) - 1;
    float minTourCost = INFINITE;
    float tourCost;
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
    int state = (1 << num_node) - 1; 
    int *tour = malloc(sizeof(int) * (num_node + 1));
    float newDist, prevDist;
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
    float minDist;
    float newDistance;
    for (int r = 3; r <= num_node; r++)
    {
        size = factorial(num_node) / (factorial(r) * factorial(num_node - r));
        combination(0, 0, r, num_node, subset);
        indx = 0;
        for (int i = 0; i < size; i++)
        {
            if (notIn(start, subset[i]))
                continue;
            for (int next = 0; next < num_node; next++)
            {
                if (next == start || notIn(next, subset[i]))
                    continue;
                state = subset[i] ^ (1 << next);
                minDist = INFINITE;
                for (int e = 0; e < num_node; e++)
                {
                    if (e == start || e == next || notIn(e, subset[i]))
                        continue;
                    newDistance = memo[e][state] + matrix[e][next];
                    if (newDistance < minDist)
                        minDist = newDistance;
                }
                memo[next][subset[i]] = minDist;
            }
        }
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

int readDemand(float a[], char filename[]){
   FILE *fin;
   float temp;
   char str[MAXLEN];
   int i = 0,d;
   float b,c;

   if((fin = fopen(filename, "r")) ==NULL ){
       printf("cannot open file");
   }
   else{

       int count = 0;
       while(fgets(str,MAXLEN,fin) != NULL){
           count++;
           sscanf(str, "%d %f",
           &d,
           &temp);
           a[i++]  = temp;
       }
   }
   fclose(fin);
   return i;

}

void readDistance(char filename[]){
    FILE* fin;   
    char str[MAXLEN];
    // char *str;
    // str = (char*)malloc(MAXLEN*sizeof(char));
    char str2[MAXLEN];
    int i,j;

    if( (fin = fopen(filename,"r")) == NULL ){
        printf("cannot open file");
        
    }
    else{
        
        char delim[2] = " ";
        int i=0;
        while(fgets(str,MAXLEN,fin) != NULL){
            char *token;
            token = (char*)malloc(MAXLEN*sizeof(char));
            token = strtok(str,delim);            
                for(j=0; j<40; j++){
                    dist[i][j] = atof(token);
                    token = strtok(NULL,delim);
                }
            i++;
        }
        
    }
}