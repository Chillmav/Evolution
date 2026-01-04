#include "individual.h"


typedef struct {

    // population
    int individualCount;
    int males;
    int females;

    // resources
    int foodOnBoard;

    // life metrics
    int averageAge;
    int averageEnergy;

    // evolution traits
    int avgSpeed;
    int avgVision;
    int avgMetabolism;

    // reproduction & survival
    int births;
    int deaths;
    int deathsStarvation;
    int deathsFight;
    int deathsAge;

} stats;


void displayMap(char map[N][N], int epoch, individual species[MAX], int *count, int *foodCount);
void displayStats(individual species[MAX], int *count, int *foodCount, int epoch);

#define EPOCH 50

int main(){
    
    srand(time(NULL));
    char map[N][N];
    int e = 1;
    individual species[MAX];
    struct point food[FOODLIMIT];
    int actions[MAX];
    int count = START;
    
    int foodCount = 0;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            map[i][j] = '*';
        }
        
    }

    spawnFood(map, food, &foodCount);
    spawnIndividuals(species, map);
    
    while (e <= EPOCH) {
    
        if (!(e % 10) || e == 1){
            displayMap(map, e, species, &count, &foodCount);
        }

        iteration(species, map, &count, &foodCount);
        e++;
    }

    return 0;

}

void displayMap(char map[N][N], int epoch, individual species[MAX], int count*, int *foodCount){

    printf("Map visualization epoch %d: \n\n", epoch);
    for (int i = 0; i < *count; i++){
        map[N][N]
    }
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            printf("%3c", map[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    displayStats(species, &count, &foodCount, epoch);
}

void displayStats(individual species[MAX], int *count, int *foodCount, int epoch){

    int females = 0;
    int males = 0;
    for (int i = 0; i < *count; i++){
        (species[i].sex == 0) ? males++ : females++;
    }
    printf("Stats (%d epoch): \n", epoch);
    printf("_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n\n");
    printf("Population Section: \n");
    printf("Individuals: %d | Males: %d | Females: %d \n", *count, males, females);
    printf("Resources Section: \n");
    printf("Food: %d \n", *foodCount);
}


