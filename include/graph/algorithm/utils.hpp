//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//


//****************************************************************************
template <typename RealT>
void zeroize(std::vector<RealT>& x) {
  for (size_t i = 0; i < x.size(); ++i) {
    x[i] = 0.0;
  }
}
