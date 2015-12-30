#ifndef __NEAREST_NEIGHBOR_H__
#define __NEAREST_NEIGHBOR_H__

#include <array>
#include <cassert>
#include <iterator>
#include <utility>
#include <vector>

#include "Common.h"
#include "ConfigParam.h"

#define CONTAINER_REQUIRES_RANDOM_ACCESS_ITERATOR(Container) typename = typename std::enable_if<\
  std::is_base_of<std::array<typename Container::value_type, Dimension>, Container>::value || \
  std::is_base_of<std::iterator<std::random_access_iterator_tag, typename Container::value_type>, \
    typename Container::iterator>::value\
  >::type


// Non-virtual interface class, need to implement insert, insert(multiple ?), erase, clear, nearest, nearest_n(?), and nearest_range.
template <template<size_t, class> class Implementation, size_t Dimension, class Data = void>
class NearestNeighbor {
  typedef Implementation<Dimension, Data> Impl;
public:

  class NNQueryResults : public std::vector<std::pair<const typename Impl::InternalNodeType*, const double>> {
    friend class Implementation<Dimension, Data>;

    void insert(const typename Impl::InternalNodeType *item, double dist_sq) {
      this->push_back(std::pair<const typename Impl::InternalNodeType*, const double>(item, dist_sq));
    }
  };

  void clear();

  template <class Container, CONTAINER_REQUIRES_RANDOM_ACCESS_ITERATOR(Container)>
  void insert(const Container& coord, const Data* data = 0) {
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

  template <class Container, CONTAINER_REQUIRES_RANDOM_ACCESS_ITERATOR(Container)>
  bool erase(const Container& coord) {
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

  template <class Container, CONTAINER_REQUIRES_RANDOM_ACCESS_ITERATOR(Container)>
  NNQueryResults nearest(const Container& coord) const {
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

  template <class Container, CONTAINER_REQUIRES_RANDOM_ACCESS_ITERATOR(Container)>
  NNQueryResults nearest_n(const Container& coord, size_t n) const {
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

  template <class Container, CONTAINER_REQUIRES_RANDOM_ACCESS_ITERATOR(Container)>
  NNQueryResults nearest_range(const Container& coord, double range) const {
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

  size_t dimension() const {
    return Dimension;
  }

private:
  double epsilon = NearestNeighborConfigParamDefaultValue.EPSILON;
  size_t layer = NearestNeighborConfigParamDefaultValue.LAYER;
};

#undef CONTAINER_REQUIRES_RANDOM_ACCESS_ITERATOR

#endif //__NEAREST_NEIGHBOR_H__
