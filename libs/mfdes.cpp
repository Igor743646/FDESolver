#include <mfdes.hpp>

namespace NEquationSolver {
    double TMFDESRule::Fill(IEquationSolver const *const solver, usize i, usize j) {
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

    double TRLFDESRule::CoefG(IEquationSolver const *const solver, usize k) {
        const double alpha = solver->GetConfig().Alpha;
        const double C0 = 1.0 / (NFunctions::Gamma(3.0 - alpha) * (std::pow(2.0, 2.0 - alpha)));
        
        if (k == 0) {
            return C0;
        } else if (k == 1) {
            return C0 * (std::pow(3.0, 2.0 - alpha) - 2.0);
        }
        return C0 * (std::pow(2.0 * k - 3.0, 2.0 - alpha) - 2.0 * std::pow(2.0 * k - 1.0, 2.0 - alpha) + std::pow(2.0 * k + 1.0, 2.0 - alpha));
    }

    double TRLFDESRule::Fill(IEquationSolver const *const solver, usize i, usize j) {
        const double spaceX = solver->Space(i);
        const double n = solver->GetConfig().SpaceCount;
        double result = 0.0;

        if (i + 1 >= j) { // Left
            if (i + 1 == j) {
                result += solver->CoefA(spaceX) * CoefG(solver, 0);
            } else if (j == 0 && i > 0) {
                result -= solver->CoefA(spaceX) * CoefG(solver, i);
            } else {
                result += solver->CoefA(spaceX) * (CoefG(solver, i - j + 1) - CoefG(solver, i - j));
            }
        }

        if (i <= j + 1) { // Right
            if (i == j + 1) {
                result += solver->CoefB(spaceX) * CoefG(solver, 0);
            } else if (j == n && i < n) {
                result -= solver->CoefB(spaceX) * CoefG(solver, n - j);
            } else {
                result += solver->CoefB(spaceX) * (CoefG(solver, j - i + 1) - CoefG(solver, j - i));
            }
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
}