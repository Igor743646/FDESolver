#include <mfdes.hpp>

namespace NEquationSolver {

    TModifiedFDES::TModifiedFDES(const TSolverConfig& config) : IEquationSolver(config) {}
    TModifiedFDES::TModifiedFDES(TSolverConfig&& config) : IEquationSolver(std::move(config)) {}


    TModifiedFDES::TResult TModifiedFDES::Solve() {
        TModifiedFDES::TResult result(Config.TimeCount + 1, Config.SpaceCount + 1, 0.0);
        const usize n = Config.SpaceCount;
        const usize k = Config.TimeCount;
        const double alpha = Config.Alpha;

        for (usize i = 0; i <= n; i++) {
            result[0][i] = Config.ZeroTimeState(Space(i));
        }

        NLinalg::TMatrix A(n + 1);

        // create matrix A
        for (usize i = 0; i <= n; i++) {
            for (usize j = 0; j <= n; j++) {
                if (i == j) {
                    A[i][j] = CoefA(Space(i)) * CoefG(alpha, 1) + CoefB(Space(i)) * CoefG(alpha, 1) - 1.0;
                } else if (i + 1 < j) {
                    A[i][j] = CoefB(Space(i)) * CoefG(alpha, j - i + 1);
                } else if (i > j + 1) {
                    A[i][j] = CoefA(Space(i)) * CoefG(alpha, i - j + 1);
                } else if (i == j + 1) {
                    A[i][j] = CoefA(Space(i)) * CoefG(alpha, 2) + CoefB(Space(i)) - CoefC(Space(i));
                } else {
                    A[i][j] = CoefA(Space(i)) + CoefB(Space(i)) * CoefG(alpha, 2) + CoefC(Space(i));
                } 
            }
        }

        //// 1-border conditions
        if (Config.BordersAvailable) {
            for (usize i = 0; i <= n; i++) {
                A[0][i] = 0.0;
                A[n][i] = 0.0;
            }

            A[0][0] = 1.0;
            A[n][n] = 1.0;
        }
        
        for (usize t = 1; t <= k; t++) {
            
            // create d-vector
            std::vector<double> d(n + 1, 0.0);

            for (usize i = 0; i <= n; i++) {
                for (usize j = 1; j <= t; j++) {
                    d[i] += CoefG(Config.Gamma, j) * (result[t-j][i] - result[0][i]);
                }
                d[i] -= result[0][i];
                d[i] -= std::pow(Config.TimeStep, Config.Gamma) * Config.SourceFunction(Space(i), Time(t));
            }

            //// borders
            if (Config.BordersAvailable) {
                d[0] = Config.LeftBoundState(Time(t));
                d[n] = Config.RightBoundState(Time(t));
            }
            
            // solve system
            auto r = A.Solve(d).value();
            
            for (usize i = 0; i < r.size(); i++) {
                result[t][i] = r[i];
            }
        }

        return result;
    }
}