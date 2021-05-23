#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>
#include <time.h>
#include <stdbool.h>

enum{FAIL,SUCCESS, MAXLEN=1000};

#define INFINITE 10000
#define GRAPH_SIZE 40
#define FLEET 10
#define CAPACITY 15


typedef struct {
    int VehicleId;
    int Route[GRAPH_SIZE + 2];
    int CurLoc;
    float load;
    int NumNode;
} Vehicle;
Vehicle Vehicles[FLEET];


typedef struct {
    int* array;
    int size;
} Route;
Route RouteFrom;
Route RouteTo;
void addElement(int* a, int size, int idx, int value);
void readDistance(char filename[]);
float BestSolutionCost;
Vehicle VehiclesForBestSolution[FLEET];
int readDemand(float a[], char filename[]);
float dist[40][40];
int nodes[GRAPH_SIZE];
float q[GRAPH_SIZE];
int subset[11000000];
int isRouted[GRAPH_SIZE];
int indx = -1;
float cost;
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
int checkFit(Vehicle v, int i); 
int UnassignedCustomerExists(int nodes[]);
void greedySolution(int nodes[]);
int isRouteEmpty(Vehicle v);
void addNode(Vehicle* v, int node);
void TabuSearch(int TABU_Horizon);
void SaveBestSolution();
void copyArray(int* a, int* b, int size);
void removeElement(int* a, int size, int idx);
int main()
{
    int TABU_Horizon = 10;
    cost = (float) 0;
    readDemand(q, "node_data.txt");
        
    for (int i = 0; i < GRAPH_SIZE; i++) {
        nodes[i] = i;
        isRouted[i] = 0;
    }

    for (int i = 0; i < FLEET; i++) {
        Vehicles[i].NumNode = 0;
        Vehicles[i].CurLoc = 0;
        Vehicles[i].load = 0;
    }

    for (int i = 0; i < FLEET; i++) {
        VehiclesForBestSolution[i].NumNode = 0;
        VehiclesForBestSolution[i].CurLoc = 0;
        VehiclesForBestSolution[i].load = 0;
    }

    printf("\nFind shortest path to pick all items at nodes\n");

    readDistance("distanceMatrix.txt");


    for (int i = 0; i < GRAPH_SIZE; i++)
    {
        for (int j = 0; j < GRAPH_SIZE; j++)
        {
            matrix[i][j] = dist[nodes[i]][nodes[j]];
        }
    }
    
    printf("\t\t*********************\n");
    printf("Number of Customer: %d.\nTotal capacity of each vehicle: %d kg.\n\n", GRAPH_SIZE - 1, CAPACITY);
    printf("SOLUTION\n\n");
    FILE *fp; 
    if( (fp= fopen("output_from_C_program.txt", "w")) == NULL ){
        printf("cannot open file");
    }

    greedySolution(nodes);
    TabuSearch(TABU_Horizon);
    float subload = (float) 0;
    float subdist = (float) 0;
    for(int j = 0; j < FLEET; j++) {
        if (!isRouteEmpty(Vehicles[j]))  {
            printf("Vehicle %d: ", j);
            int routeSize = Vehicles[j].NumNode;
            for(int k = 0; k < routeSize; k++) {
                printf("%-3d", Vehicles[j].Route[k]);
                fprintf(fp, "%-3d",Vehicles[j].Route[k]);
                if (k < routeSize - 1) subdist += matrix[Vehicles[j].Route[k]][Vehicles[j].Route[k + 1]];
            }
            fprintf(fp,"\n");
            printf("Load: %.2f kg   ", Vehicles[j].load);
            printf("Distance: %.2f km\n", subdist);
            subload=0;
            subdist = 0;
        }
    }
    printf("\nTotal Distance %.2f km\n\n", BestSolutionCost);
    printf("\nProgram execute successfully. Press Enter to exit program.");    
}

void TabuSearch(int TABU_Horizon) {
    
    RouteFrom.array = (int*) malloc(sizeof(int) * GRAPH_SIZE + 1);
    RouteTo.array = (int*) malloc(sizeof(int) * GRAPH_SIZE + 1);
    float MovingNodeDemand = (float) 0;
    int RandomDelay1;
    int RandomDelay2;
    int RandomDelay3;
    int VehIndexFrom, VehIndexTo;
    float BestNCost, NeighborCost;
    int SwapIndexA = -1, SwapIndexB = -1, SwapRouteFrom =-1, SwapRouteTo=-1;

    int MAX_ITERATIONS = 2000;
    int iteration_number= 0;
    int DimensionCustomer = GRAPH_SIZE;
    int TABU_Matrix[DimensionCustomer + 1][DimensionCustomer + 1];
    for (int i = 0; i < DimensionCustomer + 1; i++) {
        for (int j = 0; j < DimensionCustomer + 1; j++) {
            TABU_Matrix[i][j] = 0;
        }
    }
    RouteFrom.size = 0;
    RouteTo.size = 0;
    float BestSolutionCost = (float) cost;
    int Termination = 0;

    while (Termination != 1) {
        iteration_number++;
        BestNCost = (float) INFINITE;
        for (VehIndexFrom = 0; VehIndexFrom < FLEET; VehIndexFrom++) {
            RouteFrom.array = Vehicles[VehIndexFrom].Route;
            RouteFrom.size = Vehicles[VehIndexFrom].NumNode;
            int RouteFromLength = RouteFrom.size;
           
            for (int i = 1; i < RouteFromLength - 1; i++) {
                for (VehIndexTo = 0; VehIndexTo < FLEET; VehIndexTo++) {
                    RouteTo.array = Vehicles[VehIndexTo].Route;
                    RouteTo.size = Vehicles[VehIndexTo].NumNode;
                    int RouteToLength = RouteTo.size;
                    for (int j = 0; j < RouteToLength - 1; j++) {
                        MovingNodeDemand = q[RouteFrom.array[i]];
                        if ((VehIndexFrom == VehIndexTo) || (checkFit(Vehicles[VehIndexTo], RouteFrom.array[i]) == 1)) {
                            if (((VehIndexFrom == VehIndexTo) && ((j == i) || (j == i - 1))) == false) {
                                float MinusCost1 = matrix[RouteFrom.array[i-1]][RouteFrom.array[i]];
                                float MinusCost2 = matrix[RouteFrom.array[i]][RouteFrom.array[i+1]];
                                float MinusCost3 = matrix[RouteTo.array[j]][RouteTo.array[j+1]];

                                float AddedCost1 = matrix[RouteFrom.array[i-1]][RouteFrom.array[i+1]];
                                float AddedCost2 = matrix[RouteTo.array[j]][RouteFrom.array[i]];
                                float AddedCost3 = matrix[RouteFrom.array[i]][RouteTo.array[j+1]];

                                if ((TABU_Matrix[RouteFrom.array[i-1]][RouteFrom.array[i+1]] != 0) 
                                    || (TABU_Matrix[RouteTo.array[j]][RouteFrom.array[i]] != 0)
                                    || (TABU_Matrix[RouteFrom.array[i]][RouteFrom.array[j+1]] != 0)) {
                                        break;
                                    }
                                NeighborCost = AddedCost1 + AddedCost2 + AddedCost3 - MinusCost1 - MinusCost2 - MinusCost3;
                                if (NeighborCost < BestNCost) {
                                        BestNCost = NeighborCost;
                                        SwapIndexA = i;
                                        SwapIndexB = j;
                                        SwapRouteFrom = VehIndexFrom;
                                        SwapRouteTo = VehIndexTo;
                                    }            
                            }
                        }
                    }
                }
            }

        }

        for (int o = 0; o < DimensionCustomer + 1; o++) {
            for (int p = 0; p < DimensionCustomer + 1; p++) {
                if (TABU_Matrix[o][p] > 0) {
                    TABU_Matrix[o][p]--;
                }
            }
        }
        
        RouteFrom.array = Vehicles[SwapRouteFrom].Route;
        RouteFrom.size = Vehicles[SwapRouteFrom].NumNode;
        
       
        RouteTo.array = Vehicles[SwapRouteTo].Route;
        RouteTo.size = Vehicles[SwapRouteTo].NumNode;
      
        int SwapNode = RouteFrom.array[SwapIndexA];
        int NodeIDBefore = RouteFrom.array[SwapIndexA-1];
        int NodeIDAfter = RouteFrom.array[SwapIndexA+1];
        int NodeID_F = RouteTo.array[SwapIndexB];
        int NodeID_G = RouteTo.array[SwapIndexB+1];

        
        RandomDelay1 = rand() % 5;
        RandomDelay2 = rand() % 5;
        RandomDelay3 = rand() % 5;
        TABU_Matrix[NodeIDBefore][SwapNode] = TABU_Horizon + RandomDelay1;
        TABU_Matrix[SwapNode][NodeIDAfter]  = TABU_Horizon + RandomDelay2 ;
        TABU_Matrix[NodeID_F][NodeID_G] = TABU_Horizon + RandomDelay3;

        
        removeElement(RouteFrom.array, RouteFrom.size, SwapIndexA);
        if(SwapRouteFrom == SwapRouteTo) {
            RouteTo.size--;
            Vehicles[SwapRouteTo].NumNode--;
        }
        RouteFrom.size--;
        Vehicles[SwapRouteFrom].NumNode--;
      
        if (SwapRouteFrom == SwapRouteTo) {
            if (SwapIndexA < SwapIndexB) {
                addElement(RouteTo.array, RouteTo.size, SwapIndexB, SwapNode);
                RouteTo.size++;
                RouteFrom.size++;
                Vehicles[SwapRouteFrom].NumNode++;
                Vehicles[SwapRouteTo].NumNode++;
            } else {
                addElement(RouteTo.array, RouteTo.size, SwapIndexB + 1, SwapNode);
                RouteTo.size++;
                RouteFrom.size++;
                Vehicles[SwapRouteTo].NumNode++;
                Vehicles[SwapRouteFrom].NumNode++;
            }
        }
        else
        {
            addElement(RouteTo.array, RouteTo.size, SwapIndexB + 1, SwapNode);
            RouteTo.size++;
            Vehicles[SwapRouteTo].NumNode++;
        }

        copyArray(RouteFrom.array, Vehicles[SwapRouteFrom].Route, RouteFrom.size);
        MovingNodeDemand = q[SwapNode];
        Vehicles[SwapRouteFrom].NumNode = RouteFrom.size;
        Vehicles[SwapRouteFrom].load -= MovingNodeDemand;

        copyArray(RouteTo.array, Vehicles[SwapRouteTo].Route, RouteTo.size);
        Vehicles[SwapRouteTo].NumNode = RouteTo.size;
        Vehicles[SwapRouteTo].load += MovingNodeDemand;
        cost += BestNCost;
        if (cost < BestSolutionCost) {
            SaveBestSolution();
        }
        if (iteration_number == MAX_ITERATIONS) {
            Termination = 1;
        }
    }
   
    for (int j = 0 ; j < FLEET ; j++)
    {   
       
        if (!isRouteEmpty(VehiclesForBestSolution[j]))
        {
            int RouteSize = VehiclesForBestSolution[j].NumNode;
            for (int k = 0; k < RouteSize ; k++) {
                int s = VehiclesForBestSolution[j].Route[k];
                addElement(Vehicles[j].Route, k, k, s);
            }
        }
            Vehicles[j].load = VehiclesForBestSolution[j].load;
            Vehicles[j].NumNode = VehiclesForBestSolution[j].NumNode;
            Vehicles[j].CurLoc = VehiclesForBestSolution[j].CurLoc;
       
    }
    cost = BestSolutionCost;
}

void addElement(int* a, int size, int idx, int value) {
    for(int i = size; i > idx; i--) {
        a[i] = a[i-1];
    }
    a[idx] = value;
}

void removeElement(int* a, int size, int idx) {
    for (int i = idx; i < size - 1; i++) {
        a[i] = a[i + 1]; 
    } 
}

void SaveBestSolution()
    {
        BestSolutionCost = cost;
        
        for (int j=0 ; j < FLEET ; j++)
        {   
            
            if (! isRouteEmpty(Vehicles[j]))
            {
                int RouteSize = Vehicles[j].NumNode;
                for (int k = 0; k < RouteSize ; k++) {
                    int s = Vehicles[j].Route[k];
                    addElement(VehiclesForBestSolution[j].Route, k, k, s);
                }
            }
            VehiclesForBestSolution[j].NumNode = Vehicles[j].NumNode;
            VehiclesForBestSolution[j].load = Vehicles[j].load;
          
        }
}

void copyArray(int* a, int* b, int size) {
    for (int i = 0; i < size; i++){
        b[i] = a[i];
    }
    for (int i = size; i < GRAPH_SIZE; i++) {
        b[i] = 0;
    }
}

int isRouteEmpty(Vehicle v) {
    if (v.NumNode == 0) return 1;
    else return 0;
}

void addNode(Vehicle* v, int node) {
    v->Route[v->CurLoc] = node;
    v->CurLoc++;
    v->NumNode++;
    v->load += q[node];
}

int UnassignedCustomerExists(int nodes[]) {
    for (int i = 1; i < GRAPH_SIZE; i++) {
        if(!isRouted[i])
            return 1;
    }
    return 0;
}

void greedySolution(int nodes[]) {
    float CandCost, EndCost;
    int VehicleIndex = 0;
    while (UnassignedCustomerExists(nodes)) {
        int CustomerIndex = 0;
        int Candidate = -1;
        float minCost = (float) INFINITE;
        if(isRouteEmpty(Vehicles[VehicleIndex])){
            addNode(&Vehicles[VehicleIndex], nodes[0]);
        }

        for (int i = 1; i <= GRAPH_SIZE - 1; i++) {
            if(!isRouted[nodes[i]]){
                if(checkFit(Vehicles[VehicleIndex], nodes[i])) {
                    CandCost = matrix[nodes[Vehicles[VehicleIndex].Route[Vehicles[VehicleIndex].CurLoc - 1]]][i];
                    if (minCost > CandCost) {
                        minCost = CandCost;
                        CustomerIndex = i;
                        Candidate = nodes[i];
                    }
                }
            }
        }

        if (Candidate == -1) {
            if (VehicleIndex + 1 < FLEET) {
                if (Vehicles[VehicleIndex].CurLoc != 0) {
                    EndCost = matrix[nodes[Vehicles[VehicleIndex].Route[Vehicles[VehicleIndex].CurLoc - 1]]][0];
                    addNode(&Vehicles[VehicleIndex], nodes[0]);
                    cost += EndCost;
                }
                VehicleIndex++;
            }
            else {
                printf("\nThe rest customers do not fit in any Vehicle\nThe problem cannot be resolved under these constrains");
                return;
            }
        } else {
            addNode(&Vehicles[VehicleIndex], Candidate);
            isRouted[nodes[CustomerIndex]] = 1;
            cost += minCost;
        }
    }
    EndCost = matrix[nodes[Vehicles[VehicleIndex].Route[Vehicles[VehicleIndex].CurLoc - 1]]][0];
    addNode(&Vehicles[VehicleIndex], nodes[0]);
    cost += EndCost;

}

int checkFit(Vehicle v, int i) {
    if (v.load + q[i] <= CAPACITY) return 1;
    else return 0;
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