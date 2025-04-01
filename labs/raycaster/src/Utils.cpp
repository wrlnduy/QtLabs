#include "utils.h"

#include <cmath>

bool utils::isEqual(const double& x, const double& y) {
    return std::fabs(x - y) <= k_eps;
}

bool utils::isLess(const double& x, const double& y) {
    return x - y < k_eps;
}

bool utils::isMore(const double& x, const double& y) {
    return x - y > k_eps;
}


