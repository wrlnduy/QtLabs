#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Polygon.h"

#include <QPointF>
#include <vector>

class Controller {
   public:
    [[nodiscard]] const std::vector<Polygon>& GetPolygons();
    void AddPolygon(const Polygon&);
    void AddVertexToLastPolygon(const QPointF&);
    void UpdateLastPolygonVertex(const QPointF&);
    [[nodiscard]] QPointF GetLightSource() const;
    void SetLightSource(const QPointF&);

   private:
    std::vector<Polygon> polygons_{};
    QPointF light_source_{};
};

#endif