README 
1115201700023 (sdi1700023 _ Panagiotis Gkikas)
1115201700197 (sdi1700197 _ Thodoris-Maximilianos Chytis)

This read me file is about the implementation of the Project associated with the OO programming subject.
The source codes are written in C++ code.
This Project cointains three files: 1)(main.cpp ( cointaining  the main function)
				   2)ecosystem.cpp ( cointaining the source code of every function used in the Project)
				   3)ecosystem.h (header file which contains function and class declaration used in the Project))
The Project is about a functionig ecosystem containig different kind of environments, plants , and animals.
Our implementation contains Class Composition as well as Class Inheritance so that we can magnify the advantages of using an oo programming language (C++):

A few words about our class Composition:

A class named ecosystem cointains a two dimensional array made of pointers to obgects of class type tile;
Each tile contains the kind of the enviroment ,pointers to class types plant , and using the STL library an vector pointing to objects of class type Animal.
Each and everyone, of those classes has the needed and requested data members and is member functions in order to accomplish the requests about the ecosystem.
 
A few words about our class Inheritance:

Because of the different types of plants and Animals, our respective classes names plant , Animal are super classes and have public class Children.
Animal : public C (carnivores)
Animal : puvlic H (harnivores)
plant : Seedless 
plant : Seeded
These Children classes inherit protected data members as well as certain virtual member functions of their parents.

This way an ecosystem is created which allows animals to move around eat, get eaten, breed , and sleep as well as plants to breed and get eaten all according to 
the requests over the timeline of multiple seasons.
Because of the randomness in the project the result often differ and do not seem credible.This way it is more trustworthy to check out the code and the implementation of the functions.
More information about the exact implementation is found next to the respective code so that it is easier to understand.
For any more explaining feel free to contact us.


g++ -o Ecosystem ecosystem.cpp main.cpp

________________________________________
A few options to run the programma are:|
---------------------------------------- 

./Ecosystem
40
5
10

./Ecosystem
60
10
22

./Ecosystem
95
12
40


./Ecosystem
100
30
365 


./Ecosystem
200
50
700
_________________________________________________________________________________________________





.
  