/*
 Sample code from http://www.redblobgames.com/pathfinding/
 Copyright 2014 Red Blob Games <redblobgames@gmail.com>
 License: Apache v2.0 <http://www.apache.org/licenses/LICENSE-2.0.html>
*/

#include "Astar.h"
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
  
  return path;
}

inline int heuristic(Grid::Location a, Grid::Location b) {
  int x1, y1, x2, y2;
  tie (x1, y1) = a;
  tie (x2, y2) = b;
  return abs(x1 - x2) + abs(y1 - y2);
}

vector<Grid::Location> astarsearch
  (Grid grid,
   Grid::Location start,
    Grid::Location goal,
   int probabilityMap[100][100]
   ) 
   {
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
      return route;
      
    } else {
    for (auto next : grid.neighbors(current)) {
      int x,y;
      tie (x,y) = next;
      int new_cost = cost_so_far[current] + probabilityMap[x][y];
      
      if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
        cost_so_far[next] = new_cost;
        int priority = new_cost + heuristic(next, goal);
        frontier.put(next, priority);
        came_from[next] = current;

      }}
    }
  }
  
}
