#pragma once

#include <equation_solver.hpp>
#include <rules_impl.hpp>
#include <linalg/matrix.hpp>
#include <utils/utils.hpp>
#include <omp.h>

template<class T>
usize bin_search(const std::vector<T>& v, T k) {
        usize l = 0, r = v.size() - 1, mid;

    while (r - l > 1) {

        mid = (r + l) / 2;

        if (k < v[mid]) {
            r = mid;
        }
        else if (k > v[mid]) {
            l = mid;
        }
        else {
            l = mid;
            r = mid;
        }

    }

    return k <= v[l] ? l : r;
}

namespace NEquationSolver {

    template<TMatrixFillRuleConcept TFiller = TMFDESRule>
    class TStochasticFDES : public IEquationSolver {
    protected:
        using IEquationSolver::TResult;
        using TMatrix = NLinalg::TMatrix;

        std::vector<std::vector<double>> Probabilities;

    public: 

        TStochasticFDES(const TSolverConfig& config) : IEquationSolver(config) {}
        TStochasticFDES(TSolverConfig&& config) :  IEquationSolver(std::move(config)) {}

        virtual std::string Name() {
            return "Stochastic method with " + TFiller::Name();
        }

        virtual TResult DoSolve(bool saveMeta) override {
            INFO_LOG << "Start solving Stochastic fractional-derivative equation solver..." << Endl;
            const usize n = Config.SpaceCount;
            const usize k = Config.TimeCount;
            const usize count = Config.StochasticIterationCount;

            DEBUG_LOG << std::format("n: {} k: {} count: {}", n, k, count) << Endl;

            NLinalg::TMatrix result(k + 1, n + 1, 0.0);
            
            MakeProb();
            std::vector<std::vector<double>> prefsumProbs(Probabilities.size(), std::vector<double>(Probabilities[0].size(), 0.0));
            for (usize i = 0; i <= n; i++) {
                std::inclusive_scan(Probabilities[i].begin(), Probabilities[i].end(), prefsumProbs[i].begin());
            }

            // Учёт начального и граничных условий

            for (usize i = 0; i <= n; i++) {
                result[0][i] = ZeroTimeState[i];
            }

            for (usize j = 1; j <= k; j++) {
                result[j][0] = LeftBoundState[j];
                result[j][n] = RightBoundState[j];
            }

            // Симуляция
            std::random_device device;
            std::mt19937_64 engine(device());
            std::uniform_real_distribution<f64> generator(0.0, 1.0);

            for (i64 i = 1; i < n; i++) {
                #pragma omp parallel for
                for (i64 j = 1; j <= k; j++) {
                    for (i64 _n = 0; _n < count; _n++) {
                        i64 x = i, y = j;

                        while (y > 0 && x < n && x > 0) {
                            f64 rnd = generator(engine);
                            i64 idx = 0;

                            idx = std::lower_bound(prefsumProbs[x].begin(), prefsumProbs[x].end(), rnd) - prefsumProbs[x].begin();

                            result[j][i] += SourceFunction[y][x] * PowTCGamma;

                            if (idx <= 2 * n) { // перемещение по пространству
                                x += n - idx;
                                y--;
                            } else if (idx <= 2 * n + k) { // перемещение по времени
                                y -= idx - 2 * n + 1;
                            } else {
                                // x = n;
                                // y = 1;
                                break;
                            }
                        }

                        if (y <= 0 && (x >= 0) && (x <= n)) {
                            result[j][i] += ZeroTimeState[x];
                        } else if (Config.BordersAvailable) {
                            if (x <= 0 && y > 0) {
                                result[j][i] += LeftBoundState[y];
                            } else if (x >= n && y > 0) {
                                result[j][i] += RightBoundState[y];
                            }
                        }
                    }

                    result[j][i] /= static_cast<f64>(count);
                }
            }

            TResult res = {
                .MethodName = Name(),
                .Config = Config, 
                .Field = std::move(result),
            };

            return res;
        }

    private:

        void MakeProb() {
            const usize n = Config.SpaceCount;
            const usize k = Config.TimeCount;
            const double alpha = Config.Alpha;
            const double gamma = Config.Gamma;
            Probabilities.resize(n + 1);

            for (usize i = 0; i <= n; i++) {
                // Math: p[0] = p_{-i}, ..., p[n] = p_0, ..., p[2n] = p_{i}, p[2n+1] = q_{2}, ..., p[2n+k] = q_{j+k+1}
                Probabilities[i].resize(2 * n + 2 + k, 0.0);

                double a00 = CoefA(Space(i)), b00 = CoefB(Space(i)), c00 = CoefC(Space(i));

                Probabilities[i][n + 1] = a00 * GAlpha[2] + b00 - c00;
                Probabilities[i][n - 1] = a00 + b00 * GAlpha[2] + c00;
                Probabilities[i][n] = gamma - alpha * (a00 + b00);
                for (usize j = 2; j <= n; j++) {
                    Probabilities[i][n + j] = a00 * GAlpha[j + 1];
                    Probabilities[i][n - j] = b00 * GAlpha[j + 1];
                }

                for (usize j = 1; j <= k; j++) {
                    Probabilities[i][2 * n + j] = -GGamma[j + 1];
                }

                Probabilities[i][2 * n + 1 + k] = 0.0;
                Probabilities[i][2 * n + 1 + k] = 1.0 - std::accumulate(Probabilities[i].begin(), Probabilities[i].end(), 0.0);
            }
        }
    };
}
