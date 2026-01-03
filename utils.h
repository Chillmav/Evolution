#include <stdio.h>

int fight(int energy1, int energy2); // OK

int fight(int energy1, int energy2){

    if ((energy1 + rand() % 5) > (energy2 + rand() % 5)){
        return 1;
    } else {
        return 0;
    }
}   
