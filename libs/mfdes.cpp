#include <mfdes.hpp>

namespace NEquationSolver {
    double TMFDESRule::FillMatrix(IEquationSolver const *const solver, usize i, usize j) {
        const double spaceX = solver->Space(i);
        double result = 0.0;

        if (i + 1 >= j) {
            result += solver->CoefA(spaceX) * solver->CoefGAlpha(i - j + 1);
        }

        if (i <= j + 1) {
            result += solver->CoefB(spaceX) * solver->CoefGAlpha(j - i + 1);
        }

        if (i == j) {
            result -= 1.0;
        }

        if (i == j + 1) {
            result -= solver->CoefC(spaceX);
        }

        if (i + 1 == j) {
            result += solver->CoefC(spaceX);
        }

        return result;
    }

    // # Math: d_i^k = \sum_{j=1}^{k}{g_{\gamma, j}(u_i^{k-j} - u_i^0)} - (u_i^0 + \tau^\gamma f(x_i, t_j))
    double TMFDESRule::FillDestination(IEquationSolver const *const solver, const NLinalg::TMatrix& result, usize i, usize k) {
        const usize n = solver->GetConfig().SpaceCount;
        const double time = solver->Time(k);

        double d_i = 0.0;

        d_i -= result[0][i];
        d_i -= solver->PowTCGamma * solver->GetConfig().SourceFunction(solver->Space(i), time);

        for (usize j = 1; j <= k; j++) {
            d_i += solver->CoefGGamma(j) * (result[k-j][i] - result[0][i]);
        }

        return d_i;
    }

    /*
        # Math: \theta_0 = \frac{1}{(2-\alpha)\Gamma(2-\alpha)2^{2-\alpha}}
        # Math: \theta_1 = \theta_0 * (3^{2-\alpha}-2)
        # Math: \theta_k = \theta_0 * ((2k+1)^{2-\alpha}-2(2k-1)^{2-\alpha}+(2k-3)^{2-\alpha})
    */
    double TRLFDESRule::CoefGMatrix(IEquationSolver const *const solver, usize k) {
        const double alpha = solver->GetConfig().Alpha;
        const double C0 = 1.0 / (NFunctions::Gamma(3.0 - alpha) * (std::pow(2.0, 2.0 - alpha)));
        
        if (k == 0) {
            return C0;
        } else if (k == 1) {
            return C0 * (std::pow(3.0, 2.0 - alpha) - 2.0);
        }
        return C0 * (std::pow(2.0 * k - 3.0, 2.0 - alpha) - 2.0 * std::pow(2.0 * k - 1.0, 2.0 - alpha) + std::pow(2.0 * k + 1.0, 2.0 - alpha));
    }

    double TRLFDESRule::FillMatrix(IEquationSolver const *const solver, usize i, usize j) {
        const double spaceX = solver->Space(i);
        const double n = solver->GetConfig().SpaceCount;
        double result = 0.0;

        if (i + 1 >= j) { // Left
            if (i + 1 == j) {
                result += solver->CoefA(spaceX) * CoefGMatrix(solver, 0);
            } else if (j == 0 && i > 0) {
                result -= solver->CoefA(spaceX) * CoefGMatrix(solver, i);
            } else {
                result += solver->CoefA(spaceX) * (CoefGMatrix(solver, i - j + 1) - CoefGMatrix(solver, i - j));
            }
        }

        if (i <= j + 1) { // Right
            if (i == j + 1) {
                result += solver->CoefB(spaceX) * CoefGMatrix(solver, 0);
            } else if (j == n && i < n) {
                result -= solver->CoefB(spaceX) * CoefGMatrix(solver, n - j);
            } else {
                result += solver->CoefB(spaceX) * (CoefGMatrix(solver, j - i + 1) - CoefGMatrix(solver, j - i));
            }
        }

        if (i == j) {
            result -= CoefGDestination(solver, 0);
        }

        if (i == j + 1) {
            result -= solver->CoefC(spaceX);
        }

        if (i + 1 == j) {
            result += solver->CoefC(spaceX);
        }

        return result;
    }

    /*
        # Math: \theta_0 = \frac{1}{(1-\alpha)\Gamma(1-\alpha)}
        # Math: \theta_k = \theta_0 * (k^{1-\alpha} - (k-1)^{1-\alpha})
    */
    double TRLFDESRule::CoefGDestination(IEquationSolver const *const solver, usize k) {
        const double gamma = solver->GetConfig().Gamma;
        const double C0 = 1.0 / NFunctions::Gamma(2.0 - gamma);
        
        if (k == 0) {
            return C0;
        } 
        
        return C0 * (std::pow(k, 1.0 - gamma) - std::pow(k - 1.0, 1.0 - gamma));
    }

    double TRLFDESRule::FillDestination(IEquationSolver const *const solver, const NLinalg::TMatrix& result, usize i, usize k) {
        const usize n = solver->GetConfig().SpaceCount;
        const double time = solver->Time(k);

        double d_i = 0.0;

        d_i -= solver->PowTCGamma * solver->GetConfig().SourceFunction(solver->Space(i), time);

        d_i -= CoefGDestination(solver, k) * result[0][i];
        for (usize j = 1; j < k; j++) {
            d_i += (CoefGDestination(solver, k-j+1) - CoefGDestination(solver, k-j)) * result[j][i];
        }

        return d_i;
    }
}