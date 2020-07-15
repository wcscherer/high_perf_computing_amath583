#include <cmath>
#include <iostream>
#define WITHOUT_NUMPY
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main () {
  double pi = std::acos(-1.0);  // C++ does not mandate a value for pi

  plt::figure_size(1280, 960);  // Initialize a 1280 X 960 figure

  std::vector<double> x(1024);  // Create two vectors
  std::vector<double> y(1024);

  // Fill in x with values from 0 to 4*pi (equally spaced)
  double dx = 4*pi/1023;      // define the dicrete steps in x from 0 to 4pi -- use 1023 to not include 0
  for (int i = 0; i < 1024; ++i) {
        x[i] = dx*i;            //increment x vector by dx 
  }

  // Fill in y with cos of x
  for (int i = 0; i < 1024; ++i) {
        y[i] = std::cos(x[i]);  // Calculate cos(x[i]) from 0 to 4pi
  }

  // Check that the last value of x has the right value
  if (std::abs(x[1023] - 4.0*pi) < 1.0e-12) {
    std::cout << "Pass" << std::endl;
  } else {
    std::cout << "Fail" << " " << std::abs(x[1023] - 4.0*pi) << std::endl;
  }

  // Make the plot and save it to a file
  plt::plot(x,y);
  plt::save("cosx.png");

  return 0;
}