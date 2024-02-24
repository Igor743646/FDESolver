#pragma once

#include <equation_solver.hpp>

namespace NEquationSolver {
    template<class T>
    concept TMatrixFillRuleConcept = requires (IEquationSolver const *const solver, const NLinalg::TMatrix& result, usize i, usize j) {
        requires !std::is_destructible_v<T>;
        requires !std::is_constructible_v<T>;
        { T::FillMatrix(solver, i, j) } -> std::convertible_to<double>;
        { T::FillDestination(solver, result, i, j) } -> std::convertible_to<double>;
        { T::Name() } -> std::convertible_to<std::string>;
    };

    struct TMFDESRule {
        static constexpr std::string Name() { return "Grunwald-Letnikov approximation"; }; 
        static double FillMatrix(IEquationSolver const *const solver, usize i, usize j);
        static double FillDestination(IEquationSolver const *const solver, const NLinalg::TMatrix& result, usize i, usize k);
        TMFDESRule() = delete;
        ~TMFDESRule() = delete;
    };

    struct TRLFDESRule {
        static constexpr std::string Name() { return "Riemann-Liouville approximation"; };
        static double FillMatrix(IEquationSolver const *const solver, usize i, usize j);
        static double FillDestination(IEquationSolver const *const solver, const NLinalg::TMatrix& result, usize i, usize k);
        static double CoefGMatrix(IEquationSolver const *const solver, usize k);
        static double CoefGDestination(IEquationSolver const *const solver, usize k);
        TRLFDESRule() = delete;
        ~TRLFDESRule() = delete;
    };

    static_assert(TMatrixFillRuleConcept<TMFDESRule>, "Wrong type TMFDESRule for TMatrixFillRuleConcept");
    static_assert(TMatrixFillRuleConcept<TRLFDESRule>, "Wrong type TRLFDESRule for TMatrixFillRuleConcept");
}
