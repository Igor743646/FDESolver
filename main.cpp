#include <mfdes.hpp>
#include <config.pb.h>
#include <result.pb.h>
#include <matrix.pb.h>

#ifdef NDEBUG
    #define LOG_LEVEL 2
#else
    #define LOG_LEVEL 3
#endif

using namespace NEquationSolver;

auto CalculateTime(auto callback) {
    auto start = std::chrono::system_clock::now();
    auto result = callback();
    auto stop = std::chrono::system_clock::now();

    double time_ms = std::chrono::duration<double, std::milli>(stop - start).count();

    INFO_LOG << "Calculated time: " << time_ms << Endl;

    return result;
}

int main(int argc, char** argv) {
    NLogger::ChangeLogLevel(LOG_LEVEL);
    
    {   // file:///C:/Users/Igor/Desktop/c++/FDESolver/tasks/task1/task1.md
        TSolverConfig config = {
            .LeftBound = 0.0,
            .RightBound = 1.0,
            .MaxTime = 1.0,
            .Alpha = 1.7,
            .Gamma = 0.7,
            .SpaceStep = 0.1,
            .TimeStep = 0.001,
            .Beta = 1.0,
            .DiffusionCoefficient = [](double x){ return NFunctions::Gamma(3.0 - 1.7) / NFunctions::Gamma(3.0) * std::pow(x, 1.7); },
            .DemolitionCoefficient = [](double x){ return 0.0; },
            .ZeroTimeState = [](double x){ return 0.0; },
            .SourceFunction = [](double x, double t){ return NFunctions::Gamma(3.0) / NFunctions::Gamma(3.0 - 0.7) * (std::pow(t, 2.0 - 0.7) * std::pow(x, 2.0)) - std::pow(x, 2.0) * std::pow(t, 2.0); },
            .LeftBoundState = [](double t){ return 0.0; },
            .RightBoundState = [](double t){ return std::pow(t, 2.0); },
            .BordersAvailable = true,
        };

        TModifiedFDES<TMFDESRule> solver(config);

        IEquationSolver::TResult result = CalculateTime([&](){
            return solver.Solve(true);
        });

        result.AddMetaRealSolution([](double x, double t){
            return x*x*t*t;
        }, "Real solution: $u(x, t) = x^2 \\cdot t^2$");

        if (!result.SaveToFile("result1.bin")) {
            ERROR_LOG << "Crushed saving" << Endl;
        }
    }

/* u(x, t) = exp(2t) * x^2 */
    // FDESbase p1(
    //     0, 10.0, 0.2, 1.8, 0.9, 0.2, 0.01, 1.0,
    //     [](double x, double t){ return std::pow(2.0, 0.9) * GAMMA(3.0 - 1.8) / GAMMA(3.0); },                                // D
    //     [](double x, double t){ return 0.0; },                                 // V
    //     [](double x){ return std::pow(x, 2.0); },     // psi(x)
    //     [](double x, double t){ return std::pow(2.0, 0.9) * std::exp(2.0 * t) * (std::pow(x, 2.0) - std::pow(x, 0.2)); },                                  // f(x, t)
    //     [](double t){ return 0.0; },                                            // phiL(t)
    //     [](double t){ return std::exp(2.0 * t) * 100.0; }                                             // phiR(t)
    // );

    {   // file:///C:/Users/Igor/Desktop/c++/FDESolver/tasks/task2/task2.md
        TSolverConfig config = {
            .LeftBound = 0.0,
            .RightBound = 10.0,
            .MaxTime = 0.2,
            .Alpha = 1.8,
            .Gamma = 0.9,
            .SpaceStep = 0.1,
            .TimeStep = 0.001,
            .Beta = 0.0,
            .DiffusionCoefficient = [](double x){ return std::pow(2.0, 0.9) * std::pow(x, 1.8) * NFunctions::Gamma(3.0 - 1.8) / NFunctions::Gamma(3.0); },
            .DemolitionCoefficient = [](double x){ return 0.0; },
            .ZeroTimeState = [](double x){ return std::pow(x, 2.0); },
            .SourceFunction = [](double x, double t){ return (std::pow(2.0, 0.9) - 1.0) * std::exp(2.0 * t) * std::pow(x, 2.0); },
            .LeftBoundState = [](double t){ return 0.0; },
            .RightBoundState = [](double t){ return std::exp(2.0 * t) * 100.0; },
            .BordersAvailable = true,
        };

        TModifiedFDES<TMFDESRule> solver(config);

        IEquationSolver::TResult result = CalculateTime([&](){
            return solver.Solve(true);
        });

        result.AddMetaRealSolution([](double x, double t){
            return x*x*std::exp(2.0 * t);
        }, "Real solution: $u(x, t) = x^2 \\cdot e^{2t}$");

        if (!result.SaveToFile("result2.bin")) {
            ERROR_LOG << "Crushed saving" << Endl;
        }
    }

    return 0;
}