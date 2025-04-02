#include "Utils.h"

#include <cmath>

bool Utils::IsEqual(const double& x, const double& y) {
    return std::fabs(x - y) <= k_eps;
}

bool Utils::IsLess(const double& x, const double& y) {
    return x - y < k_eps;
}

bool Utils::IsMore(const double& x, const double& y) {
    return x - y > k_eps;
}


