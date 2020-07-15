#include "amath583.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <random>

int main() {

    size_t i = 1;
    assert (i > 0);

    i = i-1;
    assert(i == 0);

    i = i - 1;
    assert(i < 0);

    return 0;

}