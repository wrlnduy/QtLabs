#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Polygon.h"
#include "Ray.h"

#include <QPointF>
#include <vector>

class Controller {
   public:
    [[nodiscard]] const std::vector<Polygon>& GetPolygons() const;
    void AddPolygon(const Polygon&);
    void RemoveLastPolygon();
    void AddVertexToLastPolygon(const QPointF&);
    void UpdateLastPolygonVertex(const QPointF&);
    [[nodiscard]] QPointF GetLightSource() const;
    void SetLightSource(const QPointF&);
    [[nodiscard]] std::vector<Ray> CastRays() const;
    void IntersectRays(std::vector<Ray>* rays) const;
    static void RemoveAdjacentRays(std::vector<Ray>* rays) ;
    Polygon CreateLightArea() const;

   private:
    std::vector<Polygon> polygons_{};
    QPointF light_source_{};
};

#endif