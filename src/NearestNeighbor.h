#ifndef __NEAREST_NEIGHBOR_H__
#define __NEAREST_NEIGHBOR_H__

#include <array>
#include <cassert>
#include <iterator>
#include <utility>
#include <vector>

#include "Common.h"
#include "ConfigParam.h"

// Non-virtual interface class, need to implement insert, insert(multiple ?), erase, clear, nearest, nearest_n(?), and nearest_range.
template <template<size_t, class> class Implementation, size_t Dimension, class Data = void>
class NearestNeighbor {
  typedef Implementation<Dimension, Data> Impl;
public:

  class NNQueryResults : public std::vector<std::pair<const typename Impl::InternalNodeType*, double>> {
    friend class Implementation<Dimension, Data>;

    void insert(const typename Impl::InternalNodeType *item, double dist_sq) {
      this->push_back(std::pair<const typename Impl::InternalNodeType*, double>(item, dist_sq));
    }
  };

  void init();
  void clear();

  void insert(const std::vector<double>& coord, const Data* data = 0) {
    static_cast<Impl*>(this)->insert_impl(coord, data);
  }

  void insert(const std::array<double, Dimension>& coord, const Data* data = 0) {
    static_cast<Impl*>(this)->insert_impl(coord, data);
  }

  void insert(const std::initializer_list<double>& coord, const Data* data = 0) {
    static_cast<Impl*>(this)->insert_impl(std::vector<double>(coord), data);
  }

  void insert(const double coord[], const Data* data = 0) {
    std::array<double, Dimension> temp;
    for (size_t i = 0; i < Dimension; i++) {
      temp[i] = coord[i];
    }
    static_cast<Impl*>(this)->insert_impl(temp, data);
  }

  template <template<class> class OuterContainer, class ElementType> // Aggregate insert
  void insert_n(const OuterContainer<ElementType> &val) {
     static_cast<Impl*>(this)->insert_impl(val);
  }

  bool erase(const std::vector<double>& coord) {
    return static_cast<Impl*>(this)->erase_impl(coord);
  }

  bool erase(const std::array<double, Dimension>& coord) {
    return static_cast<Impl*>(this)->erase_impl(coord);
  }

  bool earse(const std::initializer_list<double>& coord) {
    return static_cast<Impl*>(this)->erase_impl(std::vector<double>(coord));
  }

  bool erase(const double coord[]) {
    std::array<double, Dimension> temp;
    for (size_t i = 0; i < Dimension; i++) {
      temp[i] = coord[i];
    }
    return static_cast<Impl*>(this)->erase_impl(temp);
  }

  NNQueryResults nearest(const std::vector<double>& coord) const {
    return static_cast<const Impl*>(this)->nearest_impl(coord);
  }

  NNQueryResults nearest(const std::array<double, Dimension>& coord) const {
    return static_cast<const Impl*>(this)->nearest_impl(coord);
  }

  NNQueryResults nearest(const std::initializer_list<double>& coord) const {
    return static_cast<const Impl*>(this)->nearest_impl(std::vector<double>(coord));
  }

  NNQueryResults nearest(const double coord[]) const {
    std::array<double, Dimension> temp;
    for (size_t i = 0; i < Dimension; i++) {
      temp[i] = coord[i];
    }
    return static_cast<const Impl*>(this)->nearest_impl(temp);
  }

  NNQueryResults nearest_n(const std::vector<double>& coord, size_t n) const {
    return static_cast<const Impl*>(this)->nearest_n_impl(coord, n);
  }

  NNQueryResults nearest_n(const std::array<double, Dimension>& coord, size_t n) const {
    return static_cast<const Impl*>(this)->nearest_n_impl(coord, n);
  }

  NNQueryResults nearest_n(const std::initializer_list<double>& coord, size_t n) const {
    return static_cast<const Impl*>(this)->nearest_n_impl(std::vector<double>(coord), n);
  }

  NNQueryResults nearest_n(const double coord[], size_t n) const {
    std::array<double, Dimension> temp;
    for (size_t i = 0; i < Dimension; i++) {
      temp[i] = coord[i];
    }
    return static_cast<const Impl*>(this)->nearest_n_impl(temp, n);
  }

  NNQueryResults nearest_range(const std::vector<double>& coord, double range) const {
    return static_cast<const Impl*>(this)->nearest_range_impl(coord, range);
  }

  NNQueryResults nearest_range(const std::array<double, Dimension>& coord, double range) const {
    return static_cast<const Impl*>(this)->nearest_range_impl(coord, range);
  }

  NNQueryResults nearest_range(const std::initializer_list<double>& coord, double range) const {
    return static_cast<const Impl*>(this)->nearest_range_impl(std::vector<double>(coord), range);
  }

  NNQueryResults nearest_range(const double coord[], double range) const {
    std::array<double, Dimension> temp;
    for (size_t i = 0; i < Dimension; i++) {
      temp[i] = coord[i];
    }
    return static_cast<const Impl*>(this)->nearest_range_impl(temp, range);
  }

  template <class Container>
  NNQueryResults nearest_range_sq(const Container& coord, double range_sq) const {
    return nearest_range(coord, sqrt(range_sq));
  }

  static constexpr size_t dimension() {
    return Dimension;
  }

private:
  double epsilon = NearestNeighborConfigParamDefaultValue.EPSILON;
  size_t layer = NearestNeighborConfigParamDefaultValue.LAYER;
};

#endif //__NEAREST_NEIGHBOR_H__
