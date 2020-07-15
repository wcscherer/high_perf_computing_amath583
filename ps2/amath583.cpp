//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "amath583.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include <cmath>
#include <cstddef>

void zeroize(Vector& x) {
  // Takes in a vector x and sets all components of the inupt vector x to 0
  for (size_t i = 0; i < x.num_rows(); i++){
    x(i) = 0;
  }
}

double one_norm(const Vector& x) {
  // Takes in a vector x and returns the 1-norm of vector x
  double sum = 0.0;
  for (size_t i = 0; i < x.num_rows(); ++i){

      sum += std::abs(x(i));

  }

  return sum;
}

double two_norm(const Vector& x) {
  // Takes in a vector x and returns the 2-norm of vector x
  double sum =  0.0;

  for (size_t i = 0; i < x.num_rows(); ++i){

      sum += std::pow(x(i),2); // accumulate squared elements

  }
 
  return std::sqrt(sum);
}

double inf_norm(const Vector& x) {
  // Takes in a vector x and returns the inf-norm of vector x
  double result = std::abs(x(0)); // initialize the first element as the max

  for (size_t i = 1; i < x.num_rows(); ++i){

      if (x(i) > result){

        result = std::abs(x(i)); // if the new element is bigger than current max, replace

      }

      else {
        continue;
      }

  }

  return result;
}

double dot(const Vector& x, const Vector& y) {
  // Takes in vectors x and y and returns the dot product of the two vectors
  double dp = 0.0;


  if (x.num_rows() == y.num_rows()){  // check vectors x and y are the same size

      for (size_t i = 0; i < x.num_rows(); ++i)

        dp += x(i)*y(i);
  }

  else{

      dp = 0; // return negative zero if vectors not same size
  }

  return dp;
}

double two_norm_d(const Vector& x) { 
  // Takes in vector x and returns two-norm using dot product
  return std::sqrt(dot(x,x));  
}

double one_norm(const Matrix& A) {
  double sum = 0.0;
  for (size_t j = 0; j < A.num_cols(); ++j) {    // for each column
    double tmp = 0.0;
    for (size_t i = 0; i < A.num_rows(); ++i) {    // compute column sum
      tmp += std::abs(A(i, j));
    }
    sum = std::max(sum, tmp);
  }
  return sum;
}


double inf_norm(const Matrix& A) {
  //This function takes in a matrix A and returns the inf norm of A: max row sum
  double max = 0.0;
  for (size_t i = 0; i < A.num_rows(); ++i){    // loop through each row

    double sum = 0.0;
    for (size_t j = 0; j < A.num_cols(); ++j){  // loop through each column for sum

      sum += std::abs(A(i,j));

    }
    max = std::max(sum,max);                    // determine if current sum is current max

  }
  return max;
}

double f_norm(const Matrix& A) {
    //This function takes in a matrix A and returns the Frobenius norm of A 

  double sum = 0.0;
  for (size_t i = 0; i < A.num_rows(); ++i){  // loop through each row

    for (size_t j = 0; j<A.num_cols(); ++j){  // loop through each column

      sum += std::pow(std::abs(A(i,j)),2);    // accumulate the square of each element

    }

  }
  return std::sqrt(sum);                      
}
