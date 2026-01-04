#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "utils.h"
#include "action.h"

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
    int pregnancyTimer;

} individual;

typedef struct {

    int indIndex;
    struct point target;
    int satisfied; // 0 -> if move wasn't done, 1 -> if yes 

} intent;

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

void iteration(individual species[MAX], int *count, int *foodCount, struct point food[FOODLIMIT], int actions[MAX], intent oldIntents[MAX]) {

    int i = 0;
    while (i < *count) {

        species[i].lP.energy--;
        species[i].lP.age++;

        if (species[i].pregnancyTimer >= 0) {
            if (species[i].pregnancyTimer == 0){
                bornInd(&species[i], species, count, actions); // have to involve actions there yet
                species[i].lP.energy--;
            }
            species[i].pregnancyTimer--;
        }

        if (species[i].lP.energy <= 0 || species[i].lP.age >= 100) {
            removeIndividual(species, count, i); // have to involve actions there yet
            continue;
        }

        // RESOLVING INTENTIONS:


        //

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

    // NEW INTENTIONS:

    intent newIntents[MAX];
    while (i < *count){

        intent newIntent;
        struct point target = move(&species[i], count, foodCount, species, food);
        newIntent.target.x = target.x;
        newIntent.target.y = target.y;
        newIntent.indIndex = i;
        newIntent.satisfied = 0;
        newIntents[i] = newIntent;
        i++;

    }

    oldIntents = newIntents;
        
    //

}

struct point move(individual *ind, int *count, int *foodCount, individual species[MAX], struct point food[FOODLIMIT]){


    // in the future there will be "choosing move" logic incorporating genes of each individual

    int option = rand() % 9;
    struct point next = ind->p;
    if (option == 0) return next;

    int dx[] = {0, -1,  1,  0,  0,  -1,  -1,  1,  1, };
    int dy[] = {0,  0,  0,  1, -1,   1,  -1,  1, -1, };

    next.x = ind->p.x + dx[option];
    next.y = ind->p.y + dy[option];

    if ((next.x < N && next.x >= 0) && (next.y < N && next.y >= 0)) {
        return next;
    }
    
    return ind->p;

    // if ((next.x < N && next.x >= 0) && (next.y < N && next.y >= 0)){
    //     switch (action(ind, next, count, foodCount, species, food, victimIndex))
    //     {
    //     case MOVE: {
    //                 ind->p.x += dx[option];
    //                 ind->p.y += dy[option];
    //                 return MOVE;
    //             }
    //             case STAY: {
    //                 return STAY;
    //             }
    //             case REPRODUCE: {
    //                 // works the same as stay just for now
    //                 return REPRODUCE;
    //             }
    //             case DIE: {
    //                 return DIE;
    //                 // removing in iteration
    //             }
    //             case KILL_AND_MOVE: {
    //                 ind->p.x += dx[option];
    //                 ind->p.y += dy[option];
    //                 return KILL_AND_MOVE;
    //                 // removing in iteration
    //             }
    //             default:
    //                 return STAY;
    // }
    // }
    // return STAY;
    
}

void removeIndividual(individual species[MAX], int *count, int i, intent intents[MAX]){

    species[i] = species[*count - 1];
    intents[i] = intents[*count - 1];
    intents[i].indIndex = i;
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

void bornInd(individual *ind, individual species[MAX], int *count, intent intents[MAX]) {

    if (*count >= MAX) return;

    int dx[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int dy[] = {0, 0, 1, -1, 1, -1, 1, -1};

    struct point birthCell;
    int option = 0;
    int found = 0;

    while (!found) {

        if (option > 7) return;   // no space around parent

        birthCell.x = ind->p.x + dx[option];
        birthCell.y = ind->p.y + dy[option];

        if (birthCell.x >= 0 && birthCell.x < N &&
            birthCell.y >= 0 && birthCell.y < N)
        {
            int occupied = 0;

            for (int j = 0; j < *count; j++) {
                if (species[j].p.x == birthCell.x &&
                    species[j].p.y == birthCell.y) {
                    occupied = 1;
                    break;
                }
            }
            
            for (int j = 0; j < *count; j++) {
                if (intents[j].target.x == birthCell.x &&
                    intents[j].target.y == birthCell.y) {
                    occupied = 1;
                    break;
                }
            }


            if (!occupied) found = 1;
        }

        option++;
    }

    individual newInd;
    newInd.p = birthCell;
    newInd.lP.age = 0;
    newInd.lP.energy = 8;
    newInd.sex = rand() % 2;

    species[*count] = newInd;
    (*count)++;

    intent newIntent;
    newIntent.indIndex = (*count) - 1;
    newIntent.target = birthCell;
    newIntent.satisfied = 1;
    intents[(*count) - 1] = newIntent;

}
