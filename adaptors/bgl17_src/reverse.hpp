//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#include <vector>
#include <util.hpp>

#ifndef __REVERSE_HPP
#define __REVERSE_HPP

template<typename path_edge>
class reverse_path {
public:
  reverse_path(std::vector<path_edge> &path, vertex_index_t start, vertex_index_t stop) : path_(path), start_(start), stop_(stop) {}
  
  std::vector<path_edge> path_;
  double update_;
  vertex_index_t start_, stop_;
  bool done=false;

  bool found(vertex_index_t current){return current==stop_;}
 
  class reverse_iterator {
  private:
    reverse_path& rev_;
    vertex_index_t curr_;

  public:
    reverse_iterator(reverse_path& rev) : rev_(rev), curr_(rev.start_) {}
    
    reverse_iterator& operator++() {
      curr_=rev_.path_[curr_].predecessor;
      if(rev_.found(curr_)) {
	rev_.done=true;
      }
      
      return *this;
    }

    auto operator*() {return std::tuple<vertex_index_t>(curr_); }

    class end_sentinel_type {
    public:
      end_sentinel_type() {}
    };
    
    auto operator==(const end_sentinel_type&) const { return rev_.done;}              
    bool operator!=(const end_sentinel_type&) const { return !rev_.done;}
  };

  auto begin() { return reverse_iterator(*this); }
  auto end() { return typename reverse_iterator::end_sentinel_type(); }
};

#endif
