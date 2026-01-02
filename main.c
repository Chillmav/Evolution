#include "individual.h"

void displayMap(char map[N][N]);

int main(){
    
    srand(time(NULL));

    char map[N][N];
    individual species[MAX];
    int counts = START;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            map[i][j] = '*';
        }
        
    }
    spawnFood(map);
    spawnIndividuals(species, map);
    displayMap(map);
    iteration(species, map, &counts);
    displayMap(map);
    return 0;

}

void displayMap(char map[N][N]){

    printf("Map visualization: \n\n");

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            printf("%3c", map[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


