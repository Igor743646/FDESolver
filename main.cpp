#include <mfdes.hpp>
#include <config.pb.h>
#include <result.pb.h>
#include <matrix.pb.h>

#ifdef NDEBUG
    #define LOG_LEVEL 2
#else
    #define LOG_LEVEL 3
#endif

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
        NEquationSolver::TSolverConfig config = {
            .LeftBound = 1.0,
            .RightBound = 2.0,
            .MaxTime = 1.0,
            .Alpha = 1.7,
            .Gamma = 0.7,
            .SpaceStep = 0.1,
            .TimeStep = 0.0005,
            .Beta = 1.0,
            .DiffusionCoefficient = [](double x){ return NFunctions::Gamma(3.0 - 1.7) / NFunctions::Gamma(3.0 - 0.7) ; },
            .DemolitionCoefficient = [](double x){ return 0.0; },
            .ZeroTimeState = [](double x){ return 0.0; },
            .SourceFunction = [](double x, double t){ return NFunctions::Gamma(3.0) / NFunctions::Gamma(3.0 - 0.7) * (std::pow(t, 2.0 - 0.7) * std::pow(x, 2.0)) - std::pow(x, 2.0 - 1.7) * std::pow(t, 2.0); },
            .LeftBoundState = [](double t){ return std::pow(t, 2.0); },
            .RightBoundState = [](double t){ return 4.0 * std::pow(t, 2.0); },
            .BordersAvailable = true,
        };

        NEquationSolver::TModifiedFDES solver(config);

        NEquationSolver::IEquationSolver::TResult result = CalculateTime([&](){
            return solver.Solve(true);
        });

        result.AddMetaRealSolution([](double x, double t){
            return x*x*t*t;
        }, "Real solution: $u(x, t) = x^2 \\cdot t^2$");

        if (!result.SaveToFile("result1.bin")) {
            ERROR_LOG << "Crushed saving" << Endl;
        }
    }

    {   // file:///C:/Users/Igor/Desktop/c++/FDESolver/tasks/task2/task2.md
        NEquationSolver::TSolverConfig config = {
            .LeftBound = -0.1,
            .RightBound = 0.1,
            .MaxTime = 4.0,
            .Alpha = 1.4,
            .Gamma = 0.7,
            .SpaceStep = 0.01,
            .TimeStep = 0.05,
            .Beta = 0.5,
            .DiffusionCoefficient = [](double x){ return 0.0005; },
            .DemolitionCoefficient = [](double x){ return 0.0; },
            .ZeroTimeState = [](double x){ return -0.01 <= std::abs(x) && std::abs(x) < 0.0099999 ? 10.0 : 0.0; },
            .SourceFunction = [](double x, double t){ return 0.0; },
            .BordersAvailable = false,
        };

        NEquationSolver::TModifiedFDES solver(config);

        NEquationSolver::IEquationSolver::TResult result = CalculateTime([&](){
            return solver.Solve(true);
        });

        if (!result.SaveToFile("result2.bin")) {
            ERROR_LOG << "Crushed saving" << Endl;
        }
    }

    return 0;
}