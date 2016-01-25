/*
 Sample code from http://www.redblobgames.com/pathfinding/
 Copyright 2014 Red Blob Games <redblobgames@gmail.com>
 License: Apache v2.0 <http://www.apache.org/licenses/LICENSE-2.0.html>
*/

//include time out stuff
//start in the middle of the map
//one of the problems is with currentBounds.
//I can set it really big but it has to match up with the probabilitymap array 

#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <tuple>
#include <algorithm>

using std::unordered_map;
using std::unordered_set;
using std::array;
using std::vector;
using std::queue;
using std::priority_queue;
using std::pair;
using std::tuple;
using std::tie;
using std::string;
using std::make_tuple;
using std::get;
using std::size_t;
using std::string;

struct Grid {
 //needs to match up
  static const int currentBounds = 30;
  typedef tuple<int,int> Location;
  Location (&allLocations)[currentBounds][currentBounds]; 
  
  //for testing
  string movementArray[currentBounds][currentBounds];
  Grid(Location (&locs)[currentBounds][currentBounds]): allLocations(locs) {}//difference between array<int> and int[]?
  inline bool in_bounds(Location id) const {
  	int x, y;
  	tie (x, y) = id;
  	if (x>= currentBounds | y>=currentBounds| x<0 | y<0) {
  	return false;} else {return true;} 
  } 
  vector<Location> neighbors(Location id) const {
    int x, y;
    tie (x, y) = id;
    vector<Location> results;
    int dirs[2] = {-1, 1};
    for (int dirx: dirs) {
    	Location next(x + dirx, y);
      		if (in_bounds(next)) {
        		results.push_back(next);
        	}
    }
    for (int diry: dirs) {
    	Location next(x, y + diry);
      		if (in_bounds(next)) {
        		results.push_back(next);
        	}
    }
    std::reverse(results.begin(), results.end());
    return results;
  }
  
  //for testing
  void addGridMovement(Location from, Location to);
  void drawFinalGrid() {
  /**std::cout<< "entire array" << std::endl;
  for (auto &strarray: movementArray) {
  	for (auto &str: strarray) {
  	if (str=="") {std::cout << "empty ";}
  	std::cout << str << std::endl;
  	}
  }*/
 	std::cout << "drawing final grid" << std::endl;
    	int i = 0;
    	int j = 0;
  		while (i <currentBounds) {
  			std::cout <<  "  "<< i+1 << "-" ;
  			++i;
  		}
  
  		while (j< currentBounds) {
  			int k = 0;
  			std::cout << "\n";
  			std::cout << j+1 <<"|" ;
  			while (k<currentBounds) {
  				if (movementArray[k][j]!="") {
  					std::cout << " " << movementArray[k][j] << "  ";
  				} else {
  					std::cout << " .  "  ;
  				}
  				++k;
  			}
  			++j;
  		}
  	std::cout << "\n";
 } 
};

namespace std{
typedef typename Grid::Location Location;
template <> struct std::hash<Location> {
    std::size_t operator()(const Location k) const
    {
    int x, y;
	tie (x, y) = k;
      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:

      return x*524287+y*37;
    }
  };
}

//for testing
void Grid::addGridMovement(Location from, Location to) {
  	int x1, y1, x2, y2;
  	tie (x1, y1) = from;
  	tie (x2, y2) = to;
	std::cout << "from "<< x1 << ", " << y1 << " to " << x2 << ", "<< y2 <<"\t";
	
  	if (x1==x2) {
  		switch (y2-y1) {
  		case 1: movementArray[x1][y1]="\u2193"; std::cout << " \u2193" << std::endl; break;
  		case -1: movementArray[x1][y1]="\u2191"; std::cout << " \u2191" << std::endl; break;
  		}
  	} else if (y1==y2) {
  		switch (x2-x1) {
  		case 1: movementArray[x1][y1]="\u2192"; std::cout <<  " \u2192" << std::endl; break;
  		case -1: movementArray[x1][y1]="\u2190"; std::cout <<  " \u2190" << std::endl; break;
  		} 
  		
  	}
  	
  }
  
template<typename Location>
vector<Location> reconstruct_path(Grid grid, Location start, Location goal, unordered_map<Location, Location> &came_from) {
  vector<Location> path;
  Location current = goal;
  path.push_back(current);
  while (current != start) {
    current = came_from[current];
    path.push_back(current);
  }
  std::reverse(path.begin(), path.end());
  for (int q=0; q<path.size()-1; q++) {
  	 std::cout << "added movement: ";
     grid.addGridMovement(path.at(q), path.at(q+1));
  }
  grid.drawFinalGrid();
  
  return path;
}

inline int heuristic(Grid::Location a, Grid::Location b) {
  int x1, y1, x2, y2;
  tie (x1, y1) = a;
  tie (x2, y2) = b;
  return abs(x1 - x2) + abs(y1 - y2); //edit
}

template<typename T, typename Number=int>
struct PriorityQueue {
  typedef pair<Number, T> PQElement;
  priority_queue<PQElement, vector<PQElement>, 
                 std::greater<PQElement>> elements;

  inline bool empty() const { return elements.empty(); }

  inline void put(T item, Number priority) {
    elements.emplace(priority, item);
  }

  inline T get() {
    T best_item = elements.top().second;
    elements.pop();
    return best_item;
  }
};

vector<Grid::Location> a_star_search
  (Grid grid,
   typename Grid::Location start,
   typename Grid::Location goal,
   int probabilityMap[30][30] //bounds must match
   ) {
  typedef typename Grid::Location Location;
  unordered_map<Location, Location> came_from;
  unordered_map<Location, int> cost_so_far;
  PriorityQueue <Location> frontier;
  frontier.put(start, 0);

  came_from[start] = start;
  cost_so_far[start] = 0;
  
 while (!frontier.empty()) {
    auto current = frontier.get();

    if (current == goal) {
      vector<Location> route = reconstruct_path(grid, start, goal, came_from);
      
      //for testing
      std::cout << "done" << std::endl;
      return route;
      
    } else {
    for (auto next : grid.neighbors(current)) {
      int x,y;
      tie (x,y) = next;
      int new_cost = cost_so_far[current] + probabilityMap[x][y];
      
      //for testing
      //std::cout << "obstacle probability: " << probabilityMap[x][y]<< std::endl;
      
      if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
        cost_so_far[next] = new_cost;
        int priority = new_cost + heuristic(next, goal);
        frontier.put(next, priority);
        came_from[next] = current;

      }}
    }
  }
  
}

/**int main() {
	int cb = 3;//Grid::currentBounds
	//tried setting current bounds to the one in grid but it doesn't work
	Grid::Location locs[3][3];
	Grid::Location start= make_tuple (0,0);
	Grid::Location goal= make_tuple (0,2);
	int pm[3][3]; //has to be a parameter later on
	for (int m=0; m<cb; m++) {
	  for (int n=0; n<cb; n++) {
	    locs[m][n] = make_tuple(m,n);
	    pm[m][n] = 0;
	  }
	}	
	pm[0][1]=100; //obstacle on (0,1)
	Grid grid (locs);
	vector<Grid::Location> route;
	route=a_star_search(grid, start, goal, pm);
}*/

/**
std::basic_iostream<char>::basic_ostream& operator<<(std::basic_iostream<char>::basic_ostream& out, tuple<int,int> loc) {
  int x, y;
  tie (x, y) = loc;
  out << "(" << x << "," << y << ")";
  return out;
}
*/
