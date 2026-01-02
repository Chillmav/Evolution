#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 20
#define MAX 400
#define START 20

struct point {

    int x;
    int y;

};

struct lifeParameters {

    int energy; // <0, 10> standard -> 8, -1 per iteration;
    int age; // <0, 100> iterations;

};

struct stats {

    int childs;
    int foodEaten;

};

typedef struct {

    struct point p;
    struct lifeParameters lP;
    int sex; // 0 -> male; 1 -> female;
    int pregnancyTimer; // it last 3 iterations until new ind will born

} individual;

individual initIndividual(char map[N][N]);
void spawnIndividuals(individual species[MAX], char map[N][N]);
void iteration(individual species[MAX], char map[N][N], int *count);
void move(individual *ind, char map[N][N], int *count, individual species[MAX]);
void removeIndividual(individual species[MAX], int *count, int i);
void spawnFood(char map[N][N]);
int action(individual *ind, struct point p1, int *count, individual species[MAX], char map[N][N]); // 0 - dead, 1 - alive;
int fight(int energy1, int energy2);
int indSearch(individual *ind, int *count, individual species[MAX]);
void bornInd(individual *ind, individual species[MAX], int *count);

individual initIndividual(char map[N][N]){

    int x;
    int y;
    individual ind;
    do {
    x = rand() % N;
    y = rand() % N;
    } while (map[x][y] != '*');

    ind.lP.age = 0;
    ind.lP.energy = 8;
    ind.p.x = x;
    ind.p.y = y;
    ind.pregnancyTimer = -1;
    ind.sex = rand() % 2;   
    map[x][y] = (ind.sex == 0) ? '0' : '1';

    return ind;


}

void spawnIndividuals(individual species[MAX], char map[N][N]){

    for (int i = 0; i < START; i++){

        species[i] = initIndividual(map);

    }
}

void iteration(individual species[MAX], char map[N][N], int *count){

    int i = 0;
    while (i < *count) {

        species[i].lP.energy--;
        species[i].lP.age++;

        if (species[i].pregnancyTimer >= 0) {
            if (species[i].pregnancyTimer == 0){
                bornInd(species[i], species, count, map);
                species[i].lP.energy--;
            }
            species[i].pregnancyTimer--;
        }

        if (species[i].lP.energy <= 0 || species[i].lP.age >= 100) {
            removeIndividual(species, count, i);
            continue;
        }
        move(&species[i], map, count, species);
        i++;

    }
    spawnFood(map);

}

void move(individual *ind, char map[N][N], int *count, individual species[MAX]){

    int option = rand() % 9;

    map[ind->p.x][ind->p.y] = '*';


    switch (option)
    {
    case 0: // nothing
        break;
    case 1: // up
        if (ind->p.x > 0) {

            struct point next;
            next.x = ind->p.x - 1;
            next.y = ind->p.y;

            if (action(ind, next, count, species, map)) {
                ind->p.x--;
            }
        }
        break;
    case 2: // down
        if (ind->p.x < N - 1) {

            struct point next;
            next.x = ind->p.x + 1;
            next.y = ind->p.y;

            if (action(ind, next, count, species, map)) {
                ind->p.x++;
            }
        }
        break;
    case 3: // right
        if (ind->p.y < N - 1) {

            struct point next;
            next.x = ind->p.x;
            next.y = ind->p.y + 1;

            if (action(ind, next, count, species, map)) {
                ind->p.y++;
            }
        }
        break;
    case 4: // left
        if (ind->p.y > 0) {

            struct point next;
            next.x = ind->p.x;
            next.y = ind->p.y - 1;

            if (action(ind, next, count, species, map)) {
                ind->p.y--;
            }
        }
        break;
    case 5:
        if (ind->p.x > 0 && ind->p.y < N - 1) {

            struct point next;
            next.x = ind->p.x - 1;
            next.y = ind->p.y + 1;

            if (action(ind, next, count, species, map)) {
                ind->p.x--;
                ind->p.y++;
            }
        }
        break;
    case 6:
        if (ind->p.x > 0 && ind->p.y > 0) {

            struct point next;
            next.x = ind->p.x - 1;
            next.y = ind->p.y - 1;

            if (action(ind, next, count, species, map)) {
                ind->p.x--;
                ind->p.y--;
            }
        }
        break;
    case 7:
        if (ind->p.x < N - 1 && ind->p.y < N - 1) {

            struct point next;
            next.x = ind->p.x + 1;
            next.y = ind->p.y + 1;

            if (action(ind, next, count, species, map)) {
                ind->p.x++;
                ind->p.y++;
            }
        }
        break;
    case 8:
        if (ind->p.x < N - 1 && ind->p.y > 0) {

            struct point next;
            next.x = ind->p.x + 1;
            next.y = ind->p.y - 1;

            if (action(ind, next, count, species, map)) {
                ind->p.x++;
                ind->p.y--;
            }
        }
        break;
    default:
        break;
    }
    map[ind->p.x][ind->p.y] = (ind->sex == 0) ? '0' : '1';
}

void removeIndividual(individual species[MAX], int *count, int i){

    species[i] = species[*count - 1];
    (*count)--;

}

void spawnFood(char map[N][N]){

    int successes = 0;
    int x;
    int y;

    while (successes < 2){

        x = rand() % N;
        y = rand() % N;

        if (map[x][y] == '*'){
            successes++;
            map[x][y] = '$'; // Food
        
        }
    }
}

int action(individual *ind, struct point next, int *count, individual species[MAX], char map[N][N]){

    // next -> new position of ind who takes an action

    char cell = map[next.x][next.y];

    if (cell == '*'){
        return 1;
    } else if (cell == '$'){
        ind->lP.energy += 8; // eat food;
        return 1;
    } 

    // spotting other ind
    for (int i = 0; i < *count; i++){
        if (species[i].p.x == next.x && species[i].p.y == next.y){
            if (species[i].sex != ind->sex &&
    ind->pregnancyTimer == -1 &&
    species[i].pregnancyTimer == -1) {

                // fight if 1 ind won, 0 if lost:
                if (fight(ind->lP.energy, species[i].lP.energy)){
                    removeIndividual(species, count, i);
                    ind->lP.energy += 8;
                    
                } else {
                    species[i].lP.energy += 8;
                    int idx = indSearch(ind, count, species);
                    if (idx >= 0) removeIndividual(species, count, idx);
                    return 0;
                }

            } else if (ind->sex == 0){

                species[i].pregnancyTimer = 3;
                ind->lP.energy--;
                species[i].lP.energy--;

            } else if (ind->sex == 1) {

                ind->pregnancyTimer = 3;
                ind->lP.energy--;
                species[i].lP.energy--;

            } else {

                printf("Something strange occured");

            }
        }
    }

    return 1; // alive
}

int fight(int energy1, int energy2){

    if ((energy1 + rand() % 5) > (energy2 + rand() % 5)){
        return 1;
    } else {
        return 0;
    }
}   

int indSearch(individual *ind, int *count, individual species[MAX]){

    for (int i = 0; i < *count; i++){

        if (ind->p.x == species[i].p.x && ind->p.y == species[i].p.y){
            return i;
        }

    }
    return -1;
}

void bornInd(individual *ind, individual species[MAX], int *count){
    
    individual newInd;
    newInd.p.x = ind->p.x;
    newInd.p.y = ind->p.y;
    newInd.lP.age = 0;
    newInd.lP.energy = 8;
    newInd.sex = (rand() % 2 == 1) ? '1' : '0';
    species[*count] = newInd;
    map[newInd.p.x][newInd.p.y] = (newInd.sex == 0) ? '0' : '1';
    (*count)++;

}
