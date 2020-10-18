#include <iostream>
#include <cstdlib>
#include <cstring>
#include "ecosystem.h"


using namespace std;

tile::tile(char env):environment(env)
{
    pl = NULL;    													//no plants in the beginning
}

void tile::eraseanimal(int k)										//erase animal of index k from the animal vector
{
	anvect.erase(anvect.begin()+k);
	
}


char tile::get_environment(void)
{
    return environment;
}

void tile::set_environment(char i)
{
    environment = i;
}

void tile::set_plant(plant* pl1)
{
    pl = pl1;
}

plant* tile::get_plant()
{
    return this->pl;
}

vector < Animal * >&  tile::getanvect(void)
{
	return anvect;
}

void tile::set_animal(Animal* pa)
{
	anvect.push_back(pa);				//add animal to animal vector
}

tile::~tile()
{
	delete pl;
	int ansize = anvect.size();		
	while(ansize > 0)
	{
		delete (anvect[ansize-1]);		// destroy each animal of the animal vector in each tile
		ansize--;
	}
	anvect.clear();				// clear vectors off pointers to animals
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ecosystem::ecosystem(const int& size, int day):terrainSize(size),dayOfYear(day)
{
	terrain = new tile **[size];
    for (int i=0; i<size;i++)										//create terrain of tiles without environment
    {
    	terrain[i] = new tile *[size];
        for (int j=0; j<size;j++)
			terrain[i][j] = new tile('0');							// tiles whith 0 are the free of environmennt tiles
    }
}

ecosystem::~ecosystem()
{
    for(int i=0;i<terrainSize;i++)
    {
        for (int j = 0; j < terrainSize; j++)						//delete terrain
			delete terrain[i][j];									// destroy each tile and its components
		delete[] terrain[i];		
    }
    delete[] terrain;
}

void ecosystem::GenerateRiver() 
{
    int riversource = rand() % (terrainSize - 9) + 5;               //The beggining of the River in the map
    int i = 0;
    int chance;
    int path;
    terrain[i][riversource - 1]->set_environment('#');               //first water element in map
    for (i = 1; i <terrainSize; i++)                                //river crosses the map
    {
        chance = 1 + rand() % 10;                                   //chance from 1 to 10

        if (chance <= 3)                                            //30% chance
        {
            path = rand() % 4 + 1;                                  //from 1 to 4
            switch (path)
            {
                case (1):                                           //right by one
                {
                    if (riversource < terrainSize)                  //check if we are inside the map
                        riversource++;
                    break;
                }
                case (2):                                          //right by two
                {
                    if (riversource < terrainSize)
                    {
                        riversource++;
                        terrain[i][riversource - 1]->set_environment('#');
                    }
                    if (riversource < terrainSize)
                        riversource++;
                    break;
                }
                case (3):                                          //left by one
                {
                    if (riversource > 1)
                        riversource--;
                    break;
                }
                case (4):                                           //left by two
                {
                    if (riversource > 1)
                    {
                        riversource--;
                        terrain[i][riversource - 1]->set_environment('#');
                    }
                    if (riversource > 1)
                        riversource--;
                    break;
                }
            }
        }
        terrain[i][riversource - 1]->set_environment('#');
    }
}

void ecosystem::GenerateLake(int lakeSize)
{
    int x = rand()%(terrainSize);
    int y = rand()%(terrainSize);
    if(lakeSize==10)
    {
        for(int i=0; i<lakeSize; i++)											//lake is a square with #
        {
            for(int j=0; j<lakeSize; j++)
                terrain[i][j]->set_environment('#');
        }
    }
    else
    {
        while (x + lakeSize >= terrainSize || y + lakeSize >= terrainSize)		//if lake is larger than terrainSize make it smaller
        {
            x = rand()%(terrainSize);
            y = rand()%(terrainSize);
        }
        for (int i = x; i < (x + lakeSize); i++)
        {
            for (int j = y; j < (y + lakeSize); j++)
                terrain[i][j]->set_environment('#');
        }
    }
}

void ecosystem::GenerateHills(int hillnum)										//hills are like lakes with ^ instead of #
{
    int n, hillSize, x, y, i, j, temp_i, temp_j, counter;
    bool flag,flag_out;
    for(n=0; n<hillnum; n++)
    {
        hillSize = 2+rand()%3;													//smallest hill is of square size 2 - largest is of size 4
        counter = 0;
        flag = false;
        flag_out=false;
        x = rand()%(terrainSize);
        y = rand()%(terrainSize);
        for(i=0; i<terrainSize; i++)
        {
            for(j=0; j<terrainSize; j++)
            {
                if(terrain[i][j]->get_environment() == '0')
                    counter++;
            }
        }
        if(counter >= hillSize*hillSize) 										//check lines and columns to put hills
		{
            int lpointer,line,cpointer,column,counter;
            for(cpointer=0; cpointer<terrainSize; cpointer++)
            {
                for(lpointer=0; lpointer<terrainSize; lpointer++)
                {
                    counter=0;
                    for(line=lpointer; line<=(lpointer+hillSize-1); line++)
                    {
                        for(column=cpointer; column<=(cpointer+hillSize-1); column++)
                        {
                            if( (column+1)>terrainSize || (line+1)>terrainSize )
                                counter=0;
                            else
                            {
                                if(terrain[line][column]->get_environment()!= '#' && terrain[line][column]->get_environment()!= '^')
                                    counter++;
                            }
                        }
                    }
                    if(counter==hillSize*hillSize && flag_out==false)
                        flag_out=true;
                }
            }
            if(flag_out!=true)
                cout<<"Not enough Space for Hill." <<endl;						//we don"t have enough free tiles for hill
            counter = 0;
            while(flag == false && counter<terrainSize*terrainSize && flag_out==true)
            {
                while((terrain[x][y]->get_environment() =='^' || terrain[x][y]->get_environment() == '#' || x+hillSize > terrainSize || y+hillSize > terrainSize))
                {
                    x = rand() %(terrainSize);
                    y = rand() %(terrainSize);
                }
                counter = 0;
                i=x;
                while(i < ( x+hillSize ) && flag == false)	
				{
                    j=y;
                    while(j < ( y+hillSize ) && flag == false) 
					{
                        if(terrain[i][j]->get_environment() == '#' || terrain[i][j]->get_environment()== '^' ) //check if we already have water or hill
						{
                            temp_i = i;
                            temp_j = j;
                            flag = true;
                        }
                        j++;
                    }
                    i++;
                }
                if(flag == true)
                {
                    x = temp_i;
                    y = temp_j;
                    flag = false;
                }
                else{
                    for(i=x; i<(x+hillSize); i++) {
                        for(j=y; j<(y+hillSize); j++)
						{
                            terrain[i][j]->set_environment('^');			//we put a hill
                        }
                    }
                    flag = true;
                }
                counter++;
            }
        }
        else
		{
            cout << "Not enough space for more hills.\n";
            break;
        }
    }
}

void ecosystem::GenerateMeadow()										//remaining free tiles become meadows
{
    int i, j;
    for(i=0; i<terrainSize; i++)
        for (j = 0; j < terrainSize; j++)
            if (terrain[i][j]->get_environment() == '0')
                terrain[i][j]->set_environment('"');
}

void ecosystem::MapGenerator(int lakeSize, int hillnum)					//we call the functions above an the map of tiles is created
{
    GenerateRiver();
    GenerateLake(lakeSize);
    GenerateHills(hillnum);
    GenerateMeadow();
}

void ecosystem::print_map(void)											// map is printed
{
    for (int i=0; i<terrainSize;i++)
    {
        for (int j=0; j<terrainSize;j++)
        {
        	if(terrain[i][j]->get_plant() == NULL)
            	cout <<terrain[i][j]->get_environment()<< " ";
            else
            	cout <<terrain[i][j]->get_plant()->get_token()<<" ";
        }
        cout<<endl;
    }
}

void ecosystem::PlacePlants()																			//create plants and put them in the map
{
    int meadows=0, watertiles=0, hills=0,Gsize=0, Asize=0, Osize=0, Psize=0, Msize=0, x, y;				//plant species counters
    for(int i=0; i<terrainSize; i++)
    {
        for (int j = 0; j < terrainSize; j++)															//counting environment
        {
            if(terrain[i][j]->get_environment() == '"')
                meadows++;
            else if(terrain[i][j]->get_environment() == '#')
                watertiles++;
            else if(terrain[i][j]->get_environment() == '^')
                hills++;
        }
    }
    if(meadows!=0)
        Gsize = rand()%meadows+1;																		//create plant counters for spieces of plants
    if(watertiles!=0)
        Asize = rand()%watertiles;
    if(meadows!=0)
    {
        Osize = meadows - Gsize;
        if(Osize!=0)
            Osize = rand() % Osize+1;
    }
    if(hills!=0)
        Psize = rand()%hills+1;
    if(meadows!=0 || hills!=0)
    {
        Msize = (meadows+hills) - (Gsize+Osize+Psize);
        if(Msize!=0)
            Msize = rand() % Msize+1;
    }
    while(Gsize)																						//put grass
        {
            x = rand() % terrainSize;
            y = rand() % terrainSize;
            if(terrain[x][y]->get_environment() == '"')
            {
                terrain[x][y]->set_plant(new Seedless("Grass",'G',15,15,5,4,x,y));
                Gsize--;
            }
        }
    while(Asize)																						//put algae
    {
        x = rand() % terrainSize;
        y = rand() % terrainSize;
        if(terrain[x][y]->get_environment() == '#')
        {
            terrain[x][y]->set_plant(new Seedless("Algae",'A',25,25,5,2,x,y));
            Asize--;
        }
    }
    while(Osize)																						//put oaks
    {
        x = rand() % terrainSize;
        y = rand() % terrainSize;
        if(terrain[x][y]->get_environment() == '"')
        {
            terrain[x][y]->set_plant(new Seeded("Oak",'O',20,20,30,15,5,30,15,x,y));
            Osize--;
        }
    }
    while(Psize)																						//put pines
    {
        x = rand() % terrainSize;
        y = rand() % terrainSize;
        if(terrain[x][y]->get_environment() == '^')
        {
            terrain[x][y]->set_plant(new Seeded("Pine",'P',15,15,40,20,5,40,20,x,y));
            Psize--;
        }
    }
    while(Msize)																						//put maples
    {
        x = rand() % terrainSize;
        y = rand() % terrainSize;
        if(terrain[x][y]->get_environment() == '"' || terrain[x][y]->get_environment() == '^')
        {
            terrain[x][y]->set_plant(new Seeded("Maple",'M',5,5,20,10,2,20,10,x,y));
            Msize--;
        }
    }
}

void ecosystem::PlaceAnimals(void)							//create animals and put them in tiles
{
	int meadows=0, watertiles=0, hills=0, x, y, i ,j ;
		for(int i=0; i<terrainSize; i++)
    	{
        	for (int j = 0; j < terrainSize; j++)			//count environment
        	{
            	if(terrain[i][j]->get_environment() == '"'){
              		meadows++;}
            	else if(terrain[i][j]->get_environment() == '#')
            		watertiles++;
            	else if(terrain[i][j]->get_environment() == '^')
                	hills++;
       		}
    	}
	for (i=0 ; i<=6; i++)									// 0:deer	1: rabbit	2:groundhog	3:salmon	4:fox	5:wolf	6:bear CONSTRUCTOR
	{
		anum.push_back(0);
	}

		if (meadows != 0){
			for (i=0;i<=2;i++)
			{
				anum[i]=(rand()%meadows+1);					//deer counter, rabbit counter, groundhog counter
			}
		}
		if (watertiles!=0){
			anum[3]=(rand()%watertiles+1);					//salmon counter
		}
		if (meadows != 0)
			anum[4]=(rand()%meadows+1);						//fox counter
		if (meadows!=0 || hills!=0){
			anum[5]=(rand()%(meadows+hills)+1);				//wolf counter
		}
		if (hills!=0){
			anum[6]=(rand()%hills+1);						//bear counter
		}
	i=anum[0];
	while(i)												//create and place animals with their characteristics 
    {
        x = rand() % terrainSize;
        y = rand() % terrainSize;
        if(terrain[x][y]->get_environment() == '"')
        {
        	terrain[x][y]->set_animal(new H('H',"deer",5,8,8,false,false,2,x,y));
            i--;
        }
    }
   	i=anum[1];
   while(i) 
    {
        x = rand() % terrainSize;					//random tile
        y = rand() % terrainSize;
        if(terrain[x][y]->get_environment() == '"')
        {
        	terrain[x][y]->set_animal(new H('H',"rabbit",2,6,4,false,false,1,x,y));
            i--;
        }
    }
    i=anum[2];
    while(i)
    {
        x = rand() % terrainSize;
        y = rand() % terrainSize;
        if(terrain[x][y]->get_environment() == '"')
        {
        	terrain[x][y]->set_animal( new H('H',"groundhog",3,5,5,true,true,1,x,y));
            i--;
        }
    }
    i=anum[3];
	while(i)
    {
        x = rand() % terrainSize;
        y = rand() % terrainSize;
        if(terrain[x][y]->get_environment() == '#')
        {
        	rand() % anum[4];
       		terrain[x][y]->set_animal(new H('H',"salmon",1,5,1,false,false,1,x,y));
            i--;
        }
    }
	i=anum[4];
    while(i)
    {
        x = rand() % terrainSize;
        y = rand() % terrainSize;
        if(terrain[x][y]->get_environment() == '"')
        {
        	terrain[x][y]->set_animal(new C('C',"fox",4,6,5,5,6,false,7,x,y));
            i--;
        }
    }
    i=anum[5];
    while(i)
    {
        x = rand() % terrainSize;
        y = rand() % terrainSize;
        if(terrain[x][y]->get_environment() == '#')
        {
        	terrain[x][y]->set_animal(new C('C',"wolf",7,8,8,6,8,false,7,x,y));
            i--;
        }
    }
    i=anum[6];
    while(i)
    {
        x = rand() % terrainSize;
        y = rand() % terrainSize;
        if(terrain[x][y]->get_environment() == '#')
        {
        	terrain[x][y]->set_animal(new C('C',"bear",10,4,10,10,10,true,10,x,y));;
            i--;
        }
    }


cout << anum[0] <<" deers  |" << anum[1] <<" rabbits  |" << anum[2] << " groundhogs   |" << anum[3] <<" salmons    |"<< anum[4] << " foxes   |" << anum[5] << " wolf    |"<< anum[6]  << " bears    |"<< endl;

}

void ecosystem::DailyReset(int& pperiod, int& aperiod)						//new day
{
	for(int i=0; i<terrainSize;i++)
	{
		for(int j=0;j<terrainSize;j++)
		{
			for(int x=0; x<terrain[i][j]->getanvect().size();x++)
			{
				if(terrain[i][j]->getanvect().size()!= 0)
				{
            		if(terrain[i][j]->getanvect()[x]->getantoken() == 'H')	//	herbivorous animals can survive up to 7 days if they harvest their needed food
            		{
            			if(terrain[i][j]->getanvect()[x]->get_hungerCount() > 7 && terrain[i][j]->getanvect()[x]->get_isHungry()==false)
            			{	
            				terrain[i][j]->getanvect()[x]->set_isHungry(true);
						}
            		}
            		else													//carnivorous animals are hungry every day (they don't keep food)
            		{
            			terrain[i][j]->getanvect()[x]->set_isHungry(true);
					}
					terrain[i][j]->getanvect()[x]->set_eatenFood(0);
            		if(aperiod!=0)
            		{
            			if(dayOfYear%aperiod == 0)							//animals grow after a specific number of days
            			{
            				if(terrain[i][j]->getanvect()[x]->getname() == "deer")
            					terrain[i][j]->getanvect()[x]->RaiseH(1,5,2,8,2,8);
            				else if(terrain[i][j]->getanvect()[x]->getname() == "rabbit")
            					terrain[i][j]->getanvect()[x]->RaiseH(1,2,2,6,1,4);
            				else if(terrain[i][j]->getanvect()[x]->getname() == "groundhog")
            					terrain[i][j]->getanvect()[x]->RaiseH(1,3,1,5,1,5);
            				else if(terrain[i][j]->getanvect()[x]->getname() == "fox")
            					terrain[i][j]->getanvect()[x]->RaiseC(1,4,1,6,1,6,1,5,1,5);
            				else if(terrain[i][j]->getanvect()[x]->getname() == "wolf")
            					terrain[i][j]->getanvect()[x]->RaiseC(1,7,2,8,2,8,2,8,2,6);
            				else if(terrain[i][j]->getanvect()[x]->getname() == "bear")
            					terrain[i][j]->getanvect()[x]->RaiseC(2,10,0,4,2,10,2,10,2,10);
						}
					}
				}
            }
		}
	}
    int grow;
    if(pperiod!=0)															//plants grow after a specific number of days
    {
        if (dayOfYear % pperiod == 0)
        {
            for (int i = 0; i < terrainSize; i++)
            {
                for (int j = 0; j < terrainSize; j++)
                {
                    if (terrain[i][j]->get_plant() != NULL)
                    {
                        grow = rand() % 100 + 1;							//some plants may be ill and thus not able to grow.They may even die of sickness
                        if (grow > terrain[i][j]->get_plant()->get_illnessProb())
                            terrain[i][j]->get_plant()->growth();
                        else
                            terrain[i][j]->get_plant()->illness();
                    }
                }
            }
        }
    }
}

void ecosystem::CheckDeadEntities()											//find corpses of animals, dead plants and erase thme from the map 
{
    for(int i=0; i<terrainSize;i++)
    {
        for(int j=0;j<terrainSize;j++)
        {
		if(terrain[i][j]->getanvect().size()!= 0)
		{
        	for(int x=0; x<terrain[i][j]->getanvect().size();x++)		
			{
				terrain[i][j]->getanvect()[x]->CheckHunger();				//check if 10 days with no food are reached thus animal dies.If not +1 day with no food
				if(terrain[i][j]->getanvect()[x]->getisAlive()== false)		//animals that die get erased from tile and deleted
				{
					if(terrain[i][j]->getanvect()[x]->getname() == "deer")
                        anum[0]--;
                    else if(terrain[i][j]->getanvect()[x]->getname() == "rabbit")
                        anum[1]--;
                    else if(terrain[i][j]->getanvect()[x]->getname() == "groundhog")
                        anum[2]--;
                	else if(terrain[i][j]->getanvect()[x]->getname() == "salmon")
                        	anum[3]--;
                    else if(terrain[i][j]->getanvect()[x]->getname() == "fox")
                        	anum[4]--;
                    else if(terrain[i][j]->getanvect()[x]->getname() == "wolf")
                        anum[5]--;
                	else if(terrain[i][j]->getanvect()[x]->getname() == "bear")
                        anum[6]--;
                    delete terrain[i][j]->getanvect()[x];			// delete from thee object of the Animal from the heap
					terrain[i][j]->eraseanimal(x);					// delete its pointer from the animal vector
					x--;											// correct position of index
				}
			}
		}
            if(terrain[i][j]->get_plant() != NULL)
            {
                if (terrain[i][j]->get_plant()->get_life() == 0)		//dead plants get erased from map and deleted
                {
                    delete(terrain[i][j]->get_plant());
		    		terrain[i][j]->set_plant(NULL);
                }
            }
       }
	}
}

void ecosystem::FindFreeTile(int& x, int& y)						//search and find free of plants tiles to put newborn plant one tile next to parent plant 
{
    if (x!=0 && terrain[x - 1][y]->get_plant() == NULL)				//left from parent
    {
        if(terrain[x][y]->get_plant()->get_token() == 'G')
        {
            if(terrain[x-1][y]->get_environment() == '"')
            {
                terrain[x - 1][y]->set_plant(new Seedless("Grass",'G',15,15,5,4,x-1,y));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'A')
        {
            if(terrain[x-1][y]->get_environment() == '#')
            {
                terrain[x - 1][y]->set_plant(new Seedless("Algae",'A',25,25,5,2,x-1,y));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'O')
        {
            if(terrain[x-1][y]->get_environment() == '"')
            {
                terrain[x - 1][y]->set_plant(new Seeded("Oak",'O',20,20,30,15,5,30,15,x-1,y));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'P')
        {
            if(terrain[x-1][y]->get_environment() == '"' || terrain[x-1][y]->get_environment() == '^')
            {
                terrain[x - 1][y]->set_plant(new Seeded("Pine",'P',15,15,40,20,5,40,20,x-1,y));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'M')
        {
            if(terrain[x-1][y]->get_environment() == '"' || terrain[x-1][y]->get_environment() == '^')
            {
                terrain[x - 1][y]->set_plant(new Seeded("Maple",'M',5,5,20,10,2,20,10,x-1,y));
            }
        }
    }
    else if( x!=terrainSize-1 && terrain[x+1][y]->get_plant() == NULL)	//right to parent
    {
        if(terrain[x][y]->get_plant()->get_token() == 'G')
        {
            if(terrain[x+1][y]->get_environment() == '"')
            {
                terrain[x + 1][y]->set_plant(new Seedless("Grass",'G',15,15,5,4,x+1,y));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'A')
        {
            if(terrain[x+1][y]->get_environment() == '#')
            {
                terrain[x + 1][y]->set_plant(new Seedless("Algae",'A',25,25,5,2,x+1,y));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'O')
        {
            if(terrain[x+1][y]->get_environment() == '"')
            {
                terrain[x + 1][y]->set_plant(new Seeded("Oak",'O',20,20,30,15,5,30,15,x+1,y));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'P')
        {
            if(terrain[x+1][y]->get_environment() == '"' || terrain[x+1][y]->get_environment() == '^')
            {
                terrain[x + 1][y]->set_plant(new Seeded("Pine",'P',15,15,40,20,5,40,20,x+1,y));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'M')
        {
            if(terrain[x+1][y]->get_environment() == '"' || terrain[x+1][y]->get_environment() == '^')
            {
                terrain[x + 1][y]->set_plant(new Seeded("Maple",'M',5,5,20,10,2,20,10,x+1,y));
            }
        }
    }
    else if( y!=terrainSize-1 && terrain[x][y+1]->get_plant() == NULL) //down from parent
    {
        if(terrain[x][y]->get_plant()->get_token() == 'G')
        {
            if(terrain[x][y+1]->get_environment() == '"')
            {
                terrain[x][y + 1]->set_plant(new Seedless("Grass",'G',15,15,5,4,x,y+1));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'A')
        {
            if(terrain[x][y+1]->get_environment() == '#')
            {
                terrain[x][y + 1]->set_plant(new Seedless("Algae",'A',25,25,5,2,x,y+1));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'O')
        {
            if(terrain[x][y+1]->get_environment() == '"')
            {
                terrain[x][y + 1]->set_plant(new Seeded("Oak",'O',20,20,30,15,5,30,15,x,y+1));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'P')
        {
            if(terrain[x][y+1]->get_environment() == '"' || terrain[x][y+1]->get_environment() == '^')
            {
                terrain[x][y + 1]->set_plant(new Seeded("Pine",'P',15,15,40,20,5,40,20,x,y+1));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'M')
        {
            if(terrain[x][y+1]->get_environment() == '"' || terrain[x][y+1]->get_environment() == '^')
            {
                terrain[x][y + 1]->set_plant(new Seeded("Maple",'M',5,5,20,10,2,20,10,x,y+1));
            }
        }
    }
    else if( y!=0 && terrain[x][y-1]->get_plant() == NULL)	//above parent
    {
        if(terrain[x][y]->get_plant()->get_token() == 'G')
        {
            if(terrain[x][y-1]->get_environment() == '"')
            {
                terrain[x][y - 1]->set_plant(new Seedless("Grass",'G',15,15,5,4,x,y-1));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'A')
        {
            if(terrain[x][y-1]->get_environment() == '#')
            {
                terrain[x][y - 1]->set_plant(new Seedless("Algae",'A',25,25,5,2,x,y-1));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'O')
        {
            if(terrain[x][y-1]->get_environment() == '"')
            {
                terrain[x][y - 1]->set_plant(new Seeded("Oak",'O',20,20,30,15,5,30,15,x,y-1));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'P')
        {
            if(terrain[x][y-1]->get_environment() == '"' || terrain[x][y-1]->get_environment() == '^')
            {
                terrain[x][y - 1]->set_plant(new Seeded("Pine",'P',15,15,40,20,5,40,20,x,y-1));
            }
        }
        else if(terrain[x][y]->get_plant()->get_token() == 'M')
        {
            if(terrain[x][y-1]->get_environment() == '"' || terrain[x][y-1]->get_environment() == '^')
            {
                terrain[x][y - 1]->set_plant(new Seeded("Maple",'M',5,5,20,10,2,20,10,x,y-1));
            }
        }
    }
}

void ecosystem::PlantBreeding(int& period)
{
	if (period != 0 && dayOfYear % period == 0)							//plants breed after a certain amount of days
	{
    	int breed;
    	for(int i=0;i<terrainSize;i++)
    	{
    	    for(int j=0;j<terrainSize;j++)
    	    {
    	        if(terrain[i][j]->get_plant()!= NULL)
    	        {
    	            breed = rand()%100+1;								//there is a possibility plants are not able to breed
    	            if(breed > terrain[i][j]->get_plant()->get_breedingProb())
    	            {
    	                FindFreeTile(i,j);
    	            }
    	        }
    	    }
    	}
    }
}

void ecosystem::AnimalBreeding(int& hperiod, int& cperiod)
{
	for(int i=0;i<terrainSize;i++)
	{
		for(int j=0;j<terrainSize;j++)
		{
			int ansize = terrain[i][j]->getanvect().size();
			if(ansize!=0)
			{
				for(int k=0;k<ansize;k++)
				{
					if(dayOfYear%hperiod==0)			//herbivorous animals breed after a certain amount of days and young animals are created
					{
						if(terrain[i][j]->getanvect()[k]->getname() == "deer" && terrain[i][j]->getanvect()[k]->getsize() == 5 )
						{
							terrain[i][j]->set_animal(new H('H',"deer",2,4,4,false,false,2,i,j)); 
							anum[0]++;
						}
						else if(terrain[i][j]->getanvect()[k]->getname() == "rabbit" && terrain[i][j]->getanvect()[k]->getsize() == 2)
						{
							terrain[i][j]->set_animal(new H('H',"rabbit",1,2,2,false,false,2,i,j));
							anum[1]++;
						}
						else if(terrain[i][j]->getanvect()[k]->getname() == "groundhog" && terrain[i][j]->getanvect()[k]->getsize() == 3 && terrain[i][j]->getanvect()[k]->get_inHibernation() == false)
						{
							terrain[i][j]->set_animal(new H('H',"groundhog",2,3,3,false,false,2,i,j));
							anum[2]++;
						}
						else if(terrain[i][j]->getanvect()[k]->getname() == "salmon")
						{
							terrain[i][j]->set_animal(new H('H',"salmon",1,5,1,false,false,1,i,j));
							anum[3]++;
						}
					}
					else if(dayOfYear%cperiod==0)		//carnivorous animals breed after a certain amount of days and young animals are created
					{		
						if(terrain[i][j]->getanvect()[k]->getname() == "fox" && terrain[i][j]->getanvect()[k]->getsize() == 4)
						{
							terrain[i][j]->set_animal(new C('C',"fox",1,1,1,1,2,false,7,i,j));
							anum[4]++;
						}
						else if(terrain[i][j]->getanvect()[k]->getname() == "wolf" && terrain[i][j]->getanvect()[k]->getsize() == 7)
						{
							terrain[i][j]->set_animal(new C('C',"wolf",1,2,2,2,2,false,7,i,j));
							anum[5]++;
						}
						else if(terrain[i][j]->getanvect()[k]->getname() == "bear" && terrain[i][j]->getanvect()[k]->getsize() == 10 && terrain[i][j]->getanvect()[k]->get_inHibernation() == false)
						{
							terrain[i][j]->set_animal(new C('C',"bear",3,4,6,6,5,false,7,i,j));
							anum[6]++;
						}
					}
				}
			}
		}
	}
}

void ecosystem::ApplySeason(int& season, int& pgrowthPeriod, int& pbreedingRepPeriod, int& agrowthPeriod, int& hbreedingRepPeriod, int& cbreedingRepPeriod)
{
	if(season == 1)							//1:spring	2:summer	3:autumn	4:winter	(periods of plants and animals change after next season)
    {
    	cout<<"We have Summer!"<<endl;
    	season++;
		pgrowthPeriod = 10;
		pbreedingRepPeriod = 10;
    	agrowthPeriod = 30;
		hbreedingRepPeriod = 8;
		cbreedingRepPeriod = 9;
    }
    else if(season == 2)
	{
		cout<<"We have Autumn!"<<endl;
    	season++;
        pgrowthPeriod = 0;
        pbreedingRepPeriod = 20;
        agrowthPeriod = 15;
        hbreedingRepPeriod = 5;
        cbreedingRepPeriod = 9;
    }
    else if(season == 3)
    {
    	cout<<"We have Winter!"<<endl;
        season++;
        pgrowthPeriod = 10;
        pbreedingRepPeriod = 0;
        agrowthPeriod = 30;
        hbreedingRepPeriod = 18;
        cbreedingRepPeriod = 10;
        for(int i=0;i<terrainSize;i++)
        {
            for(int j=0;j<terrainSize;j++)
            {
            	for(int x=0; x<terrain[i][j]->getanvect().size();x++)				//some animals hibernate during winter
            		if(terrain[i][j]->getanvect()[x]->get_hibernates() == true)
            			terrain[i][j]->getanvect()[x]->sleep(season);			
			}
		}
    }
    else if(season == 4)
    {
    	cout<<"We have Spring!"<<endl;
        season = 1;
        pgrowthPeriod = 5;
        pbreedingRepPeriod = 10;
        agrowthPeriod = 20;
        hbreedingRepPeriod = 12;
        cbreedingRepPeriod = 11;
        for(int i=0;i<terrainSize;i++)
        {
            for(int j=0;j<terrainSize;j++)
            {
            	for(int x=0; x<terrain[i][j]->getanvect().size();x++)
            		if(terrain[i][j]->getanvect()[x]->get_hibernates() == true)		//some animals wake up from hibernation at spring
            			terrain[i][j]->getanvect()[x]->sleep(season);
			}
		}
    }
}

void ecosystem::PrintSystem()						//print all plants and animals of the ecosystem
{
	int pcounter=0, acounter=0,hcounter=0,ccounter=0;
	for(int i=0;i<terrainSize;i++)
	{
		for(int j=0;j<terrainSize;j++)
		{
			if(terrain[i][j]->get_plant()!= NULL)
				pcounter++;
		}
	}
	for(int n=0;n<=6;n++)				//print total animals
	{
		acounter = acounter + anum[n];
	}
	for(int n=0;n<4;n++)				//print herbivorous
	{
		hcounter = hcounter + anum[n];
	}
	for(int n=4;n<=6;n++)		//print carnivorous
	{
		ccounter = ccounter + anum[n];
	}
	cout<<"The ecoystem has "<<pcounter<<" plants "<<acounter<<" animals, "<<hcounter<<" of them are herbivorous and the other "<<ccounter<<" are carnivorous."<<endl;
}

void ecosystem::PrintPlantStatistics()									//print number of species of plants of the ecosystem
{
	int Gcounter =0, Acounter=0,Ocounter=0,Pcounter=0,Mcounter=0;
	for(int i=0;i<terrainSize;i++)
	{
		for(int j=0;j<terrainSize;j++)
		{
			if(terrain[i][j]->get_plant() !=NULL)
			{
				if(terrain[i][j]->get_plant()->get_token() == 'G')
					Gcounter++;
				else if(terrain[i][j]->get_plant()->get_token() == 'A')
					Acounter++;
				else if(terrain[i][j]->get_plant()->get_token() == 'O')
					Ocounter++;
				else if(terrain[i][j]->get_plant()->get_token() == 'P')
					Pcounter++;
				else if(terrain[i][j]->get_plant()->get_token() == 'M')
					Mcounter++;
			}
		}
	}
	cout<<"The ecosystem has "<<Gcounter<<" grass, "<<Acounter<<" algaes, "<<Ocounter<<" oaks, "<<Pcounter<<" pines and "<<Mcounter<<" maples."<<endl;
}

void ecosystem::PrintAnimalStatistics()			//print species of animals of the ecosystem
{
	cout<<"The ecosystem has "<<anum[0]<<" deers, "<<anum[1]<<" rabbits, "<<anum[2]<< " groundhogs, "<<anum[3]<< " salmons, "<<anum[4]<<" foxes, "<<anum[5]<<" wolves and "<<anum[6]<<" bears."<<endl;
}

void ecosystem::RunEcosystem()
{
    int days, season, hour, pgrowthPeriod, pbreedingRepPeriod, agrowthPeriod, hbreedingRepPeriod, cbreedingRepPeriod;
    season = rand()%4+1;															//ecosystem begins with a random season
    cout<<"Enter the amount of days needed for the ecosystem to run: ";
    cin>>days;
    PrintSystem();																	//starting stats
    PrintPlantStatistics();
    PrintAnimalStatistics();
    switch(season)																	//first season period numbers
    {																				
        case(1):
        {
        	cout<<"We have Spring!"<<endl;
            pgrowthPeriod = 5;
            pbreedingRepPeriod = 10;
            agrowthPeriod = 20;
            hbreedingRepPeriod = 12;
            cbreedingRepPeriod = 11;
            break;

        }
        case(2):
        {
        	cout<<"We have Summer!"<<endl;									
            pgrowthPeriod = 10;
            pbreedingRepPeriod = 10;
        	agrowthPeriod = 30;
            hbreedingRepPeriod = 8;
            cbreedingRepPeriod = 9;
            break;
        }
        case(3):
        {
        	cout<<"We have Autumn!"<<endl;
            pgrowthPeriod = 0;
            pbreedingRepPeriod = 20;
            agrowthPeriod = 15;
            hbreedingRepPeriod = 5;
            cbreedingRepPeriod = 9;
            break;
        }
        case(4):
        {
        	cout<<"We have Winter!"<<endl;
            pgrowthPeriod = 10;
            pbreedingRepPeriod = 0;
            agrowthPeriod = 30;
            hbreedingRepPeriod = 18;
            cbreedingRepPeriod = 10;
            for(int i=0;i<terrainSize;i++)
            {
            	for(int j=0;j<terrainSize;j++)
            	{
            		for(int x=0; x<terrain[i][j]->getanvect().size();x++)
            			if(terrain[i][j]->getanvect()[x]->get_hibernates() == true)
            				terrain[i][j]->getanvect()[x]->sleep(season);
				}
			}
            break;
        }
    }
    for(dayOfYear=1;dayOfYear<days;dayOfYear++)										//ecosystem begins running for number of days
    {
    	cout<<"dayOfYear = "<<dayOfYear<<endl;
        if(dayOfYear%90 == 1)														//change of season 
        {
        	cout<<"Season just changed!"<<endl;
			ApplySeason(season,pgrowthPeriod,pbreedingRepPeriod,agrowthPeriod,hbreedingRepPeriod,cbreedingRepPeriod);
			PrintSystem();
    		PrintPlantStatistics();
    		PrintAnimalStatistics();
        }
        DailyReset(pgrowthPeriod, agrowthPeriod);									//daily check
        for(hour=0;hour<24;hour++)													//animals moving in search of food and eat for 24 hours
        {
        	Animalmovement();
			Animaleating();		
		}
		CheckDeadEntities();														//check for dead entities and clear them
		AnimalBreeding(hbreedingRepPeriod,cbreedingRepPeriod);						//breeding
        PlantBreeding(pbreedingRepPeriod);
    }
    cout<<"Ecosystem quits running whith final day "<<dayOfYear<<"!"<<endl;			//ecosystem finish
    PrintSystem();																	//show final stats
    PrintPlantStatistics();
    PrintAnimalStatistics();
}

void ecosystem::Animaleating()
{
	int i, j, size;
	for(i=0;i<terrainSize; i++){
		for(j=0;j<terrainSize;j++)
		{
			size=terrain[i][j]->getanvect().size();
			for (int k=0 ; k < size ; k++)
			{
				if (terrain[i][j]->getanvect()[k]->getantoken()=='H')				//herbivorous animals eat plants of the same tile 
				{
					terrain[i][j]->getanvect()[k]->EatH(terrain[i][j]->get_plant());
				}
				else
				{
					for (int m=0 ; m<size; m++)										//carnivorous animals eat other animals of the same tile
					{
						if (m!=k)
						{
							terrain[i][j]->getanvect()[k]->EatC(terrain[i][j]->getanvect()[m]);
						}				
					}
				}
			}
		}
	}

	
}

void ecosystem::Animalmovement(void)
{ 
	int i, j, size,k,temp;
	char env;
	for(i=0;i<terrainSize; i++){
		for (j=0; j<terrainSize ; j++){
			env=terrain[i][j]->get_environment();
			size = terrain[i][j]->getanvect().size();
			if (size!=0)			
			{
				for (k=0 ; k<size ; k++)				//each animal of the tile
				{
					if(terrain[i][j]->getanvect()[k]->get_hibernates()== false || terrain[i][j]->getanvect()[k]->get_inHibernation() == false) //check if animal is sleeping
					{
						if (terrain[i][j]->getanvect()[k]->gethasmoved()==false )							//animal k moves if it hasnt before
						{
							int direction=rand()%4 +1;									//random direction
							int counter=1;
							int _x, _y;
							while (counter<=4)
							{
								int temp=terrain[i][j]->getanvect()[k]->getspeed();
								switch(direction)
								{
									case(1):							//direction 1 move left
									{
										while (i-temp<0)				// move as much as the terrain size allows you to
											temp--;
										_y=j;
										_x=i;
										int l =1;
										while (l<=temp && terrain[i][j]->getanvect()[k]->Move(terrain[i-l][_y]->get_environment())==true)
										{
											_x=i-l;					//we have to check if the animal can enter this tile beacuse of their characteristics
											l++;
										}
										break;
									}
									case(2):						//direction 2 move right
									{
										while(i+temp>terrainSize-1)
											temp--;
										_x=i;	
										_y=j;
										int l =1;
										while (l<=temp && terrain[i][j]->getanvect()[k]->Move(terrain[i+l][_y]->get_environment())==true)
										{
											_x=i+l;
											l++;
										}
										break;
									}
									case(3):   // direction 3 move down
									{
										while(j-temp<0)
											temp--;
										_x=i;
										_y=j;
										int l =1;
										while (l<=temp && terrain[i][j]->getanvect()[k]->Move(terrain[_x][j-l]->get_environment())==true)
										{
											_y=j-l;
											l++;
										}
										break;
									}
									case(4):				//direction 4 move up
									{
										while(j+temp>terrainSize-1)
											temp--;
										_x=i;
										_y=j;
										int l =1;
										while (l<=temp && terrain[i][j]->getanvect()[k]->Move(terrain[_x][j+l]->get_environment())==true)
										{
											_y=j+l-1;
											l++;
										}
										break;
									}
								}	
								if(i!=_x || j!=_y) // _x _y are the new coordinates
								{
									counter=5; 												//works like a flag to exit the direction loop
									terrain[i][j]->getanvect()[k]->set_coordinates(_x,_y);		// set new coordiantes
									terrain[i][j]->getanvect()[k]->sethasmoved(true);
									terrain[_x][_y]->set_animal(terrain[i][j]->getanvect()[k]); // set the animal to its new location
									terrain[i][j]->eraseanimal(k);			//erase animal from the animal vector
									k--; 										//correct the index
									size= terrain[i][j]->getanvect().size();
								}
								else
								{
									direction++;				//change direction
									if (direction==5) 			
										direction=1;
								}
								counter++;    //increase  the amount of directions changed
							}
						}		
					}
				}
			}
		}
	}		
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

plant::plant(string nam, char tok, int bp, int ip, int l, int lf, int si, int _x, int _y): name(nam),token(tok),breedingProb(bp),illnessProb(ip),life(l),lifeFactor(lf),size(si),x(_x),y(_y)
{}

char plant::get_token()
{
    return this->token;
}

int plant::get_illnessProb()
{
    return this->illnessProb;
}

int plant::get_breedingProb()
{
    return this->breedingProb;
}

void plant::growth(){}

void plant::illness(){}

int plant::get_life()
{
    return this->life;
}

int plant::get_size()
{
	return this->size;
}

void plant::loselife(int l){}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Seedless::Seedless(string nam, char tok, int bp, int ip, int l, int lf, int _x, int _y): plant(nam,tok,bp,ip,l,lf,0,_x,_y){}

void Seedless::loselife(int eaten)			//seedless plants lose life points if they get eaten
{
    life = life - eaten;
    if(life<0)
	life = 0;
}

void Seedless::illness()					//ill seedless plants lose lifefactor number life points 
{
    life = life - lifeFactor;
    if(life<0)
	life = 0;
}

void Seedless::growth()					//seedless plants gain lifefactor life points 
{
    life = life + lifeFactor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Seeded::Seeded(string nam, char tok, int bp, int ip, int l, int lf, int si, int fol, int s, int _x, int _y): plant(nam,tok,bp,ip,l,lf,si,_x,_y),foliage(fol),seeds(s){}

void Seeded::loselife(int eaten) 	//seeded plants lose life points after they have lost all seeds and foliage
{
    if(seeds>eaten)
    {
        while(eaten)
        {
            seeds--;
            eaten--;
        }

    }
    else if(seeds<=eaten)
    {
        while(seeds)
        {
            seeds--;
            eaten--;
        }
        if(foliage>eaten)
        {

            while(eaten)
            {
                foliage--;
                eaten--;
            }
        }
        else if(foliage<=eaten)
        {
            while(foliage)
            {
                foliage--;
                eaten--;
            }
            if(life>eaten)
            {
                while(eaten)
                {
                    life--;
                    eaten--;
                }
            }
            else if(life<=eaten)
            {
                life = 0;
            }
        }
    }
}

void Seeded::illness()				//ill seeded plants lose lifefactor number foliage then double lifefactor number seeds and they get smaller
{
    foliage = foliage - lifeFactor;
    seeds = seeds - (2*lifeFactor);
    if(foliage%lifeFactor == 0)
        size--;
}

void Seeded::growth()				//seeded plants gain everything ill plants lose and get bigger-taller
{
    foliage = foliage + lifeFactor;
    seeds = seeds + (2*foliage);
    if(foliage%lifeFactor == 0)
        size++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Animal::Animal(char _token,string _name,int _size, int _speed,int _neededFood, int _eatCount, bool _hibernates, int _x, int _y):name(_name), size(_size), speed(_speed),
		eatCount(_eatCount), hibernates(_hibernates),token(_token),hungerCount(1),isHungry(true), neededFood(_neededFood),isAlive(true),hasmoved(false),x(_x),y(_y)
		{} // Animal constructor
		
void Animal::set_coordinates(int& i, int& j)
{
	x=i;
	y=j;
}

string Animal::getname(void){ return name;}

int Animal::getsize(){ return this->size; }

bool Animal::Move(char env){						//salmon can't move away from water, rabbit can't move to hill and groundhog can't swim
	if (name=="salmon" && env!='#') return false;
	if (name=="rabbit" && env=='^') return false;
	if (name=="groundhog" && env=='#') return false;
	return true;
			
}

int Animal::getneededFood(){
	return this->neededFood;
}

void Animal::setisAlive(bool _isAlive){
	isAlive=_isAlive;
}

bool Animal::getisAlive(void){
	return isAlive;
}


bool Animal::gethasmoved(void){
	return hasmoved;
}


void Animal::set_isHungry(bool _isHungry){
	isHungry=_isHungry;
}

bool Animal::get_isHungry(void){
	return this->isHungry;
}

void Animal::sethasmoved(bool l){
	hasmoved=l;
}

int Animal::getspeed(void){
	return speed;
}

char Animal::getantoken(void){
	return this->token;
}

bool Animal::get_inHibernation()
{
	return this->inHibernation;
}

bool Animal::get_hibernates()
{
	return this->hibernates;
}

void Animal::sleep(int& season)
{
	if(hibernates == true)
	{
		if(season == 1)
			inHibernation = false;				//set hibernation accordint to season (1 spring) ( 4 winter)
		if(season == 4)
			inHibernation = true;
	}
}

int Animal::get_hungerCount()
{
	return this->hungerCount;
}


void Animal::set_eatenFood(int food)
{
	eatenFood =food;
}

void Animal::Raise(void){}

void Animal::EatH(plant* pl){}

void Animal::EatC(Animal* an){}

int Animal::getdefense(){}


void Animal::set_hungerCount(int count)
{
	hungerCount = count;
}

void Animal::RaiseH(int i1, int i2, int i3, int i4 , int i5, int i6 ){}

void Animal::RaiseC(int i1, int i2, int i3, int i4 , int i5, int i6, int i7, int i8 , int i9, int i10 ){}

void Animal::CheckHunger()		//if an animal hasn't eaten for 10 days it dies from hunger
{
	if(hungerCount == 10)
		isAlive = false;
	else
		hungerCount++;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



H::H(char _token, string _name, int _size, int _speed, int _neededFood, bool _canClimb,bool _hibernates,int _eatCount, int _x, int _y):
	Animal(_token,_name,_size,_speed,_neededFood,_eatCount,_hibernates,_x,_y), canClimb(_canClimb)
 	{}
 	
void H::RaiseH(int a, int maxa, int b,int maxb, int c, int maxc)				//herbivorous animal grows in size,speed and needs more food
{	
	size += a;
	if (size>maxa) size=maxa;
	
	speed +=b;
	if (speed>maxb) speed=maxb;
	
	neededFood +=c;
	if (neededFood>maxc) size=maxc;
}

void H::EatH(plant *& plap){										//herbivorous eating
	if (plap!=NULL && isHungry==true){										//if there is a plnt and the animal has not eaten 
		if (eatenFood<neededFood && plap->get_life()>0){
			int plsize=plap->get_size();		
			char pltoken=plap->get_token();
			if (name=="deer"){								//deer
				if (plsize<=size+4 ){						//plantsize has to be smaller by 4
					if (plap->get_life()>=eatCount)
					{
						plap->loselife(eatCount);			//plant loses life
						eatenFood += eatCount;				//animal eats its food
					}
					else										
					{
						plap->loselife(plap->get_life());			//animal eats as much as there is left of the plant
						eatenFood += plap->get_life();
					}	
					
				}
			}
			else if(name=="rabbit"){						//rabbit
				if (pltoken!='A'){
					if (plap->get_life()>=eatCount)
					{
						plap->loselife(eatCount);
						eatenFood += eatCount;
					}
					else
					{
						plap->loselife(plap->get_life());
						eatenFood += plap->get_life();
					}
					
				}
			}
			else if(name=="groundhog")
			{
				if(inHibernation == true)
					eatenFood = neededFood;			//hibernates so does not need food
				else
				{
					if ( plsize <=3*size)			//can climb trees
					{
						if (plap->get_life()>=eatCount)
						{
							plap->loselife(eatCount);
							eatenFood += eatCount;
						}
						else
						{
							plap->loselife(plap->get_life());
							eatenFood += plap->get_life();
						}
					}
				}
			}
			else{					//salmon
				if (pltoken=='A'){				//only eats Alges
					if (plap->get_life()>=eatCount)				
					{
						plap->loselife(eatCount);
						eatenFood += eatCount;
					}
					else
					{
						plap->loselife(plap->get_life());
						eatenFood += plap->get_life();
					}
				}
			}
		}
		if (eatenFood>=neededFood)
		{
			isHungry=false;							//has eaten 
		}
	}
}
 	
 	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



C::C(char _token,string _name, int _size, int _speed,  int _attack, int _defense,int _neededFood, bool _hibernates,int _eatCount, int _x, int _y):
	Animal(_token,_name,_size,_speed,_neededFood,_eatCount,_hibernates,_x,_y),
	attack(_attack), defense(_defense)
	{}
	
 void C::RaiseC(int a,int maxa, int b,int maxb,  int c,int maxc, int d,int maxd ,int e, int maxe)		//carnivorous animal grows in size,speed,attack,defence and needs more food
 {
	size += a;
	if (size>maxa) size=maxa;
	
	speed +=b;
	if (speed>maxb) speed=maxb;
	
	neededFood +=c;
	if (neededFood>maxc) neededFood=maxc;
	
	attack +=d;
	if (attack>maxd) attack=maxd;
	
	defense +=e;
	if (defense>maxe) defense=maxe;
}

int C::getdefense(void)
{
	return defense;
}

void C::EatC(Animal*& ap)
{
	
	if (ap->getisAlive()==true && isAlive==true && isHungry==true)
	{
		if (ap->getantoken()=='H')			//carnivorous eating herbivorous
		{
			if (name == " fox " || name ==" wolf ")					
			{
				if (ap->getname()!="salmon")			//they dont eat salmons
				{
					if (size<=ap->getsize() && speed > ap->getspeed())			 
					{
						eatenFood=neededFood;
						hungerCount=0;
						ap->setisAlive(false);			//attacked animal dies
						isHungry=false;					//attacking animal has eaten
					}
				}
			}
			else if(name == "bear")
			{
				eatenFood=neededFood;
				hungerCount=0;
				if(inHibernation == false)
					ap->setisAlive(false);
				isHungry=false;
			}
		}
		else if(hungerCount>=8)					//carnivorous eating carnivourous
		{
			if (name=="bear")
			{
				if (ap->getname()!="bear")
				{
					eatenFood=neededFood;
					hungerCount=0;
					if(inHibernation == false)
						ap->setisAlive(false);
					isHungry=false;
				}
			}
			else if(name=="fox")
			{
				if (ap->getname()=="wolf")
				{
					if (size > ap->getsize() || (size == ap->getsize() && attack > ap->getdefense()) )
					{
						eatenFood=neededFood;
						hungerCount=0;
						ap->setisAlive(false);
						isHungry=false;
					}
				}
			}
			else
			{
				if (ap->getname()=="fox")
				{
					if (size > ap->getsize() || (size == ap->getsize() && attack > ap->getdefense()))
					{
						eatenFood=neededFood;
						hungerCount=0;
						ap->setisAlive(false);
						isHungry=false;
					}
				}
			}
		}
	}
}

