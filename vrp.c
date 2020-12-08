#include <stdio.h>
#include <stdlib.h>

#define INFINITY 10000
#define V 12
int D[13], Q[13];

int main(){

    int d[13] = {0, 4, 7, 7, 8, 7, 3, 8, 6, 8, 4, 3, 3};
    int d0[13] = {0, 4, 5, 10, 9, 14, 12, 16, 11, 5, 3, 5, 6};
    int q[13] = {-1, 11, 3, 6, 5, 7, 8, 1, 7, 8, 7, 3, 2};
    int p[13];
    int pred[13] = {0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int Q = 30;
    int T = 300;
    int check = 0;
    int load, cost;
    int i;
    p[0] = 0;
    printf("MAX CAPACITY: %d.\n\n", Q);
    for(int i = 1; i < 13; i++){
        p[i] = INFINITY;
    }
    int t = 0, s;
    while (t < 12) {
        load = 0;
        cost = 0;
        i = t + 1;
        while (i <= 12) {
            check = 1;
            if (load + q[i] > Q) {
                t = i - 1;
                break;
            }
            load += q[i];
            if (i == t + 1) {
                cost = d0[i];
                if (cost > T){
                    printf("Too far for %d.\n", i);
                    check = 0;
                    t = i;
                    break;
                }
                pred[i] = 0;
            }
            else {
                cost += d[i];
                if (cost > T){
                    t = i - 1;
                    check = 0;
                    break;
                }
                pred[i] = i - 1;
            }
            if (check == 0) break;
            i++;
        }
        if (i > 12) break;
    }
    int count = 0;
    int capacity = 0;
    printf("Routes for each vehicles:\n");
    for (int i = 1; i < 13; i++){
        if (pred[i] == 0) {
            printf("\nRoute %d: 0  ", ++count);
            capacity = 0;
        } 
            printf("%-3d", i);
            capacity += q[i];
            if (pred[i+1] == 0) {
                printf(" Demands: %5d", capacity);
            }
        }
}

