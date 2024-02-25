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

        f64 LeftBound, RightBound;   // границы отрезка сетки по x координате
        f64 MaxTime;                 // граница отрезка времени по t координате
        f64 Alpha, Gamma;            // степени производных по x и t координатах соответственно
        f64 SpaceStep, TimeStep;     // шаги по сетки по x и t координатах соответственно
        f64 Beta;                    // коэффициент доли лево/правосторонней производных [-1; +1]

        f64 AlphaLeft, BetaLeft;     // коэффициенты граничных условий третьего рода для x == L
        f64 AlphaRight, BetaRight;   // коэффициенты граничных условий третьего рода для x == R

        std::function<f64(f64)> DiffusionCoefficient;     // коэффициент диффузии при дробной производной по пространству
        std::function<f64(f64)> DemolitionCoefficient;    // коэффициент сноса при производной первой степени
        std::function<f64(f64)> ZeroTimeState;            // начальное условие при t = 0, u(x, 0) = psi(x)
        std::function<f64(f64, f64)> SourceFunction;   // функция источник
        std::function<f64(f64)> LeftBoundState;           // граничное условие u(L, t) = phiL(t)
        std::function<f64(f64)> RightBoundState;          // граничное условие u(R, t) = phiR(t)

        bool BordersAvailable;           // стоит ли учитывать граничные условия

        usize StochasticIterationCount = 10;    // количество итераций для стохастического алгоритма

        std::optional<std::string> RealSolutionName;                        // latex формула функции c эталонным решением обрамленная $$
        std::optional<std::function<f64(f64, f64)>> RealSolution;  // функция с эталонным решением

        friend std::ostream& operator<<(std::ostream&, const TSolverConfig&);
        PFDESolver::TSolverConfig ToProto() const;
    };

    class IEquationSolver {
    protected:
        TSolverConfig Config;
        std::vector<f64> GAlpha;
        std::vector<f64> GGamma;

    public:
        /* From config */
        std::vector<f64> DiffusionCoefficient;
        std::vector<f64> DemolitionCoefficient;
        std::vector<f64> ZeroTimeState;
        NLinalg::TMatrix SourceFunction;
        std::vector<f64> LeftBoundState;
        std::vector<f64> RightBoundState;

    private:

        void PrefetchData();

    public:

        f64 PowTCGamma, PowSCAlpha;

        struct TResult {
            std::string MethodName;
            const TSolverConfig& Config;
            NLinalg::TMatrix Field;
            std::optional<NLinalg::TMatrix> SolveMatrix;

            PFDESolver::TResult ToProto() const;
            bool SaveToFile(std::string name) const;
        };

        IEquationSolver(const TSolverConfig&);
        IEquationSolver(TSolverConfig&&);
        IEquationSolver(const IEquationSolver&);
        IEquationSolver(IEquationSolver&&);
        virtual ~IEquationSolver();

        /// @brief Возвращает x координату
        /// @param  i номер шага по пространственной координате
        /// @return x(i) = LeftBound + i * SpaceStep
        f64 Space(usize) const ;

        /// @brief Возвращает t координату
        /// @param  j номер шага по временной координате
        /// @return t(j) = j * TimeStep
        f64 Time(usize) const;
        f64 CoefA(f64) const;
        f64 CoefB(f64) const;
        f64 CoefC(f64) const;
        f64 CoefG(f64, usize) const;
        f64 CoefGAlpha(usize) const;
        f64 CoefGGamma(usize) const;

        virtual std::string Name() = 0;
        virtual TResult Solve(bool saveMeta) final;
        virtual TResult DoSolve(bool saveMeta) = 0;

        const TSolverConfig& GetConfig() const;
        virtual void Validate() const final;

        friend std::ostream& operator<<(std::ostream& out, const IEquationSolver& solver) {

            out << "Parameters:\n" << solver.Config << Endl;
            out << "GAlpha:\n" << solver.GAlpha << Endl;
            out << "GGamma:\n" << solver.GGamma << Endl;

            return out;
        }
    };
}
