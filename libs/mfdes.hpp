#pragma once

#include <equation_solver.hpp>
#include <linalg/matrix.hpp>
#include <concepts>

namespace NEquationSolver {
    template<class T>
    concept TMatrixFillRuleConcept = requires (IEquationSolver const *const solver, const NLinalg::TMatrix& result, usize i, usize j) {
        requires !std::is_destructible_v<T>;
        requires !std::is_constructible_v<T>;
        { T::FillMatrix(solver, i, j) } -> std::convertible_to<double>;
        { T::FillDestination(solver, result, i, j) } -> std::convertible_to<double>;
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

namespace NEquationSolver {

    template<TMatrixFillRuleConcept TFiller = TMFDESRule>
    class TModifiedFDES : public IEquationSolver {
    protected:
        using IEquationSolver::TResult;
        using TMatrix = NLinalg::TMatrix;

    public:

        TModifiedFDES(const TSolverConfig& config) : IEquationSolver(config) {
            Validate();
        }

        TModifiedFDES(TSolverConfig&& config) : IEquationSolver(std::move(config)) {
            Validate();
        }

        void Validate() {
            auto range = std::ranges::views::iota(0ull, Config.SpaceCount + 1);
            usize i = *std::ranges::max_element(range.begin(), range.end(), [&](usize i, usize j){
                return Config.DiffusionCoefficient(i) < Config.DiffusionCoefficient(j);
            });

            double const left = Config.DiffusionCoefficient(i) * PowTCGamma / PowSCAlpha;
            double const right = Config.Gamma / Config.Alpha;
            DEBUG_LOG << "Left: " << left << " (" << i << ") Right: " << right << Endl;
            if (left > right) {
                WARNING_LOG << "May be problem with condition" << Endl
                            << "\t\tD * pow(tau, gamma) / pow(h, alpha): " << left << Endl
                            << "\t\tgamma/alpha: " << right << Endl;

                WARNING_LOG << "May make h >= " << std::pow(PowTCGamma * Config.DiffusionCoefficient(i) / right, 1.0/Config.Alpha) << Endl;
                WARNING_LOG << "Or tau <= " << std::pow(right * PowSCAlpha / Config.DiffusionCoefficient(i), 1.0/Config.Gamma) << Endl;
            }
        }

        virtual TResult Solve(bool saveMeta) override {
            INFO_LOG << "Start solving..." << Endl;
            const usize n = Config.SpaceCount;
            const usize k = Config.TimeCount;

            DEBUG_LOG << "n: " << n << " k: " << k << Endl;

            NLinalg::TMatrix result(k + 1, n + 1, 0.0);
            
            // # Math: u_i^0=\psi(x_i)
            for (usize i = 0; i <= n; i++) {
                result[0][i] = Config.ZeroTimeState(Space(i));
            }

            TMatrix A(n + 1);
            FillMatrix(A);

            auto plu = A.LUFactorizing();
            
            // # Math: Au^k=d^k
            for (usize t = 1; t <= k; t++) {
                // create d-vector
                std::vector<double> d(n + 1, 0.0);
                FillDestination(d, result, t);

                // solve system
                const auto r = TMatrix::Solve(plu, d).value();
                std::memcpy(result[t], r.data(), r.size() * sizeof(double));
            }

            TResult res = {
                .MethodName = TFiller::Name(),
                .Config = Config, 
                .Field = std::move(result),
            };

            if (saveMeta)
                res.SolveMatrix = std::move(A);

            return res;
        }

    private:

        void FillMatrix(TMatrix& A) {
            const usize n = Config.SpaceCount;

            // create matrix A
            for (usize i = 0; i <= n; i++) {
                for (usize j = 0; j <= n; j++) {
                    A[i][j] = TFiller::FillMatrix(this, i, j);
                }
            }

            //// 1-border conditions
            if (Config.BordersAvailable) {
                std::fill(A[0], A[1], 0.0);
                std::fill(A[n], &A[n][n + 1], 0.0);

                A[0][0] = 1.0;
                A[n][n] = 1.0;
            }
        }

        void FillDestination(std::vector<double>& d, const NLinalg::TMatrix& result, const usize k) {
            const usize n = Config.SpaceCount;
            const double time = Time(k);

            for (usize i = 0; i <= n; i++) {
                d[i] = TFiller::FillDestination(this, result, i, k);
            }

            // borders
            if (Config.BordersAvailable) {
                d[0] = Config.LeftBoundState(time);
                d[n] = Config.RightBoundState(time);
            }
        }
    };
}