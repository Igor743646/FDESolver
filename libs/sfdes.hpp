#pragma once

#include <equation_solver.hpp>
#include <rules_impl.hpp>
#include <linalg/matrix.hpp>
#include <utils/utils.hpp>
#include <omp.h>

namespace NEquationSolver {

    template<TProbabilitiesFillRuleConcept TFiller = TMFDESRule>
    class TStochasticFDES : public IEquationSolver {
    protected:
        using IEquationSolver::TResult;
        using TMatrix = NLinalg::TMatrix;

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
            NLinalg::TMatrix probabilities(n - 1, 2 * n + 2 + k, 0.0);
            NLinalg::TMatrix prefsumProbs(n - 1, 2 * n + 2 + k, 0.0);

            for (usize i = 0; i < n - 1; i++) {
                for (usize p = 0; p < 2 * n + 2 + k; p++) {
                    probabilities[i][p] = TFiller::FillProbabilities(this, probabilities, i + 1, p);
                }
                std::inclusive_scan(probabilities[i], probabilities[i + 1], prefsumProbs[i]);
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
            std::knuth_b engine(device());  // knuth better than mt19937
            std::uniform_real_distribution<f64> generator(0.0, 1.0);

            std::vector<f64> randoms(count * (k) * k * (n - 1));
            std::generate(randoms.begin(), randoms.end(), [&generator, &engine](){return generator(engine);});

            for (i64 i = 1; i < n; i++) {
                #pragma omp parallel for
                for (i64 j = 1; j <= k; j++) {
                    for (i64 _n = 0; _n < count; _n++) {
                        i64 x = i, y = j;

                        usize rnd_id = _n * ((k) * k * (n - 1)) + (j-1) * (k * (n - 1)) + (i-1) * (k);
                        f64 sf = 0.0;

                        while (y > 0 && x < n && x > 0) {
                            f64 rnd = randoms[rnd_id + y - 1];
                            i64 idx = std::lower_bound(prefsumProbs[x-1], prefsumProbs[x], rnd) - prefsumProbs[x-1];

                            sf += SourceFunction[y][x];

                            if (idx <= 2 * n) { // перемещение по пространству
                                x += n - idx;
                                y--;
                            } else if (idx <= 2 * n + k) { // перемещение по времени
                                y -= idx - 2 * n + 1;
                            } else {
                                break;
                            }
                        }

                        result[j][i] += sf * PowTCGamma;

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
    };
}
