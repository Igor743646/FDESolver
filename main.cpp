#include <mfdes.hpp>
#include <sfdes.hpp>
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

    f64 time_ms = std::chrono::duration<f64, std::milli>(stop - start).count();

    INFO_LOG << "Calculated time: " << time_ms << Endl;

    return result;
}

template<class T>
bool SaveResultsToFile(std::string fileName, const T& protobuf) {
    INFO_LOG << "Save result in file: " << fileName << Endl;
    std::ofstream binaryFile(fileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

    if (binaryFile.is_open()) {
        binaryFile << protobuf.SerializeAsString();
    } else {
        ERROR_LOG << "Bad openning" << Endl;
        return false;
    }

    return true;
}

void SolveTaskAndSave(IEquationSolver& solver, 
                      PFDESolver::TResults& results, 
                      bool calculateTime = true, 
                      bool saveMeta = true) 
{
    IEquationSolver::TResult result = calculateTime ? CalculateTime([&solver, saveMeta](){
        return solver.Solve(saveMeta);
    }) : solver.Solve(saveMeta);

    auto pbResult = results.add_results();
    pbResult->Swap(new PFDESolver::TResult(result.ToProto()));

    const auto& config = solver.GetConfig();

    if (saveMeta && config.RealSolution.has_value()) {
        INFO_LOG << "Adding real solution" << Endl;
        const usize n = config.SpaceCount;
        const usize k = config.TimeCount;

        results.set_realsolutionname(*config.RealSolutionName);
        auto RealSolution = NLinalg::TMatrix(k + 1, n + 1);
        for (usize j = 0; j <= k; j++) {
            for (usize i = 0; i <= n; i++) {
                if (i == n) {
                    RealSolution[j][i] = config.RightBoundState(static_cast<f64>(j) * config.TimeStep);
                } else {
                    RealSolution[j][i] = (*config.RealSolution)(config.LeftBound + static_cast<f64>(i) * config.SpaceStep, static_cast<f64>(j) * config.TimeStep);
                }
            }
        }

        PFDESolver::TMatrix realSolution(std::move(RealSolution.ToProto()));
        results.mutable_realsolution()->Swap(&realSolution);
    }
}

int main(int argc, char** argv) {
    NLogger::ChangeLogLevel(LOG_LEVEL);
    
    {   // file:///C:/Users/Igor/Desktop/c++/FDESolver/tasks/task1/task1.md
        const f64 alpha = 1.5, gamma = 0.9;
        TSolverConfig config = {
            .LeftBound = 0.0,
            .RightBound = 1.0,
            .MaxTime = 1.0,
            .Alpha = alpha,
            .Gamma = gamma,
            .SpaceStep = 0.052,
            .TimeStep = 0.01,
            .Beta = 1.0,
            .DiffusionCoefficient = [alpha](f64 x){ return NFunctions::Gamma(3.0 - alpha) / NFunctions::Gamma(3.0) * std::pow(x, alpha); },
            .DemolitionCoefficient = [](f64 x){ return 0.0; },
            .ZeroTimeState = [](f64 x){ return 0.0; },
            .SourceFunction = [gamma](f64 x, f64 t){ return NFunctions::Gamma(3.0) / NFunctions::Gamma(3.0 - gamma) * (std::pow(t, 2.0 - gamma) * std::pow(x, 2.0)) - std::pow(x, 2.0) * std::pow(t, 2.0); },
            .LeftBoundState = [](f64 t){ return 0.0; },
            .RightBoundState = [](f64 t){ return std::pow(t, 2.0); },
            .BordersAvailable = true,
            .StochasticIterationCount = 1000,
            .RealSolutionName = "Real solution: $u(x, t) = x^2 \\cdot t^2$",
            .RealSolution = [](f64 x, f64 t){ return std::pow(x, 2.0) * std::pow(t, 2.0); },
        };

        TMatrixFDES<TMFDESRule> solver1(config);
        TMatrixFDES<TRLFDESRule> solver2(config);
        TStochasticFDES<TMFDESRule> solver3(config);

        PFDESolver::TResults results;
        results.set_allocated_task(new PFDESolver::TSolverConfig(config.ToProto()));

        SolveTaskAndSave(solver1, results, true, true);
        SolveTaskAndSave(solver2, results, true, true);
        SolveTaskAndSave(solver3, results, true, true);

        if (!SaveResultsToFile("result1.bin", results)) {
            ERROR_LOG << "Crushed saving" << Endl;
        }
    }

    {   // file:///C:/Users/Igor/Desktop/c++/FDESolver/tasks/task1/task1.md
        const f64 alpha = 1.5, gamma = 0.9;
        const f64 a = 3.0;
        const f64 b = 6.0;
        TSolverConfig config = {
            .LeftBound = a+0.000001,
            .RightBound = b,
            .MaxTime = 0.01,
            .Alpha = alpha,
            .Gamma = gamma,
            .SpaceStep = 0.1,
            .TimeStep = 0.0001,
            .Beta = 1.0,
            .DiffusionCoefficient = [alpha, a](f64 x){ return NFunctions::Gamma(2.0 - alpha) * std::pow(x - a, alpha) * (std::log(x - a) + 1); },
            .DemolitionCoefficient = [alpha, a](f64 x){ return (a - x) * (std::log(x - a) + std::lgamma(2) - std::lgamma(2.0 - alpha)); },
            .ZeroTimeState = [](f64 x){ return 0.0; },
            .SourceFunction = [gamma, a](f64 x, f64 t){ return 1.0 / NFunctions::Gamma(2.0 - gamma) * (x - a) * std::log(x - a) * std::pow(t, 1 - gamma); },
            .LeftBoundState = [](f64 t){ return 0.0; },
            .RightBoundState = [a, b](f64 t){ return (b - a) * std::log(b - a) * t; },
            .BordersAvailable = true,
            .StochasticIterationCount = 1000,
            .RealSolutionName = "Real solution: $u(x, t) = (x-a) \\cdot ln(x-a) \\cdot t$",
            .RealSolution = [a](f64 x, f64 t){ return (x - a) * std::log(x - a) * t; },
        };

        TMatrixFDES<TMFDESRule> solver1(config);
        TMatrixFDES<TRLFDESRule> solver2(config);
        TStochasticFDES<TMFDESRule> solver3(config);

        PFDESolver::TResults results;
        results.set_allocated_task(new PFDESolver::TSolverConfig(config.ToProto()));

        SolveTaskAndSave(solver1, results, true, true);
        SolveTaskAndSave(solver2, results, true, true);
        SolveTaskAndSave(solver3, results, true, true);

        if (!SaveResultsToFile("result2.bin", results)) {
            ERROR_LOG << "Crushed saving" << Endl;
        }
    }

    {   // file:///C:/Users/Igor/Desktop/c++/FDESolver/tasks/task2/task2.md
        TSolverConfig config = {
            .LeftBound = -0.4,
            .RightBound = 0.4,
            .MaxTime = 0.2,
            .Alpha = 1.8,
            .Gamma = 0.9,
            .SpaceStep = 0.02,
            .TimeStep = 0.001,
            .Beta = 0.5,
            .DiffusionCoefficient = [](f64 x){ return .1; },
            .DemolitionCoefficient = [](f64 x){ return 0.0; },
            .ZeroTimeState = [](f64 x){ return -0.01 < x && x < 0.01 ? 50.0 : 0.0; },
            .SourceFunction = [](f64 x, f64 t){ return 0.0; },
            .LeftBoundState = [](f64 t){ return 0.0; },
            .RightBoundState = [](f64 t){ return 0.0; },
            .BordersAvailable = false,
            .StochasticIterationCount = 1000,
        };

        TMatrixFDES<TMFDESRule> solver1(config);
        TMatrixFDES<TRLFDESRule> solver2(config);
        TStochasticFDES<TMFDESRule> solver3(config);

        PFDESolver::TResults results;
        results.set_allocated_task(new PFDESolver::TSolverConfig(config.ToProto()));

        SolveTaskAndSave(solver1, results, true, true);
        SolveTaskAndSave(solver2, results, true, true);
        SolveTaskAndSave(solver3, results, true, true);

        if (!SaveResultsToFile("result3.bin", results)) {
            ERROR_LOG << "Crushed saving" << Endl;
        }
    }

    return 0;
}