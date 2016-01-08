#ifndef __CONFIG_PARAM_H__
#define __CONFIG_PARAM_H__

struct NearestNeighborConfigParam {
  double EPSILON;
  unsigned int LAYER;

  constexpr NearestNeighborConfigParam(NearestNeighborConfigParam &) = default;
  constexpr NearestNeighborConfigParam(double epsilon, unsigned int layer) :
    EPSILON(epsilon), LAYER(layer) {
  }
};

static NearestNeighborConfigParam
  NearestNeighborConfigParamDefaultValue(1e-7, 10u);


struct MapParam {
  static constexpr int GRID_SIZE = 1000;
  static constexpr int LATTICE_STRIDE = 100;

  static constexpr int LATTICE_NUM = (GRID_SIZE / LATTICE_STRIDE) | 1;
};

#endif //__CONFIG_PARAM_H__
