#include <array>
#include <vector>

#include "../KDTree.h"

using namespace std;

vector<array<double, 3>> points = {
  {0, 0, 0},
  {0, 1, 0},
  {0.4, 0, 1},
  {1, 0, 0},
  {1, 1.5, 0},
  {999, 999, -0001111}
};

vector<array<double, 3>> testpoints = {
  {3, -1, 2},
  {3, -1, 0},
  {0, 0, 0},
  {0, 0.5, 0},
  {-3, -1, -7}
};


int main() {

  KDTree<3> kdtree;
  kdtree.insert({1, 3, 4});
  for (auto &point : points) {
    kdtree.insert(point);
  }

  kdtree.nearest({2, 3, 4}).front().first->getData();
}
