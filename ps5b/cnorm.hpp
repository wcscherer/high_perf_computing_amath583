//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH583_CNORM_HPP
#define AMATH583_CNORM_HPP

#include "Timer.hpp"
#include "Vector.hpp"
#include "amath583.hpp"
#include "norm_utils.hpp"

#include <cassert>
#include <cmath>
#include <future>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
std::mutex mtx_lk;


void worker_a(const Vector& x, unsigned long begin, unsigned long step, double& partial) {
  double sum_of_squares = 0.0;
  for (unsigned long i = begin; i < x.num_rows(); i += step)
    sum_of_squares += x(i) * x(i);
  
    {std::lock_guard<std::mutex> ptl_lock(mtx_lk);
  partial += sum_of_squares;
  }

  //return sum_of_squares;
  
}
// Write me
double cyclic_two_norm(const Vector& x, size_t partitions) {
  double sum = 0.0;
  //std::vector<std::future<double>> parts;   // vector for storing results of each job
  //size_t begin = 0;
  std::vector<std::thread> threads;
  for (size_t k = 0; k < partitions; ++k){
      //double pts = 0;

      threads.push_back(std::thread(worker_a, std::cref(x), k, partitions, std::ref(sum)));
      //parts.push_back(
      //std::async(std::launch::async,worker_a, std::cref(x), k, partitions));
      
      //begin +;
  }

  for (unsigned long j = 0; j < partitions; ++j )
      threads[j].join();

  return std::sqrt(sum);
}

#endif    // AMATH583_CNORM_HPP
