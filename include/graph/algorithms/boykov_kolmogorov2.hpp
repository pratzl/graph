//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#pragma once
#include <tuple>
#include <algorithm>
#include <utility>
#include <util.hpp>
#include <reverse.hpp>
#include <stack>
#include <ctime>
const size_t INFINITE_D = 100000000;

template<typename Graph>
std::tuple<double, std::vector<bool> > bk_maxflow2(Graph &A, const std::vector<double> &cap, std::vector<double> &flow)
{
  auto resid_cap = [](auto capacity, auto flow, auto backflow)
    { return capacity - flow + backflow; };

  std::clock_t start;
  double grow = 0;
  double augment = 0;
  double adopt = 0;
  size_t n_vtx = A.size();
  std::vector<vertex_index_t> preds(n_vtx, null_vertex);
  vertex_index_t source = n_vtx;
  vertex_index_t terminal = n_vtx+1;
  double max_flow = 0;
  double tol = 0.0;
  std::queue<vertex_index_t> active;
  std::queue<vertex_index_t> orphans;
  
  //std::vector<double> flow(n_vtx, 0);
  double stbackflow = 0;
  size_t gtime = 0;
  
  //std::vector<bool> in_tree(n_vtx, false);
  std::vector<bool> tree_id(n_vtx);
  std::vector<std::tuple<double, double*, double*> > trees(n_vtx);
  std::vector<size_t> timestamp(n_vtx);
  std::vector<size_t> dist(n_vtx);
  //capacity forward, flow forward
  //capacity back, flow back
  //residuals are never stored explicitly
  double cf,*ff,*cb,*fb;
  vertex_index_t connect_s;
  vertex_index_t connect_t;
  int augment_ct = 0;
  int path_length = 0;
  for(auto i = 0; i < n_vtx; ++i) {
    if(cap[i] > 0) {
      tree_id[i] = false;
      preds[i] = source;  
      trees[i] = std::make_tuple(cap[i], &flow[i], &stbackflow); 
    }
    else if(cap[i] < 0) {
      max_flow += cap[i];
      tree_id[i] = true;
      preds[i] = terminal;
      trees[i] = std::make_tuple(-cap[i], &flow[i], &stbackflow); 
    }
    else {
      continue;
    }
    
    dist[i] = 1;
    timestamp[i] = 0;
    
    active.push(i);
  }
  double min_cap;
  auto G = A.begin();
  for(auto p = 0; p < n_vtx; ++p) {
    for (auto it = G[p].begin(); it != G[p].end(); ++it) {
      vertex_index_t q = std::get<0>(*it);
      int idx = 0;
      if(std::get<3>(*it) == NULL || std::get<4>(*it) == NULL) {
	auto backp = G[q].begin();
	for(; std::get<0>(*backp) != p; ++backp, ++idx){
	}
	std::get<3>(*it) = &(std::get<1>(*backp));
	std::get<4>(*it) = &(std::get<2>(*backp));
	std::get<3>(*backp) = &(std::get<1>(*it));
	std::get<4>(*backp) = &(std::get<2>(*it));
      }
    }
  }


  
  while(true) {
    bool found = false;
    //start = std::clock();
    //Grow
    while(!active.empty()) { 
      auto p = active.front();

      if(preds[p] == null_vertex) {
	active.pop();
	continue;
      }
      
      bool t = tree_id[p];
      for (auto it = G[p].begin(); it != G[p].end(); ++it) {
	auto q = std::get<0>(*it);
	
	if(t) {
	  connect_s = q;
	  connect_t = p;
	  cf = *std::get<3>(*it);
	  ff = std::get<4>(*it);
	  fb = &std::get<2>(*it);
	}
	else {
	  connect_s = p;
	  connect_t = q;
	  cf = std::get<1>(*it);
	  ff = &std::get<2>(*it);
	  fb = std::get<4>(*it);
	}

	if(resid_cap(cf, *ff, *fb) <= tol)
	  continue;
	
	if(preds[q] == null_vertex) {
	  tree_id[q] = t;
	  preds[q] = p;
	  
	  trees[q] = std::make_tuple(cf, ff, fb);
	  active.push(q);
	  timestamp[q] = timestamp[p];
	  dist[q] = dist[p] + 1;
	}

	//found path
	else if(tree_id[q] != t) {
	  found = true;
	  break;
	}
	
	else if(timestamp[q] <= timestamp[p] && dist[q] > dist[p]) {
	  preds[q] = p;
	  trees[q] = std::make_tuple(cf, ff, fb);
	  timestamp[q] = timestamp[p];
	  dist[q] = dist[p] + 1;
	}	
      }
      
      gtime++;
      
      if(found)
	break;
      
      active.pop();
    }
    
    if(!found)
      break;
    //grow += (std::clock() - start);
    //start = std::clock();
    //Augment
    augment_ct += 1;
    min_cap = resid_cap(cf, *ff, *fb);
    path_length++;
    for(auto vtx = connect_t; vtx != terminal; vtx = preds[vtx]) {
      path_length++;
      auto edge = trees[vtx];
      min_cap = std::min(min_cap,resid_cap(std::get<0>(edge),
					   *std::get<1>(edge),
					   *std::get<2>(edge)));
    }
    
    for(auto vtx = connect_s; vtx != source; vtx = preds[vtx]) {
      path_length++;
      auto edge = trees[vtx];
      min_cap = std::min(min_cap,resid_cap(std::get<0>(edge),
					   *std::get<1>(edge),
					   *std::get<2>(edge)));
    }
    
    max_flow += min_cap;
    *ff += min_cap;
    double excess = *ff - cf;
    
    if(excess > 0) {
      *ff = cf;
      *fb -= excess;
    }
        
    for(auto vtx = connect_t, next = preds[vtx]; vtx != terminal; vtx = next, next = preds[vtx]) {
      auto edge = trees[vtx];    
      *std::get<1>(edge) += min_cap;
      excess = *std::get<1>(edge) - std::get<0>(edge);
      if(excess > 0) {
	*std::get<1>(edge) = std::get<0>(edge);
	  *std::get<2>(edge) -= excess;
	  excess = 0;
      }
      if(excess == 0) {
	preds[vtx] = null_vertex;
	orphans.push(vtx);
      }
    }
    
    
    for(auto vtx = connect_s, next = preds[vtx]; vtx != source; vtx = next, next = preds[vtx]) {
      auto edge = trees[vtx];
      *std::get<1>(edge) += min_cap;
      excess = *std::get<1>(edge) - std::get<0>(edge);
      if(excess > 0) {
	*std::get<1>(edge) = std::get<0>(edge);
	*std::get<2>(edge) -= excess;
	excess = 0;
      }
      if(excess == 0) {
	preds[vtx] = null_vertex;
	orphans.push(vtx);
      }
    }
    
    //augment += (std::clock() - start);
    //start = std::clock();
    //Adopt
    
    while(!orphans.empty()) {
      auto p = orphans.front();
      orphans.pop();
      
      bool t = tree_id[p];
      vertex_index_t min_pred;
      size_t d_min = INFINITE_D;
      std::tuple<double, double*, double*> min_edge;
      
      for (auto it = G[p].begin(); it != G[p].end(); ++it) {
	vertex_index_t q = std::get<0>(*it);
	vertex_index_t target;
	
	size_t d = 0;
	       
	if(tree_id[q] != t || preds[q] == p)
	  continue;

	if(t) {
	  cf = std::get<1>(*it);
	  ff = &std::get<2>(*it);
	  fb = std::get<4>(*it);
	  target = terminal;
	}
	
	else {
	  cf = *std::get<3>(*it);
	  ff = std::get<4>(*it);
	  fb = &std::get<2>(*it);
	  target = source;
	}
	
	if(resid_cap(cf, *ff, *fb) <= tol)
	  continue;

	for(auto vtx = q; vtx != target; vtx = preds[vtx]) {
	  if(timestamp[vtx] == gtime) {
	    d += dist[vtx];
	    break;
	  }

	  d++;

	  if(preds[vtx] == target) {
	    timestamp[vtx] = gtime;
	    dist[vtx] = 1;
	    break;
	  }

	  else if(preds[vtx] == null_vertex) {
	    d = INFINITE_D;
	    break;
	  }
	}
	
	if(d < INFINITE_D) {
	  if(d < d_min) {
	    min_pred = q;
	    min_edge = std::make_tuple(cf, ff, fb);    
	    d_min = d;
	  }

	  for(auto vtx = q; vtx != target; vtx = preds[vtx]) {
	    if(timestamp[vtx] == gtime)
	      break;

	    timestamp[vtx] = gtime;
	    dist[vtx] = d--;
	  }
	}
      }
      
      //can't find new parent
      if(preds[p] == null_vertex) {
	timestamp[p] = 0;
	
	for (auto it = G[p].begin(); it != G[p].end(); ++it) {
	  auto q = std::get<0>(*it);
	  	  
	  if(tree_id[q] != t)
	    continue;
	  
	  if(!tree_id[q] && resid_cap(std::get<1>(*it), std::get<2>(*it),
				      *std::get<4>(*it)) > tol) {
	    active.push(q);
	  }
	  else if(tree_id[q] && resid_cap(*std::get<3>(*it), *std::get<4>(*it),
					  std::get<2>(*it)) > tol) {
	    active.push(q);
	  }
	  
	  if(preds[q] == p) {
	    orphans.push(q);
	    preds[q] = null_vertex;
	  }
	}
      }
      //Found parent
      else {
	//in_tree[p] = true;
	preds[p] = min_pred;
	trees[p] = min_edge;
	timestamp[p] = gtime;
	dist[p] = d_min + 1;
      }
    }
    //adopt += (std::clock() - start);
  }
  /*std::cout << "grow time: " << grow / (double) CLOCKS_PER_SEC <<
    " augment time: " << augment / (double) CLOCKS_PER_SEC <<
    " adopt time: " << adopt / (double) CLOCKS_PER_SEC << std::endl;*/
  std::cout << augment_ct << " aguments" << std::endl;
  std::cout << path_length*(1.0)/augment_ct << " avg path length" << std::endl;
  for(size_t i = 0; i < n_vtx; ++i) {
    if(preds[i] == null_vertex) {
      tree_id[i] = true;
    }
  }
  return std::make_tuple(max_flow, tree_id);
}
