#ifndef UTILS_H
#define UTILS_H

#include <QPointF>

class Utils {
   public:
    static bool IsEqual(const double&, const double&);

    static bool IsEqual(const QPointF&, const QPointF&);

    static bool IsLess(const double&, const double&);

    static bool IsMore(const double&, const double&);

    static double GetDistance(const QPointF&, const QPointF&);

    static double GetAngle(const QPointF&, const QPointF&);

    static void Scale(QPointF&, const QPointF&);

    static double Sqr(const double&);

   private:
    static constexpr double kEps = 1e-9;
};

#endif  // UTILS_H
