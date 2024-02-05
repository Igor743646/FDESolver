#include <mfdes.hpp>

void Test1() {
    NLinalg::TMatrix m = NLinalg::TMatrix(3, 3);
    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;
    m[1][0] = 4;
    m[1][1] = 6;
    m[1][2] = 6;
    m[2][0] = -7;
    m[2][1] = 8;
    m[2][2] = 9;

    std::cout << "m:\n" << m << std::endl;

    std::cout << "m:\n" << m.Solve({2,-5,8}) << std::endl;
}

int main() {

    Test1();

    // NEquationSolver::TSolverParameters parameters = {
    //     .LeftBound = 1.0,
    //     .RightBound = 2.0,
    //     .MaxTime = 1.0,
    //     .Alpha = 1.7,
    //     .Gamma = 0.7,
    //     .SpaceStep = 0.1,
    //     .TimeStep = 0.0005,
    //     .Beta = 1.0,
    //     .DiffusionCoefficient = [](double x){ return NFunctions::Gamma(3.0 - 1.7) / NFunctions::Gamma(3.0 - 0.7) ; },
    //     .DemolitionCoefficient = [](double x){ return 0.0; },
    //     .ZeroTimeState = [](double x){ return 0.0; },
    //     .SourceFunction = [](double x, double t){ return NFunctions::Gamma(3.0) / NFunctions::Gamma(3.0 - 0.7) * (std::pow(t, 2.0 - 0.7) * std::pow(x, 2.0)) - std::pow(x, 2.0 - 1.7) * std::pow(t, 2.0); },
    //     .LeftBoundState = [](double t){ return 0.5 * std::pow(t, 2.0); },
    //     .RightBoundState = [](double t){ return 4.0 * std::pow(t, 2.0); },
    //     .BordersAvailable = false,
    // };

    // NEquationSolver::TModifiedFDES solver(parameters);

    // std::cout << solver << std::endl;

    return 0;
}