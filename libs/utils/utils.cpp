#include <utils.hpp>
#include <cmath>
#include <limits>

namespace NFunctions {
    constexpr f64 Gamma(f64 x) {
        return x > 0 
            ? std::tgamma(x) 
            : -std::numbers::pi_v<f64> / (x * std::sin(std::numbers::pi_v<f64> * x) * std::tgamma(-x));
    }

    f64 Beta(f64 x, f64 y) {
        f64 res = Gamma(x) * Gamma(y) / Gamma(x + y);
        if (std::isnan(res)) {
            res = std::numeric_limits<f64>::infinity();
        }
        return res;
    }
}
