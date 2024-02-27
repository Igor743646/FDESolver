#include <equation_solver.hpp>
#include <cassert>
#include <config.pb.h>
#include <result.pb.h>
#include <matrix.pb.h>

namespace NEquationSolver {

    PFDESolver::TSolverConfig TSolverConfig::ToProto() const {
        PFDESolver::TSolverConfig config;

        config.set_spacecount(static_cast<usize>((RightBound - LeftBound) / SpaceStep));
        config.set_timecount(static_cast<usize>(MaxTime / TimeStep));
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

    PFDESolver::TResult IEquationSolver::TResult::ToProto() const {
        PFDESolver::TResult res;
        PFDESolver::TSolverConfig conf(std::move(Config.ToProto()));
        res.mutable_config()->Swap(&conf);
        res.set_methodname(MethodName);
        PFDESolver::TMatrix field(std::move(Field.ToProto()));
        res.mutable_field()->Swap(&field);

        if (SolveMatrix.has_value()) {
            PFDESolver::TMatrix solveMatrix(std::move(SolveMatrix->ToProto()));
            res.mutable_solvematrix()->Swap(&solveMatrix);
        }
        
        return res;
    }

    bool IEquationSolver::TResult::SaveToFile(std::string name) const {
        INFO_LOG << "Save result in file: " << name << Endl;
        std::ofstream binaryFile(name, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

        if (binaryFile.is_open()) {
            PFDESolver::TResult res = ToProto();
            binaryFile << res.SerializeAsString();
        } else {
            ERROR_LOG << "Bad openning" << Endl;
            return false;
        }

        return true;
    }

    IEquationSolver::IEquationSolver(const TSolverConfig& config) : Config(config) {
        Config.SpaceCount = static_cast<usize>((Config.RightBound - Config.LeftBound) / Config.SpaceStep);
        Config.TimeCount = static_cast<usize>(Config.MaxTime / Config.TimeStep);
        PrefetchData();
        Validate();
    }

    IEquationSolver::IEquationSolver(TSolverConfig&& config) : Config(std::move(config)) {
        Config.SpaceCount = static_cast<usize>((Config.RightBound - Config.LeftBound) / Config.SpaceStep);
        Config.TimeCount = static_cast<usize>(Config.MaxTime / Config.TimeStep);
        PrefetchData();
        Validate();
    }

    IEquationSolver::IEquationSolver(const IEquationSolver& solver) : Config(solver.Config) {
        PrefetchData();
        Validate();
    }

    IEquationSolver::IEquationSolver(IEquationSolver&& solver) : Config(std::move(solver.Config)) {
        PrefetchData();
        Validate();
    }

    void IEquationSolver::PrefetchData() {
        GAlpha.resize(Config.SpaceCount + 2);
        GGamma.resize(Config.TimeCount + 2);
        GAlpha[0] = GGamma[0] = 1.0;

        for (usize i = 1; i < Config.SpaceCount + 2; i++) {
            GAlpha[i] = (static_cast<f64>(i) - 1.0 - Config.Alpha) / static_cast<f64>(i) * GAlpha[i - 1];
        }

        for (usize i = 1; i < Config.TimeCount + 2; i++) {
            GGamma[i] = (static_cast<f64>(i) - 1.0 - Config.Gamma) / static_cast<f64>(i) * GGamma[i - 1];
        }

        PowTCGamma = std::pow(Config.TimeStep, Config.Gamma);
        PowSCAlpha = std::pow(Config.SpaceStep, Config.Alpha);

        DiffusionCoefficient.resize(Config.SpaceCount+1);
        DemolitionCoefficient.resize(Config.SpaceCount+1);
        ZeroTimeState.resize(Config.SpaceCount+1);
        SourceFunction = NLinalg::TMatrix(Config.TimeCount+1, Config.SpaceCount+1);
        LeftBoundState.resize(Config.TimeCount+1);
        RightBoundState.resize(Config.TimeCount+1);

        for (usize i = 0; i <= Config.SpaceCount; i++) {
            DiffusionCoefficient[i] = Config.DiffusionCoefficient(Space(i));
            DemolitionCoefficient[i] = Config.DemolitionCoefficient(Space(i));
            ZeroTimeState[i] = Config.ZeroTimeState(Space(i));
        }

        for (usize j = 0; j <= Config.TimeCount; j++) {
            LeftBoundState[j] = Config.LeftBoundState(Time(j));
            RightBoundState[j] = Config.RightBoundState(Time(j));
            
            for (usize i = 0; i <= Config.SpaceCount; i++) {
                SourceFunction[j][i] = Config.SourceFunction(Space(i), Time(j));
            }
        }
    }

    IEquationSolver::~IEquationSolver() {
        
    }

    f64 IEquationSolver::CoefG(f64 a, usize i) const {
        if (a != Config.Alpha && a != Config.Gamma) {
            throw "ERROR: g function using memoization only for \"alpha\" and \"gamma\". If you need more use comments below\n";

            /*
                if (std::abs(a) - (ull)std::abs(a) < 0.000001 && i >= a + 1) {
                    return 0.0;
                }
                return (i % 2 == 0 ? 1 : -1) / (a + 1) / BETA((f64)i + 1.0, a - (f64)i + 1.0);
            */
        }

        if (a == Config.Alpha) {
            assert(i < GAlpha.size());
            return GAlpha[i];
        }

        assert(i < GGamma.size());
        return GGamma[i];
    }

    f64 IEquationSolver::CoefGAlpha(usize i) const {
        assert(i < GAlpha.size());
        return GAlpha[i];
    }

    f64 IEquationSolver::CoefGGamma(usize j) const {
        assert(j < GGamma.size());
        return GGamma[j];
    }

    f64 IEquationSolver::Space(usize i) const {
        return Config.LeftBound + static_cast<f64>(i) * Config.SpaceStep;
    }

    f64 IEquationSolver::Time(usize j) const {
        return Config.TimeStep * static_cast<f64>(j);
    }

    f64 IEquationSolver::CoefA(f64 x) const {
        return (1.0 + Config.Beta) 
        * (Config.DiffusionCoefficient(x) / 2.0) 
        * (PowTCGamma / PowSCAlpha);
    }

    f64 IEquationSolver::CoefB(f64 x) const {
        return (1.0 - Config.Beta) 
        * (Config.DiffusionCoefficient(x) / 2.0) 
        * (PowTCGamma / PowSCAlpha);
    }

    f64 IEquationSolver::CoefC(f64 x) const {
        return Config.DemolitionCoefficient(x) * PowTCGamma / 2.0 / Config.SpaceStep;
    }

    f64 IEquationSolver::CoefA(usize i) const {
        return (1.0 + Config.Beta) 
        * (DiffusionCoefficient[i] / 2.0) 
        * (PowTCGamma / PowSCAlpha);
    }

    f64 IEquationSolver::CoefB(usize i) const {
        return (1.0 - Config.Beta) 
        * (DiffusionCoefficient[i] / 2.0) 
        * (PowTCGamma / PowSCAlpha);
    }

    f64 IEquationSolver::CoefC(usize j) const {
        return DemolitionCoefficient[j] * PowTCGamma / 2.0 / Config.SpaceStep;
    }

    const TSolverConfig& IEquationSolver::GetConfig() const {
        return Config;
    }

    void IEquationSolver::Validate() const {
        DEBUG_LOG << DiffusionCoefficient << Endl;
        auto diffusionCMaxId = std::ranges::max_element(DiffusionCoefficient.begin(), DiffusionCoefficient.end());
        f64 const diffusionCMax = *diffusionCMaxId;
        auto demolitionCMaxId = std::ranges::max_element(DemolitionCoefficient.begin(), DemolitionCoefficient.end());
        f64 const demolitionCMax = *demolitionCMaxId;

        f64 left = diffusionCMax * PowTCGamma / PowSCAlpha;
        f64 right = Config.Gamma / Config.Alpha;
        INFO_LOG << std::format("Left: {} ({}) Right: {}", left, diffusionCMaxId - DiffusionCoefficient.begin(), right) << Endl;
        if (left > right) {
            WARNING_LOG << "May be problem with condition" << Endl
                        << "\t\tD * pow(tau, gamma) / pow(h, alpha): " << left << Endl
                        << "\t\tgamma/alpha: " << right << Endl;

            WARNING_LOG << "May make h >= " << std::pow(PowTCGamma * diffusionCMax / right, 1.0/Config.Alpha) << Endl;
            WARNING_LOG << "Or tau <= " << std::pow(right * PowSCAlpha / diffusionCMax, 1.0/Config.Gamma) << Endl;
        }

        left = 2.0 * Config.SpaceStep * (CoefA(Space(diffusionCMaxId - DiffusionCoefficient.begin())) * GAlpha[2] + CoefB(Space(diffusionCMaxId - DiffusionCoefficient.begin())));
        right = demolitionCMax * PowTCGamma;
        if (left > right) {
            WARNING_LOG << "May be problem with condition" << Endl
                        << "\t\t2h * (C+ * g_a_2 + C-): " << left << Endl
                        << "\t\tpow(tau, gamma)*V: " << right << Endl;
        }

        left = 2.0 * Config.SpaceStep * (CoefB(Space(diffusionCMaxId - DiffusionCoefficient.begin())) * GAlpha[2] + CoefA(Space(diffusionCMaxId - DiffusionCoefficient.begin())));
        right = demolitionCMax * PowTCGamma;
        if (left < right) {
            WARNING_LOG << "May be problem with condition" << Endl
                        << "\t\t2h * (C- * g_a_2 + C+): " << left << Endl
                        << "\t\tpow(tau, gamma)*V: " << right << Endl;
        }
    }

    IEquationSolver::TResult IEquationSolver::Solve(bool saveMeta) {
        try {
            return DoSolve(saveMeta);
        } catch(...) {
            ERROR_LOG << "Something wrong with solving" << Endl;
            throw "Solve error";
        }
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
