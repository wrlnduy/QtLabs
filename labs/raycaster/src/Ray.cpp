#include "Ray.h"

#include <QPointF>
#include <cmath>

Ray::Ray(const QPointF& begin, const QPointF& end, const double& angle)
    : begin_(begin), end_(end), angle_(angle) {
}

void Ray::SetBegin(const QPointF& begin) {
    begin_ = begin;
}

QPointF Ray::GetBegin() const {
    return begin_;
}

void Ray::SetEnd(const QPointF& end) {
    end_ = end;
}

QPointF Ray::GetEnd() const {
    return end_;
}

void Ray::SetAngle(const double& angle) {
    angle_ = angle;
}

double Ray::GetAngle() const {
    return angle_;
}

Ray Ray::Rotate(const double& angle) const {
    const QPointF end(
        begin_.x() + std::cos(angle_ + angle),
        begin_.y() + std::sin(angle_ + angle));
    return {begin_, end, angle_ + angle};
}
