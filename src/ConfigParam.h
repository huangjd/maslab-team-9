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

#endif //__CONFIG_PARAM_H__
