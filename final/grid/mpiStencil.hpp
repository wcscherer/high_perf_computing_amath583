//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH583_MPI_STENCIL_HPP
#define AMATH583_MPI_STENCIL_HPP

#include "Grid.hpp"
#include <mpi.h>

class mpiStencil { };


Grid mult(const mpiStencil& A, const Grid& x) {
  Grid y(x);

  size_t myrank = MPI::COMM_WORLD.Get_rank();
  size_t mysize = MPI::COMM_WORLD.Get_size();

  // Ghost cell (halo) update goes here
  // Boundaries are row 0 and row x.num_x() - 1 <= update these

  // don't do periodic boundary conditions
  /*
  MPI::COMM_WORLD.Recv(&x(0,0), x.num_y(), MPI::DOUBLE, myrank - 1, 321);
  MPI::COMM_WORLD.Recv(&x(x.num_x()-1,0), x.num_y(), MPI::DOUBLE, myrank + 1, 322);

  MPI::COMM_WORLD.Send(&x(1,0), x.num_y(), MPI::DOUBLE, myrank - 1, 322);
  MPI::COMM_WORLD.Send(&x(x.num_x()-1,0), x.num_y(), MPI::DOUBLE, myrank + 1, 321);
  */
  MPI::Request req_s_n, req_r_n, req_s_s, req_r_s;
if (myrank != 0) {
    req_r_n = MPI::COMM_WORLD.Irecv(const_cast<double*>(&x(0, 0)), x.num_y(), MPI::DOUBLE, myrank - 1, 321);
    req_s_n = MPI::COMM_WORLD.Isend(&x(1, 0), x.num_y(), MPI::DOUBLE, myrank - 1, 322);
    req_r_n.Wait();
    req_s_n.Wait();
  }

  if (myrank != mysize-1) {
    req_r_s = MPI::COMM_WORLD.Irecv(const_cast<double*>(&x(x.num_x()-1, 0)), x.num_y(), MPI::DOUBLE, myrank + 1, 322);
    req_s_s = MPI::COMM_WORLD.Isend(&x(x.num_x()-2, 0), x.num_y(), MPI::DOUBLE, myrank + 1, 321);
    req_r_s.Wait();
    req_s_s.Wait();
  }


  // SPMD stencil application
  for (size_t i = 1; i < x.num_x() - 1; ++i) {
    for (size_t j = 1; j < x.num_y() - 1; ++j) {
      y(i, j) = x(i, j) - (x(i - 1, j) + x(i + 1, j) + x(i, j - 1) + x(i, j + 1)) / 4.0;
    }
  }

  return y;
}


Grid operator*(const mpiStencil& A, const Grid& x) {
  return mult(A, x);
}


#endif // AMATH583_MPI_STENCIL_HPP
