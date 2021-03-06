#include <map>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <climits>


int test() PURE;
int test() {
  std::vector<int> x;
  x.push_back(1);
  return x[0];
}

template <typename Type>
class Lattice {
public:
  using Coordinate = std::vector<std::string>;

  Lattice(std::vector<std::string> dimension_names)
    : dimension_names_(dimension_names) {
      for (std::string dimension : dimension_names) {
        values_by_dimension_[dimension] = std::vector<std::string>();
      }
  }


  void add_endpoints_for_sector(Coordinate sector_coordinates, std::vector<Type> endpoints) {
    bool pre_exists = endpoints_by_coordinate_[sector_coordinates].size();
    // Add endpoints
    for (Type endpoint : endpoints) {
      endpoints_by_coordinate_[sector_coordinates].push_back(endpoint); // optimize
    }
    // Add coordinate value
    if (!pre_exists)
      for (uint i = 0; i < dimension_names_.size(); i++)
        values_by_dimension_[dimension_names_[i]].push_back(sector_coordinates[i]);
  }

  std::vector<std::string> get_dimension_names() {
    return dimension_names_;
  }

  std::vector<Type> get_endpoints_for_sector(Coordinate sector_coordinates) {
    return endpoints_by_coordinate_[sector_coordinates];
  }

  std::vector<std::string> get_dimension_values(std::string dimension_name) {
    return values_by_dimension_[dimension_name];
  }

  std::map<std::string, int> get_dimensionality() {
    std::map<std::string, int> dimensionality;
    for (auto dimension : dimension_names_) {
      dimensionality[dimension] = get_dimension_values(dimension).size();
    }
    return dimensionality;
  }

  void print_values_by_dimension() {
    std::cout << "Values By Dimension:" << std::endl;
    for (auto it = values_by_dimension_.begin(); it != values_by_dimension_.end(); it++) {
      std::cout << it->first << ": ";
      for (auto s : it->second) {
        std::cout << s << ", ";
      }
      std::cout << std::endl;
    }
  }

  void print_endpoints_by_coordinate() {
    std::cout << "Endpoints by Coordinate:" << std::endl;
    for (auto it = endpoints_by_coordinate_.begin(); it != endpoints_by_coordinate_.end(); it++) {
      std::cout << "Coordinate(";
      for (auto c : it->first) std::cout << c << ", ";
      std::cout << "): ";
      for (auto s : it->second) std::cout << s << ", ";
      std::cout << std::endl;
    }
  }

  const std::vector<std::string> dimension_names_;
  std::map<std::string, std::vector<std::string> > values_by_dimension_;
  std::map<Coordinate, std::vector<Type>> endpoints_by_coordinate_;
};

template <typename Type>
class ShuffleSharder {
public:
  ShuffleSharder(uint64_t seed) : seed_(seed) { }

  Lattice<Type>* shuffleShard(Lattice<Type> lattice, uint64_t hash, int endpoints_per_cell) {
    Lattice<Type> * chosen = new Lattice<Type>(lattice.get_dimension_names());

    std::vector<std::vector<std::string>> shuffled_dimension_values;
    std::mt19937 g(seed_ + hash);

    for (std::string dimensionName : lattice.get_dimension_names()) {
      std::vector<std::string> shuffled_values = lattice.get_dimension_values(dimensionName);
      std::shuffle(shuffled_values.begin(), shuffled_values.end(), g);
      shuffled_dimension_values.push_back(shuffled_values);
    }

    auto dimensionality = lattice.get_dimensionality();

    if (dimensionality.size() == 1) {
      for (auto dimension_value : shuffled_dimension_values[0]) {
        std::vector<std::string> c{dimension_value};
        auto available_endpoints = lattice.get_endpoints_for_sector(c);
        std::shuffle(available_endpoints.begin(), available_endpoints.end(), g);
        std::vector<Type> returned_endpoints(available_endpoints.begin(), available_endpoints.begin() + endpoints_per_cell);
        chosen->add_endpoints_for_sector(c, returned_endpoints);
      }
      return chosen;
    }

    int minimum_dimension_size = INT_MAX;
    for (auto it = dimensionality.begin(); it != dimensionality.end(); it++) {
      if (it->second < minimum_dimension_size) {
        minimum_dimension_size = it->second;
      }
    }

    for (int i = 0; i < minimum_dimension_size; i++) {
      std::vector<std::string> coordinates;

      for (uint j = 0; j < lattice.get_dimension_names().size(); j++) {
          coordinates.push_back(shuffled_dimension_values[j][0]);
          shuffled_dimension_values[j].erase(shuffled_dimension_values[j].begin());
      }
      auto available_endpoints = lattice.get_endpoints_for_sector(coordinates);
      std::shuffle(available_endpoints.begin(), available_endpoints.end(), g);
      std::vector<Type> returned_endpoints(available_endpoints.begin(), available_endpoints.begin() + endpoints_per_cell);
      chosen->add_endpoints_for_sector(coordinates, returned_endpoints);
    }

    return chosen;
  }

  const uint64_t seed_;
};
// public Lattice<T> shuffleShard(Lattice<T> lattice, byte[] identifier, int endpointsPerCell)
//         throws NoSuchAlgorithmException {
//     Lattice<T> chosen = new Lattice<T>(lattice.getDimensionNames());

int main() {
    std::cout << "Start" << std::endl;
    std::vector<std::string> x{ "AZ", "region"};
    Lattice<int> l = Lattice<int>(x);
    // Lattice<int>::Coordinate c{ "1", "us-east"};
    //std::vector<int> e{1,2,3};
    l.add_endpoints_for_sector(Lattice<int>::Coordinate{"1", "us-east"}, std::vector<int>{1,2,3,4,5});
    l.add_endpoints_for_sector(Lattice<int>::Coordinate{"1", "us-east"}, std::vector<int>{0});
    l.add_endpoints_for_sector(Lattice<int>::Coordinate{"1", "us-west"}, std::vector<int>{6,7,8,9});
    l.add_endpoints_for_sector(Lattice<int>::Coordinate{"2", "us-east"}, std::vector<int>{10,11,12,13});
    l.add_endpoints_for_sector(Lattice<int>::Coordinate{"2", "us-west"}, std::vector<int>{14,15,16,17});
    // for (auto it = l.endpoints_by_coordinate_.begin(); it != l.endpoints_by_coordinate_.end(); it++) {
    //   std::cout << "S" << std::endl;
    //   for (auto f : it->first) {
    //     std::cout << "Endpoint: " << f << std::endl;
    //   }
    //   for (auto f : it->second) {
    //     std::cout << f << std::endl;
    //   }
    // }
    l.print_values_by_dimension();
    l.print_endpoints_by_coordinate();


    ShuffleSharder<int> s = ShuffleSharder<int>(2);

    std::cout << std::endl << std::endl;
    auto final = s.shuffleShard(l, 0, 2);
    std::cout << std::endl << std::endl;
    for (auto it = final->endpoints_by_coordinate_.begin(); it != final->endpoints_by_coordinate_.end(); it++) {
      std::cout << "S" << std::endl;
      for (auto f : it->first) {
        std::cout << "Endpoint: " << f << std::endl;
      }
      for (auto f : it->second) {
        std::cout << f << std::endl;
      }
    }
    test();
    return 0;
}
