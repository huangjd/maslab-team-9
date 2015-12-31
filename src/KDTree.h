/*
This file is part of ``kdtree'', a library for working with kd-trees.
Copyright (C) 2007-2011 John Tsiombikas <nuclear@member.fsf.org>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/
#ifndef __KDTREE_H__
#define __KDTREE_H__

#include <algorithm>
#include <cmath>

#include "NearestNeighbor.h"

template <size_t Dimension, class Data = void>
class KDTree : public NearestNeighbor<KDTree, Dimension, Data> {
  friend class NearestNeighbor<KDTree, Dimension, Data>;

  struct kdhyperrect {
    std::array<double, Dimension> min, max; /* minimum/maximum coords */

    template <class Container>
    kdhyperrect(const Container& min, const Container& max) {
      for (size_t i = 0; i < Dimension; i++) {
        this->min[i] = min[i];
        this->max[i] = max[i];
        assert(this->min[i] <= this->max[i]);
      }
    }

    kdhyperrect(const kdhyperrect &rect) = default;
    ~kdhyperrect() = default;

    template <class Container>
    void extend(const Container& pos) {
      for (size_t i = 0; i < Dimension; i++) {
        min[i] = std::min(min[i], pos[i]);
        max[i] = std::max(max[i], pos[i]);
      }
    }

    template <class Container>
    double dist_sq(const Container& pos) const {
      double dist = 0;
      for (size_t i = 0; i < Dimension; i++) {
        if (pos[i] < min[i]) {
          dist += sqr(min[i] - pos[i]);
        } else if (pos[i] > max[i]) {
          dist += sqr(pos[i] - max[i]);
        }
      }
      return dist;
    }
  };

public:

  class InternalNodeType;
  using typename NearestNeighbor<KDTree, Dimension, Data>::NNQueryResults;

  class InternalNodeType {

    std::array<double, Dimension> pos;
    size_t dir;
    const Data *data;
    InternalNodeType *left, *right;	/* negative/positive side */

    friend class KDTree;
    template <class Container>
    InternalNodeType* insert_rec(const Container& pos, size_t dir, const Data* data = nullptr) {
      size_t new_dir = (dir + 1) % Dimension;
      if (pos[dir] < this->pos[dir]) {
        if (left) {
          return left->insert_rec(pos, new_dir, data);
        } else {
          return left = new InternalNodeType(pos, new_dir, data);
        }
      } else {
        if (this->right) {
          return right->insert_rec(pos, new_dir, data);
        } else {
          return right = new InternalNodeType(pos, new_dir, data);
        }
      }
    }

    template <class Container>
    InternalNodeType(const Container& pos, size_t dir, const Data* data = nullptr) :
      dir(dir), data(data), left(nullptr), right(nullptr) {
      for (size_t i = 0; i < Dimension; i++) {
        this->pos[i] = pos[i];
      }
    }

    ~InternalNodeType() {
      delete left;
      delete right;
    }

    template <class Container>
    void nearest_i(const Container& pos, InternalNodeType** const result, double *result_dist_sq, kdhyperrect *rect) {
      InternalNodeType *nearer_subtree, *farther_subtree;
      double *nearer_hyperrect_coord, *farther_hyperrect_coord;

      /* Decide whether to go left or right in the tree */
      double dummy = pos[dir] - this->pos[dir];
      if (dummy <= 0) {
        nearer_subtree = this->left;
        farther_subtree = this->right;
        nearer_hyperrect_coord = &rect->max[0] + dir;
        farther_hyperrect_coord = &rect->min[0] + dir;
      } else {
        nearer_subtree = this->right;
        farther_subtree = this->left;
        nearer_hyperrect_coord = &rect->min[0] + dir;
        farther_hyperrect_coord = &rect->max[0] + dir;
      }

      if (nearer_subtree) {
        /* Slice the hyperrect to get the hyperrect of the nearer subtree */
        dummy = *nearer_hyperrect_coord;
        *nearer_hyperrect_coord = this->pos[dir];
        /* Recurse down into nearer subtree */
        nearer_subtree->nearest_i(pos, result, result_dist_sq, rect);
        /* Undo the slice */
        *nearer_hyperrect_coord = dummy;
      }

      /* Check the distance of the point at the current node, compare it
       * with our best so far */
      double dist_sq = 0;
      for(size_t i = 0; i < Dimension; i++) {
        dist_sq += sqr(this->pos[i] - pos[i]);
      }
      if (dist_sq < *result_dist_sq) {
        *result = this;
        *result_dist_sq = dist_sq;
      }

      if (farther_subtree) {
        /* Get the hyperrect of the farther subtree */
        dummy = *farther_hyperrect_coord;
        *farther_hyperrect_coord = this->pos[dir];
        /* Check if we have to recurse down by calculating the closest
         * point of the hyperrect and see if it's closer than our
         * minimum distance in result_dist_sq. */
        if (rect->dist_sq(pos) < *result_dist_sq) {
          /* Recurse down into farther subtree */
          farther_subtree->nearest_i(pos, result, result_dist_sq, rect);
        }
        /* Undo the slice on the hyperrect */
        *farther_hyperrect_coord = dummy;
      }
    }

    template <class Container>
    void find_nearest(const Container& pos, double range, NNQueryResults* list) {

      double dist_sq = 0;
      for(size_t i = 0; i < Dimension; i++) {
        dist_sq += sqr(this->pos[i] - pos[i]);
      }

      if(dist_sq <= sqr(range)) {
        list->insert(this, dist_sq);
      }

      double dx = pos[dir] - this->pos[dir];

      InternalNodeType *temp = (dx <=0.0 ? left : right);
      if (temp) {
        temp->find_nearest(pos, range, list);
      }
      if(std::fabs(dx) < range) {
        temp = (dx <= 0.0 ? right : left);
        if (temp) {
          temp->find_nearest(pos, range, list);
        }
      }
    }

  public:
    const Data* getData() const {
      return data;
    }

    template <class Container>
    void getPosition(Container *result) const {
      *result = Container(pos.begin(), pos.end());
    }

    std::array<double, Dimension> getPosition() const {
      return pos;
    }

    double getPosition(size_t dim) const {
      assert(dim < Dimension);
      return pos[dim];
    }
  };

  KDTree() : root(nullptr), rect(nullptr) {
  }

  ~KDTree() {
    clear();
  }

  void clear() {
    delete root;
    root = nullptr;
    delete rect;
    rect = nullptr;
  }

private:
  InternalNodeType *root;
  kdhyperrect *rect;

  /* insert a node, specifying its position, and optional data */
  template <class Container>
  void insert_impl(const Container& pos, const Data* data = 0) {
    assert(Dimension == pos.size());
    if (rect) {
      rect->extend(pos);
    } else {
      rect = new kdhyperrect(pos, pos);
    }

    if (root) {
      root->insert_rec(pos, 0, data);
    } else {
      root = new InternalNodeType(pos, 0, data);
    }
  }

  /* Find the nearest node from a given point.
   *
   * This function returns a pointer to a result set with at most one element.
   */
  template <class Container>
  NNQueryResults nearest_impl(const Container& pos) const {
    assert(Dimension == pos.size());
    NNQueryResults rset;

    /* Our first guesstimate is the root node */
    InternalNodeType *result = root;

    if (!root) {
      return rset;
    }

    double dist_sq = 0;
    for (int i = 0; i < Dimension; i++) {
      dist_sq += sqr(result->pos[i] - pos[i]);
    }

    /* Search for the nearest neighbour recursively */
    kdhyperrect rect(*this->rect);
    root->nearest_i(pos, &result, &dist_sq, &rect);

    /* Store the result */
    if (result) {
      rset.insert(result, dist_sq);
    }
    return rset;
  }

  /* Find the N nearest nodes from a given point.
   *
   * This function returns a pointer to a result set, with at most N elements,
   * which can be manipulated with the kd_res_* functions.
   * The returned pointer can be null as an indication of an error. Otherwise
   * a valid result set is always returned which may contain 0 or more elements.
   * The result set must be deallocated with kd_res_free after use.
   */
  /*
  kdres *nearest_n(const double *pos, int num);
  kdres *nearest_n(double x, double y, double z);
  */

  /* Find any nearest nodes from a given point within a range.
   *
   * This function returns a pointer to a result set, which can be manipulated
   * by the kd_res_* functions.
   * The returned pointer can be null as an indication of an error. Otherwise
   * a valid result set is always returned which may contain 0 or more elements.
   * The result set must be deallocated with kd_res_free after use.
   */
  template <class Container>
  NNQueryResults nearest_range_impl(const Container& pos, double range) const {
    assert(Dimension == pos.size());
    NNQueryResults rset;

    if (!root) {
      return rset;
    }

    root->find_nearest(pos, range, &rset);
    std::sort(rset.begin(), rset.end(), [](const typename NNQueryResults::value_type &a, const typename NNQueryResults::value_type &b)->bool {
      return a.second < b.second;
    });
    return rset;
  }
};

#endif //__KDTREE_H__
