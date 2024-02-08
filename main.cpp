#include <mfdes.hpp>

void CalculateTime(auto callback) {
    auto start = std::chrono::system_clock::now();
    callback();
    auto stop = std::chrono::system_clock::now();

    double time_ms = std::chrono::duration<double, std::milli>(stop - start).count();

    INFO_LOG << time_ms << Endl;
}

int main() {
    NLogger::TLogHelper<>::ChangeLogLevel(2);

    NEquationSolver::TSolverConfig config = {
        .LeftBound = 1.0,
        .RightBound = 2.0,
        .MaxTime = 1.0,
        .Alpha = 1.7,
        .Gamma = 0.7,
        .SpaceStep = 0.002,
        .TimeStep = 0.005,
        .Beta = 1.0,
        .DiffusionCoefficient = [](double x){ return NFunctions::Gamma(3.0 - 1.7) / NFunctions::Gamma(3.0 - 0.7) ; },
        .DemolitionCoefficient = [](double x){ return 0.0; },
        .ZeroTimeState = [](double x){ return 0.0; },
        .SourceFunction = [](double x, double t){ return NFunctions::Gamma(3.0) / NFunctions::Gamma(3.0 - 0.7) * (std::pow(t, 2.0 - 0.7) * std::pow(x, 2.0)) - std::pow(x, 2.0 - 1.7) * std::pow(t, 2.0); },
        .LeftBoundState = [](double t){ return 0.5 * std::pow(t, 2.0); },
        .RightBoundState = [](double t){ return 4.0 * std::pow(t, 2.0); },
        .BordersAvailable = false,
    };

    NEquationSolver::TModifiedFDES solver(config);

    NLinalg::TMatrix result;

    CalculateTime([&](){
        result = solver.Solve();
    });

    std::ofstream textFile("result.txt", std::ios_base::out | std::ios_base::trunc);
    std::ofstream binaryFile("result.bin", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

    if (textFile.is_open() && binaryFile.is_open()) {
        result.WriteText(textFile);
        result.WriteBinary(binaryFile);
    } else {
        ERROR_LOG << "Bad openning" << Endl;
    }

    return 0;
}