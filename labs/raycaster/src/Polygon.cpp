#include "Polygon.h"

#include "Ray.h"
#include "Utils.h"

#include <QPointF>
#include <cmath>
#include <cstddef>
#include <limits>
#include <optional>
#include <vector>

Polygon::Polygon(const std::vector<QPointF>& vertices) : vertices_(vertices) {
}

const std::vector<QPointF>& Polygon::GetVertices() const {
    return vertices_;
}

QPointF Polygon::GetVertex(const size_t& ind) const {
    if (ind == vertices_.size()) {
        return vertices_[0];
    }
    return vertices_.at(ind);
}

void Polygon::AddVertex(const QPointF& vertex) {
    vertices_.emplace_back(vertex);
}

void Polygon::UpdateLastVertex(const QPointF& vertex) {
    vertices_.back() = vertex;
}

std::optional<QPointF> Polygon::IntersectRay(const Ray& ray) {
    QPointF closest;
    double min_dist = std::numeric_limits<double>::max();
    bool found = false;

    const QPointF ray_begin = ray.GetBegin();
    const QPointF ray_dir = ray.GetEnd() - ray_begin;

    for (size_t i = 0; i < vertices_.size(); ++i) {
        const QPointF v1 = GetVertex(i);
        const QPointF v2 = GetVertex(i + 1);

        auto intersection = findIntersection(ray_begin, ray_dir, v1, v2);
        if (intersection.has_value()) {
            const double dist = std::hypot(
                intersection.value().x() - ray.GetBegin().x(),
                intersection.value().y() - ray.GetBegin().y());
            if (Utils::IsLess(dist, min_dist)) {
                min_dist = dist;
                closest = intersection.value();
                found = true;
            }
        }
    }

    if (found) {
        return closest;
    }

    return std::nullopt;
}

std::optional<QPointF> Polygon::findIntersection(
    const QPointF& ray_begin, const QPointF& ray_dir, const QPointF& edge_begin,
    const QPointF& edge_end) {
    const QPointF edge_dir = edge_end - edge_begin;
    const double det = (ray_dir.x() * edge_dir.y()) - (ray_dir.y() * edge_dir.x());

    if (Utils::IsEqual(det, .0)) {
        return std::nullopt;
    }

    const QPointF diff = edge_begin - ray_begin;
    const double t = (diff.x() * edge_dir.y() - diff.y() * edge_dir.x()) / det;
    const double s = (diff.x() * ray_dir.y() - diff.y() * ray_dir.x()) / det;

    if (!Utils::IsLess(t, .0) && !Utils::IsLess(s, .0) && !Utils::IsMore(s, 1.)) {
        return ray_begin + ray_dir * t;
    }
    return std::nullopt;
}
