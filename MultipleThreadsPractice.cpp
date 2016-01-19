#include <pthread.h>
#include <stdio.h>
//#include "PathFinderNew.cpp"
#include "SerialUSBHost.h"
#include "ProtocolConstants.h" //this might have to be changed 
#include <iostream>
#include "map.cpp"

using std::tuple;
using std::vector;

//example 
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
	
};

class Robot {
private:
	int xlocation;
	int ylocation;
	Map map;
	vector<tuple<int, int>> IRDataU;
	vector<tuple<int, int>> IRDataL;
	vector<tuple<int, int>> IRDataR;
	
public:
	Robot() {
		std::ifstream file ("PracticeMap.txt"); //change to file name here
	
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
		file.close();
		std::tie(xlocation, ylocation)= map.getStart();
	}
	
	void moveU () {
		++ylocation;
	}
	
	void moveL () {
		--xlocation;
	}
	
	void moveR () {
		++xlocation;
	}
	
	void getIRDataU() {
		IRDataU.clear();
		tuple<int, int> data;
		int i=1;
		while (map.lookForObstacles(xlocation, ylocation+i)==' ') {
			data=std::make_tuple(i, 0);
			IRDataU.push_back(data);
		}
		data=std::make_tuple(i, 1);
		IRDataU.push_back(data);
		
	}
	
	void  getIRDataL() {
		IRDataL.clear();
		tuple<int, int> data;
		int i=1;
		while (map.lookForObstacles(xlocation-i, ylocation)==' ') {
			data=std::make_tuple(i, 0);
			IRDataL.push_back(data);
		}
		data=std::make_tuple(i, 1);
		IRDataL.push_back(data);

		
	}
	
	void  getIRDataR() {
		IRDataR.clear();
		tuple<int, int> data;
		int i=1;
		while (map.lookForObstacles(xlocation+i, ylocation)==' ') {
			data=std::make_tuple(i, 0);
			IRDataR.push_back(data);
		}
		data=std::make_tuple(i, 1);
		IRDataR.push_back(data);
		
	}
	
};

struct info {
	public:
		string com;
		Robot robot;
		info(void) {};
		info(Robot roborto): robot(roborto){};
		
};

void *robotDo(void *com) {
		struct info *commands= (struct info*)com;
		string command=commands->com;
		Robot rob=commands->robot;
			if (command=="sense")  {rob.getIRDataU(); rob.getIRDataL(); rob.getIRDataR();} 
			else if (command=="moveU") { rob.moveU(); }
			else if (command=="moveL") {rob.moveL(); } 
			else if (command=="moveR") {rob.moveR();} 
			else if (command=="stop") { return 0; }
			else { return 0;}	
		
			/**int (*gridPositions)[10][10];
			gridPositions = (int(*)[10][10]) positions_ptr; // does this need to be in a catch
			for (auto &n: *gridPositions) {
			for (auto &m: n){
				m=1;
				}
			}*/
			
		return NULL;
}
	
		//internal
		void updateGrid() {
		
		}
		
		void updatePosition() {
		
		}
		
		void updateDirectionFacing() {
		
		}
		
		void updateCount() {
		
		}
		
		tuple<int, int> getLocation() {
		}
		
		char getDirectionFacing() {
		}
		
		void interrupt() {
			
		}
		
		//sending
		bool changeDirectionFacing () {
		
		}
		
		bool move() {
		
		}
		
		bool randomMovement() {
		
		}
		
		//receiving
		bool getInfoFromCam() {
		
		}
		
		bool getInfoFromIR() {
		
		}


int main() {
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
	
	int positions[10][10]; // needs to match grid in map
	Robot robot;
	bool interrupt;
	
	std::cout<<"initial array"<<std::endl;
	for (auto &n: positions) {
		for (auto &m : n) {
				std::cout<< m << "\t" ;
			}
			std::cout<<"\n";
	}
	std::cout<<"\n";
	
	//struct info = new struct();
	pthread_t brainThread;
	if(pthread_create(&brainThread, NULL, robotDo , &positions)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
		
	}
	
	if(pthread_join(brainThread, NULL)) {
			fprintf(stderr, "Error joining thread\n");
			return 2;
	}
	
	std::cout<<"final array"<<std::endl;
	for (auto &n: positions) {
		for (auto &m: n) {
				std::cout<< m << "\t" ;
			}
			std::cout<<"\n";
	}
	
	
	

	
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