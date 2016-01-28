#include <pthread.h>
#include <stdio.h>
#include "Astar.h"
#include <iostream>
#include "FileMapMaker.h"
#include "../app/StackGet.h"

using std::tuple;
using std::vector;
pthread_mutex_t mutex;


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
		//std::cout<<"start x= "<<xlocation<<" start y= "<<ylocation<<std::endl;
	};
	
	//can walk off grid rn
	void moveU () {
		ylocation+=0.1;
		//std::cout<<"new y= "<<ylocation<<std::endl;
	}
	
	void moveL () {
		xlocation-=0.1;
		//std::cout<<"new x= "<<xlocation<<std::endl;
	}
	
	void moveR () {
		xlocation+=0.1;
		//std::cout<<"new x= "<<xlocation<<std::endl;
	}
	
	void moveD () {
		ylocation-=0.1;
		//std::cout<<"new y= "<<ylocation<<std::endl;
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
	
	//combine this with move later
	int getDirectionFacing() {
		return directionFacing;
	}
	
	tuple<double, double> getLocation() {
		//std::cout<<"getlocation= "<<xlocation<<", "<<ylocation<<std::endl;
		return std::make_tuple(xlocation, ylocation);
	}
	
	//currently makes all walls 5x5 in the 100x100 grid
	//returns a vector of distance in the 100x100 grid and obstacle prob
	vector<tuple<int, int>> getIRDataU() {
		//std::cout<<"U IR Data\n";
		vector<tuple<int,int>> IRDataU;
		IRDataU.push_back(std::make_tuple(1, 0));
		/**tuple<int, int> data;
		int i=1;
		int y=(int)(ylocation+(i/10));
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
		}*/
		return IRDataU;
	}
	
	vector<tuple<int, int>> getIRDataL() {
		vector<tuple<int,int>> IRDataL;
		//std::cout<<"L IR Data\n";
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
		IRDataL.push_back(std::make_tuple(1, 0));
		return IRDataL;
	}
	
	vector<tuple<int, int>> getIRDataR() {
		vector<tuple<int,int>> IRDataR;
		//std::cout<<"R IR Data\n";
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
		IRDataR.push_back(std::make_tuple(1, 0));
		return IRDataR;
	}
	
	vector<tuple<int, int>> getIRDataD() {
		vector<tuple<int,int>> IRDataD;
		//std::cout<<"R IR Data\n";
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
			//std::cout<<"x= "<<xprecised<<" y= "<<yprecised<<" and "<<"x "<<x<<" y "<<y<<"x: "<<xprecise<<"y: "<<yprecise<<std::endl;
			
			for (tuple<int, int> tup: infop.IRDataU) {
				int d;
				int p;
				tie(d, p) = tup;
				//std::cout<<"yprecise= "<<yprecise<<std::endl;
				if (yprecise+d>9) {
					int innery=y+1;
					int newVal=yprecise+d-10;
					//std::cout<<"reached this if"<<std::endl;
					grids[x][innery].grid[xprecise][newVal]+=p;
					//std::cout<<"x= "<<x<<xprecise+d<<" y= "<<innery<<yprecise<<"\tirdatar= "<<grids[x][innery].grid[newVal][yprecise]+p<<std::endl;
				} else {
					//std::cout<<"HERE: "<<y<<" "<<yprecise+d<<std::endl;
					//std::cout<<"x= "<<x<<xprecise<<" y= "<<y<<yprecise+d<<"\tirdatau= "<<grids[x][y].grid[xprecise][yprecise+d]+p<<std::endl;
					grids[x][y].grid[xprecise][yprecise+d]+=p;
					//std::cout<<"problem spot: "<<grids[0][1].grid[0][1]<<std::endl;
				}
				
			}
			//std::cout<<infop.IRDataU.size()<<std::endl;
			//std::cout<<"done with R"<<std::endl;
			for (tuple<int, int> tup: infop.IRDataR) {
				int d;
				int p;
				tie(d, p) = tup;
				if (xprecise+d>9) {
					int innerx;
					innerx=x+1;
					int newVal=(xprecise+d)-10;
					grids[innerx][y].grid[newVal][yprecise]+=p;
					//std::cout<<"x= "<<innerx<<newVal<<" y= "<<y<<yprecise<<"\tirdatar= "<<grids[innerx][y].grid[newVal][yprecise]+p<<std::endl;
				} else {
					//std::cout<<"x= "<<x<<xprecise+d<<" y= "<<y<<yprecise<<"\tirdatar= "<<grids[x][y].grid[(xprecise+d)][yprecise]+p<<std::endl;
					grids[x][y].grid[xprecise+d][yprecise]+=p;
				}
			}
			//std::cout<<"done with U"<<std::endl;
			for (tuple<int, int> tup: infop.IRDataL) {
				int d;
				int p;
				tie(d, p) = tup;
				if (xprecise-d<0) {
					int innerx=x-1;
					int newVal=xprecise-d+10;
					grids[innerx][y].grid[newVal][yprecise]+=p;
					//std::cout<<"x= "<<innerx<<newVal<<" y= "<<y<<yprecise<<"\tirdatar= "<<grids[innerx][y].grid[newVal][yprecise]+p<<std::endl;
				} else {
					//std::cout<<"x= "<<x<<(xprecise-d)<<" y= "<<y<<yprecise<<"\tirdatal= "<<grids[x][y].grid[xprecise-d][yprecise]+p<<std::endl;
					grids[x][y].grid[xprecise-d][yprecise]+=p;
				}
				
			}
			//std::cout<<"done with L"<<std::endl;
			for (tuple<int, int> tup: infop.IRDataD) {
				int d;
				int p;
				tie(d, p) = tup;
				//std::cout<<"yprecise= "<<yprecise<<std::endl;
				if (yprecise-d<0) {
					int innery=y-1;
					int newVal=yprecise-d+10;
					//std::cout<<"reached this if"<<std::endl;
					grids[x][innery].grid[xprecise][newVal]+=p;
					//std::cout<<"x= "<<x<<xprecise+d<<" y= "<<innery<<yprecise<<"\tirdatar= "<<grids[x][innery].grid[newVal][yprecise]+p<<std::endl;
				} else {
					//std::cout<<"HERE: "<<y<<" "<<yprecise+d<<std::endl;
					//std::cout<<"x= "<<x<<xprecise<<" y= "<<y<<yprecise+d<<"\tirdatau= "<<grids[x][y].grid[xprecise][yprecise+d]+p<<std::endl;
					grids[x][y].grid[xprecise][yprecise-d]+=p;
					//std::cout<<"problem spot: "<<grids[0][1].grid[0][1]<<std::endl;
				}
				
			}
			
		}
		
		void getCube() {
			//break blocks
			//get cube
			//update count
			std::cout<<"got cube"<<std::endl;
		
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
		
		void path(info &infop) {
			Grid::Location locs[10][10];
			for (int m=0; m<10; m++) {
	  		for (int n=0; n<10; n++) {
	    		locs[m][n] = make_tuple(m,n);
			}	
		}
			Grid grid (locs);
		//this is terrible for mazes but kind of works for the arena
		//first figure out where you can move
		//move until you are free to move in the general direction you are supposed to move
		//identify current location
		//then move from current position to edge of 10x10 grid using astar
		//reset
		
		while (route.size()!=0) {
			vector<tuple<int, int>> nextRoute=route.front();
			//std::cout<<"\n\nNEW STACK!!!"<<"\n\n";
			//getting route to new stack
			for (auto &lroute: route) {
			tuple<int,int> pos=lroute.front();
			int x;
			int y;
			tie (x,y)=pos;
			//std::cout<<"size= "<<route.size()<<" size of smaller= "<<lroute.size()<<"\tx from path= "<<x<<" y from path= "<<y<<std::endl; 
			}
			string toNewGrid;
			
			while (nextRoute.size()!=0) {
			//std::cout<<"\n\nTO NEXT POS IN ROUTE\n\n";
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
				//std::cout<<"CURRENT bigLC: x1= "<<x1<<" y1= "<<y1<<" bigNEXTPOS: x2= "<<x2<<" y2= "<<y2<<std::endl;

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
				//std::cout<<"smallstart= "<<xprecise<<" "<<yprecise<<"\t";
				//std::cout<<"smallgoal= "<<x_x<<" "<<y_y<<std::endl;
				if (start==goal) {
					//just ignore, this is bad practice but I give
				} else {
				//std::cout<<"size of smallRoute rn= "<<smallRoute.size()<<"\n";
				vector<Grid::Location> smallRoute=a_star_search(grid, start, goal, grids[x][y].grid);
				//std::cout<<"size of smallRoute rn= "<<smallRoute.size()<<"\n";
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
				//std::cout<<str<<"\t";
				//std::cout<<"to= "<<x2_<<", "<<y2_<<std::endl;
				/**if (check) {
					std::cout<<"check is true"<<std::endl;
				} else {std::cout<<"check is false"<<std::endl;}*/
		
				do {
					if (pthread_mutex_trylock(&mutex)==0){
					updateGrid(infop);
					if (check) {
						//std::cout<<"checked"<<std::endl;
						str="stop";
						check=false;
					}
					pthread_mutex_unlock(&mutex);
					//getline(std::cin, str);
		
					if (str=="moveU") {
						move(infop, 'U');
						check=true;
						//std::cout<<"check moveU?"<<"\t";
					} else if (str=="moveR") {
						move(infop, 'R');
						check=true;
						//std::cout<<"check moveR?\t";
					} else if (str=="moveL") {
						move(infop, 'L');
						check=true;
						//std::cout<<"check moveL?\t";
					} else if (str=="moveD") {
						move(infop, 'D');
						check=true;
						//std::cout<<"check moveD?\t";
					}
					}
					
				} while (str!="stop"); 
				//std::cout<<"\n";
				}
				} 
				
				//std::cout<<"movementStr= "<<toNewGrid<<"\n";
				//if (check) {std::cout<<"checkis true";} else {std::cout<<"check is false";}
				do{
				if (pthread_mutex_trylock(&mutex)==0){
					updateGrid(infop);
					if (check) {
						toNewGrid="stop";
						check=false;
					}
					pthread_mutex_unlock(&mutex);
					//getline(std::cin, str);
		
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
				//std::cout<<"currentBigPos= "<<xC<<" "<<yC<<"\n";
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
		
		//for testing
		void print() {
		
			int a;
			int b;
			int c;
			int d;
			bool initial=true;
			for (a=0; a<3; a++) {
				for (b=0; b<10; b++) {
					if (initial) {std::cout<<"   ";for (int j=0; j<30; j++){if (j<10) {std::cout<<j<<"   ";} else {std::cout<<j<<"  ";}} initial=false;};
					std::cout<<"\n";
					std::cout<<a<<b<<" ";
					for (c=0; c<3; c++) {
						for (d=0; d<10; d++) {
							//std::cout<<c<<d<<" ";
							std::cout<<grids[c][a].grid[d][b];
							int n=grids[c][a].grid[d][b];
							int length;
							do {
     							++length; 
    							n /= 10;
							} while (n);
							length=4-length;
							while (length>0) {
								std::cout<<" ";
								--length;
							}
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
		Grid::Location locs[10][10];
		Grid::Location start= map.getStart();
		int pm[10][10]={0};

		for (int m=0; m<10; m++) {
	  	for (int n=0; n<10; n++) {
	    	locs[m][n] = make_tuple(m,n);
	    	if (map.lookForObstacles(m,n)!='\0') {
	    		pm[m][n] = 100;
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
				brain.route.push_back(a_star_search(grid, start, goal, pm));
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
		
		for (int pp=0; pp<10; pp++) {
		for (int qq=0; qq<10; qq++) {
			std::cout<<" "<<map.gridLink[pp][qq].startx<<map.gridLink[pp][qq].starty;
		}
		std::cout<<"\n";
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
	brain.print();
	pthread_mutex_destroy(&mutex);
	std::cout << "done program, exiting" << std::endl;
	
}
