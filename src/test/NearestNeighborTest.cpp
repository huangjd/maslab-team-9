#include <algorithm>
#include <array>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <gtest/gtest.h>
#include <gtest/gtest-typed-test.h>

#include "../KDTree.h"

using namespace std;

static const double EPSILON = 1e-7;

static constexpr unsigned int TEST_INSERT =        1u << 0;
static constexpr unsigned int TEST_INSERT_N =      1u << 1;
static constexpr unsigned int TEST_ERASE =         1u << 2;
static constexpr unsigned int TEST_NEAREST =       1u << 3;
static constexpr unsigned int TEST_NEAREST_N =     1u << 4;
static constexpr unsigned int TEST_NEAREST_RANGE = 1u << 5;

static constexpr unsigned int DEFAULT_TESTING_FEATURES = TEST_INSERT | TEST_NEAREST | TEST_NEAREST_RANGE;

class Node {};

typedef ::testing::Types<
  KDTree<2, Node>,
  KDTree<1>,
  KDTree<2>,
  KDTree<3>,
  KDTree<4>
> NearestNeighborImplementations;

typedef ::testing::Types<
  KDTree<3>
> NearestNeighborImplementations3D;


template <typename T>
class NearestNeighborTest : public ::testing::Test {
public:
  template <typename U>
  struct TypeProxy {
    U value;
    TypeProxy() : value() {}
  };

  TypeProxy<T> proxy;
  static constexpr size_t dimension = T::dimension();
};

template <typename T>
class NearestNeighborTest3D : public NearestNeighborTest<T> {
};

TYPED_TEST_CASE(NearestNeighborTest, NearestNeighborImplementations);
TYPED_TEST_CASE(NearestNeighborTest3D, NearestNeighborImplementations3D);

template <unsigned int enabled>
struct TestInsert {
  template <typename Container, template<size_t, class> class Impl, size_t Dimension, class Data = void>
  static bool test(Impl<Dimension, Data> *impl, const Container& container, const Data* d = nullptr) {
    impl->insert(container, d);
    return true;
  }
};

template <>
struct TestInsert<0> {
  template <typename Container, template<size_t, class> class Impl, size_t Dimension, class Data = void>
  static bool test(Impl<Dimension, Data> *impl, const Container& container, const Data* d = nullptr) {
    return false;
  }
};

template <unsigned int enabled>
struct TestInsertN {
  template <typename Container, template<size_t, class> class Impl, size_t Dimension, class Data = void>
  static bool test(Impl<Dimension, Data> *impl, const Container& container) {
    impl->insert_n(container);
    return true;
  }
};

template <>
struct TestInsertN<0> {
  template <typename Container, template<size_t, class> class Impl, size_t Dimension, class Data = void>
  static bool test(Impl<Dimension, Data> *impl, const Container& container) {
    return false;
  }
};

template <unsigned int enabled>
struct TestErase {
  template <typename Container, template<size_t, class> class Impl, size_t Dimension, class Data = void>
  static bool test(Impl<Dimension, Data> *impl, const Container& container) {
    impl->erase(container);
    return true;
  }
};

template <>
struct TestErase<0> {
  template <typename Container, template<size_t, class> class Impl, size_t Dimension, class Data = void>
  static bool test(Impl<Dimension, Data> *impl, const Container& container) {
    return false;
  }
};

template <unsigned int enabled>
struct TestNearest {
  template <typename Container, template<size_t, class> class Impl, size_t Dimension, class Data = void>
  static bool test(Impl<Dimension, Data> *impl, const Container& container) {
    impl->nearest(container);
    return true;
  }
};

template <>
struct TestNearest<0> {
  template <typename Container, template<size_t, class> class Impl, size_t Dimension, class Data = void>
  static bool test(Impl<Dimension, Data> *impl, const Container& container) {
    return false;
  }
};

template <unsigned int enabled>
struct TestNearestN {
  template <typename Container, template<size_t, class> class Impl, size_t Dimension, class Data = void>
  static bool test(Impl<Dimension, Data> *impl, const Container& container, size_t N = 10) {
    impl->nearest_n(container, N);
    return true;
  }
};

template <>
struct TestNearestN<0> {
  template <typename Container, template<size_t, class> class Impl, size_t Dimension, class Data = void>
  static bool test(Impl<Dimension, Data> *impl, const Container& container, size_t N = 10) {
    return false;
  }
};

template <unsigned int enabled>
struct TestNearestRange {
  template <typename Container, template<size_t, class> class Impl, size_t Dimension, class Data = void>
  static bool test(Impl<Dimension, Data> *impl, const Container& container, double range = 10.0) {
    impl->nearest_range(container, range);
    impl->nearest_range_sq(container, range);
    return true;
  }
};

template <>
struct TestNearestRange<0> {
  template <typename Container, template<size_t, class> class Impl, size_t Dimension, class Data = void>
  static bool test(Impl<Dimension, Data> *impl, const Container& container, double range = 10.0) {
    return false;
  }
};

TYPED_TEST(NearestNeighborTest, FeatureTest) {
  struct DummyInitializerList {
    static initializer_list<double> value(size_t dimension) {
      switch (dimension) {
      case 2:
        return {0.0, 0.0};
      case 3:
        return {0.0, 0.0, 0.0};
      case 4:
        return {0.0, 0.0, 0.0, 0.0};
      case 5:
        return {0.0, 0.0, 0.0, 0.0, 0.0};
      case 6:
        return {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
      case 1:
      default:
        return {0.0};
      }
    }
  };

  auto& impl = this->proxy.value;
  constexpr size_t dimension = this->dimension;
  vector<double>                vectorContainer; vectorContainer.resize(dimension);
  array<double, dimension>      arrayContainer;
  initializer_list<double>      initializerContainer(DummyInitializerList::value(dimension));
  double                        cArrayContainer[dimension] = {0.0};

  TestInsert       <DEFAULT_TESTING_FEATURES & TEST_INSERT>::test(&impl, vectorContainer);
  TestInsert       <DEFAULT_TESTING_FEATURES & TEST_INSERT>::test(&impl, arrayContainer);
  TestInsert       <DEFAULT_TESTING_FEATURES & TEST_INSERT>::test(&impl, initializerContainer);
  TestInsert       <DEFAULT_TESTING_FEATURES & TEST_INSERT>::test(&impl, cArrayContainer);

  vector<array<double, dimension>> testInsertNContainer; testInsertNContainer.resize(10);
  TestInsertN      <DEFAULT_TESTING_FEATURES & TEST_INSERT_N>::test(&impl, testInsertNContainer);

  TestErase        <DEFAULT_TESTING_FEATURES & TEST_ERASE>::test(&impl, vectorContainer);
  TestErase        <DEFAULT_TESTING_FEATURES & TEST_ERASE>::test(&impl, arrayContainer);
  TestErase        <DEFAULT_TESTING_FEATURES & TEST_ERASE>::test(&impl, initializerContainer);
  TestErase        <DEFAULT_TESTING_FEATURES & TEST_ERASE>::test(&impl, cArrayContainer);

  impl.insert(vectorContainer);
  TestNearest      <DEFAULT_TESTING_FEATURES & TEST_NEAREST>::test(&impl, vectorContainer);
  TestNearest      <DEFAULT_TESTING_FEATURES & TEST_NEAREST>::test(&impl, arrayContainer);
  TestNearest      <DEFAULT_TESTING_FEATURES & TEST_NEAREST>::test(&impl, initializerContainer);
  TestNearest      <DEFAULT_TESTING_FEATURES & TEST_NEAREST>::test(&impl, cArrayContainer);

  TestNearestN     <DEFAULT_TESTING_FEATURES & TEST_NEAREST_N>::test(&impl, vectorContainer);
  TestNearestN     <DEFAULT_TESTING_FEATURES & TEST_NEAREST_N>::test(&impl, arrayContainer);
  TestNearestN     <DEFAULT_TESTING_FEATURES & TEST_NEAREST_N>::test(&impl, initializerContainer);
  TestNearestN     <DEFAULT_TESTING_FEATURES & TEST_NEAREST_N>::test(&impl, cArrayContainer);

  TestNearestRange <DEFAULT_TESTING_FEATURES & TEST_NEAREST_RANGE>::test(&impl, vectorContainer);
  TestNearestRange <DEFAULT_TESTING_FEATURES & TEST_NEAREST_RANGE>::test(&impl, arrayContainer);
  TestNearestRange <DEFAULT_TESTING_FEATURES & TEST_NEAREST_RANGE>::test(&impl, initializerContainer);
  TestNearestRange <DEFAULT_TESTING_FEATURES & TEST_NEAREST_RANGE>::test(&impl, cArrayContainer);
}

template <size_t Dimension, size_t N = std::numeric_limits<size_t>::max()>
static vector<double> nearestBruteForce(const vector<array<double, Dimension>> &points,
                                        const array<double, Dimension>& testPoint,
                                        double range_sq = -1.0) {
  vector<double> distances;
  for (const auto& point : points) {
    double dist_sq = 0;
    for (size_t i = 0; i < Dimension; i++) {
      dist_sq += sqr(testPoint[i] - point[i]);
    }
    if (range_sq == -1.0 || dist_sq <= range_sq) {
      distances.push_back(dist_sq);
    }
  }
  sort(distances.begin(), distances.end());
  distances.resize(min(distances.size(), N));
  return distances;
}

static void assertVectorEqual(const auto& expected, const auto& actual) {
  ASSERT_EQ(expected.size(), actual.size());
  for (size_t i = 0; i < expected.size(); i++) {
    ASSERT_DOUBLE_EQ(expected[i], actual[i].second);
  }
}

static vector<array<double, 3>> points = {
  {0, 0, 0},
  {0, 1, 0},
  {0.4, 0, 1},
  {1, 0, 0},
  {1, 1.5, 0},
  {999, 999, -0001111}
};

static vector<array<double, 3>> testPoints = {
  {3, -1, 2},
  {3, -1, 0},
  {0, 0, 0},
  {0, 0.5, 0},
  {-3, -1, -7}
};

TYPED_TEST(NearestNeighborTest3D, NearestQueryGivenPoints) {
  if (DEFAULT_TESTING_FEATURES & TEST_NEAREST) {
    auto& impl = this->proxy.value;
    constexpr size_t dimension = this->dimension;

    if (DEFAULT_TESTING_FEATURES & TEST_INSERT_N) {
      ASSERT_TRUE(TestInsertN<DEFAULT_TESTING_FEATURES & TEST_INSERT_N>::test(&impl, points));
    } else {
      for (const auto & point : points) {
        ASSERT_TRUE(TestInsert<DEFAULT_TESTING_FEATURES & TEST_INSERT>::test(&impl, point));
      }
    }

    for (const auto& testPoint : testPoints) {
      vector<double> expected = nearestBruteForce<dimension, 1>(points, testPoint);
      auto actual = impl.nearest(testPoint);
      assertVectorEqual(expected, actual);
    }
  }
}

template <size_t Dimension>
static void generateRandomPoints(std::vector<std::array<double, Dimension>>& points, size_t pointsCount,
                                 std::vector<std::array<double, Dimension>>& testPoints, size_t testPointsCount) {
  volatile int a = 0;
  srand((unsigned int) clock() + (size_t)(&a));

  points.resize(pointsCount);
  for (size_t i = 0; i < pointsCount; i++) {
    for (size_t j = 0; j < Dimension; j++) {
      points[i][j] = randomValue(10000.0);
    }
  }

  testPoints.resize(testPointsCount);
  for (size_t i = 0; i < testPointsCount; i++) {
    for (size_t j = 0; j < Dimension; j++) {
      testPoints[i][j] = randomValue(10000.0);
    }
  }
}

TYPED_TEST(NearestNeighborTest3D, NearestRangeGivenPoints) {
  if (DEFAULT_TESTING_FEATURES & TEST_NEAREST_N) {
    auto& impl = this->proxy.value;
    constexpr size_t dimension = this->dimension;

    if (DEFAULT_TESTING_FEATURES & TEST_INSERT_N) {
      ASSERT_TRUE(TestInsertN<DEFAULT_TESTING_FEATURES & TEST_INSERT_N>::test(&impl, points));
    } else {
      for (const auto & point : points) {
        ASSERT_TRUE(TestInsert<DEFAULT_TESTING_FEATURES & TEST_INSERT>::test(&impl, point));
      }
    }

    double range = 3;
    for (const auto& testPoint : testPoints) {
      vector<double> expected = nearestBruteForce<dimension>(points, testPoint, sqr(range));
      auto actual = impl.nearest_range(testPoint, range);
      assertVectorEqual(expected, actual);
    }
  }
}

TYPED_TEST(NearestNeighborTest, NearestRandomPoints) {
  if (DEFAULT_TESTING_FEATURES & TEST_NEAREST) {
    volatile int a = 0;
    srand((unsigned int) clock() + (size_t)(&a));

    auto& impl = this->proxy.value;
    constexpr size_t dimension = this->dimension;

    std::vector<std::array<double, dimension>> points;
    std::vector<std::array<double, dimension>> testPoints;
    generateRandomPoints<dimension>(points, 100, testPoints, 10);

    if (DEFAULT_TESTING_FEATURES & TEST_INSERT_N) {
      ASSERT_TRUE(TestInsertN<DEFAULT_TESTING_FEATURES & TEST_INSERT_N>::test(&impl, points));
    } else {
      for (const auto & point : points) {
        ASSERT_TRUE(TestInsert<DEFAULT_TESTING_FEATURES & TEST_INSERT>::test(&impl, point));
      }
    }

    for (const auto& testPoint : testPoints) {
      vector<double> expected = nearestBruteForce<dimension, 1>(points, testPoint);
      auto actual = impl.nearest(testPoint);
      assertVectorEqual(expected, actual);
    }
  }
}

TYPED_TEST(NearestNeighborTest, NearestRangeRandomPoints) {
  if (DEFAULT_TESTING_FEATURES & TEST_NEAREST_RANGE) {
    volatile int a = 0;
    srand((unsigned int) clock() + (size_t)(&a));

    auto& impl = this->proxy.value;
    constexpr size_t dimension = this->dimension;

    std::vector<std::array<double, dimension>> points;
    std::vector<std::array<double, dimension>> testPoints;
    generateRandomPoints<dimension>(points, 100, testPoints, 10);

    if (DEFAULT_TESTING_FEATURES & TEST_INSERT_N) {
      ASSERT_TRUE(TestInsertN<DEFAULT_TESTING_FEATURES & TEST_INSERT_N>::test(&impl, points));
    } else {
      for (const auto & point : points) {
        ASSERT_TRUE(TestInsert<DEFAULT_TESTING_FEATURES & TEST_INSERT>::test(&impl, point));
      }
    }

    double range = 10000.0;
    for (const auto& testPoint : testPoints) {
      vector<double> expected = nearestBruteForce<dimension>(points, testPoint, sqr(range));
      auto actual = impl.nearest_range(testPoint, range);
      assertVectorEqual(expected, actual);
    }
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
