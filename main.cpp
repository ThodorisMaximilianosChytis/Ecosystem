#include <iostream>
#include <ctime>
#include <stdlib.h>
#include "ecosystem.h"

using namespace std;


int main(void)
{
    srand(time(NULL));
    int size, lsize, hnum;
    cout<<"enter size of terrain(>=10): ";
    cin>> size;
    lsize = 1+rand()%size;
    ecosystem system(size,2);
    cout<<"enter number of hills: ";
    cin>> hnum;
    system.MapGenerator(lsize,hnum);
    system.print_map();
    cout<<endl;
    system.PlacePlants();
    system.PlaceAnimals();
    system.Animalmovement();
	system.print_map();
    system.RunEcosystem();
    system.print_map();
    return 0;
}
