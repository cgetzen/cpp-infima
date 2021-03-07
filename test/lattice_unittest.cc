#include "gtest/gtest.h"
#include "infima/infima.h"

template <typename T>
void equal_vector(std::vector<T> x, std::vector<T> y) {
  ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";

  for (long unsigned int i = 0; i < x.size(); ++i) {
    EXPECT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;
  }
}

TEST(Lattice, SingleCell) {
  std::vector<std::string> dimensions {"AZ"};
  Lattice<int> lattice = Lattice<int>(dimensions);
  lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1a"}, std::vector<int>{1});
  lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1a"}, std::vector<int>{2,3,4,5});
  equal_vector(lattice.get_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1a"}), std::vector<int>{1,2,3,4,5});
}

TEST(Lattice, OneDimension) {
  std::vector<std::string> dimensions {"AZ"};
  Lattice<int> lattice = Lattice<int>(dimensions);
  lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1a"}, std::vector<int>{1});
  lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1b"}, std::vector<int>{2,3,4,5});
  equal_vector(lattice.get_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1b"}), std::vector<int>{2,3,4,5});
}

TEST(Lattice, TwoDimension) {
 std::vector<std::string> dimensions {"Region", "AZ"};
 Lattice<int> lattice = Lattice<int>(dimensions);
 lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1","a"}, std::vector<int>{1,2});
 lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1","b"}, std::vector<int>{3,4});
 lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-2","a"}, std::vector<int>{5,6});
 lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-2","b"}, std::vector<int>{7,8});
 equal_vector(lattice.get_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-2", "a"}), std::vector<int>{5,6});
}
