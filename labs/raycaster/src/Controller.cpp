#include "Controller.h"

#include "Polygon.h"
#include "Ray.h"
#include "Utils.h"

#include <QPointF>
#include <cmath>
#include <utility>
#include <vector>
#include <algorithm>

const std::vector<Polygon>& Controller::GetPolygons() const {
    return polygons_;
}

void Controller::AddPolygon(const Polygon& polygon) {
    polygons_.emplace_back(polygon);
}

void Controller::RemoveLastPolygon() {
    polygons_.pop_back();
}

void Controller::AddVertexToLastPolygon(const QPointF& vertex) {
    polygons_.back().AddVertex(vertex);
}

void Controller::UpdateLastPolygonVertex(const QPointF& vertex) {
    polygons_.back().UpdateLastVertex(vertex);
}

QPointF Controller::GetLightSource() const {
    return light_source_;
}

void Controller::SetLightSource(const QPointF& light_source) {
    light_source_ = light_source;
}

std::vector<Ray> Controller::CastRays() const {
    std::vector<Ray> rays;
    rays.reserve(polygons_.size() * 9);
    for (const auto& polygon : polygons_) {
        for (const auto& vertex : polygon.GetVertices()) {
            const auto ray =
                Ray(light_source_, vertex,
                    std::atan2(vertex.y() - light_source_.y(), vertex.x() - light_source_.x()));
            rays.emplace_back(ray);
            rays.emplace_back(ray.Rotate(+.000'1));
            rays.emplace_back(ray.Rotate(-.000'1));
        }
    }
    return std::move(rays);
}

void Controller::IntersectRays(std::vector<Ray>* rays) const {
    for (auto& ray : *rays) {
        for (const auto& polygon : polygons_) {
            const auto intersection = polygon.IntersectRay(ray);
            if (!intersection.has_value()) {
                continue;
            }
            const auto& begin = ray.GetBegin();
            const auto& end = ray.GetEnd();
            const auto& new_end = intersection.value();
            if (!Utils::IsLess(Utils::GetDistance(begin, new_end), Utils::GetDistance(begin, end))) {
                continue;
            }
            ray.SetEnd(new_end);
        }
    }
}

void Controller::RemoveAdjacentRays(std::vector<Ray>* rays) {
    std::ranges::sort(*rays, [](const auto& lhs, const auto& rhs) {
        return Utils::IsLess(lhs.GetAngle(), rhs.GetAngle());
    });
    int new_end = 1;
    for (int i = 1; i < std::ssize(*rays); i++) {
        if (!Utils::IsEqual((*rays)[i].GetEnd(), (*rays)[i - 1].GetEnd())) {
            (*rays)[new_end++] = (*rays)[i];
        }
    }
    while (std::ssize(*rays) > new_end) {
        rays->pop_back();
    }
}

Polygon Controller::CreateLightArea() const {
    auto rays = CastRays();
    IntersectRays(&rays);
    RemoveAdjacentRays(&rays);
    Polygon light_area{};
    for (const auto& ray : rays) {
        light_area.AddVertex(ray.GetEnd());
    }
    return std::move(light_area);
}

