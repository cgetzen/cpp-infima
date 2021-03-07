#include "gtest/gtest.h"
#include "infima/infima.h"

template <typename T>
void equal_vector(std::vector<T> x, std::vector<T> y) {
  ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";

  for (long unsigned int i = 0; i < x.size(); ++i) {
    EXPECT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;
  }
}

TEST(Shuffle, SingleCell) {
  std::vector<std::string> dimensions {"AZ"};
  Lattice<int> lattice = Lattice<int>(dimensions);
  lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1a"}, std::vector<int>{1});
  lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1a"}, std::vector<int>{2,3,4,5});

  ShuffleSharder<int> sharder = ShuffleSharder<int>(5353);
  for(int i=0; i<100; i++) {
    Lattice<int> * shard = sharder.shuffleShard(lattice, i, 2);
    EXPECT_EQ(shard->get_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1a"}).size(), 2);
  }
}

TEST(Shuffle, OneDimensionsWithLess) {
  // Shuffle shard when endpoints_per_cell > available_endpoints
  std::vector<std::string> dimensions {"Region"};
  Lattice<int> lattice = Lattice<int>(dimensions);
  lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"a"}, std::vector<int>{1});
  lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"b"}, std::vector<int>{2,3,4,5});

  ShuffleSharder<int> sharder = ShuffleSharder<int>(5353);

  Lattice<int> * shard = sharder.shuffleShard(lattice, 1, 2);
  equal_vector(shard->get_endpoints_for_sector(Lattice<int>::Coordinate{"a"}), (std::vector<int>{1}));
}

TEST(Shuffle, Empty2DCell) {
  std::vector<std::string> dimensions {"Region", "AZ"};
  Lattice<int> lattice = Lattice<int>(dimensions);
  lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1", "a"}, std::vector<int>{1,2});
  lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-2", "b"}, std::vector<int>{3,4,5});

  ShuffleSharder<int> sharder = ShuffleSharder<int>(5353);

  Lattice<int> * shard = sharder.shuffleShard(lattice, 1, 3);
  equal_vector(shard->get_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1", "a"}), (std::vector<int>{1, 2}));
}
