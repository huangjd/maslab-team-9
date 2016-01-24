#include <pthread.h>
#include <stdio.h>
#include "Astar.cpp"
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
		std::cout<<"start x= "<<xlocation<<" start y= "<<ylocation<<std::endl;
	};
	
	void moveU () {
		ylocation+=0.1;
		std::cout<<"new y= "<<ylocation<<std::endl;
	}
	
	void moveL () {
		xlocation-=0.1;
		std::cout<<"new x= "<<xlocation<<std::endl;
	}
	
	void moveR () {
		xlocation+=0.1;
		std::cout<<"new x= "<<xlocation<<std::endl;
	}
	
	//stack is supposed to be 5x5 on 100x100 grid
	char lookWithCamera(string str) {
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
	
	tuple<double, double> getLocation() {
		std::cout<<"getlocation= "<<xlocation<<", "<<ylocation<<std::endl;
		return std::make_tuple(xlocation, ylocation);
	}
	
	//currently makes all walls 5x5 in the 100x100 grid
	//returns a vector of distance in the 100x100 grid and obstacle prob
	vector<tuple<int, int>> getIRDataU() {
		//std::cout<<"U IR Data\n";
		vector<tuple<int,int>> IRDataU;
		IRDataU.push_back(std::make_tuple(1, 100));
		/**tuple<int, int> data;
		int i=1;
		int y;
		while (mapForTesting.lookForObstacles(xlocation, y)==' ' && i<5 && y<10) {
		//for testing, added /10
			y=(int)(ylocation+(i/10));
			data=std::make_tuple(i, 0);
			IRDataU.push_back(data);
			++i;
		}
		y=(int)(ylocation+(i/10));
		if (mapForTesting.lookForObstacles(xlocation, y)!=' ') {
			data=std::make_tuple(i, 1);
			IRDataU.push_back(data);
		} */
		return IRDataU;
	}
	
	vector<tuple<int, int>> getIRDataL() {
		//std::cout<<"L IR Data\n";
		vector<tuple<int,int>> IRDataL;
		/**tuple<int, int> data;
		int i=1;
		int x;
		while (mapForTesting.lookForObstacles(x, ylocation)==' ' && i<5 && x>0) {
		//for testing, added /10
			x=(int)(xlocation-(i/10));
			data=std::make_tuple(i, 0);
			IRDataL.push_back(data);
			++i;
		}
		x=(int)(ylocation-(i/10));
		if (mapForTesting.lookForObstacles(x, ylocation)!=' ') {
			data=std::make_tuple(i, 1);
			IRDataL.push_back(data);
		}*/
		IRDataL.push_back(std::make_tuple(1, 100));
		return IRDataL;
	}
	
	vector<tuple<int, int>> getIRDataR() {
		//std::cout<<"R IR Data\n";
		vector<tuple<int,int>> IRDataR;
		/**tuple<int, int> data;
		int i=1;
		int x;
		while (mapForTesting.lookForObstacles(x, ylocation)==' ' &&i<5 && x<10) {
		//for testing, added /10
			x=(int)(xlocation+(i/10));
			data=std::make_tuple(i, 0);
			IRDataR.push_back(data);
			++i;
		}
		x=(int)(ylocation+(i/10));
		if (mapForTesting.lookForObstacles(x, ylocation)!=' ') {
			data=std::make_tuple(i, 1);
			IRDataR.push_back(data);
		}*/
		IRDataR.push_back(std::make_tuple(1, 100));
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
	std::cout<<"command= "<<command<<std::endl;
	Robot rob=commands->robot;
	while (command!="stop") {
		command=commands->com;
		commands->IRDataU=rob.getIRDataU(); 
		commands->IRDataL=rob.getIRDataL(); 
		commands->IRDataR=rob.getIRDataR();
	}
	std::cout<<"ending"<<std::endl;
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
			double xprecised;
			double yprecised;
			tie(xprecised, yprecised)=infop.robot.getLocation();
			tie(x, y) =infop.robot.getLocation();
			std::cout<<"x= "<<xprecised<<" y= "<<yprecised<<std::endl;
			int xprecise=(xprecised-x)*10;
			int yprecise=(yprecised-y)*10;
			
			for (tuple<int, int> tup: infop.IRDataU) {
				int d;
				int p;
				tie(d, p) = tup;
				std::cout<<"x= "<<x<<xprecise<<" y= "<<y<<yprecise<<"\tirdatau= "<<grids[x][y].grid[xprecise][yprecise+d]<<std::endl;
				grids[x][y].grid[xprecise][yprecise+d]+=p;
			}
			for (tuple<int, int> tup: infop.IRDataR) {
				int d;
				int p;
				tie(d, p) = tup;
				std::cout<<"x= "<<x<<xprecise<<" y= "<<y<<yprecise<<"\tirdatar= "<<grids[x][y].grid[xprecise+d][yprecise]<<std::endl;
				grids[x][y].grid[xprecise+d][yprecise]+=p;
			}
			for (tuple<int, int> tup: infop.IRDataL) {
				int d;
				int p;
				tie(d, p) = tup;
				std::cout<<"x= "<<x<<xprecise<<" y= "<<y<<yprecise<<"\tirdatal= "<<grids[x][y].grid[xprecise-d][yprecise]<<std::endl;
				grids[x][y].grid[xprecise-d][yprecise]+=p;
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
			if (degree==90) {infop.robot.changeDirectionFacing(90);}
			else if (degree==180) {infop.robot.changeDirectionFacing(180);}
			else if (degree==270) {infop.robot.changeDirectionFacing(270);}
			else {std::cout<<"I refuse."<<std::endl;}
		}
		
		void move(info &infop, char movement) {
			if (movement=='U') {infop.robot.moveU();std::cout<<"bot moved up"<<std::endl;}
			else if (movement=='L') {infop.robot.moveL();}
			else if (movement=='R') {infop.robot.moveR();}
		}
		
		void randomMovement() {
			//do later
		}
		
		void getInfoFromCam(info &infop) {
			//just filler for now, this will all get scrapped later
			//infop.makeDo("camera");
			if (infop.robot.lookWithCamera("top")=='R') {std::cout<<"RTop"<<std::endl;}
			if (infop.robot.lookWithCamera("mid")=='R') {std::cout<<"RMid"<<std::endl;}
			if (infop.robot.lookWithCamera("bot")=='R') {std::cout<<"RBot"<<std::endl;}
		}
		
		bool checkStack(info infop) {
			//filler, also will get scrapped later on
			if (infop.robot.lookWithCamera("bot")==' ') {return false;}
			else {return true;}
		}
		
		void path() {
		//this is terrible for mazes but kind of works for the arena
		//first figure out where you can move
		//move until you are free to move in the general direction you are supposed to move
		//identify current location
		//then move from current position to edge of 10x10 grid using astar
		//reset
		/**
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
		}	*/
		}
		
		//for testing
		void print() {
			int a;
			int b;
			int c;
			int d;
			bool initial=true;
			for (a=0; a<5; a++) {
				for (b=0; b<5; b++) {
					if (initial) {std::cout<<"   ";for (int j=0; j<25; j++){if (j<10) {std::cout<<j<<"  ";} else {std::cout<<j<<" ";}} initial=false;};
					std::cout<<"\n";
					std::cout<<a<<b<<" ";
					for (c=0; c<5; c++) {
						for (d=0; d<5; d++) {
							std::cout<<grids[c][a].grid[d][b]<<"  ";
						}
					}
				}
			}
			std::cout<<"\n";
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
		
		//manually plot basic route, doesn't work for protruding maps
		Grid::Location start= map.getStart();
		int x;
	   	int y;
	   	tie(x,y)=start;
		for (int m=0; m<10; m++) {
	  	for (int n=0; n<10; n++) {
	    	
	   		if (map.lookForStacks(m,n).getPosX()!=-1) {
	   		int p;
	   		vector<tuple<int,int>> oneRoute;
	   		tuple<int, int> toPush;
	   		std::cout<< "start x= "<<x<<"\ty= "<<y<<"\n";
	    	std::cout<< "goal x= "<<m<<"\ty= "<<n<<"\n";

	    		if (x<m) {
					for (p=0;p<(m-x);) {
						++p;
						toPush=std::make_tuple(x+p, y);
						std::cout<<(x+p)<<"and "<<y<<std::endl;
						oneRoute.push_back(toPush);
					}
				} else if (x>m) {
					for (p=0;p<(x-m);) {
						++p;
						toPush=std::make_tuple(x-p, y);
						std::cout<<x-p<<"and "<<y<<std::endl;
						oneRoute.push_back(toPush);
					}
				}
				x=m;
				
				if (y<n) {
					for (p=0;p<(n-y);) {
						++p;
						toPush=std::make_tuple(x, y+p);
						std::cout<<x<<"and "<<y+p<<std::endl;
						oneRoute.push_back(toPush);
					}
				} else if (y>n) {
					for (p=0;p<(y-n);) {
						++p;
						toPush=std::make_tuple(x, y-p);
						std::cout<<x<<"and "<<y-p<<std::endl;
						oneRoute.push_back(toPush);
					}
				}

				y=n;
				route.push_back(oneRoute);
	    	} 
	    	
	 		}
		}	
	
	//make thread 
	Robot robot(map);
	struct info infos(robot);
	
	pthread_t brainthread;
	if(pthread_create(&brainthread, NULL, robotDo , &infos)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
		
	}
	string str;

	getline(std::cin, str);
	do {
		brain.updateGrid(infos);
		getline(std::cin, str);
		if (str=="moveU") {
			brain.move(infos, 'U');
		} else if (str=="moveR") {
			brain.move(infos, 'R');
		} else if (str=="moveL") {
			brain.move(infos, 'L');
		}
	} while (str!="stop"); 
	brain.stop(infos);
	
	
	if(pthread_join(brainthread, NULL)) {
			fprintf(stderr, "Error joining thread\n");
			return 2;
	}
	brain.print();

	std::cout << "done program, exiting" << std::endl;
	
}
