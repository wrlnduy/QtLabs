#ifndef RAY_H
#define RAY_H

#include <QPointF>

class Ray {
   public:
    Ray() = default;
    Ray(const QPointF&, const QPointF&, const double&);

    void SetBegin(const QPointF&);

    [[nodiscard]] QPointF GetBegin() const;

    void SetEnd(const QPointF&);

    [[nodiscard]] QPointF GetEnd() const;

    void SetAngle(const double&);

    [[nodiscard]] double GetAngle() const;

   private:
    QPointF begin_{};
    QPointF end_{};
    double angle_{};
};

#endif