#include "Controller.h"

const std::vector<Polygon>& Controller::GetPolygons() {
    return polygons_;
}

void Controller::AddPolygon(const Polygon& polygon) {
    polygons_.emplace_back(polygon);
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
