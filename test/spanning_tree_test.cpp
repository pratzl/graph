#include <edge_list.hpp>
#include <mmio.hpp>
#include <algorithms/spanning_tree.hpp>
#include "common/test_header.hpp"


TEST( max_min_weight_spanning_tree, mst ) {
  auto aos_a = read_mm<undirected, double>("../data/msttest.mtx");
  edge_list<undirected, double> A_list(aos_a.size());
  for(auto y : aos_a) {
    A_list.push_back(std::get<0>(y), std::get<1>(y), std::get<2>(y));
  }

  /*SECTION("min weight")*/ {
    edge_list<undirected, double> T_list = kruskal(A_list);
    
    double totalweight = 0.0;
    for(auto y : T_list) {
      totalweight += std::get<2>(y);
    }
    
    EXPECT_TRUE(totalweight == 39);
  }
  
  /*SECTION("max weight")*/ {
    auto compare=[](auto t1, auto t2) { return std::get<2>(t1) > std::get<2>(t2) ;};
    edge_list<undirected, double> T_list = kruskal(A_list, compare);
    
    double totalweight = 0.0;
    for(auto y : T_list) {
      totalweight += std::get<2>(y);
    }
    
    EXPECT_TRUE(totalweight == 59);
  }
}
