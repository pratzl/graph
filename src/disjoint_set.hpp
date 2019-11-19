#ifndef __DISJOINT_SET_HPP
#define __DISJOINT_SET_HPP

vertex_index_t disjoint_find(std::vector<std::pair<vertex_index_t, size_t> > &subsets, vertex_index_t vtx)
{
  vertex_index_t parent = subsets[vtx].first;
  while(parent != subsets[parent].first) {
    parent = subsets[parent].first;
  }
  while(vtx != parent) {
    vtx = subsets[vtx].first;
    subsets[vtx].first = parent;
  }
  
  return parent;
}

void disjoint_union(std::vector<std::pair<vertex_index_t, size_t> > &subsets, vertex_index_t u, vertex_index_t v) {
  vertex_index_t u_root = disjoint_find(subsets, u);
  vertex_index_t v_root = disjoint_find(subsets, v);

  if (subsets[u_root].second < subsets[v_root].second)
    subsets[u_root].first = v_root;

  else if (subsets[u_root].second > subsets[v_root].second)
    subsets[v_root].first = u_root;
  
  else {
    subsets[v_root].first = u_root;
    subsets[u_root].second++;
  }
}

bool disjoint_union_find(std::vector<std::pair<vertex_index_t, size_t> > &subsets, vertex_index_t u, vertex_index_t v) {
  vertex_index_t u_root = disjoint_find(subsets, u);
  vertex_index_t v_root = disjoint_find(subsets, v);

  if(u_root != v_root) {
    
    if (subsets[u_root].second < subsets[v_root].second)
      subsets[u_root].first = v_root;
    
    else if (subsets[u_root].second > subsets[v_root].second)
      subsets[v_root].first = u_root;
    
    else {
      subsets[v_root].first = u_root;
      subsets[u_root].second++;
    }

    return true;
  }
  
  return false;
}

#endif    // __DISJOINT_SET_HPP
