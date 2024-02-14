#pragma once

#include <equation_solver.hpp>
#include <linalg/matrix.hpp>
#include <concepts>

namespace NEquationSolver {
    template<class T>
    concept TMatrixFillRuleConcept = requires (IEquationSolver const *const solver, usize i, usize j) {
        requires !std::is_destructible_v<T>;
        requires !std::is_constructible_v<T>;
        { T::Fill(solver, i, j) } -> std::convertible_to<double>;
    };

    struct TMFDESRule {
        static double Fill(IEquationSolver const *const solver, usize i, usize j);
        TMFDESRule() = delete;
        ~TMFDESRule() = delete;
    };

    static_assert(TMatrixFillRuleConcept<TMFDESRule>, "Wrong type TMFDESRule for TMatrixFillRuleConcept");
}

namespace NEquationSolver {

    template<TMatrixFillRuleConcept TFiller = TMFDESRule>
    class TModifiedFDES : public IEquationSolver {
    protected:
        using IEquationSolver::TResult;
        using TMatrix = NLinalg::TMatrix;

    public:

        TModifiedFDES(const TSolverConfig& config) : IEquationSolver(config) {}
        TModifiedFDES(TSolverConfig&& config) : IEquationSolver(std::move(config)) {}

        virtual TResult Solve(bool saveMeta) override {
            INFO_LOG << "Start solving..." << Endl;
            const usize n = Config.SpaceCount;
            const usize k = Config.TimeCount;

            DEBUG_LOG << "n: " << n << " k: " << k << Endl;

            NLinalg::TMatrix result(k + 1, n + 1, 0.0);
            
            for (usize i = 0; i <= n; i++) {
                result[0][i] = Config.ZeroTimeState(Space(i));
            }

            TMatrix A(n + 1);
            FillMatrix(A);

            auto plu = A.LUFactorizing();
            
            for (usize t = 1; t <= k; t++) {
                // create d-vector
                std::vector<double> d(n + 1, 0.0);
                FillDestination(d, result, t);

                // solve system
                const auto r = TMatrix::Solve(plu, d).value();
                std::memcpy(result[t], r.data(), r.size() * sizeof(double));
            }

            TResult res = {
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
                    A[i][j] = TFiller::Fill(this, i, j);
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

        void FillDestination(std::vector<double>& d, const NLinalg::TMatrix& result, const usize t) {
            const usize n = Config.SpaceCount;
            const double coef = PowTCGamma;
            const double time = Time(t);

            for (usize i = 0; i <= n; i++) {
                d[i] -= result[0][i];
                d[i] -= coef * Config.SourceFunction(Space(i), time);
            }

            for (usize j = 1; j <= t; j++) {
                for (usize i = 0; i <= n; i++) {
                    d[i] += CoefGGamma(j) * (result[t-j][i] - result[0][i]);
                }
            }

            //// borders
            if (Config.BordersAvailable) {
                d[0] = Config.LeftBoundState(time);
                d[n] = Config.RightBoundState(time);
            }
        }
    };
}