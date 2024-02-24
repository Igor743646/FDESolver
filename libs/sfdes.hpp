#pragma once

#include <equation_solver.hpp>
#include <rules_impl.hpp>
#include <linalg/matrix.hpp>
#include <numeric>
#include <random>

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
                INFO_LOG << prefsumProbs[i] << Endl;
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
            std::uniform_real_distribution<double> generator(0.0, 1.0);

            for (usize i = 1; i < n; i++) {
                for (usize j = 1; j <= k; j++) {
                    for (usize _n = 0; _n < count; _n++) {
                        long long x = i, y = j;

                        while (y > 0 && x < n && x > 0) {
                            double rnd = generator(engine);
                            long long idx = 0;

                            idx = std::lower_bound(prefsumProbs[x].begin(), prefsumProbs[x].end(), rnd) - prefsumProbs[x].begin();
                            if (idx > 2 * n + k + 1) {
                                ERROR_LOG << std::format("LB founded {} while size: {}", idx, prefsumProbs[0].size()) << Endl;
                            }

                            result[j][i] += SourceFunction[y][x] * PowTCGamma;

                            if (idx <= 2 * n) { // перемещение по пространству
                                x += idx - n;
                                y--;
                            } else if (idx <= 2 * n + k) { // перемещение по времени
                                y -= idx - 2 * n + 1;
                            } else {
                                break;
                            }
                        }

                        if (y <= 0 && (x >= 0) && (x <= n)) {
                            result[j][i] += ZeroTimeState[x];
                        } else if (Config.BordersAvailable) {
                            if (x == 0 && y > 0) {
                                result[j][i] += LeftBoundState[y];
                            } else if (x == n && y > 0) {
                                result[j][i] += RightBoundState[y];
                            }
                        }
                    }

                    result[j][i] /= (double)count;
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

        std::vector<std::vector<double>> MakeProb() {
            const usize n = Config.SpaceCount;
            const usize k = Config.TimeCount;
            const double alpha = Config.Alpha;
            const double gamma = Config.Gamma;
            Probabilities.resize(n + 1);

            for (usize i = 0; i <= n; i++) {
                Probabilities[i].resize(2 * n + 2 + k, 0.0);

                double a00 = CoefA(Space(i)), b00 = CoefB(Space(i)), c00 = CoefC(Space(i)); // Только для D = const

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
            
            
            return Probabilities;
        }
    };
}