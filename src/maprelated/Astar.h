#ifndef _ASTAR
#define _ASTAR
/*
 Sample code from http://www.redblobgames.com/pathfinding/
 Copyright 2014 Red Blob Games <redblobgames@gmail.com>
 License: Apache v2.0 <http://www.apache.org/licenses/LICENSE-2.0.html>
*/

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
  static const int currentBounds = 100;
  typedef tuple<int,int> Location;
  Location (&allLocations)[currentBounds][currentBounds]; 
  

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
};

namespace std{
typedef typename Grid::Location Location;
template <> struct hash<Location> {
    size_t operator()(const Location k) const
    {
    int x, y;
	tie (x, y) = k;
    return x*524287+y*37;
    }
  };
}
  
template<typename Location>
vector<Location> reconstruct_path(Grid grid, Location start, Location goal, unordered_map<Location, Location> &came_from);

inline int heuristic(Grid::Location a, Grid::Location b);

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

vector<Grid::Location> astarsearch(Grid grid, Grid::Location start, Grid::Location goal, int probabilityMap[100][100]);
  
#endif
