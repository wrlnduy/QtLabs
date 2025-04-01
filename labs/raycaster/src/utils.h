#ifndef UTILS_H
#define UTILS_H

class utils {
   public:
    static bool isEqual(const double&, const double&);

    static bool isLess(const double&, const double&);

    static bool isMore(const double&, const double&);

   private:
    static constexpr double k_eps = 1e-9;
};

#endif  // UTILS_H
