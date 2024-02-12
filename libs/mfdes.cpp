#include <mfdes.hpp>
#include <omp.h>

namespace NEquationSolver {

    using NLinalg::TMatrix;

    TModifiedFDES::TModifiedFDES(const TSolverConfig& config) : IEquationSolver(config) {}
    TModifiedFDES::TModifiedFDES(TSolverConfig&& config) : IEquationSolver(std::move(config)) {}

    void TModifiedFDES::FillMatrix(TMatrix& A) {
        const usize n = Config.SpaceCount;

        // create matrix A
        for (usize i = 0; i <= n; i++) {
            const double spaceX = Space(i);
            const double coefA = CoefA(spaceX);
            const double coefB = CoefB(spaceX);
            const double coefC = CoefC(spaceX);

            for (usize j = 0; j <= n; j++) {
                if (i == j) {
                    A[i][j] = (coefA + coefB) * CoefGAlpha(1) - 1.0;
                } else if (i + 1 < j) {
                    A[i][j] = coefB * CoefGAlpha(j - i + 1);
                } else if (i > j + 1) {
                    A[i][j] = coefA * CoefGAlpha(i - j + 1);
                } else if (i == j + 1) {
                    A[i][j] = coefA * CoefGAlpha(2) + coefB - coefC;
                } else {
                    A[i][j] = coefA + coefB * CoefGAlpha(2) + coefC;
                } 
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

    void TModifiedFDES::FillDestination(std::vector<double>& d, const NLinalg::TMatrix& result, const usize t) {
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

    TModifiedFDES::TResult TModifiedFDES::Solve(bool saveMeta) {
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
}