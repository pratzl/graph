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
#include <unordered_set>
#include <reverse.hpp>
#include <stack>
#include <ctime>
const size_t INFINITE_D = 100000000;

template<typename Graph>
double bk_maxflow3(Graph &A, std::vector<double> &cap)
{
  //std::clock_t start;
  double grow = 0;
  double augment = 0;
  double adopt = 0;
  size_t n_vtx = A.size();
  std::vector<vertex_index_t> preds(n_vtx, null_vertex);
  std::vector<size_t> indices(n_vtx, 0);
  vertex_index_t source = n_vtx;
  vertex_index_t terminal = n_vtx+1;
  double max_flow = 0;
  double tol = 1e-6;
  std::queue<vertex_index_t> active;
  std::queue<vertex_index_t> orphans;
  
  std::vector<double> flow(n_vtx, 0);
  double zero = 0;
  size_t gtime = 0;
  std::vector<bool> tree_id(n_vtx);
  std::vector<bool> in_tree(n_vtx,false);
  std::vector<size_t> timestamp(n_vtx);
  std::vector<size_t> dist(n_vtx);
  //capacity forward, flow forward
  //capacity back, flow back
  //residuals are never stored explicitly
  vertex_index_t connect_s;
  vertex_index_t connect_t;
  int augment_ct = 0;
  for(auto i = 0; i < n_vtx; ++i) {
    if(cap[i] > 0) {
      tree_id[i] = false;
      in_tree[i] = true;
      preds[i] = source;  
    }
    else if(cap[i] < 0) {
      cap[i] = -cap[i];
      max_flow -= cap[i];
      tree_id[i] = true;
      in_tree[i] = true;
      preds[i] = terminal;
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
      size_t idx = 0;
      auto backp = G[q].begin();
      for(; std::get<0>(*backp) != p; ++backp, ++idx){
      }
      std::get<3>(*it) = idx;
    }
  }

  size_t d;
  size_t d_min;
  vertex_index_t p, q;
  vertex_index_t target;
  
  auto connect = G[0].begin();
  auto backp = connect;
  
  while(true) {
    bool found = false;
    //start = std::clock();
    //Grow
    while(!active.empty()) {    
      
      p = active.front();
      //if(preds[p] == null_vertex && p != source && p != terminal) {
      //if(preds[p] == null_vertex) {
      if(!in_tree[p]) {
	active.pop();
	continue;
      }
      
      bool t = tree_id[p];
      auto last = G[p].end();
      size_t idx1 = 0;
      for (auto it = G[p].begin(); it != last; ++it, ++idx1) {
	q = std::get<0>(*it);
	
	if(t) {
	  backp = G[q].begin() + std::get<3>(*it);
	  if(std::get<4>(*backp) <= tol)
	    continue;
	  connect_s = q;
	  connect_t = p;
	}
	else {
	  if(std::get<4>(*it) <= tol)
	    continue;
	  connect_s = p;
	  connect_t = q;
	}

	//if(preds[q] == null_vertex) {
	if(!in_tree[q]) {
	  tree_id[q] = t;
	  preds[q] = p;
	  in_tree[q] = true;
	  if(t) {
	    indices[q] = std::get<3>(*it);
	  }
	  else {
	    indices[q] = idx1;
	  }
	  
	  active.push(q);
	  timestamp[q] = timestamp[p];
	  dist[q] = dist[p] + 1;
	}

	//found path
	else if(tree_id[q] != t) {
	  found = true;
	  if(t) {
	    connect = backp;
	  }
	  else {
	    connect = it;
	  }
	  break;
	}
	
	else if(timestamp[q] <= timestamp[p] && dist[q] > dist[p]) {
	  preds[q] = p;
	  if(t) {
	    indices[q] = std::get<3>(*it);
	  }
	  else {
	    indices[q] = idx1;
	  }
	  
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
    
    min_cap = std::get<4>(*connect);
    auto backedge = G[connect_t].begin() + std::get<3>(*connect);
    for(auto vtx = connect_t; vtx != terminal; vtx = preds[vtx]) {
      if(preds[vtx] == terminal) {
	min_cap = std::min(min_cap,cap[vtx] - flow[vtx]);
      }
      else {
	auto edge = G[vtx].begin() + indices[vtx];
	min_cap = std::min(min_cap, std::get<4>(*edge));
      }
    }
    
    for(auto vtx = connect_s; vtx != source; vtx = preds[vtx]) {
      if(preds[vtx] == source) {
	min_cap = std::min(min_cap,cap[vtx] - flow[vtx]);
      }
      else {
	auto edge = G[preds[vtx]].begin() + indices[vtx];
	min_cap = std::min(min_cap, std::get<4>(*edge));
      }
    }
    
    max_flow += min_cap;
    std::get<2>(*connect) += min_cap;
    double excess = std::get<2>(*connect) - std::get<1>(*connect);
    if(excess > 0) {
      std::get<2>(*connect) = std::get<1>(*connect);
      std::get<2>(*backedge) -= excess;
      std::get<4>(*connect) = std::get<2>(*backedge);
    }
    else {
      std::get<4>(*connect) -= min_cap;
    }
    std::get<4>(*backedge) = std::get<1>(*backedge) - std::get<2>(*backedge) + std::get<2>(*connect);
    
    for(auto vtx = connect_t, next = preds[vtx]; vtx != terminal; vtx = next, next = preds[vtx]) {
      if(next == terminal) {
	flow[vtx] += min_cap;
	if(flow[vtx] >= cap[vtx]) {
	  preds[vtx] = null_vertex;
	  in_tree[vtx] = false;
	  orphans.push(vtx);
	}
      }
      else {
	auto edge = G[vtx].begin() + indices[vtx];
	auto backedge = G[next].begin() + std::get<3>(*edge);
	std::get<2>(*edge) += min_cap;
	excess = std::get<2>(*edge) - std::get<1>(*edge);
	if(excess > 0) {
	  std::get<2>(*edge) = std::get<1>(*edge);
	  std::get<2>(*backedge) -= excess;
	  excess = 0;
	  std::get<4>(*edge) = std::get<2>(*backedge);
	}
	else {
	  std::get<4>(*edge) -= min_cap;
	}
	if(excess == 0) {
	  preds[vtx] = null_vertex;
	  in_tree[vtx] = false;
	  orphans.push(vtx);
	}	
	std::get<4>(*backedge) = std::get<1>(*backedge) - std::get<2>(*backedge) + std::get<2>(*edge);
      }
    }
    
    for(auto vtx = connect_s, next = preds[vtx]; vtx != source; vtx = next, next = preds[vtx]) {
      if(next == source) {
	flow[vtx] += min_cap;
	if(flow[vtx] >= cap[vtx]) {
	  preds[vtx] = null_vertex;
	  in_tree[vtx] = false;
	  orphans.push(vtx);
	}
      }
      else {
	auto edge = G[next].begin() + indices[vtx];
	auto backedge = G[vtx].begin() + std::get<3>(*edge);
	std::get<2>(*edge) += min_cap;
	excess = std::get<2>(*edge) - std::get<1>(*edge);
	if(excess > 0) {
	  std::get<2>(*edge) = std::get<1>(*edge);
	  std::get<2>(*backedge) -= excess;
	  excess = 0;
	  std::get<4>(*edge) = std::get<2>(*backedge);
	}
	else {
	  std::get<4>(*edge) -= min_cap;
	}
	if(excess == 0) {
	  preds[vtx] = null_vertex;
	  in_tree[vtx] = false;
	  orphans.push(vtx);
	}
	std::get<4>(*backedge) = std::get<1>(*backedge) - std::get<2>(*backedge) + std::get<2>(*edge);
      }
    }

    //augment += (std::clock() - start);
    //start = std::clock();
    //Adopt
    
    while(!orphans.empty()) {
      p = orphans.front();
      orphans.pop();

      bool t = tree_id[p];
      vertex_index_t min_pred;
      d_min = INFINITE_D;
      size_t min_idx;
      size_t idx1 = 0;
      for (auto it = G[p].begin(); it != G[p].end(); ++it, ++idx1) {
	q = std::get<0>(*it);
	d = 0;
	
	
	if(t) {
	  if(std::get<4>(*it) <= tol)
	    continue;
	  target = terminal;
	}
	else {
	  backp = G[q].begin() + std::get<3>(*it);
	  if(std::get<4>(*backp) <= tol)
	    continue;
	  target = source;
	}
	if(tree_id[q] != t || preds[q] == p)
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

	  //else if(preds[vtx] == null_vertex) {
	  else if(!in_tree[vtx]) {
	    d = INFINITE_D;
	    break;
	  }
	}
	
	if(d < INFINITE_D) {
	  if(d < d_min) {
	    min_pred = q;
	    
	    if(t) {
	      min_idx = idx1;
	    }
	    else {
	      min_idx = std::get<3>(*it);
	    }
	    
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
      //if(preds[p] == null_vertex) {
      if(!in_tree[p]) {
	timestamp[p] = 0;
	
	for (auto it = G[p].begin(); it != G[p].end(); ++it) {
	  q = std::get<0>(*it);
	  auto backp = G[q].begin() + std::get<3>(*it);
	  
	  if(tree_id[q] != t)
	    continue;

	  if(!tree_id[q] && std::get<4>(*it) > tol)
	    active.push(q);

	  else if(tree_id[q] && std::get<4>(*backp) > tol)
	    active.push(q);

	  if(preds[q] == p) {
	    orphans.push(q);
	    preds[q] = null_vertex;
	    in_tree[q] = false;
	  }
	}
      }
      //Found parent
      else {
	preds[p] = min_pred;
	in_tree[p] = true;
	indices[p] = min_idx;
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
  return max_flow;
}
