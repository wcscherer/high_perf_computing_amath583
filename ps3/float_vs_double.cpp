//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//

#include <iostream>
#include <vector>
#include "Timer.hpp"
#include <cstdlib>


int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " size" << std::endl;
    return -1;
  }
  size_t dim = std::stol(argv[1]);
  double flopsf = 0, flopsd = 0, iflopsf = 0, iflopsd = 0;
  double max_flops = 2e+09;
  double tmd_temp=0, tid_temp = 0, tif_temp = 0;

  Timer t;    

  t.start();
  std::vector<double> dx(dim, 3.14);
  std::vector<double> dy(dim, 27.0);
  std::vector<double> dz(dim, 0.0);
  t.stop();
  tid_temp = t.elapsed();
  iflopsd = 3*double(dim)*1000/t.elapsed();

  std::cout << "Construction time for double: " << t.elapsed() << std::endl;
  std::cout << "Construction FLOPs for double: " << iflopsd << std::endl;

  t.start();
  for (size_t i = 0; i < dim; ++i) {
    dz[i] = dx[i] * dy[i];
  }
  t.stop();
  flopsd = 2*double(dim)*1000/t.elapsed();
  std::cout << "Multiplication time for double: " << t.elapsed() << std::endl;
  std::cout << "Multiplication FLOPS for double: " << flopsd << std::endl;
  std::cout << "Current FLOPS/Max FLOPS (%): " << flopsd*100/max_flops << "\n" << std::endl;
  tmd_temp = t.elapsed();

  t.start();
  std::vector<float> fx(dim, 3.14);
  std::vector<float> fy(dim, 27.0);
  std::vector<float> fz(dim, 0.0);
  t.stop();
  tif_temp = t.elapsed();
  iflopsf = 3*double(dim)*1000/t.elapsed();

  std::cout << "Construction time for float: " << t.elapsed() << std::endl;
  std::cout << "Construction FLOPs for float: " << iflopsf << std::endl;

  t.start();
  for (size_t i = 0; i < dim; ++i) {
    fz[i] = fx[i] * fy[i];
  }
  t.stop();
  flopsf = 2*double(dim)*1000/t.elapsed();
  std::cout << "Multiplication time for float: " << t.elapsed() << std::endl;
  std::cout << "Multiplication FLOPS for float: " << flopsf << std::endl;
  std::cout << "Current FLOPS/Max FLOPS (%): " << flopsf*100/max_flops << "\n" << std::endl;
  std::cout << "Ratio of float/double initialization time: " << tif_temp/tid_temp << std::endl;
  std::cout << "Ratio of float/double initialization FLOPS: " << iflopsf/iflopsd << std::endl;
  std::cout << "Ratio of float/double multiplication time: " << t.elapsed()/tmd_temp << std::endl;
  std::cout << "Ratio of float/double multiplication FLOPS: " << flopsf/flopsd << std::endl;

  return 0;
}
