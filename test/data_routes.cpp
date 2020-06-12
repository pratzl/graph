//#include "pch.h"
#include "data_routes.hpp"
#include <ostream>
#include <range/v3/action/unique.hpp>

using std::vector;
using std::string;

routes_t const germany_routes{
      {"Frankfürt", "Mannheim", 85}, {"Frankfürt", "Würzburg", 217}, {"Frankfürt", "Kassel", 173},
      {"Mannheim", "Karlsruhe", 80}, {"Karlsruhe", "Augsburg", 250}, {"Augsburg", "München", 84},
      {"Würzburg", "Erfurt", 186},   {"Würzburg", "Nürnberg", 103},  {"Nürnberg", "Stuttgart", 183},
      {"Nürnberg", "München", 167},  {"Kassel", "München", 502}};

GraphXlate<daa_routes_edge_mapper> germany_routes_directed_graph(daa_routes_edge_mapper(), germany_routes);
GraphXlate<ual_routes_edge_mapper> germany_routes_undirected_graph(ual_routes_edge_mapper(), germany_routes);

/**
 * @graph
 * ------
 * a1 -------- b1 -------- c1
 *            ^  \  
 *           /    \  
 *           |    |
 *           \    /     
 *            \  v  
 * a2 -------- b2 -------- c2
 *
 * @author Phillip Ratzloff
 * 
 * A graph to demonstrate a simple cycle (directed graph) or multiple edges (undirected
 * graph).
 *
 * "Dollar" refers to the shape of the graph (imagine the portrait of George Washington in 
 * the middle, like a dollar bill). 
 */
data_edges_t const dollar_edges = {{"a1", "b1", 100.0}, {"a2", "b2", 100.0}, {"b1", "c1", 50.0},
                                   {"b2", "c2", 90.0},  {"b1", "b2", 50.0},  {"b2", "b1", 10.0}};

GraphXlate<daa_data_edge_mapper> dollar_directed_graph(daa_data_edge_mapper(), dollar_edges);
GraphXlate<ual_data_edge_mapper> dollar_undirected_graph(ual_data_edge_mapper(), dollar_edges);


/**
 * @graph
 * 
 *    s1             s2       s3        s4          s5   
 *
 *    ------       ------   ------    ------      ------ 
 *    | r1 |------>| a1 |-->| a3 |--->| a5 |----->| c1 |
 *    ------       ------   ------\   ------      ------ 
 *     | ^ \                 ^ | \ \    \                
 *     | |  \               /  |  \ \    \              
 *     | |   |             /   |   \ \    \            
 *     | |   v            /    v    \ \    \          
 *     | ------          /  ------   \ \    ----->------ 
 *     | | r2 |         /   | a4 |    \ \-------->| c2 | 
 *     | ------        /    ------     \      --->------ 
 *     |              /    /      ^     \     |   
 *     |             /    /        \     \    |          
 *     |            /    /          \     \   |
 *     |           |    /            \     |  |          
 *     |           |   /              \    |  |       
 *     |           |  |                |   |  |       
 *     v           |  v                |   v /        
 *    ------       ------             ------/     ------ 
 *    | r3 |------>| a2 |<------------| a6 |----->| c3 |
 *    ------       ------             ------      ------ 
 *    
 * @author Phillip Ratzloff
 * 			
 * A graph used to exercise cycle detection for directed & undirected graphs.
 *
 * "Candle' is a whimsical interpretation of of this graph to represent a 
 * one-person candle manufacturing shop. 
 */
data_edges_t const candle_edges = {
      {"Joe", "Design", 10.0},                  // r1 --> r2
      {"Joe", "Pellets", 2.0},                  // r1 --> r3
      {"Joe", "Prep", 5.0},                     // r1 --> a1
      {"Design", "Joe", 10.0},                  // r2 --> r1
      {"Pellets", "Pour Wax", 1.0},             // r3 --> a2
      {"Prep", "Sculpt Rough", 1.0},            // a1 --> a3
      {"Pour Wax", "Sculpt Rough", 0},          // a2 --> a3
      {"Sculpt Rough", "Melt Shavings", 0.5},   // a3 --> a4
      {"Sculpt Rough", "Varnish Rough", 2.0},   // a3 --> a5
      {"Sculpt Rough", "Decorate Rough", 3.0},  // a3 --> a6
      {"Sculpt Rough", "Box Mixed", 1.0},       // a3 --> c2
      {"Melt Shavings", "Pour Wax", 1.0},       // a4 --> a2
      {"Varnish Rough", "Box Sculpted", 1.0},   // a5 --> c1
      {"Varnish Rough", "Box Mixed", 1.0},      // a5 --> c2
      {"Decorate Rough", "Pour Wax", 2.0},      // a6 --> a2
      {"Decorate Rough", "Melt Shavings", 0.5}, // a6 --> a4
      {"Decorate Rough", "Box Mixed", 1.0},     // a6 --> c2
      {"Decorate Rough", "Box Decorated", 1.0}, // a6 --> c3
};

GraphXlate<daa_data_edge_mapper> candle_directed_graph(daa_data_edge_mapper(), candle_edges);
GraphXlate<ual_data_edge_mapper> candle_undirected_graph(ual_data_edge_mapper(), candle_edges);
