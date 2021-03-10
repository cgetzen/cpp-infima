#include "infima/infima.h"

#include <algorithm>

int main() {
  std::vector<std::string> dimensions {"AZ"};
  Lattice<int> lattice = Lattice<int>(dimensions);
  lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1a"}, std::vector<int>{1,2,3,4,5});
  lattice.add_endpoints_for_sector(Lattice<int>::Coordinate{"us-east-1a"}, std::vector<int>{2,3,4,5});
  ShuffleSharder<int> sharder = ShuffleSharder<int>(5353);
  Lattice<int> * shard = sharder.shuffleShard(lattice, 1, 2);
  std::vector<int> endpoints = shard->get_endpoints();
  std::vector<int> selection;
  std::mt19937 g(5353);
  std::sample(endpoints.begin(), endpoints.end(), std::back_inserter(selection), 2, g);
  auto chosen = std::min_element(selection.begin(), selection.end());
}
