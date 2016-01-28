#include <pthread.h>
#include <stdio.h>
#include "../maprelated/Astar.h"
#include <iostream>
#include "../maprelated/FileMapMaker.h"
#include "StackGet.h"
#include "../HAL.h"

using std::tuple;
using std::vector;
pthread_mutex_t mutex;


struct Array {
	public:
	int grid[10][10]={{0}};
	Array(void) {};
	
};

class threadWatcher {
	//timer
};

class Robot {
private:
	Map mapForTesting;
	
	double xlocation;
	double ylocation;
	int directionFacing;
	
public:
	Robot(void) {};
	Robot(Map fake): mapForTesting(fake) {
		std::tie(xlocation, ylocation)= mapForTesting.getStart();
		//std::cout<<"start x= "<<xlocation<<" start y= "<<ylocation<<std::endl;
	};
	
	void moveU () {
		int turn=360-directionFacing;
		turn%=360;
		changeDirectionFacing(turn);
		double turnd=turn;
		void turn(turnd);
		void move_forward(1);
		ylocation+=0.1;
		//std::cout<<"new y= "<<ylocation<<std::endl;
	}
	
	void moveL () {
		int turn=360-directionFacing;
		turn%=360;
		changeDirectionFacing(turn);
		double turnd=turn;
		void turn(turnd);
		void move_forward(1);
		xlocation-=0.1;
		//std::cout<<"new x= "<<xlocation<<std::endl;
	}
	
	void moveR () {
		int turn=360-directionFacing;
		turn%=360;
		changeDirectionFacing(turn);
		double turnd=turn;
		void turn(turnd);
		void move_forward(1);
		xlocation+=0.1;
		//std::cout<<"new x= "<<xlocation<<std::endl;
	}
	
	void moveD () {
		int turn=360-directionFacing;
		turn%=360;
		changeDirectionFacing(turn);
		double turnd=turn;
		void turn(turnd);
		void move_forward(1);
		ylocation-=0.1;
		//std::cout<<"new y= "<<ylocation<<std::endl;
	}
	
	char lookWithCamera(string str) {
		int x, y;
		while (xlocation>10) {
			xlocation-=10;
			++x;
		}
		while (ylocation>10) {
			ylocation-=10;
			++y;
		}
		if (str=="top") {char color=mapForTesting.lookForStacks(x, y).getTopBlock();}
		else if (str=="mid") {char color=mapForTesting.lookForStacks(x, y).getMidBlock();}
		else if (str=="bot") {char color=mapForTesting.lookForStacks(x, y).getBotBlock();}
		else {std::cout<<"wrong look at camera"<<std::endl;}
	}
	
	int getDirectionFacing() {
		return directionFacing;
	}
	
	tuple<double, double> getLocation() {
		return std::make_tuple(xlocation, ylocation);
	}
	
	//currently makes all walls 5x5 in the 100x100 grid
	//returns a vector of distance in the 100x100 grid and obstacle prob
	vector<tuple<int, int>> getIRDataU() {
		vector<tuple<int,int>> IRDataU;
		IRDataU.push_back(std::make_tuple(1, 0));
		return IRDataU;
	}
	
	vector<tuple<int, int>> getIRDataL() {
		vector<tuple<int,int>> IRDataL;
		IRDataL.push_back(std::make_tuple(1, 0));
		return IRDataL;
	}
	
	vector<tuple<int, int>> getIRDataR() {
		vector<tuple<int,int>> IRDataR;
		IRDataR.push_back(std::make_tuple(1, 0));
		return IRDataR;
	}
	
	vector<tuple<int, int>> getIRDataD() {
		vector<tuple<int,int>> IRDataD;
		IRDataD.push_back(std::make_tuple(1, 0));
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
		vector<tuple<int, int>> IRDataU;
		vector<tuple<int, int>> IRDataL;
		vector<tuple<int, int>> IRDataR;
		vector<tuple<int, int>> IRDataD;
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
	//vector<tuple<int,int>> locationsVisited; 
	vector<tuple<int, int>> IRDataU; 
	vector<tuple<int, int>> IRDataL;
	vector<tuple<int, int>> IRDataR;
	vector<tuple<int, int>> IRDataD;
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
			x=xprecised*1.002;
			y=yprecised*1.002;
			int xprecise=(xprecised-double(x))*10.002;
			int yprecise=(yprecised-double(y))*10.002;
			
			for (tuple<int, int> tup: infop.IRDataU) {
				int d;
				int p;
				tie(d, p) = tup;

				if (yprecise+d>9) {
					int innery=y+1;
					int newVal=yprecise+d-10;
					grids[x][innery].grid[xprecise][newVal]+=p;
				} else {
					grids[x][y].grid[xprecise][yprecise+d]+=p;
				}
				
			}
		
			for (tuple<int, int> tup: infop.IRDataR) {
				int d;
				int p;
				tie(d, p) = tup;
				if (xprecise+d>9) {
					int innerx;
					innerx=x+1;
					int newVal=(xprecise+d)-10;
					grids[innerx][y].grid[newVal][yprecise]+=p;
					
				} else {
					
					grids[x][y].grid[xprecise+d][yprecise]+=p;
				}
			}
			
			for (tuple<int, int> tup: infop.IRDataL) {
				int d;
				int p;
				tie(d, p) = tup;
				if (xprecise-d<0) {
					int innerx=x-1;
					int newVal=xprecise-d+10;
					grids[innerx][y].grid[newVal][yprecise]+=p;
					
				} else {
					
					grids[x][y].grid[xprecise-d][yprecise]+=p;
				}
				
			}
			
			for (tuple<int, int> tup: infop.IRDataD) {
				int d;
				int p;
				tie(d, p) = tup;
				if (yprecise-d<0) {
					int innery=y-1;
					int newVal=yprecise-d+10;
					grids[x][innery].grid[xprecise][newVal]+=p;
				} else {
					grids[x][y].grid[xprecise][yprecise-d]+=p;
				}
				
			}
			
		}
		
		void getCube() {
			//break blocks
			//get cube
			//update count
			if (getStack()) {
				std::cout<<"got cube"<<std::endl;
			} else {
			//	std::cout<<"we screwed up"<<std::endl;
			}
		
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
			tuple<double, double> loc=infop.robot.getLocation();
			int x;
			int y;
			double xprecise;
			double yprecise;
			tie (xprecise, yprecise)=loc;
			x=xprecise*1.002;
			y=yprecise*1.002;
			int xsmall=(xprecise-double(x))*10.002;
			int ysmall=(yprecise-double(y))*10.002;
			grids[x][y].grid[xsmall][ysmall]+=5;
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
			Grid::Location locs[10][10];
			for (int m=0; m<10; m++) {
	  		for (int n=0; n<10; n++) {
	    		locs[m][n] = make_tuple(m,n);
			}	
		}
			Grid grid (locs);
		//put in override function when surrounded by walls
		
		while (route.size()!=0) {
			vector<tuple<int, int>> nextRoute=route.front();
			//getting route to new stack
			for (auto &lroute: route) {
			tuple<int,int> pos=lroute.front();
			int x;
			int y;
			tie (x,y)=pos;
			}
			string toNewGrid;
			
			while (nextRoute.size()!=0) {
			//getting new position in current route to stack
				tuple<int,int> nextPos=nextRoute.front();
				bool check=false;
				string str;
				vector<tuple<int,int>> smallRoute;
				double x1d;
				double y1d;
				double x2d;
				double y2d;
				int x1;
				int x2;
				int y1;
				int y2;
				tie (x1d, y1d) = infop.robot.getLocation();
				x1=x1d*1.002;
				y1=y1d*1.002;
				tie (x2d, y2d) = nextPos;
				x2=x2d*1.002;
				y2=y2d*1.002;

				//this is going to be changed
				//moving on small grid to next big grid
				Grid::Location goal;
				Grid::Location start;
				if (x1==x2 && y1==y2) {
					getCube();
					std::cout<<"at "<<x1<<" "<<y1<<"\n";
				} else {
				if (x1==x2){
					if (y1>y2) {
						goal=std::make_tuple(5,0);
						toNewGrid="moveD";
					} else if (y2>y1) {
						goal=std::make_tuple(5, 9);
						toNewGrid="moveU";
					} 
				} else if (y1==y2) {
					if (x1>x2) {
						goal=std::make_tuple(0, 5);
						toNewGrid="moveL";
					} else if (x2>x1) {
						goal=std::make_tuple(9, 5);
						toNewGrid="moveR";
					} 
				
				} else {std::cout<<"problem"<<std::endl;}
				
				//testing
				int x_x;
				int y_y;
				tie(x_x, y_y)=goal;
				
				while (start!=goal) {
				double xprecised;
				double yprecised;
				tie (xprecised, yprecised)=infop.robot.getLocation();
				int x=xprecised*1.001;
				int y=yprecised*1.001;
				xprecised-=double(x);
				yprecised-=double(y);
				int xprecise=xprecised*10.001;
				int yprecise=yprecised*10.001;
				start= std::make_tuple(xprecise, yprecise);

				if (start==goal) {
				} else {
				vector<Grid::Location> smallRoute=astarsearch(grid, start, goal, grids[x][y].grid);
				smallRoute.erase(smallRoute.begin());
				tuple<int, int> firstPos=smallRoute.front();
				int x1_=xprecise;
				int x2_;
				int y1_=yprecise;
				int y2_;
				tie (x2_, y2_)=firstPos;
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
					if (pthread_mutex_trylock(&mutex)==0){
					updateGrid(infop);
					if (check) {
						str="stop";
						check=false;
					}
					pthread_mutex_unlock(&mutex);
		
					if (str=="moveU") {
						move(infop, 'U');
						check=true;
					} else if (str=="moveR") {
						move(infop, 'R');
						check=true;
					} else if (str=="moveL") {
						move(infop, 'L');
						check=true;
					} else if (str=="moveD") {
						move(infop, 'D');
						check=true;
					}
					}
					
				} while (str!="stop"); 
				}
				} 
				
				do{
				if (pthread_mutex_trylock(&mutex)==0){
					updateGrid(infop);
					if (check) {
						toNewGrid="stop";
						check=false;
					}
					pthread_mutex_unlock(&mutex);
		
					if (toNewGrid=="moveU") {
						move(infop, 'U');
						check=true;
					} else if (toNewGrid=="moveR") {
						move(infop, 'R');
						std::cout<<"tonewgrid moveR"<<"\n";
						check=true;
					} else if (toNewGrid=="moveL") {
						move(infop, 'L');
						check=true;
					}  else if (toNewGrid=="moveD") {
						move(infop, 'D');
						check=true;
					}
					}
				} while (toNewGrid!="stop"); 
				//reached new big grid
				//be careful of the final direction not being in the correct direction
				//shouldn't happen, but might
				double xC;
				double yC;
				tie (xC, yC)=infop.robot.getLocation();
				}
	
				nextRoute.erase(nextRoute.begin());
			}
			getCube();
			double xx2;
			double yy2;
			tie (xx2, yy2)=infop.robot.getLocation();
			std::cout<<"at "<<xx2<<" "<<yy2<<"\n";
			route.erase(route.begin());
		}
		
		}
		
		
};

	void floodFill(int x, int y, Map &map) {
		
		if (x+1<10 && map.grid[x+1][y]=='\0')	{
			map.grid[x+1][y]='W';
			floodFill(x+1, y, map);
		}
		if (x-1>0 && map.grid[x-1][y]=='\0')	{
			map.grid[x-1][y]='W';
			floodFill(x-1, y, map);
		}
		if (y+1<10 && map.grid[x][y+1]=='\0')	{
			map.grid[x][y+1]='W';
			floodFill(x, y+1, map);
		}
		if (y-1>0 && map.grid[x][y-1]=='\0')	{
			map.grid[x][y-1]='W';
			floodFill(x, y-1, map);
		}
	}
	
	void spread(int x, int y, int grid[10][10]) {
		int p=1;
		while (y+p<10 && p<3) {
			grid[x][y+p]+=50;
			++p;
		}
		
		p=1;
		while (x-p>0 && p<3) {
			grid[x-p][y]+=50;
			++p;
		}
		p=1;
		while (x+p<10 && p<3) {
			grid[x+p][y]+=50;
			++p;
		}
		p=1;
		while (y-p>0 && p<3) {
			grid[x][y-p]+=50;
			++p;
		}
	}

int main() {
	Map map;
	Brain brain;
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
		
		//flood fill
		int x,y;
		tie (x,y) = map.getStart();
		Map map2=map;
		floodFill(x,y,map2);
		
		for (int x=0; x<10; x++) {
			for (int y=0; y<10; y++) {
				int xi, yi;
				tie (xi, yi) = map.gridLinks[x][y];
				if (xi>0){
					if (xi>x && yi>y) {
						for (int p=5; p<10; p++) {
							brain.grids[x][y].grid[p][p]+=100;
							spread(p, p, brain.grids[x][y].grid);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi][yi].grid[p][p]+=100;
							spread(p, p, brain.grids[xi][yi].grid);
						}
					} else if (xi>x && yi==y) {
						for (int p=5; p<10; p++) {
							brain.grids[x][y].grid[p][5]+=100;
							spread(p, 5, brain.grids[x][y].grid);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi][yi].grid[p][5]+=100;
							spread(p, 5, brain.grids[xi][yi].grid);
						}
					} else if (xi>x && yi<y) {
						for (int p=5; p<10; p++) {
							brain.grids[x][y].grid[p][9-p]+=100;
							spread(p, 9-p, brain.grids[x][y].grid);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi][yi].grid[p][9-p]+=100;
							spread(p, 9-p, brain.grids[xi][yi].grid);
						}
					} else if (xi<x && yi>y) {
						for (int p=5; p<10; p++) {
							brain.grids[x][y].grid[9-p][p]+=100;
							spread(9-p, p, brain.grids[x][y].grid);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi][yi].grid[9-p][p]+=100;
							spread(9-p, p, brain.grids[xi][yi].grid);
						}
					} else if (xi<x && yi==y) {
						for (int p=5; p<10; p++) {
							brain.grids[x][y].grid[9-p][5]+=100;
							spread(9-p, 5, brain.grids[x][y].grid);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi][yi].grid[9-p][5]+=100;
							spread(9-p, 5, brain.grids[xi][yi].grid);
						}
					} else if (xi<x && yi<y) {
						for (int p=5; p<10; p++) {
							brain.grids[x][y].grid[9-p][9-p]+=100;
							spread(9-p, 9-p, brain.grids[x][y].grid);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi][yi].grid[9-p][9-p]+=100;
							spread(9-p, 9-p, brain.grids[xi][yi].grid);
						}
					} else if (xi==x && yi>y) {
						for (int p=5; p<10; p++) {
							brain.grids[x][y].grid[5][p]+=100;
							spread(5, p, brain.grids[x][y].grid);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi][yi].grid[5][p]+=100;
							spread(5, p, brain.grids[xi][yi].grid);
						}
					} else if (xi==x && yi==y) {
						for (int p=5; p<10; p++) {
							brain.grids[x][y].grid[5][5]+=100;
							spread(5, 5, brain.grids[x][y].grid);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi][yi].grid[5][5]+=100;
							spread(5, 5, brain.grids[xi][yi].grid);
						}
					} else if (xi==x && yi<y) {
						for (int p=5; p<10; p++) {
							brain.grids[x][y].grid[5][9-p]+=100;
							spread(5, 9-p, brain.grids[x][y].grid);
						}
						for (int p=0; p<5; p++) {
							brain.grids[xi][yi].grid[5][9-p]+=100;
							spread(5, 9-p, brain.grids[xi][yi].grid);
						}
					} 
			}}
		
		}
		
		//for testing
		for (int x=0; x<10; x++) {
			for (int p=0; p<10; p++) {
				std::cout<<"\n";
				for (int y=0;y<10;y++) {
					for (int q=0; q<10; q++) {
						if (brain.grids[y][x].grid[q][p]>=100) {
							std::cout<<".";
						} else std::cout<<" ";
					}
					
				}
			
			}
		
		}
		
		//run A*
		Grid::Location locs[10][10];
		Grid::Location start= map.getStart();
		int pm[10][10]={{0}};

		for (int m=0; m<10; m++) {
	  	for (int n=0; n<10; n++) {
	    	locs[m][n] = make_tuple(m,n);
	    	if (map2.lookForObstacles(m,n)!='\0') {
	    		pm[m][n] = 0;
	    	} else {pm[m][n]=100;}

	    	if (map.lookForObstacles(m,n)!='\0') {
	    		pm[m][n] = 10;
	    	} 
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
				brain.route.push_back(astarsearch(grid, start, goal, pm));
				start=goal;
	    	} 
	    	
	 		}
		}	
		
		for (auto &nextRoute: brain.route) {
		for (auto &pos:nextRoute) {
			int x;
			int y;
			tie (x,y) = pos;
			std::cout<<"x= "<<x<<" y= "<<y<<"\n";
		}
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
	brain.stop(infos);
	
	
	if(pthread_join(brainthread, NULL)) {
			fprintf(stderr, "Error joining thread\n");
			return 2;
	}
	
	pthread_mutex_destroy(&mutex);
	
}
