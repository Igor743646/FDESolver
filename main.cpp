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

    double time_ms = std::chrono::duration<double, std::milli>(stop - start).count();

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
        for (usize i = 0; i <= k; i++) {
            for (usize j = 0; j <= n; j++) {
                RealSolution[i][j] = (*config.RealSolution)(config.LeftBound + j * config.SpaceStep, i * config.TimeStep);
            }
        }

        PFDESolver::TMatrix realSolution(std::move(RealSolution.ToProto()));
        results.mutable_realsolution()->Swap(&realSolution);
    }
}

int main(int argc, char** argv) {
    NLogger::ChangeLogLevel(LOG_LEVEL);
    
    {   // file:///C:/Users/Igor/Desktop/c++/FDESolver/tasks/task1/task1.md
        const double alpha = 1.5, gamma = 0.9;
        TSolverConfig config = {
            .LeftBound = 0.0,
            .RightBound = 1.0,
            .MaxTime = 1.0,
            .Alpha = alpha,
            .Gamma = gamma,
            .SpaceStep = 0.052,
            .TimeStep = 0.01,
            .Beta = 1.0,
            .DiffusionCoefficient = [alpha](double x){ return NFunctions::Gamma(3.0 - alpha) / NFunctions::Gamma(3.0) * std::pow(x, alpha); },
            .DemolitionCoefficient = [](double x){ return 0.0; },
            .ZeroTimeState = [](double x){ return 0.0; },
            .SourceFunction = [gamma](double x, double t){ return NFunctions::Gamma(3.0) / NFunctions::Gamma(3.0 - gamma) * (std::pow(t, 2.0 - gamma) * std::pow(x, 2.0)) - std::pow(x, 2.0) * std::pow(t, 2.0); },
            .LeftBoundState = [](double t){ return 0.0; },
            .RightBoundState = [](double t){ return std::pow(t, 2.0); },
            .BordersAvailable = true,
            .StochasticIterationCount = 100,
            .RealSolutionName = "Real solution: $u(x, t) = x^2 \\cdot t^2$",
            .RealSolution = [](double x, double t){ return x*x*t*t; },
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
            .DiffusionCoefficient = [](double x){ return .1; },
            .DemolitionCoefficient = [](double x){ return 0.0; },
            .ZeroTimeState = [](double x){ return -0.01 < x && x < 0.01 ? 50.0 : 0.0; },
            .SourceFunction = [](double x, double t){ return 0.0; },
            .LeftBoundState = [](double t){ return 0.0; },
            .RightBoundState = [](double t){ return 0.0; },
            .BordersAvailable = false,
            .StochasticIterationCount = 100,
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

    return 0;
}