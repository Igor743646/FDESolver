#include <equation_solver.hpp>

namespace NEquationSolver {
    IEquationSolver::IEquationSolver(const TSolverParameters& params) : Parameters(params) {
        Parameters.SpaceCount = static_cast<usize>((Parameters.RightBound - Parameters.LeftBound) / Parameters.SpaceStep);
        Parameters.TimeCount = static_cast<usize>(Parameters.MaxTime / Parameters.TimeStep);
        MemoGAlpha[0] = 1.0;
        MemoGGamma[0] = 1.0;
    }

    IEquationSolver::IEquationSolver(TSolverParameters&& params) : Parameters(std::move(params)) {
        Parameters.SpaceCount = static_cast<usize>((Parameters.RightBound - Parameters.LeftBound) / Parameters.SpaceStep);
        Parameters.TimeCount = static_cast<usize>(Parameters.MaxTime / Parameters.TimeStep);
        MemoGAlpha[0] = 1.0;
        MemoGGamma[0] = 1.0;
    }

    IEquationSolver::IEquationSolver(const IEquationSolver& solver) : Parameters(solver.Parameters) {
        MemoGAlpha[0] = 1.0;
        MemoGGamma[0] = 1.0;
    }

    IEquationSolver::IEquationSolver(IEquationSolver&& solver) : Parameters(std::move(solver.Parameters)) {
        MemoGAlpha[0] = 1.0;
        MemoGGamma[0] = 1.0;
    }

    IEquationSolver::~IEquationSolver() {
        
    }

    double IEquationSolver::CoefG(double a, usize i) {
        if (a != Parameters.Alpha && a != Parameters.Gamma) {
            throw "ERROR: g function using memoization only for \"alpha\" and \"gamma\". If you need more use comments below\n";

            /*
                if (std::abs(a) - (ull)std::abs(a) < 0.000001 && i >= a + 1) {
                    return 0.0;
                }
                return (i % 2 == 0 ? 1 : -1) / (a + 1) / BETA((double)i + 1.0, a - (double)i + 1.0);
            */
        }

        if (a == Parameters.Alpha) {
            if (MemoGAlpha.find(i) == MemoGAlpha.end()) {
                MemoGAlpha[i] = (static_cast<double>(i) - 1.0 - Parameters.Alpha) / static_cast<double>(i) * CoefG(a, i - 1);
            }

            return MemoGAlpha[i];
        } 
        
        if (MemoGGamma.find(i) == MemoGGamma.end()) {
            MemoGGamma[i] = (static_cast<double>(i) - 1.0 - Parameters.Gamma) / static_cast<double>(i) * CoefG(a, i - 1);
        }

        return MemoGGamma[i];
    }

    double IEquationSolver::Space(usize i) {
        return Parameters.LeftBound + static_cast<double>(i) * Parameters.SpaceStep;
    }

    double IEquationSolver::Time(usize i) {
        return Parameters.TimeStep * static_cast<double>(i);
    }

    double IEquationSolver::CoefA(double x) {
        return (1.0 + Parameters.Beta) 
        * (Parameters.DiffusionCoefficient(x) / 2.0) 
        * (std::pow(Parameters.TimeStep, Parameters.Gamma) / std::pow(Parameters.SpaceStep, Parameters.Alpha));
    }

    double IEquationSolver::CoefB(double x) {
        return (1.0 - Parameters.Beta) 
        * (Parameters.DiffusionCoefficient(x) / 2.0) 
        * (std::pow(Parameters.TimeStep, Parameters.Gamma) / std::pow(Parameters.SpaceStep, Parameters.Alpha));
    }

    double IEquationSolver::CoefC(double x) {
        return Parameters.DemolitionCoefficient(x) * std::pow(Parameters.TimeStep, Parameters.Gamma) / 2.0 / Parameters.SpaceStep;
    }
}

std::ostream& NEquationSolver::operator<<(std::ostream& out, const NEquationSolver::TSolverParameters& parameters) {

    out << "Space count N:\t\t"       << parameters.SpaceCount << Endl;
    out << "Time count K:\t\t"        << parameters.TimeCount << Endl;
    out << "Left bound L:\t\t"        << parameters.LeftBound << Endl;
    out << "Right bound R:\t\t"       << parameters.RightBound << Endl;
    out << "Max time:\t\t"            << parameters.MaxTime << Endl;
    out << "Alpha:\t\t\t"             << parameters.Alpha << Endl;
    out << "Gamma:\t\t\t"             << parameters.Gamma << Endl;
    out << "Space step:\t\t"          << parameters.SpaceStep << Endl;
    out << "Time step:\t\t"           << parameters.TimeStep << Endl;
    out << "Beta:\t\t\t"              << parameters.Beta << Endl;
    out << "Borders available:\t"     << parameters.BordersAvailable;

    return out;
}

