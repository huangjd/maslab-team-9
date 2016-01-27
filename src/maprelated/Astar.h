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
  static const int currentBounds = 10;
  typedef tuple<int,int> Location;
  Location (&allLocations)[currentBounds][currentBounds]; 
  
  //for testing
  string movementArray[currentBounds][currentBounds];
  Grid(Location (&locs)[currentBounds][currentBounds]): allLocations(locs) {}
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
void Grid::addGridMovement(Location from, Location to) {}
  
template<typename Location>
vector<Location> reconstruct_path(Grid grid, Location start, Location goal, unordered_map<Location, Location> &came_from) {}

inline int heuristic(Grid::Location a, Grid::Location b) {}

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
   int probabilityMap[10][10] //bounds must match
   ) {}
  
