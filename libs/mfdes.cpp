#include <mfdes.hpp>

namespace NEquationSolver {
    double TMFDESRule::Fill(IEquationSolver const *const solver, usize i, usize j) {
        const double spaceX = solver->Space(i);

        if (i == j) {
            return (solver->CoefA(spaceX) + solver->CoefB(spaceX)) * solver->CoefGAlpha(1) - 1.0;
        } else if (i + 1 < j) {
            return solver->CoefB(spaceX) * solver->CoefGAlpha(j - i + 1);
        } else if (i > j + 1) {
            return solver->CoefA(spaceX) * solver->CoefGAlpha(i - j + 1);
        } else if (i == j + 1) {
            return solver->CoefA(spaceX) * solver->CoefGAlpha(2) + solver->CoefB(spaceX) - solver->CoefC(spaceX);
        } else {
            return solver->CoefA(spaceX) + solver->CoefB(spaceX) * solver->CoefGAlpha(2) + solver->CoefC(spaceX);
        }

        __builtin_unreachable();
    }
}