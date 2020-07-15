//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH583_CSCMATRIX_HPP
#define AMATH583_CSCMATRIX_HPP

#include "Vector.hpp"
#include <algorithm>
#include <cassert>
#include <vector>

class CSCMatrix {
public:
  CSCMatrix(size_t M, size_t N) : is_open(false), num_rows_(M), num_cols_(N), col_indices_(num_cols_ + 1, 0) {}

  void open_for_push_back() { is_open = true; }

  void close_for_push_back() {
    is_open = false;
    for (size_t i = 0; i < num_cols_; ++i) {
      col_indices_[i + 1] += col_indices_[i];
    }
    for (size_t i = num_cols_; i > 0; --i) {
      col_indices_[i] = col_indices_[i - 1];
    }
    col_indices_[0] = 0;
  }

  void push_back(size_t i, size_t j, double value) { /* Write Me */
    assert(is_open);
    assert(i < num_rows_ && i >= 0);
    assert(j < num_cols_ && j >= 0);

    ++col_indices_[j];
    row_indices_.push_back(i);
    storage_.push_back(value);
  }

  void clear() {
    row_indices_.clear();
    storage_.clear();
    std::fill(col_indices_.begin(), col_indices_.end(), 0);
  }

  size_t num_rows() const { return num_rows_; }
  size_t num_cols() const { return num_cols_; }
  size_t num_nonzeros() const { return storage_.size(); }

  void stream_coordinates(std::ostream& output_file) const { /* Write Me */
    for (size_t i = 0; i < num_cols_; ++i) {
      for (size_t j = col_indices_[i]; j < col_indices_[i + 1]; ++j) {
        output_file << row_indices_[j] << " ";
        output_file << i << " ";
        output_file << storage_[j];
        output_file << std::endl;
      }
    }
  }

  void matvec(const Vector& x, Vector& y) const {
    // For CSC, traverse each element (row_index) within a column and the pointers defining when a new column starts is determined by the number of cols
    // same as t_matvec for CSR implementation
      for (size_t i = 0; i < num_cols_; ++i) {

        for (size_t j = col_indices_[i]; j < col_indices_[i + 1]; ++j) {

          size_t col = i;

          size_t row = row_indices_[j];
          
          double val = storage_[j];

          y(row) += val * x(col);

      }

    }
    
  }

  void t_matvec(const Vector& x, Vector& y) const {
   //same as matvec for CSR - CSC is the equialent of A^T for CSR of A
    
    for (size_t i = 0; i < num_rows_; ++i) {
        for (size_t j = col_indices_[i]; j < col_indices_[i + 1]; ++j) {
            y(i) += storage_[j] * x(row_indices_[i]);
         }
      }
    
  }

  void matmat(const Matrix& B, Matrix& C) const {
    // Extract row indices by looping through num cols
    for (size_t m = 0; m < num_cols_; ++m){ 
      for (size_t j = 0; j < C.num_cols(); ++j){
      
        size_t rA  = row_indices_[m];
        size_t cA  = m;
        double val = storage_[m];

        C(rA,j) += val*B(cA,j);

      }
    }
  }

private:
  bool                is_open;
  size_t              num_rows_, num_cols_;
  std::vector<size_t> col_indices_, row_indices_;
  std::vector<double> storage_;
};

#endif    // AMATH583_CSCMATRIX_HPP
