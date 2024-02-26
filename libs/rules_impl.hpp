#pragma once

#include <equation_solver.hpp>

namespace NEquationSolver {
    template<class T>
    concept TMatrixFillRuleConcept = requires (IEquationSolver const *const solver, const NLinalg::TMatrix& result, usize i, usize j) {
        requires !std::is_destructible_v<T>;
        requires !std::is_constructible_v<T>;
        { T::FillMatrix(solver, i, j) } -> std::convertible_to<f64>;
        { T::FillDestination(solver, result, i, j) } -> std::convertible_to<f64>;
        { T::Name() } -> std::convertible_to<std::string>;
    };

    template<class T>
    concept TProbabilitiesFillRuleConcept = requires (IEquationSolver const *const solver, const NLinalg::TMatrix& probabilities, usize i, usize p) {
        requires !std::is_destructible_v<T>;
        requires !std::is_constructible_v<T>;
        { T::FillProbabilities(solver, probabilities, i, p) } -> std::convertible_to<f64>;
        { T::Name() } -> std::convertible_to<std::string>;
    };

    struct TMFDESRule {
        static constexpr std::string Name() { return "Grunwald-Letnikov approximation"; }; 
        static f64 FillMatrix(IEquationSolver const *const, usize, usize);
        static f64 FillDestination(IEquationSolver const *const, const NLinalg::TMatrix&, usize, usize);
        static f64 FillProbabilities(IEquationSolver const *const, const NLinalg::TMatrix&, usize, usize);
        TMFDESRule() = delete;
        ~TMFDESRule() = delete;
    };

    struct TRLFDESRule {
        static constexpr std::string Name() { return "Riemann-Liouville approximation"; };
        static f64 FillMatrix(IEquationSolver const *const, usize, usize);
        static f64 FillDestination(IEquationSolver const *const, const NLinalg::TMatrix&, usize, usize);
        static f64 FillProbabilities(IEquationSolver const *const, const NLinalg::TMatrix&, usize, usize);
        static f64 CoefGMatrix(IEquationSolver const *const, usize);
        static f64 CoefGDestination(IEquationSolver const *const, usize);
        TRLFDESRule() = delete;
        ~TRLFDESRule() = delete;
    };

    static_assert(TMatrixFillRuleConcept<TMFDESRule>, "Wrong type TMFDESRule for TMatrixFillRuleConcept");
    static_assert(TMatrixFillRuleConcept<TRLFDESRule>, "Wrong type TRLFDESRule for TMatrixFillRuleConcept");
    static_assert(TProbabilitiesFillRuleConcept<TMFDESRule>, "Wrong type TMFDESRule for TProbabilitiesFillRuleConcept");
    static_assert(TProbabilitiesFillRuleConcept<TRLFDESRule>, "Wrong type TRLFDESRule for TProbabilitiesFillRuleConcept");
}
