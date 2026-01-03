#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "utils.h"

#define N 20
#define MAX 400
#define START 20
#define FOODLIMIT 10

struct point {

    int x;
    int y;

};

struct lifeParameters {

    int energy; // <0, 10> standard -> 8, -1 per iteration;
    int age; // <0, 100> iterations;

};

typedef struct {

    struct point p;
    struct lifeParameters lP;
    int sex; // 0 -> male; 1 -> female;
    int pregnancyTimer; // it last 3 iterations until new ind will born

} individual;

typedef enum {

    DIE, // die
    STAY, // stay
    MOVE, // move
    KILL_AND_MOVE, // kill other ind and move
    REPRODUCE // mate and stay

} ActionResult;

individual initIndividual(individual species[MAX], int *count); // OK
void spawnIndividuals(individual species[MAX], int *count); // OK
void iteration(individual species[MAX], int *count, int *foodCount, struct point food[FOODLIMIT]);
ActionResult move(individual *ind, int *count, int *foodCount, individual species[MAX], struct point food[FOODLIMIT], int *victimIndex);
void removeIndividual(individual species[MAX], int *count, int i); // OK
void removeFood(struct point food[FOODLIMIT], int *foodCount, int i); // OK
void spawnFood(struct point food[FOODLIMIT], int *foodCount); // OK
ActionResult action(individual *ind, struct point next, int *count, int *foodCount, individual species[MAX], struct point food[FOODLIMIT], int *victimIndex);
int indSearch(individual *ind, int *count, individual species[MAX]); // OK
void bornInd(individual *ind, individual species[MAX], int *count); // OK

individual initIndividual(individual species[MAX], int *count){

    int x;
    int y;
    individual ind;
    int done = 0;
    while (!done){

        x = rand() % N;
        y = rand() % N;
        int exist = 0;
        for (int i = 0; i < *count; i++){
            if (species[i].p.x == x && species[i].p.y == y){
                exist = 1;
                break;
            }
        }

        if (!exist){
            ind.lP.age = 0;
            ind.lP.energy = 20;
            ind.p.x = x;
            ind.p.y = y;
            ind.pregnancyTimer = -1;
            ind.sex = rand() % 2; 
            done = 1;
        }
    }

    return ind;

}

void spawnIndividuals(individual species[MAX], int *count){

    for (int i = 0; i < START; i++){

        species[*count] = initIndividual(species, count);
        (*count)++;
    }
}

void iteration(individual species[MAX], int *count, int *foodCount, struct point food[FOODLIMIT]) {

    int i = 0;
    while (i < *count) {

        species[i].lP.energy--;
        species[i].lP.age++;

        if (species[i].pregnancyTimer >= 0) {
            if (species[i].pregnancyTimer == 0){
                bornInd(&species[i], species, count);
                species[i].lP.energy--;
            }
            species[i].pregnancyTimer--;
        }

        if (species[i].lP.energy <= 0 || species[i].lP.age >= 100) {
            removeIndividual(species, count, i);
            continue;
        }
        int victimIndex = -1;
        ActionResult r = move(&species[i], count, foodCount, species, food, &victimIndex);

        if (r == DIE) {
            removeIndividual(species, count, i);
            continue;
        }

        if (r == KILL_AND_MOVE && victimIndex != -1) {
            removeIndividual(species, count, victimIndex);
        }

        i++;

    }
    spawnFood(food, foodCount);

}

ActionResult move(individual *ind, int *count, int *foodCount, individual species[MAX], struct point food[FOODLIMIT], int *victimIndex){

    int option = rand() % 9;
    struct point next = ind->p;

    switch (option)
    {
    case 0: // nothing
        break;
    case 1: // up
        if (ind->p.x > 0) {

            next.x = ind->p.x - 1;
            next.y = ind->p.y;

            switch (action(ind, next, count, foodCount, species, food, victimIndex)) {
                case MOVE: {
                    ind->p.x--;
                    break;
                }
                case STAY: {
                    break;
                }
                case REPRODUCE: {
                    // works the same as stay just for now
                    break;
                }
                case DIE: {
                    break;
                    // removing in iteration
                }
                case KILL_AND_MOVE: {
                    ind->p.x--;
                    break;
                    // removing in iteration
                }
                default:
                    break;
                
            }
        }
        break;
    case 2: // down
        if (ind->p.x < N - 1) {

            next.x = ind->p.x + 1;
            next.y = ind->p.y;

            switch (action(ind, next, count, foodCount, species, food, victimIndex)) {
                case MOVE: {
                    ind->p.x++;
                    break;
                }
                case STAY: {
                    break;
                }
                case REPRODUCE: {
                    // works the same as stay just for now
                    break;
                }
                case DIE: {
                    break;
                    // removing in iteration
                }
                case KILL_AND_MOVE: {
                    ind->p.x++;
                    break;
                    // removing in iteration
                }
                default:
                    break;
                
            }
        
        }
        break;

    case 3: // right
        if (ind->p.y < N - 1) {

            next.x = ind->p.x;
            next.y = ind->p.y + 1;

            switch (action(ind, next, count, foodCount, species, food, victimIndex)) {
                case MOVE: {
                    ind->p.y++;
                    break;
                }
                case STAY: {
                    break;
                }
                case REPRODUCE: {
                    // works the same as stay just for now
                    break;
                }
                case DIE: {
                    break;
                    // removing in iteration
                }
                case KILL_AND_MOVE: {
                    ind->p.y++;
                    break;
                    // removing in iteration
                }
                default:
                    break;
                
            }
        }

        break;
    case 4: // left
        if (ind->p.y > 0) {

            next.x = ind->p.x;
            next.y = ind->p.y - 1;

            switch (action(ind, next, count, foodCount, species, food, victimIndex)) {
                case MOVE: {
                    ind->p.y--;
                    break;
                }
                case STAY: {
                    break;
                }
                case REPRODUCE: {
                    // works the same as stay just for now
                    break;
                }
                case DIE: {
                    break;
                    // removing in iteration
                }
                case KILL_AND_MOVE: {
                    ind->p.y--;
                    break;
                    // removing in iteration
                }
                default:
                    break;
                
            }

        }

        break;
    case 5:
        if (ind->p.x > 0 && ind->p.y < N - 1) {

            next.x = ind->p.x - 1;
            next.y = ind->p.y + 1;

            switch (action(ind, next, count, foodCount, species, food, victimIndex)) {
                case MOVE: {
                    ind->p.x--;
                    ind->p.y++;
                    break;
                }
                case STAY: {
                    break;
                }
                case REPRODUCE: {
                    // works the same as stay just for now
                    break;
                }
                case DIE: {
                    break;
                    // removing in iteration
                }
                case KILL_AND_MOVE: {
                    ind->p.x--;
                    ind->p.y++;
                    break;
                    // removing in iteration
                }
                default:
                    break;
                
            }
            
        }
        break;
    case 6:
        if (ind->p.x > 0 && ind->p.y > 0) {

            next.x = ind->p.x - 1;
            next.y = ind->p.y - 1;
            
            switch (action(ind, next, count, foodCount, species, food, victimIndex)) {
                case MOVE: {
                    ind->p.x--;
                    ind->p.y--;
                    break;
                }
                case STAY: {
                    break;
                }
                case REPRODUCE: {
                    // works the same as stay just for now
                    break;
                }
                case DIE: {
                    break;
                    // removing in iteration
                }
                case KILL_AND_MOVE: {
                    ind->p.x--;
                    ind->p.y--;
                    break;
                    // removing in iteration
                }
                default:
                    break;
                
            }
        }
        break;
    case 7:
        if (ind->p.x < N - 1 && ind->p.y < N - 1) {

            next.x = ind->p.x + 1;
            next.y = ind->p.y + 1;

            switch (action(ind, next, count, foodCount, species, food, victimIndex)) {
                case MOVE: {
                    ind->p.x++;
                    ind->p.y++;
                    break;
                }
                case STAY: {
                    break;
                }
                case REPRODUCE: {
                    // works the same as stay just for now
                    break;
                }
                case DIE: {
                    break;
                    // removing in iteration
                }
                case KILL_AND_MOVE: {
                    ind->p.x++;
                    ind->p.y++;
                    break;
                    // removing in iteration
                }
                default:
                    break;
                
            }

        }
        break;
    case 8:
        if (ind->p.x < N - 1 && ind->p.y > 0) {

            next.x = ind->p.x + 1;
            next.y = ind->p.y - 1;

            switch (action(ind, next, count, foodCount, species, food, victimIndex)) {
                case MOVE: {
                    ind->p.x++;
                    ind->p.y--;
                    break;
                }
                case STAY: {
                    break;
                }
                case REPRODUCE: {
                    // works the same as stay just for now
                    break;
                }
                case DIE: {
                    break;
                    // removing in iteration
                }
                case KILL_AND_MOVE: {
                    ind->p.x++;
                    ind->p.y--;
                    break;
                    // removing in iteration
                }
                default:
                    break;
                
            }

        }
        break;
    default:
        break;
    }

    return STAY;

}

void removeIndividual(individual species[MAX], int *count, int i){

    species[i] = species[*count - 1];
    (*count)--;

}

void removeFood(struct point food[FOODLIMIT], int *foodCount, int i){
    food[i] = food[*foodCount - 1];
    (*foodCount)--;
}

void spawnFood(struct point food[FOODLIMIT], int *foodCount){ 

    int x;
    int y;
    int attempts = 0;

    while (*foodCount < FOODLIMIT){

        x = rand() % N;
        y = rand() % N;
        int exist = 0;
        for (int i = 0; i < *foodCount; i++){

            if (food[i].x == x && food[i].y == y){
                exist = 1;
                break;
            }

        }
        if (!(exist)) {
            food[*foodCount].x = x;
            food[*foodCount].y = y;
            (*foodCount)++;
        } else {
            attempts++;
            if (attempts > 200) break;
        }
    }
}

ActionResult action(individual *ind, struct point next, int *count, int *foodCount, individual species[MAX], struct point food[FOODLIMIT], int *victimIndex){

    // next -> new position of ind who takes an action

    // finding out what stays on next ind position:
    
    // food option
    for (int i = 0; i < *foodCount; i++){
        if (food[i].x == next.x && food[i].y == next.y){
            ind->lP.energy += 8;
            removeFood(food, foodCount, i);
            return MOVE;
        }
    }
    // other ind option
    for (int i = 0; i < *count; i++){

        if (&species[i] == ind) continue;
        if (species[i].p.x == next.x && species[i].p.y == next.y){

            if ((ind->sex == species[i].sex) || (ind->pregnancyTimer != -1) || (species[i].pregnancyTimer != -1)){

                if (fight(ind->lP.energy, species[i].lP.energy)){
                    ind->lP.energy += 8;
                    *victimIndex = i;
                    return KILL_AND_MOVE;
                    
                } else {
                    species[i].lP.energy += 8;
                    return DIE;
                }

            } else {
                if (ind->sex == 0) /*male*/{

                    species[i].pregnancyTimer = 3;
                    ind->lP.energy--;
                    species[i].lP.energy--;
                    return REPRODUCE;
                } else if (ind->sex == 1) /*female*/{
                    
                    ind->pregnancyTimer = 3;
                    ind->lP.energy--;
                    species[i].lP.energy--;
                    return REPRODUCE;
                } else /*smth strange*/{
                    printf("Something strange occured");
                    return STAY;
                }
            }    

            
    }
     
}
    // free cell
    return MOVE; 

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
    
    if (*count >= MAX) return; // avoid overflow

    individual newInd;
    newInd.p.x = ind->p.x;
    newInd.p.y = ind->p.y;
    newInd.lP.age = 0;
    newInd.lP.energy = 8;
    newInd.sex = rand() % 2;

    species[*count] = newInd;
    (*count)++;

}
