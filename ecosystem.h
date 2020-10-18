#include <vector>
#include <cstring>

using namespace std;

class tile;
class ecosystem;
class plant;
class Animal;

class tile{
    private:
        char environment;
        plant* pl;
        vector < Animal * > anvect;
    public:
        tile(char);
        char get_environment(void);
        void set_environment(char);
        void set_ptoken(char);
        char get_ptoken(void);
        void eraseanimal(int);
        void set_plant(plant*);
        plant* get_plant(void);
    	void set_animal(Animal*);
    	vector < Animal * >&  getanvect(void);
    	~tile();
};

class ecosystem{
    private:
		tile ***terrain;
        const int terrainSize;
        int dayOfYear;
        vector < int > anum;
    public:
        ecosystem( const int& , int);
        ~ecosystem();
        void GenerateRiver(void);
        void GenerateLake(int);
        void GenerateHills(int);
        void GenerateMeadow(void);
        void MapGenerator(int, int);
        void print_map();
        void PlacePlants(void);
        void PlaceAnimals(void);
        void Animalmovement(void);
        void Animaleating(void);
        void RunEcosystem(void);
        void DailyReset(int&, int& );
        void CheckDeadEntities(void);
        void PlantBreeding(int&);
        void AnimalBreeding(int&,int&);
        void FindFreeTile(int&,int&);
        void ApplySeason(int&, int&, int&, int&, int&, int&);
        void PrintSystem(void);
        void PrintPlantStatistics(void);
        void PrintAnimalStatistics(void);
        
};

class plant{
    protected:
        string name;
        int x;
        int y;
        char token;
        int breedingProb;
        int illnessProb;
        int life;
        int lifeFactor;
        int size;
    public:
        plant(string, char, int, int, int, int,int,int,int);
        char get_token(void);
        int get_size(void);
        int get_illnessProb(void);
        int get_breedingProb(void);
        virtual void growth(void);
        virtual void illness(void);
        int get_life(void);
        virtual void loselife(int);

};

class Seedless: public plant{
    public:
        Seedless(string, char, int, int, int, int, int, int);
        void loselife(int);
        void  illness(void);
        void growth(void);

};

class Seeded: public plant{
    protected:									
        int foliage;
        int seeds;
    public:
        Seeded(string, char, int, int, int, int, int, int, int, int, int);
        void loselife(int);
        void  illness(void);
        void growth(void);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////




class Animal {
	protected:
		string name;		
		int size;
		int hungerCount;
		int eatenFood;
		int eatCount;
		int x,y;
		bool isAlive;
		bool isHungry;
		bool inHeat;
		int speed;
		bool hibernates;
		bool inHibernation;
		int neededFood;
		bool hasmoved;
		char token;
	public:
		Animal(char,string, int , int , int, int ,bool, int, int);
		bool Move(char);
		virtual void Raise(void);
		string getname(void);
		void set_coordinates(int&,int&);
		bool gethasmoved(void);
		void sethasmoved(bool);
		int getspeed(void);
		bool get_hibernates(void);
		bool get_inHibernation(void);
		void sleep(int&);
		void setisAlive(bool);
		bool getisAlive(void);
		char getantoken(void);
		virtual void EatH(plant*);
		virtual void EatC(Animal*);
		int getsize(void);
		int getneededFood(void);
		virtual int getdefense(void);
		int get_hungerCount(void);
		void set_hungerCount(int);
		void set_eatenFood(int );
		void set_isHungry(bool);
		bool get_isHungry(void);
		void CheckHunger(void);
		virtual void RaiseH(int , int , int , int , int ,int );
		virtual void RaiseC(int , int , int  ,int , int ,int ,int ,int, int , int  );
};


class H : public Animal {
	private:
		bool canClimb;
	public:
		H(char,string, int, int, int, bool, bool , int, int, int); 
		void RaiseH(int , int ,int, int , int ,int );
		void EatH(plant*&);
};

class C : public Animal {
		private:
			int attack;
			int defense;
		public:
		C(char,string, int, int , int ,int ,int , bool, int, int, int);
		void RaiseC(int, int , int ,int, int, int , int , int , int ,int);
		void EatC(Animal *&);
		int getdefense(void);
};
