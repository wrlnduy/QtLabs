#ifndef UTILS_H
#define UTILS_H

class Utils {
   public:
    static bool IsEqual(const double&, const double&);

    static bool IsLess(const double&, const double&);

    static bool IsMore(const double&, const double&);

   private:
    static constexpr double k_eps = 1e-9;
};

#endif  // UTILS_H
