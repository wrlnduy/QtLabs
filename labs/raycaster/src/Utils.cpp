#include "Utils.h"

#include <QPointF>
#include <cmath>

bool Utils::IsEqual(const double& x, const double& y) {
    return std::fabs(x - y) <= kEps;
}

bool Utils::IsEqual(const QPointF& a, const QPointF& b) {
    return IsEqual(a.x(), b.x()) && IsEqual(a.y(), b.y());
}

bool Utils::IsLess(const double& x, const double& y) {
    return x - y < kEps;
}

bool Utils::IsMore(const double& x, const double& y) {
    return x - y > kEps;
}

double Utils::GetDistance(const QPointF& a, const QPointF& b) {
    return std::hypot(a.x() - b.x(), a.y() - b.y());
}

void Utils::Scale(QPointF& point, const QPointF& scale) {
    point = {point.x() * scale.x(), point.y() * scale.y()};
}
