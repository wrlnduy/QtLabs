#ifndef POLYGON_H
#define POLYGON_H

#include "Ray.h"

#include <QPointF>
#include <cstddef>
#include <optional>
#include <vector>

class Polygon {
   public:
    Polygon() = default;
    explicit Polygon(const std::vector<QPointF>&);

    [[nodiscard]] std::vector<QPointF> GetVertices() const;

    [[nodiscard]] QPointF GetVertex(const size_t&) const;

    void AddVertex(const QPointF&);

    void UpdateLastVertex(const QPointF&);

    [[nodiscard]] std::optional<QPointF> IntersectRay(const Ray&);

   private:
    std::vector<QPointF> vertices_{};

    std::optional<QPointF> findIntersection(
        const QPointF&, const QPointF&, const QPointF&, const QPointF&);
};

#endif  // POLYGON_H
