//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <iostream>
#include <string>
#include <mpi.h>

int main(int argc, char* argv[]) {
  MPI::Init();

  int token     = 0;
  size_t rounds = 1;
  if (argc >= 2) rounds = std::stol(argv[1]);

  int myrank = MPI::COMM_WORLD.Get_rank();
  int mysize = MPI::COMM_WORLD.Get_size();

  while (rounds--) {
    if (0 == myrank) {
      int left  = myrank + 1;  // Fix me
      int right = (myrank - 1) % mysize;  // And me - adjust using mod for "periodic" motion
      std::cout << myrank << ": sending  " << token << std::endl;
      MPI::COMM_WORLD.Send(&token, 1, MPI::INT, left, 321);
      MPI::COMM_WORLD.Recv(&token, 1, MPI::INT, right, 321);
      std::cout << myrank << ": received " << token << std::endl;
      ++token;
    } else {
      int left  = (myrank + 1) % mysize;  // Fix me same as int right above except + 1
      int right = myrank - 1;  // And me same as left above except -1
      MPI::COMM_WORLD.Recv(&token, 1, MPI::INT, left, 321);
      std::cout << myrank << ": received " << token << std::endl;
      ++token;
      std::cout << myrank << ": sending  " << token << std::endl;
      MPI::COMM_WORLD.Send(&token, 1, MPI::INT, right, 321);
    }
  }

  MPI::Finalize();

  return 0;
}
