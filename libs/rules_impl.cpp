#include <rules_impl.hpp>

namespace NEquationSolver {
    f64 TMFDESRule::FillMatrix(IEquationSolver const *const solver, usize i, usize j) {
        f64 result = 0.0;

        if (i + 1 >= j) {
            result += solver->CoefA(i) * solver->CoefGAlpha(i - j + 1);
        }

        if (i <= j + 1) {
            result += solver->CoefB(i) * solver->CoefGAlpha(j - i + 1);
        }

        if (i == j) {
            result -= 1.0;
        }

        if (i == j + 1) {
            result -= solver->CoefC(i);
        }

        if (i + 1 == j) {
            result += solver->CoefC(i);
        }

        return result;
    }

    // Math: d_i^k = \sum_{j=1}^{k}{g_{\gamma, j}(u_i^{k-j} - u_i^0)} - (u_i^0 + \tau^\gamma f(x_i, t_j))
    f64 TMFDESRule::FillDestination(IEquationSolver const *const solver, const NLinalg::TMatrix& result, usize i, usize k) {
        const usize n = solver->GetConfig().SpaceCount;

        f64 d_i = 0.0;

        d_i -= result[0][i];
        d_i -= solver->PowTCGamma * solver->SourceFunction[k][i];

        for (usize j = 1; j <= k; j++) {
            d_i += solver->CoefGGamma(j) * (result[k-j][i] - result[0][i]);
        }

        return d_i;
    }

    f64 TMFDESRule::FillProbabilities(IEquationSolver const *const solver, const NLinalg::TMatrix& probabilities, usize i, usize p) {
        const usize n = solver->GetConfig().SpaceCount;
        const usize k = solver->GetConfig().TimeCount;
        const double alpha = solver->GetConfig().Alpha;
        const double gamma = solver->GetConfig().Gamma;

        if (p < n - 1) {
            return solver->CoefB(i) * solver->CoefGAlpha(n - p + 1); 
        } 
        if (p == n - 1) {
            return solver->CoefA(i) + solver->CoefB(i) * solver->CoefGAlpha(2) + solver->CoefC(i);
        } 
        if (p == n) {
            return gamma - alpha * (solver->CoefA(i) + solver->CoefB(i));
        } 
        if (p == n + 1) {
            return solver->CoefA(i) * solver->CoefGAlpha(2) + solver->CoefB(i) - solver->CoefC(i);
        } 
        if (n + 1 < p && p <= 2 * n) {
            return solver->CoefA(i) * solver->CoefGAlpha(p - n + 1);
        } 
        if (2 * n < p && p <= 2 * n + k) {
            return -solver->CoefGGamma(p - 2 * n + 1);
        } 

        return 1.0 - std::accumulate(probabilities[i - 1], probabilities[i], 0.0);
    }

    /*
        // Math: \theta_0 = \frac{1}{(2-\alpha)\Gamma(2-\alpha)2^{2-\alpha}}
        // Math: \theta_1 = \theta_0 * (3^{2-\alpha}-2)
        // Math: \theta_k = \theta_0 * ((2k+1)^{2-\alpha}-2(2k-1)^{2-\alpha}+(2k-3)^{2-\alpha})
    */
    f64 TRLFDESRule::CoefGMatrix(IEquationSolver const *const solver, usize k) {
        const f64 alpha = solver->GetConfig().Alpha;
        const f64 C0 = 1.0 / (NFunctions::Gamma(3.0 - alpha) * (std::pow(2.0, 2.0 - alpha)));
        
        if (k == 0) {
            return C0;
        } else if (k == 1) {
            return C0 * (std::pow(3.0, 2.0 - alpha) - 2.0);
        }
        return C0 * (std::pow(2.0 * k - 3.0, 2.0 - alpha) - 2.0 * std::pow(2.0 * k - 1.0, 2.0 - alpha) + std::pow(2.0 * k + 1.0, 2.0 - alpha));
    }

    f64 TRLFDESRule::FillMatrix(IEquationSolver const *const solver, usize i, usize j) {
        const f64 n = solver->GetConfig().SpaceCount;
        f64 result = 0.0;

        if (i + 1 >= j) { // Left
            if (i + 1 == j) {
                result += solver->CoefA(i) * CoefGMatrix(solver, 0);
            } else if (j == 0 && i > 0) {
                result -= solver->CoefA(i) * CoefGMatrix(solver, i);
            } else {
                result += solver->CoefA(i) * (CoefGMatrix(solver, i - j + 1) - CoefGMatrix(solver, i - j));
            }
        }

        if (i <= j + 1) { // Right
            if (i == j + 1) {
                result += solver->CoefB(i) * CoefGMatrix(solver, 0);
            } else if (j == n && i < n) {
                result -= solver->CoefB(i) * CoefGMatrix(solver, n - i);
            } else {
                result += solver->CoefB(i) * (CoefGMatrix(solver, j - i + 1) - CoefGMatrix(solver, j - i));
            }
        }

        if (i == j) {
            result -= CoefGDestination(solver, 0);
        }

        if (i == j + 1) {
            result -= solver->CoefC(i);
        }

        if (i + 1 == j) {
            result += solver->CoefC(i);
        }

        return result;
    }

    /*
        // Math: \theta_0 = \frac{1}{(1-\gamma)\Gamma(1-\gamma)}
        // Math: \theta_k = \theta_0 * (k^{1-\gamma} - (k-1)^{1-\gamma})
    */
    f64 TRLFDESRule::CoefGDestination(IEquationSolver const *const solver, usize k) {
        const f64 gamma = solver->GetConfig().Gamma;
        const f64 C0 = 1.0 / NFunctions::Gamma(2.0 - gamma);
        
        if (k == 0) {
            return C0;
        } 
        
        return C0 * (std::pow(k, 1.0 - gamma) - std::pow(k - 1.0, 1.0 - gamma));
    }

    // Math: d_i^k = -\theta_{k}u_i^0 + \sum_{j=1}^{k-1}{(\theta_{k-j+1}-\theta_{k-j})u_i^{j}} - \tau^\gamma f(x_i, t_j)
    f64 TRLFDESRule::FillDestination(IEquationSolver const *const solver, const NLinalg::TMatrix& result, usize i, usize k) {
        const usize n = solver->GetConfig().SpaceCount;

        f64 d_i = 0.0;

        d_i -= CoefGDestination(solver, k) * result[0][i];
        for (usize j = 1; j < k; j++) {
            d_i += (CoefGDestination(solver, k-j+1) - CoefGDestination(solver, k-j)) * result[j][i];
        }

        d_i -= solver->PowTCGamma * solver->SourceFunction[k][i];
        
        return d_i;
    }

    f64 TRLFDESRule::FillProbabilities(IEquationSolver const *const solver, const NLinalg::TMatrix& probabilities, usize i, usize p) {
        const usize n = solver->GetConfig().SpaceCount;
        const usize k = solver->GetConfig().TimeCount;
        const double alpha = solver->GetConfig().Alpha;
        const double gamma = solver->GetConfig().Gamma;

        if (p == n) {
            return ((CoefGMatrix(solver, 1) - CoefGMatrix(solver, 0)) * (solver->CoefA(i) + solver->CoefB(i)) + (CoefGDestination(solver, 0) - CoefGDestination(solver, 2))) / CoefGDestination(solver, 0);
        }
        if (p == n - 1) {
            double result = (CoefGMatrix(solver, 0) * solver->CoefA(i) + (CoefGMatrix(solver, 2) - CoefGMatrix(solver, 1)) * solver->CoefB(i) + solver->CoefC(i)) / CoefGDestination(solver, 0);
            if (i == n - 1) {
                result -= (CoefGMatrix(solver, 2) * solver->CoefB(i)) / CoefGDestination(solver, 0);
            }
            return result;
        }
        if (p == n + 1) {
            double result = ((CoefGMatrix(solver, 2) - CoefGMatrix(solver, 1)) * solver->CoefA(i) + CoefGMatrix(solver, 0) * solver->CoefB(i) - solver->CoefC(i)) / CoefGDestination(solver, 0);
            if (i == 1) {
                result -= (CoefGMatrix(solver, 2) * solver->CoefA(i)) / CoefGDestination(solver, 0);
            }
            return result;
        } 

        if (i <= p && p < n - 1) {
            if (p == i) {
                return -CoefGMatrix(solver, n - p) * solver->CoefB(i) / CoefGDestination(solver, 0); 
            }
            return (CoefGMatrix(solver, n - p + 1) - CoefGMatrix(solver, n - p)) * solver->CoefB(i) / CoefGDestination(solver, 0); 
        }

        if (n + 1 < p && p <= n + i) {
            if (p == n + i) {
                return -CoefGMatrix(solver, p - n) * solver->CoefA(i) / CoefGDestination(solver, 0);
            }
            return (CoefGMatrix(solver, p - n + 1) - CoefGMatrix(solver, p - n)) * solver->CoefA(i) / CoefGDestination(solver, 0);
        }

        if (2 * n < p && p <= 2 * n + k) {
            if (p == 2 * n + k) {
                return CoefGDestination(solver, p - 2 * n + 2) / CoefGDestination(solver, 0);
            }
            return (CoefGDestination(solver, p - 2 * n + 1) - CoefGDestination(solver, p - 2 * n + 2)) / CoefGDestination(solver, 0);
        }

        if (p == 2 * n + k + 1) {
            return 1.0 - std::accumulate(probabilities[i - 1], probabilities[i], 0.0);
        }

        return 0.0;
    }
}
