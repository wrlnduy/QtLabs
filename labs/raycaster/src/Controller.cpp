#include "Controller.h"

#include "Polygon.h"
#include "Ray.h"
#include "Utils.h"

#include <QPointF>
#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

const std::vector<Polygon>& Controller::GetPolygons() const {
    return polygons_;
}

void Controller::AddPolygon(const Polygon& polygon, PolygonType type) {
    polygons_.emplace_back(polygon);
    polygons_.back().SetType(type);
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

void Controller::SetLastPolygonType(PolygonType type) {
    polygons_.back().SetType(type);
}

QPointF Controller::GetLightSource() const {
    return light_source_;
}

void Controller::SetLightSource(const QPointF& light_source) {
    light_source_ = light_source;
}

std::vector<Ray> Controller::CastRays(const QPointF& light) const {
    std::vector<Ray> rays;
    rays.reserve(polygons_.size() * 9);
    for (const auto& polygon : polygons_) {
        for (const auto& vertex : polygon.GetVertices()) {
            const auto ray = Ray(light, vertex, Utils::GetAngle(light, vertex));
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
            if (!Utils::IsLess(
                    Utils::GetDistance(begin, new_end), Utils::GetDistance(begin, end))) {
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

Polygon Controller::CreateLightArea(const QPointF& light) const {
    auto rays = CastRays(light);
    IntersectRays(&rays);
    RemoveAdjacentRays(&rays);
    Polygon light_area{};
    for (const auto& ray : rays) {
        light_area.AddVertex(ray.GetEnd());
    }
    return std::move(light_area);
}

void Controller::Scale(const QPointF& scale) {
    for (auto& polygon : polygons_) {
        polygon.Scale(scale);
    }
    Utils::Scale(light_source_, scale);
}

bool Controller::IsTooClose(const QPointF& point, const double& k_max_dist) const {
    for (const auto& polygon : polygons_) {
        if (Utils::IsLess(polygon.GetDistance(point), k_max_dist)) {
            return true;
        }
    }
    return false;
}

const double& Controller::GetLightRadius() const {
    return kLightRadius;
}

const std::vector<QPointF>& Controller::GetDeltaLights() const {
    return kDeltaLights;
}
