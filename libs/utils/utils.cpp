#include <utils.hpp>
#include <cmath>
#include <limits>
#include <numbers>

namespace NFunctions {
    constexpr double Gamma(double x) {
        return x > 0 
            ? std::tgamma(x) 
            : -std::numbers::pi_v<double> / (x * std::sin(std::numbers::pi_v<double> * x) * std::tgamma(-x));
    }

    double Beta(double x, double y) {
        double res = Gamma(x) * Gamma(y) / Gamma(x + y);
        if (std::isnan(res)) {
            res = std::numeric_limits<double>::infinity();
        }
        return res;
    }
}
