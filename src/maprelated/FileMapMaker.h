#ifndef _FILEMAPMAKER
#define _FILEMAPMAKER

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
	public:
		tuple<int, int> startLocation;
		char grid[10][10]={{0}};
		Stack gridStacks[10][10];
		Map (void) {};
		tuple<int, int> gridLinks[10][10];

//gets startlocation
	tuple<int, int> getStart () {
		return startLocation;
	}

//finds walls, platforms
	char lookForObstacles(int x, int y) {
		return grid[x][y]; 
	}

	void setStart(int x, int y) {
		startLocation=std::make_tuple(x,y);
	}
//finds stacks
	Stack lookForStacks(int x, int y) {
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
		} else if (item=='S') { 							
			string units[2];
			for (int i=0; i<2; i++) {
				units[i]=info.substr(0, 1); 
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
			case 'P': {
				grid[nums.at(0)][nums.at(1)]= 'P'; grid[nums.at(2)][nums.at(3)]='P'; 
				gridLinks[nums.at(0)][nums.at(1)]= std::make_tuple(nums.at(2), nums.at(3));
					
			} break;
			case 'W': {
				grid[nums.at(0)][nums.at(1)]= 'W'; grid[nums.at(2)][nums.at(3)]='W'; 
				gridLinks[nums.at(0)][nums.at(1)]= std::make_tuple(nums.at(2), nums.at(3));
				//will not work if stuff in file is not unique
				
			} break;	
		}

	}
	
	
	
};

#endif

