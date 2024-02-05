#include <equation_solver.hpp>

namespace NEquationSolver {
    IEquationSolver::IEquationSolver(const TSolverConfig& config) : Config(config) {
        Config.SpaceCount = static_cast<usize>((Config.RightBound - Config.LeftBound) / Config.SpaceStep);
        Config.TimeCount = static_cast<usize>(Config.MaxTime / Config.TimeStep);
        MemoGAlpha[0] = 1.0;
        MemoGGamma[0] = 1.0;
    }

    IEquationSolver::IEquationSolver(TSolverConfig&& config) : Config(std::move(config)) {
        Config.SpaceCount = static_cast<usize>((Config.RightBound - Config.LeftBound) / Config.SpaceStep);
        Config.TimeCount = static_cast<usize>(Config.MaxTime / Config.TimeStep);
        MemoGAlpha[0] = 1.0;
        MemoGGamma[0] = 1.0;
    }

    IEquationSolver::IEquationSolver(const IEquationSolver& solver) : Config(solver.Config) {
        MemoGAlpha[0] = 1.0;
        MemoGGamma[0] = 1.0;
    }

    IEquationSolver::IEquationSolver(IEquationSolver&& solver) : Config(std::move(solver.Config)) {
        MemoGAlpha[0] = 1.0;
        MemoGGamma[0] = 1.0;
    }

    IEquationSolver::~IEquationSolver() {
        
    }

    double IEquationSolver::CoefG(double a, usize i) {
        if (a != Config.Alpha && a != Config.Gamma) {
            throw "ERROR: g function using memoization only for \"alpha\" and \"gamma\". If you need more use comments below\n";

            /*
                if (std::abs(a) - (ull)std::abs(a) < 0.000001 && i >= a + 1) {
                    return 0.0;
                }
                return (i % 2 == 0 ? 1 : -1) / (a + 1) / BETA((double)i + 1.0, a - (double)i + 1.0);
            */
        }

        if (a == Config.Alpha) {
            if (MemoGAlpha.find(i) == MemoGAlpha.end()) {
                MemoGAlpha[i] = (static_cast<double>(i) - 1.0 - Config.Alpha) / static_cast<double>(i) * CoefG(a, i - 1);
            }

            return MemoGAlpha[i];
        } 
        
        if (MemoGGamma.find(i) == MemoGGamma.end()) {
            MemoGGamma[i] = (static_cast<double>(i) - 1.0 - Config.Gamma) / static_cast<double>(i) * CoefG(a, i - 1);
        }

        return MemoGGamma[i];
    }

    double IEquationSolver::Space(usize i) {
        return Config.LeftBound + static_cast<double>(i) * Config.SpaceStep;
    }

    double IEquationSolver::Time(usize j) {
        return Config.TimeStep * static_cast<double>(j);
    }

    double IEquationSolver::CoefA(double x) {
        return (1.0 + Config.Beta) 
        * (Config.DiffusionCoefficient(x) / 2.0) 
        * (std::pow(Config.TimeStep, Config.Gamma) / std::pow(Config.SpaceStep, Config.Alpha));
    }

    double IEquationSolver::CoefB(double x) {
        return (1.0 - Config.Beta) 
        * (Config.DiffusionCoefficient(x) / 2.0) 
        * (std::pow(Config.TimeStep, Config.Gamma) / std::pow(Config.SpaceStep, Config.Alpha));
    }

    double IEquationSolver::CoefC(double x) {
        return Config.DemolitionCoefficient(x) * std::pow(Config.TimeStep, Config.Gamma) / 2.0 / Config.SpaceStep;
    }
}

std::ostream& NEquationSolver::operator<<(std::ostream& out, const NEquationSolver::TSolverConfig& config) {

    out << "Space count N:\t\t"       << config.SpaceCount << Endl;
    out << "Time count K:\t\t"        << config.TimeCount << Endl;
    out << "Left bound L:\t\t"        << config.LeftBound << Endl;
    out << "Right bound R:\t\t"       << config.RightBound << Endl;
    out << "Max time:\t\t"            << config.MaxTime << Endl;
    out << "Alpha:\t\t\t"             << config.Alpha << Endl;
    out << "Gamma:\t\t\t"             << config.Gamma << Endl;
    out << "Space step:\t\t"          << config.SpaceStep << Endl;
    out << "Time step:\t\t"           << config.TimeStep << Endl;
    out << "Beta:\t\t\t"              << config.Beta << Endl;
    out << "Borders available:\t"     << config.BordersAvailable;

    return out;
}

