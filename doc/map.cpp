//map creation yay, follows their specs


#include <iostream>
#include <string>
#include <regex>
#include <fstream>

using std::string;
using std::regex;
using std::tuple;
using std::vector;


class Stack {
	private:
		int x;
		int y;
		char cube1Clr;
		char cube2Clr;
		char cube3Clr;
	
	public:
		Stack(void): x(-1), y(-1) {};
		int getPosX () {return x;}
		int getPosY () {return y;}
		char getTopBlock() {return cube1Clr;}
		char getMidBlock() {return cube2Clr;}
		char getBotBlock() {return cube3Clr;}
		Stack(vector<int> values, vector<char> colors): x(values.at(0)), y(values.at(1)), cube1Clr(colors.at(0)), cube2Clr(colors.at(1)), cube3Clr(colors.at(2)) {};
	
};

struct Map {
//map with platform, stacks, startLocation, walls
	private:
		tuple<int, int> startLocation;
//bounds have to match with the stuff on the other program
		char grid[30][30];
		Stack gridStacks[30][30];

	public:
	Map (void) {};

//gets startlocation
	tuple<int, int> getStart () {
		return startLocation;
	}

//finds walls, platforms
	char lookForObstacles(int x, int y) {
	if (grid[x][y]!='\0'){
	//std::cout<<"look for obstacles"<<x<<"\t"<<y<<"\t"<<grid[x][y]<<std::endl;
	}
		return grid[x][y]; 
	}

	void setStart(int x, int y) {
		startLocation=std::make_tuple(x,y);
	}
//finds stacks
	Stack lookForStacks(int x, int y) {
	//std::cout<<"look for stacks"<<x<<"\t"<<y<<"\t"<<gridStacks[x][y].getPosX()<<std::endl;
		return gridStacks[x][y];
	}

//makes everything from text file
	void construct(char item, string input) {
		vector<int> nums;
		vector<char> chars;
		string info=input;
		std::smatch capture;
		int length=info.length();
		regex parser ("(.*?),");
		int iteration=0;
		if (item!='S') {
			while (regex_search(info, capture, parser)==true) {
				int num = atoi(capture.str(iteration).c_str()); 
				nums.push_back(num);
				info=info.substr(capture.position(iteration)+2);
			}
			int num2 = atoi(info.c_str()); 
			nums.push_back(num2);
		} else if (item=='S') { 							//just realized I didnt actually need to use regex derp
			string units[2];
			for (int i=0; i<2; i++) {
				units[i]=info.substr(0, 1); //cjanfe
				std::cout<<units[i]<<std::endl;
				info=info.substr(2);
				int num = atoi(units[i].c_str()); 
				nums.push_back(num);
			}

			char *type= new char[info.length() + 1];
        	std::strcpy(type,info.c_str());
			chars.push_back(type[0]);
			chars.push_back(type[2]);
			chars.push_back(type[4]);
		}
		switch (item) {
			case 'L': {grid[nums.at(0)][nums.at(1)]='L'; setStart(nums.at(0), nums.at(1));}break;
			case 'S': {Stack *stack = new Stack(nums, chars); gridStacks[nums.at(0)][nums.at(1)] =  *stack;} break;
			case 'P': grid[nums.at(0)][nums.at(1)]= 'P'; grid[nums.at(2)][nums.at(3)]='P'; break;
			case 'W': grid[nums.at(0)][nums.at(1)]='W'; grid[nums.at(2)][nums.at(3)]='W'; break;	
		}

	/**
	std::cout << "full array= ";
	for (int a=0; a<10; a++) {
	for (int b=0; b<10; b++) {
		std::cout<< a << " " << b<< " " << grid[a][b] << std::endl;
		}
	}
	std::cout << "gridstacks \n";
	for (int a=0; a<10; a++) {
	for (int b=0; b<10; b++) {
	bool boolean=false;
		if (gridStacks[a][b].getPosX()!=-1) {boolean=true;}
		std::cout<< a << " " << b<< " " << boolean << std::endl;
		}
	}*/
	}
};


