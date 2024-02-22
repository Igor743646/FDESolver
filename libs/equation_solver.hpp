/*
    Equation solver interface
*/

#pragma once

#include <cmath>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <utils/utils.hpp>
#include <linalg/matrix.hpp>

namespace PFDESolver {
    class TSolverConfig;
    class TResult;
}

namespace NEquationSolver {
    
    struct TSolverConfig {
        usize SpaceCount, TimeCount;    // количество ячеек по x и t координатах соответственно

        double LeftBound, RightBound;   // границы отрезка сетки по x координате
        double MaxTime;                 // граница отрезка времени по t координате
        double Alpha, Gamma;            // степени производных по x и t координатах соответственно
        double SpaceStep, TimeStep;     // шаги по сетки по x и t координатах соответственно
        double Beta;                    // коэффициент доли лево/правосторонней производных [-1; +1]

        double AlphaLeft, BetaLeft;     // коэффициенты граничных условий третьего рода для x == L
        double AlphaRight, BetaRight;   // коэффициенты граничных условий третьего рода для x == R

        std::function<double(double)> DiffusionCoefficient;     // коэффициент диффузии при дробной производной по пространству
        std::function<double(double)> DemolitionCoefficient;    // коэффициент сноса при производной первой степени
        std::function<double(double)> ZeroTimeState;            // начальное условие при t = 0, u(x, 0) = psi(x)
        std::function<double(double, double)> SourceFunction;   // функция источник
        std::function<double(double)> LeftBoundState;           // граничное условие u(L, t) = phiL(t)
        std::function<double(double)> RightBoundState;          // граничное условие u(R, t) = phiR(t)

        bool BordersAvailable;           // стоит ли учитывать граничные условия

        friend std::ostream& operator<<(std::ostream&, const TSolverConfig&);
        PFDESolver::TSolverConfig ToProto() const;
    };

    class IEquationSolver {
    protected:
        TSolverConfig Config;
        std::vector<double> GAlpha;
        std::vector<double> GGamma;
        double PowTCGamma, PowSCAlpha;

    private:

        void PrefetchCoefficients();

    public:

        struct TResult {
            std::string MethodName;
            const TSolverConfig& Config;
            NLinalg::TMatrix Field;
            std::optional<NLinalg::TMatrix> SolveMatrix;
            std::optional<NLinalg::TMatrix> RealSolution;
            std::optional<std::string> RealSolutionName;

            PFDESolver::TResult ToProto() const;
            bool SaveToFile(std::string name) const;
            void AddMetaRealSolution(const std::function<double(double, double)>& func, const std::string& name = "");
        };

        IEquationSolver(const TSolverConfig&);
        IEquationSolver(TSolverConfig&&);
        IEquationSolver(const IEquationSolver&);
        IEquationSolver(IEquationSolver&&);
        virtual ~IEquationSolver();

        /// @brief Возвращает x координату
        /// @param  i номер шага по пространственной координате
        /// @return x(i) = LeftBound + i * SpaceStep
        double Space(usize) const ;

        /// @brief Возвращает t координату
        /// @param  j номер шага по временной координате
        /// @return t(j) = j * TimeStep
        double Time(usize) const;
        double CoefA(double) const;
        double CoefB(double) const;
        double CoefC(double) const;
        double CoefG(double, usize) const;
        double CoefGAlpha(usize) const;
        double CoefGGamma(usize) const;

        virtual TResult Solve(bool saveMeta) = 0;

        const TSolverConfig& GetConfig() const;

        friend std::ostream& operator<<(std::ostream& out, const IEquationSolver& solver) {

            out << "Parameters:\n" << solver.Config << Endl;
            out << "GAlpha:\n" << solver.GAlpha << Endl;
            out << "GGamma:\n" << solver.GGamma << Endl;

            return out;
        }
    };
}

