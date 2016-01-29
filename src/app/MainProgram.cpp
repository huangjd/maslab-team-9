#include <pthread.h>
#include <stdio.h>
#include "../Astar.h"
#include <iostream>
#include "../FileMapMaker.h"
#include "../StackGet.h"
#include "../HAL.h"
#include <cstdio>
#include <ctime>
#include <cmath>

using std::tuple;
using std::vector;

pthread_mutex_t mutex;
std::clock_t timer; //currently not used

class Robot {
private:
	Map mapForTesting;
	
	int xlocation;
	int ylocation;
	int directionFacing;
	
public:
	Robot(void) {};
	Robot(Map fake): mapForTesting(fake) {
		std::tie(xlocation, ylocation)= mapForTesting.getStart();
		xlocation=xlocation*10-5;
		ylocation=ylocation*10-5;
	};
	
	void moveU () {
		int turni=360-directionFacing;
		turni%=360;
		double turnd=360-directionFacing;
		changeDirectionFacing(turni);
		turn(turnd);
		move_forward(1);
		ylocation+=1;
	}
	
	void moveL () {
		int turni=270+360-directionFacing;
		turni%=360;
		double turnd=360-directionFacing;
		changeDirectionFacing(turni);
		turn(turnd);
		move_forward(1);
		xlocation-=1;
	}
	
	void moveR () {
		int turni=90+360-directionFacing;
		turni%=360;
		double turnd=360-directionFacing;
		changeDirectionFacing(turni);
		turn(turnd);
		move_forward(1);
		xlocation+=1;
	}
	
	void moveD () {
		int turni=180+360-directionFacing;
		turni%=360;
		double turnd=360-directionFacing;
		changeDirectionFacing(turni);
		turn(turnd);
		move_forward(1);
		ylocation-=1;
	}
	
	char lookWithCamera(string str) {
	}
	
	double getDirectionFacing() {
		return directionFacing;
	}
	
	tuple<double, double> getLocation() {
		return std::make_tuple(xlocation, ylocation);
	}
	
	tuple<int, int> getIRDataU() {
		tuple<int,int> IRDataU;
		//get IR data from pin
		//int IRVal;
		//int distance= log(24.979/IRVal)/log(1.167);
		//IRData=std::make_tuple(distance, 10);
		IRDataU=std::make_tuple(1, 10);
		return IRDataU;
	}
	
	tuple<int, int> getIRDataL() {
		tuple<int,int> IRDataL;
		//int IRVal;
		//int distance= log(24.979/IRVal)/log(1.167);
		//IRData=std::make_tuple(distance, 10);
		IRDataL=std::make_tuple(1, 10);
		return IRDataL;
	}
	
	tuple<int, int> getIRDataR() {
		tuple<int,int> IRDataR;
		//int IRVal;
		//int distance= log(24.979/IRVal)/log(1.167);
		//IRData=std::make_tuple(distance, 10);
		IRDataR=std::make_tuple(1, 10);
		return IRDataR;
	}
	
	tuple<int, int> getIRDataD() {
		tuple<int,int> IRDataD;
		//int IRVal;
		//int distance= log(24.979/IRVal)/log(1.167);
		//IRData=std::make_tuple(distance, 10);
		IRDataD=std::make_tuple(1, 10);
		return IRDataD;
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
		tuple<int, int> IRDataU;
		tuple<int, int> IRDataL;
		tuple<int, int> IRDataR;
		tuple<int, int> IRDataD;
		info(void) {};
		info(Robot roborto): robot(roborto){};
		
		void makeDo(string str) {	
			com=str;
		}
		
};

void *robotDo(void *com) {
	std::cout<< "reached multithread" << std::endl;
	struct info *commands= (struct info*)com;
	string command=commands->com;
	//std::cout<<"command= "<<command<<std::endl;
	while (command!="stop") {
		command=commands->com;
		pthread_mutex_lock(&mutex);
		commands->IRDataU=commands->robot.getIRDataU(); 
		commands->IRDataL=commands->robot.getIRDataL(); 
		commands->IRDataR=commands->robot.getIRDataR();
		commands->IRDataD=commands->robot.getIRDataD();
		pthread_mutex_unlock(&mutex);
	}
	std::cout<<"ending"<<std::endl;
	return NULL;
		
}

class Brain {
	public:
	vector<vector<Grid::Location>> route; 
	//below not being used currently
	vector<tuple<int,int>> stacksVisited;
	vector<tuple<int,int>> stacksToBeVisited;
	vector<tuple<int,int>> blocksCollected;
	vector<tuple<int,int>> blocksDiscarded;
	tuple<int, int> IRDataU; 
	tuple<int, int> IRDataL;
	tuple<int, int> IRDataR;
	tuple<int, int> IRDataD;
	int grids[100][100]={{0}};
	//each time you revisit a position, increase its obstacle prob. value with
	//diminishing returns
	//implement timeout with threads
	
	Brain (void) {};
	
		void updateGrid(info infop) {
			int x; 
			int y;
			tie(x, y)=infop.robot.getLocation();
			int d;
			int p;
			tie(d, p) = infop.IRDataU;
			grids[x][y+d]+=p;

			tie(d, p) = infop.IRDataR;
			grids[d+x][y]+=p;

			tie(d, p) = infop.IRDataL;
			grids[x-d][y]+=p;

			tie(d, p) = infop.IRDataD;
			grids[x][y-d]+=p;
		
			
		}
		
		void getCube() {
			//break blocks
			//get cube
			//update count
			getStack();
		
		}
		
		void stop(info &infop) {
			infop.makeDo("stop");
		}
		
		void changeDirectionFacing (info &infop, int degree) {
			if (degree==90) {infop.robot.changeDirectionFacing(90);}
			else if (degree==180) {infop.robot.changeDirectionFacing(180);}
			else if (degree==270) {infop.robot.changeDirectionFacing(270);}
		}
		
		void move(info &infop, char movement) {
			tuple<double, double> loc=infop.robot.getLocation();
			int x;
			int y;
			tie (x, y)=loc;
			grids[x][y]+=2;
			if (movement=='U') {infop.robot.moveU();}
			else if (movement=='L') {infop.robot.moveL();}
			else if (movement=='R') {infop.robot.moveR();}
			else if (movement=='D') {infop.robot.moveD();}
		}
		
		void randomMovement() {
			//do later
		}
		
		void getInfoFromCam(info &infop) {
			//just filler for now, this will all get scrapped later
			if (infop.robot.lookWithCamera("top")=='R') {std::cout<<"RTop"<<std::endl;}
			if (infop.robot.lookWithCamera("mid")=='R') {std::cout<<"RMid"<<std::endl;}
			if (infop.robot.lookWithCamera("bot")=='R') {std::cout<<"RBot"<<std::endl;}
		}
		
		bool checkStack(info infop) {
			//filler, also will get scrapped later on
			if (infop.robot.lookWithCamera("bot")==' ') {return false;}
			else {return true;}
		}
		
		void path(info &infop) {
		//put in override function when surrounded by walls
		Grid::Location locs[100][100];
		for (int m=0; m<100; m++) {
	  	for (int n=0; n<100; n++) {
	    	locs[m][n] = make_tuple(m,n);
		}	
		}
		Grid grid (locs);
		while (route.size()!=0) {
			vector<tuple<int, int>> nextRoute=route.front();
			string str;

			while (nextRoute.size()!=0) {
				Grid::Location start=infop.robot.getLocation();
				Grid::Location goal= nextRoute.at(nextRoute.size()-1);
				nextRoute=astarsearch(grid, start, goal, grids);
				nextRoute.erase(nextRoute.begin());
				tuple<int,int> nextPos=nextRoute.front();
				int x1_;
				int x2_;
				int y1_;
				int y2_;
				tie (x1_, y1_)=infop.robot.getLocation();
				tie (x2_, y2_)=nextPos;
				if (x1_==x2_){
					if (y1_>y2_) {
						str="moveD";
					} else if (y2_>y1_) {
						str="moveU";
					}
				} else if (y1_==y2_) {
					if (x1_>x2_) {
						str="moveL";
					} else if (x2_>x1_) {
						str="moveR";
					}
				
				} else {std::cout<<"problem 2"<<std::endl;}
				do {
				if (pthread_mutex_lock(&mutex)==0){
					
					updateGrid(infop);
		
					if (str=="moveU") {
						move(infop, 'U');
						str="stop";
					} else if (str=="moveR") {
						move(infop, 'R');
						str="stop";
					} else if (str=="moveL") {
						move(infop, 'L');
						str="stop";
					} else if (str=="moveD") {
						move(infop, 'D');
						str="stop";
					}
					pthread_mutex_unlock(&mutex);
					}
					
				} while (str!="stop"); 
				
				nextRoute.erase(nextRoute.begin());
				//std::cout<<"at "<<x2_<<" "<<y2_<<"\n";
				}

			getCube();
			
			//testing
			/**int xx2;
			int yy2;
			tie (xx2, yy2)=infop.robot.getLocation();
			//std::cout<<"at "<<xx2<<" "<<yy2<<"\n";*/
			route.erase(route.begin());
		}
		
	}
		
		void unload() {
			std::cout<<"unloaded\n";
		}
		
		void spread(int x, int y) {
		int p=1;
		while (y+p<100 && p<3) {
			grids[x][y+p]+=50;
			++p;
		}
		
		p=1;
		while (x-p>0 && p<3) {
			grids[x-p][y]+=50;
			++p;
		}
		p=1;
		while (x+p<100 && p<3) {
			grids[x+p][y]+=50;
			++p;
		}
		p=1;
		while (y-p>0 && p<3) {
			grids[x][y-p]+=50;
			++p;
		}
	}
	
		void goToPlatform(info &infop, vector<tuple<int, int>> toPlatform) {
			Grid::Location locs[100][100];
			
			for (int m=0; m<100; m++) {
	  			for (int n=0; n<100; n++) {
	    			locs[m][n] = make_tuple(m,n);
				}	
			}
			Grid grid (locs);
			string str;

			while (toPlatform.size()!=0) {
				Grid::Location start=infop.robot.getLocation();
				Grid::Location goal= toPlatform.at(toPlatform.size()-1);
				toPlatform=astarsearch(grid, start, goal, grids);
				toPlatform.erase(toPlatform.begin());
				tuple<int,int> nextPos=toPlatform.front();
				int x1_;
				int x2_;
				int y1_;
				int y2_;
				tie (x1_, y1_)=infop.robot.getLocation();
				tie (x2_, y2_)=nextPos;
				if (x1_==x2_){
					if (y1_>y2_) {
						str="moveD";
					} else if (y2_>y1_) {
						str="moveU";
					}
				} else if (y1_==y2_) {
					if (x1_>x2_) {
						str="moveL";
					} else if (x2_>x1_) {
						str="moveR";
					}
				
				} else {std::cout<<"problem 2"<<std::endl;}
				do {
				if (pthread_mutex_lock(&mutex)==0){
					
					updateGrid(infop);
		
					if (str=="moveU") {
						move(infop, 'U');
						str="stop";
					} else if (str=="moveR") {
						move(infop, 'R');
						str="stop";
					} else if (str=="moveL") {
						move(infop, 'L');
						str="stop";
					} else if (str=="moveD") {
						move(infop, 'D');
						str="stop";
					}
					pthread_mutex_unlock(&mutex);
					}
					
				} while (str!="stop"); 
				
			}

		unload();
	}
	
};



int main() {
	Map map;
	map.createLinkArray();
	Brain brain;
	std::ifstream file ("MapFile.txt"); //change to file name here
	
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
		
		//draw boundaries
		for (int x=0; x<10; x++) {
			for (int y=0; y<10; y++) {
				int xi, yi;
				tie (xi, yi) = map.gridLinks[x][y];
				if (xi>-1){
					if (xi>x && yi>y) {
						for (int p=5; p<10; p++) {
							brain.grids[x*10+p][y*10+p]+=100;
							brain.spread(x*10+p, y*10+p);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi*10+p][yi*10+p]+=100;
							brain.spread(xi*10+p, yi*10+p);
						}
					} else if (xi>x && yi==y) {
						for (int p=5; p<10; p++) {
							brain.grids[x*10+p][y*10+5]+=100;
							brain.spread(x*10+p, y*10+5);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi*10+p][yi*10+5]+=100;
							brain.spread(xi*10+p, yi*10+5);
						}
					} else if (xi>x && yi<y) {
						for (int p=5; p<10; p++) {
							brain.grids[x*10+p][y*10+9-p]+=100;
							brain.spread(x*10+p, y*10+9-p);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi*10+p][yi*10+9-p]+=100;
							brain.spread(xi*10+p, yi*10+9-p);
						}
					} else if (xi<x && yi>y) {
						for (int p=5; p<10; p++) {
							brain.grids[x*10+9-p][y*10+p]+=100;
							brain.spread(x*10+9-p, y*10+p);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi*10+9-p][yi*10+p]+=100;
							brain.spread(xi*10+9-p, yi*10+p);
						}
					} else if (xi<x && yi==y) {
						for (int p=5; p<10; p++) {
							brain.grids[x*10+9-p][y*10+5]+=100;
							brain.spread(x*10+9-p, y*10+5);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi*10+9-p][yi*10+5]+=100;
							brain.spread(xi*10+9-p, yi*10+5);
						}
					} else if (xi<x && yi<y) {
						for (int p=5; p<10; p++) {
							brain.grids[x*10+9-p][y*10+9-p]+=100;
							brain.spread(x*10+9-p, y*10+9-p);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi*10+9-p][yi*10+9-p]+=100;
							brain.spread(xi*10+9-p, yi*10+9-p);
						}
					} else if (xi==x && yi>y) {
						for (int p=5; p<10; p++) {
							brain.grids[x*10+5][y*10+p]+=100;
							brain.spread(x*10+5, y*10+p);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi*10+5][yi*10+p]+=100;
							brain.spread(xi*10+5, yi*10+p);
						}
					} else if (xi==x && yi==y) {
						for (int p=5; p<10; p++) {
							brain.grids[x*10+5][y*10+5]+=100;
							brain.spread(x*10+5, y*10+5);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi*10+5][yi*10+5]+=100;
							brain.spread(xi*10+5, yi*10+5);
						}
					} else if (xi==x && yi<y) {
						for (int p=5; p<10; p++) {
							brain.grids[x*10+5][y*10+9-p]+=100;
							brain.spread(x*10+5, y*10+9-p);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi*10+5][yi*10+9-p]+=100;
							brain.spread(xi*10+5, yi*10+9-p);
						}
					} 
			}}
		
		}
		
		//for testing
		for (int y=0; y<100; y++) {
		std::cout<<"\n";
			for (int x=0; x<100; x++) {
						if (brain.grids[x][y]>=50) {
							std::cout<<".";
						} else std::cout<<" ";
					}
					
		
		}
		
		//run A*
		Grid::Location locs[100][100];
		int x, y;
		tie (x,y) = map.getStart();
		Grid::Location start= std::make_tuple((x*10-5), (y*10-5));

		for (int m=0; m<100; m++) {
	  	for (int n=0; n<100; n++) {
	    	locs[m][n] = make_tuple(m,n);
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
	    		Grid::Location goal= std::make_tuple(m*10-5,n*10-5);
	    		std::cout<< "goal x= "<<(m*10-5)<<"\ty= "<<(n*10-5)<<"\n";
				brain.route.push_back(astarsearch(grid, start, goal, brain.grids));
				start=goal;
	    	} 
	    	
	 		}
		}	
		
		//for testing
		for (auto &nextRoute: brain.route) {
		for (auto &pos:nextRoute) {
			int x;
			int y;
			tie (x,y) = pos;
			std::cout<<"x= "<<x<<" y= "<<y<<"\n";
		}
		std::cout<<"\n\n";
		}

	//make thread 
	Robot robot(map);
	struct info infos(robot);
	
	if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        printf("mutex creation failed\n");
        return 1;
    }
    
	pthread_t brainthread;
	if(pthread_create(&brainthread, NULL, robotDo , &infos)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
		
	}
	
	brain.path(infos);
	/**vector<tuple<int, int>> toPlatform;
	start=robot.getLocation();
	for (int m=0; m<10; m++) {
	  	for (int n=0; n<10; n++) {
	    	
	   		if (map.lookForObstacles(m,n)=='P') {
	    		Grid::Location goal= std::make_tuple(m*10-5,n*10-5);
				toPlatform=(astarsearch(grid, start, goal, brain.grids));
	    	} 
	    	
		}
	}	
	brain.goToPlatform(infos, toPlatform);*/
		
	brain.stop(infos);
	/**for (int x=0; x<100; x++) {
		std::cout<<"\n";
		for (int y=0; y<100; y++) {
			
			std::cout<<brain.grids[x][y];
			
		}
	}*/
	if(pthread_join(brainthread, NULL)) {
			fprintf(stderr, "Error joining thread\n");
			return 2;
	}
	
	pthread_mutex_destroy(&mutex);
	
}
