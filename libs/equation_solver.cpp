#include <equation_solver.hpp>
#include <cassert>
#include <config.pb.h>

namespace NEquationSolver {

    PFDESolver::TSolverConfig TSolverConfig::ToProto() const {
        PFDESolver::TSolverConfig config;

        config.set_spacecount(SpaceCount);
        config.set_timecount(TimeCount);
        config.set_leftbound(LeftBound);
        config.set_rightbound(RightBound);
        config.set_maxtime(MaxTime);
        config.set_alpha(Alpha);
        config.set_gamma(Gamma);
        config.set_spacestep(SpaceStep);
        config.set_timestep(TimeStep);
        config.set_beta(Beta);
        config.set_alphaleft(AlphaLeft);
        config.set_alpharight(AlphaRight);
        config.set_betaleft(BetaLeft);
        config.set_betaright(BetaRight);
        config.set_bordersavailable(BordersAvailable);

        return config;
    }

    IEquationSolver::IEquationSolver(const TSolverConfig& config) : Config(config) {
        Config.SpaceCount = static_cast<usize>((Config.RightBound - Config.LeftBound) / Config.SpaceStep);
        Config.TimeCount = static_cast<usize>(Config.MaxTime / Config.TimeStep);
        PrefetchCoefficients();
    }

    IEquationSolver::IEquationSolver(TSolverConfig&& config) : Config(std::move(config)) {
        Config.SpaceCount = static_cast<usize>((Config.RightBound - Config.LeftBound) / Config.SpaceStep);
        Config.TimeCount = static_cast<usize>(Config.MaxTime / Config.TimeStep);
        PrefetchCoefficients();
    }

    IEquationSolver::IEquationSolver(const IEquationSolver& solver) : Config(solver.Config) {
        PrefetchCoefficients();
    }

    IEquationSolver::IEquationSolver(IEquationSolver&& solver) : Config(std::move(solver.Config)) {
        PrefetchCoefficients();
    }

    void IEquationSolver::PrefetchCoefficients() {
        GAlpha.resize(Config.SpaceCount + 2);
        GGamma.resize(Config.TimeCount + 2);
        GAlpha[0] = GGamma[0] = 1.0;

        for (usize i = 1; i < Config.SpaceCount + 2; i++) {
            GAlpha[i] = (static_cast<double>(i) - 1.0 - Config.Alpha) / static_cast<double>(i) * GAlpha[i - 1];
        }

        for (usize i = 1; i < Config.TimeCount + 2; i++) {
            GGamma[i] = (static_cast<double>(i) - 1.0 - Config.Gamma) / static_cast<double>(i) * GGamma[i - 1];
        }

        PowTCGamma = std::pow(Config.TimeStep, Config.Gamma);
        PowSCAlpha = std::pow(Config.SpaceStep, Config.Alpha);
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
            assert(i < GAlpha.size());
            return GAlpha[i];
        }

        assert(i < GGamma.size());
        return GGamma[i];
    }

    double IEquationSolver::CoefGAlpha(usize i) {
        assert(i < GAlpha.size());
        return GAlpha[i];
    }

    double IEquationSolver::CoefGGamma(usize j) {
        assert(j < GGamma.size());
        return GGamma[j];
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
        * (PowTCGamma / PowSCAlpha);
    }

    double IEquationSolver::CoefB(double x) {
        return (1.0 - Config.Beta) 
        * (Config.DiffusionCoefficient(x) / 2.0) 
        * (PowTCGamma / PowSCAlpha);
    }

    double IEquationSolver::CoefC(double x) {
        return Config.DemolitionCoefficient(x) * PowTCGamma / 2.0 / Config.SpaceStep;
    }

    const TSolverConfig& IEquationSolver::GetConfig() const {
        return Config;
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

