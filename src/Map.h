#ifndef __MAP_H__
#define __MAP_H__

#include "ConfigParam.h"
#include "State.h"
#include "Array2D.h"

class DynamicsControlQueue;

class Map {

  class Objective {
    bool visited;
  };

  Array2D<Objective> objectives;

  class Grid {
    int value;
  };

  Array2D<Grid> grid;

public:

  Map(const MapParam &param) :
    objectives(MapParam::LATTICE_NUM, MapParam::LATTICE_NUM),
    grid(MapParam::GRID_SIZE, MapParam::GRID_SIZE)
    {
  }

  void routine(const Position &currentPos);

  int estimateCost(size_t x, size_t y);

  double estimateCostToObjective(int x, int y);
  void computeDynamics(DynamicsControlQueue *queue);

};

#endif // __MAP_H__
