#include <pthread.h>
#include <stdio.h>
#include "A*.cpp"
#include "SerialUSBHost.h"
#include "../firmware/src/ProtocolConstants.h" //this might have to be changed 
#include <iostream>
#include "../doc/map.cpp"

using std::tuple;
using std::vector;

/**
using namespace std;

  SerialUSBHost host;
  host.sendRaw(ECHO);

  while (true) {
    int pin;
    int level;
    cin >> pin >> level;
    level = (bool)level;
    host.sendCmd({0, SET_PINMODE, 2, pin, 1});
    host.sendCmd({0, SET_DIGITAL, 2, pin, level});
  }
} */

class threadWatcher {
	//timer
};

class Robot {
private:
	Map map;
	int xlocation;
	int ylocation;
	char directionFacing;
	vector<tuple<int, int>> IRDataU; //backup
	vector<tuple<int, int>> IRDataL;
	vector<tuple<int, int>> IRDataR;
	
public:
	Robot(void) {};
	Robot(Map grid): map(grid) {std::tie(xlocation, ylocation)= map.getStart();};
	
	void moveU () {
		++ylocation;
	}
	
	void moveL () {
		--xlocation;
	}
	
	void moveR () {
		++xlocation;
	}
	
	char getDirectionFacing() {
		return directionFacing;
	}
	
	tuple<int, int> getLocaton() {
		return std::make_tuple(xlocation, ylocation);
	}
	
	vector<tuple<int, int>> getIRDataU() {
		IRDataU.clear();
		tuple<int, int> data;
		int i=1;
		while (map.lookForObstacles(xlocation, ylocation+i)==' ') {
			data=std::make_tuple(i, 0);
			IRDataU.push_back(data);
		}
		data=std::make_tuple(i, 1);
		IRDataU.push_back(data);
		return IRDataU;
	}
	
	vector<tuple<int, int>> getIRDataL() {
		IRDataL.clear();
		tuple<int, int> data;
		int i=1;
		while (map.lookForObstacles(xlocation-i, ylocation)==' ') {
			data=std::make_tuple(i, 0);
			IRDataL.push_back(data);
		}
		data=std::make_tuple(i, 1);
		IRDataL.push_back(data);
		return IRDataL;
	}
	
	vector<tuple<int, int>> getIRDataR() {
		IRDataR.clear();
		tuple<int, int> data;
		int i=1;
		while (map.lookForObstacles(xlocation+i, ylocation)==' ') {
			data=std::make_tuple(i, 0);
			IRDataR.push_back(data);
		}
		data=std::make_tuple(i, 1);
		IRDataR.push_back(data);
		return IRDataR;
	}
	
	void changeDirectionFacing(int degree) {
		//something
	}
	
};

struct info {
	public:
		string com;
		Robot robot;
		vector<tuple<int, int>> IRDataU;
		vector<tuple<int, int>> IRDataL;
		vector<tuple<int, int>> IRDataR;
		info(void) {};
		info(Robot roborto): robot(roborto){};
		
};

void *robotDo(void *com) {
std::cout<< "reached multithread" << std::endl;
		struct info *commands= (struct info*)com;
		string command=commands->com;
		std::cout<<"command= "<<command<<" \t";
		Robot rob=commands->robot;
			if (command=="sense")  {
				commands->IRDataU=rob.getIRDataU(); 
				commands->IRDataL=rob.getIRDataL(); 
				commands->IRDataR=rob.getIRDataR();
				} 
			else if (command=="moveU") { rob.moveU();}
			else if (command=="moveL") {rob.moveL(); } 
			else if (command=="moveR") {rob.moveR();} 
			else if (command=="change90") {rob.changeDirectionFacing(90);}
			else if (command=="change270") {rob.changeDirectionFacing(270);}
			else if (command=="change180") {rob.changeDirectionFacing(180);}
			else if (command=="stop") { return 0; }
			else { return 0;}	
		std::cout<<"\n";
			/**int (*gridPositions)[10][10];
			gridPositions = (int(*)[10][10]) positions_ptr; // does this need to be in a catch
			for (auto &n: *gridPositions) {
			for (auto &m: n){
				m=1;
				}
			}*/
			
		return NULL;
}

class Brain {
	public:
	
	vector<Grid::Location> route; 
	tuple<int, int> Location;
	char directionFacing;
	vector<tuple<int,int>> stacksVisited;
	vector<tuple<int,int>> stacksToBeVisited;
	vector<tuple<int,int>> blocksCollected;
	vector<tuple<int,int>> blocksDiscarded;
	vector<tuple<int,int>> locationsVisited; 
	//each time you revisit a position, increase its obstacle prob. value with
	//diminishing returns
	//implement timeout with threads
	
	Brain (void) {};
	
		//internal
		void updateGrid() {
		
		}
		
		//void updatePosition() {}
		
		//void updateDirectionFacing() {}
		
		void updateCount() {
		
		}
		
		void stop() {
			
		}
		
		bool changeDirectionFacing () {
		
		}
		
		bool move(struct *info) {
			info->com="stop";
		}
		
		bool randomMovement() {
		
		}
		
		bool getInfoFromIR() {
		
		}
		bool getInfoFromCam() {
		
		}
		
		bool checkStack() {}
};


int main() {
	Map map;
	Brain brain;
	vector<vector<Grid::Location>> route;
	std::ifstream file ("red_map.txt"); //change to file name here
	
		//read file
		if (file.is_open()) { 
			std::cout << "reading file" << std::endl;
    		for (string line; std::getline(file, line);) {
        		std::cout << "newline= " << line << std::endl;
        		char *type= new char[line.length() + 1];
        		std::strcpy(type,line.c_str()); 
        		string remainder = line.substr(2);
        		map.construct(type[0], remainder);
   			} 
   		} else {
   			std::cout << "file not opening" << std::endl;
   		}
   		std::cout<<"done reading"<<std::endl;
		file.close();
		//run A*
		Grid::Location locs[30][30];
		Grid::Location start= map.getStart();
		int pm[30][30]; //has to be a parameter later on

		for (int m=0; m<30; m++) {
	  	for (int n=0; n<30; n++) {
	    	locs[m][n] = make_tuple(m,n);
	    	if (map.lookForObstacles(m,n)!='\0' && map.lookForObstacles(m,n)=='W') {
	    		pm[m][n] = 100;
	    		std::cout<<"wall= "<<m<<" " <<n<<std::endl;
	    	} else {pm[m][n]=0;}
	 		}
		}	
		
		
		Grid grid (locs);
		for (int m=0; m<30; m++) {
	  	for (int n=0; n<30; n++) {
	    	
	   		if (map.lookForStacks(m,n).getPosX()!=-1) {
	   		int x;
	   		int y;
	   		tie(x,y)=start;
	   		std::cout<< "start x= "<<x<<"\ty= "<<y<<"\n";
	    		Grid::Location goal= std::make_tuple(m,n);
	    		std::cout<< "goal x= "<<m<<"\ty= "<<n<<"\n";
				route.push_back(a_star_search(grid, start, goal, pm));
				start=goal;
	    	} 
	    	
	 		}
		}	
		
	
	int positions[30][30]; // needs to match grid in map
	Robot robot(map);
	bool interrupt;
	struct info *infos;
	brain.move();
	
	pthread_t brainThread;
	if(pthread_create(&brainThread, NULL, robotDo , &infos)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
		
	}
	std::cout << "the first" << std::endl;
	if(pthread_join(brainThread, NULL)) {
			fprintf(stderr, "Error joining thread\n");
			return 2;
	}
	std::cout << "exiting" << std::endl;
	
}

/**
//example code
void *inc_x(void *x_void_ptr) {

	int *x_ptr = (int *)x_void_ptr;
	while(++(*x_ptr) < 100);
	printf("x increment finished\n");
	return NULL;

}

int main() {

int x = 0, y = 0;
	printf("x: %d, y: %d\n", x, y);
	pthread_t inc_x_thread;
	if(pthread_create(&inc_x_thread, NULL, inc_x, &x)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	while(++y < 100);
	printf("y increment finished\n");
	if(pthread_join(inc_x_thread, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 2;
	}

printf("x: %d, y: %d\n", x, y);
return 0;

}*/
