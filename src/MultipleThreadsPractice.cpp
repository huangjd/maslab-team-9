#include <pthread.h>
#include <stdio.h>
#include "PathFinderNew.cpp"
#include "SerialUSBHost.h"
#include "ProtocolConstants.h" //these might have to be changed 
#include <iostream>
#include "MapMaker.cpp"

using std::tuple;
using std::vector;

struct Array {
	//because you cant put 2d arrays into 2darrays or can you
	public:
	int grid[10][10]={0};
	Array(void) {};
	
};

class threadWatcher {
	//timer
};

class Robot {
private:
	Map mapForTesting; //delete later
	
	double xlocation;
	double ylocation;
	int directionFacing;
	
public:
	Robot(void) {};
	Robot(Map fake): mapForTesting(fake) {
	std::tie(xlocation, ylocation)= mapForTesting.getStart();
	};
	
	void moveU () {
		ylocation+=0.1;
	}
	
	void moveL () {
		xlocation-=0.1;
	}
	
	void moveR () {
		xlocation+=0.1;
	}
	
	//stack is supposed to be 5x5 on 100x100 grid
	char lookAtCamera(string str) {
		int x=xlocation+0.5;
		int y=ylocation+0.5;
		if (str=="top") {char color=mapForTesting.lookForStacks(x, y).getTopBlock();}
		else if (str=="mid") {char color=mapForTesting.lookForStacks(x, y).getMidBlock();}
		else if (str=="bot") {char color=mapForTesting.lookForStacks(x, y).getBotBlock();}
		else {std::cout<<"wrong look at camera"<<std::endl;}
	}
	
	int getDirectionFacing() {
		return directionFacing;
	}
	
	tuple<int, int> getLocation() {
		return std::make_tuple(xlocation, ylocation);
	}
	
	//currently makes all walls 5x5 in the 100x100 grid
	//returns a vector of distance in the 100x100 grid and obstacle prob
	vector<tuple<int, int>> getIRDataU() {
		vector<tuple<int,int>> IRDataU;
		tuple<int, int> data;
		int i=1;
		int y;
		while (mapForTesting.lookForObstacles(xlocation, y)==' ' && i<5) {
		//for testing, added /10
			y=(int)(ylocation+(i/10));
			data=std::make_tuple(i, 0);
			IRDataU.push_back(data);
			++i;
		}
		data=std::make_tuple(i, 1);
		IRDataU.push_back(data);
		return IRDataU;
	}
	
	vector<tuple<int, int>> getIRDataL() {
		vector<tuple<int,int>> IRDataL;
		tuple<int, int> data;
		int i=1;
		int x;
		while (mapForTesting.lookForObstacles(x, ylocation)==' ' && i<5) {
		//for testing, added /10
			x=(int)(xlocation-(i/10));
			data=std::make_tuple(i, 0);
			IRDataL.push_back(data);
			++i;
		}
		data=std::make_tuple(i, 1);
		IRDataL.push_back(data);
		return IRDataL;
	}
	
	vector<tuple<int, int>> getIRDataR() {
		vector<tuple<int,int>> IRDataR;
		tuple<int, int> data;
		int i=1;
		int x;
		while (mapForTesting.lookForObstacles(x, ylocation)==' ' &&i<5) {
		//for testing, added /10
			x=(int)(xlocation+(i/10));
			data=std::make_tuple(i, 0);
			IRDataR.push_back(data);
			++i;
		}
		data=std::make_tuple(i, 1);
		IRDataR.push_back(data);
		return IRDataR;
	}
	
	void changeDirectionFacing(int degree) {
		directionFacing+=degree;
		directionFacing%=360;
	}
	
};

//communication between robot and brain
struct info {
	public:
		string com;
		Robot robot;
		vector<tuple<int, int>> IRDataU;
		vector<tuple<int, int>> IRDataL;
		vector<tuple<int, int>> IRDataR;
		info(void) {};
		info(Robot roborto): robot(roborto){};
		
		void makeDo(string str) {	
			com=str;
			std::cout<< "current command= "<<com<<std::endl;
		}
		
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
				std::cout<<"sensing"<<std::endl;
				} 
			else if (command=="moveU") { rob.moveU(); std::cout<<"moveU"<<std::endl;}
			else if (command=="moveL") {rob.moveL(); std::cout<<"moveL"<<std::endl;} 
			else if (command=="moveR") {rob.moveR();std::cout<<"moveR"<<std::endl;} 
			else if (command=="change90") {rob.changeDirectionFacing(90);std::cout<<"90"<<std::endl;}
			else if (command=="change270") {rob.changeDirectionFacing(270);std::cout<<"270"<<std::endl;}
			else if (command=="change180") {rob.changeDirectionFacing(180);std::cout<<"180"<<std::endl;}
			else if (command=="stop") {std::cout<<"stop"<<std::endl; } //change
			else { return 0;std::cout<<"stop"<<std::endl;}	
		std::cout<<"\n";
		return NULL;
		
}

class Brain {
	public:
	vector<Grid::Location> route; 
	vector<tuple<int,int>> stacksVisited;
	vector<tuple<int,int>> stacksToBeVisited;
	vector<tuple<int,int>> blocksCollected;
	vector<tuple<int,int>> blocksDiscarded;
	vector<tuple<int,int>> locationsVisited; 
	vector<tuple<int, int>> IRDataU; 
	vector<tuple<int, int>> IRDataL;
	vector<tuple<int, int>> IRDataR;
	Array grids[10][10]; //this is the 100x100 grid
	//each time you revisit a position, increase its obstacle prob. value with
	//diminishing returns
	//implement timeout with threads
	
	Brain (void) {};
	
		//updates the 100x100 grid
		void updateGrid(info infop) {
			int x; 
			int y;
			int xprecise;
			int yprecise;
			tie(x, y) =infop.robot.getLocation();
			tie(xprecise, yprecise)=infop.robot.getLocation();
			xprecise=(xprecise-x)*10;
			yprecise=(yprecise-y)*10;
			
			for (tuple<int, int> tup: infop.IRDataU) {
				int d;
				int p;
				tie(d, p) = tup;
				grids[x][y].grid[xprecise][yprecise+d]=p;
			}
			for (tuple<int, int> tup: infop.IRDataR) {
				int d;
				int p;
				tie(d, p) = tup;
				grids[x][y].grid[xprecise+d][yprecise]=p;
			}
			for (tuple<int, int> tup: infop.IRDataL) {
				int d;
				int p;
				tie(d, p) = tup;
				grids[x][y].grid[xprecise-d][yprecise]=p;
			}
			
		}
		
		void getCube() {
			/**
			//run A*
		Grid::Location locs[10][10];
		Grid::Location start= map.getStart();
		int pm[10][10]; //has to be a parameter later on

		for (int m=0; m<10; m++) {
	  	for (int n=0; n<10; n++) {
	    	locs[m][n] = make_tuple(m,n);
	    	if (map.lookForObstacles(m,n)!='\0' && map.lookForObstacles(m,n)=='W') {
	    		pm[m][n] = 100;
	    		//std::cout<<"wall= "<<m<<" " <<n<<std::endl;
	    	} else {pm[m][n]=0;}
	 		}
		}	
		
		
		Grid grid (locs);
		for (int m=0; m<10; m++) {
	  	for (int n=0; n<10; n++) {
	    	
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
		}	*/
			//break blocks
			//get cube
			//update count
		
		}
		
		void stop(info &infop) {
			infop.makeDo("stop");
		}
		
		void changeDirectionFacing (info &infop, int degree) {
			string str;
			if (degree==90) {str="change90";}
			else if (degree==180) {str="change180";}
			else if (degree==270) {str="change270";}
			else {std::cout<<"I refuse."<<std::endl;}
			infop.makeDo(str);
		}
		
		void move(info &infop, char movement) {
			string str;
			if (movement=='U') {str="moveU";}
			else if (movement=='L') {str="moveL";}
			else if (movement=='R') {str="moveR";}
			else {std::cout<<"I refuse."<<std::endl;}
			
			infop.makeDo(str);
			std::cout<<"command in move= "<<infop.com<<std::endl;
		}
		
		void randomMovement() {
			//do later
		}
		
		void getInfoFromIR(info &infop) {
			infop.makeDo("sense");
		}
		
		void getInfoFromCam(info &infop) {
			//just filler for now, this will all get scrapped later
			//infop.makeDo("camera");
			if (infop.robot.lookAtCamera("top")=='R') {std::cout<<"RTop"<<std::endl;}
			if (infop.robot.lookAtCamera("mid")=='R') {std::cout<<"RMid"<<std::endl;}
			if (infop.robot.lookAtCamera("bot")=='R') {std::cout<<"RBot"<<std::endl;}
		}
		
		bool checkStack(info infop) {
			//filler, also will get scrapped later on
			if (infop.robot.lookAtCamera("bot")==' ') {return false;}
			else {return true;}
		}
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
		
		//manually plot route
		vector<tuple<int,int>> oneRoute;
		Grid::Location start= map.getStart();
		for (int m=0; m<10; m++) {
	  	for (int n=0; n<10; n++) {
	    	
	   		if (map.lookForStacks(m,n).getPosX()!=-1) {
	   		int x;
	   		int y;
	   		int p;
	   		tie(x,y)=start;
	   		std::cout<< "start x= "<<x<<"\ty= "<<y<<"\n";
	    	std::cout<< "goal x= "<<m<<"\ty= "<<n<<"\n";
	    		if (x<m) {
					for (p=0;p<x-m;p++) {
						oneRoute.push_back(std::make_tuple(x+p+1, y));
					}
				} else if (x>m) {
					for (p=0;p<m-x;p++) {
						oneRoute.push_back(std::make_tuple(x-p-1, y));
					}
				}
				
				if (y<n) {
					for (p=0;p<y-n;p++) {
						oneRoute.push_back(std::make_tuple(x, y+p+1));
					}
				} else if (y>n) {
					for (p=0;p<n-y;p++) {
						oneRoute.push_back(std::make_tuple(x, y-p-1));
					}
				}
				x=m;
				y=n;
				route.push_back(oneRoute);
	    	} 
	    	
	 		}
		}	
		
	//make thread 
	int positions[10][10]; 
	Robot robot(map);
	bool interrupt;
	struct info infos(robot);
	brain.stop(infos);
	std::cout<<"command after function= " <<infos.com<<std::endl;
	brain.getInfoFromIR(infos);
	brain.updateGrid(infos);
	
	pthread_t brainthread;
	if(pthread_create(&brainthread, NULL, robotDo , &infos)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
		
	}
	
	if(pthread_join(brainthread, NULL)) {
			fprintf(stderr, "Error joining thread\n");
			return 2;
	}
	std::cout << "done program, exiting" << std::endl;
	
}
