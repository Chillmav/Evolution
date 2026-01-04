#include <stdio.h>

#define MAX 400


void passiveEffects(individual species[MAX], int *count, intent intents[MAX]) {

    int i = 0;

    while (i < *count) {

        species[i].lP.energy--;
        species[i].lP.age++;

        if (species[i].lP.energy <= 0 || species[i].lP.age >= 100) {
            removeIndividual(species, count, i, intents);
            continue;     // don't increment i — new individual moved here
        }

        if (species[i].pregnancyTimer >= 0) {
            if (species[i].pregnancyTimer == 0) {
                bornInd(&species[i], species, count, intents);
                species[i].lP.energy--;
            }
            species[i].pregnancyTimer--;
        }

        i++;   // move to next individual
    }

}

void satisfyIntents(individual species[MAX], int *count, intent intents[MAX]) {

    // MOVES WITHOUT COLLISIONS // 2+ individuals want to enter the same cell (fight, mate)

        
        // PERFORM ACTIONS FOR THESE SPECIES

        //

    //

    // MOVES WITH COLLISIONS // only 1 individual want to enter the cell (just move, move + food, stay)

        // PERFORM ACTIONS FOR THESE SPECIES

        //

    //

    // END
}