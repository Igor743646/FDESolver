/*
    Equation solver interface
*/

#pragma once

#include <cmath>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <utils/utils.hpp>

namespace NEquationSolver {
    
    struct TSolverParameters {
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

        friend std::ostream& operator<<(std::ostream&, const TSolverParameters&);
    };

    class IEquationSolver {
    protected:
        TSolverParameters Parameters;
        std::unordered_map<usize, double> MemoGAlpha;
        std::unordered_map<usize, double> MemoGGamma;

    public:

        using TResult = std::vector<std::vector<double>>;

        IEquationSolver(const TSolverParameters&);
        IEquationSolver(TSolverParameters&&);
        IEquationSolver(const IEquationSolver&);
        IEquationSolver(IEquationSolver&&);
        virtual ~IEquationSolver();

        double Space(usize);
        double Time(usize);
        double CoefA(double);
        double CoefB(double);
        double CoefC(double);
        double CoefG(double, usize);

        virtual TResult Solve() const = 0;

        friend std::ostream& operator<<(std::ostream& out, const IEquationSolver& solver) {

            out << "Parameters:\n" << solver.Parameters << Endl;
            out << "MemoGAlpha:\n" << solver.MemoGAlpha << Endl;
            out << "MemoGGamma:\n" << solver.MemoGGamma << Endl;

            return out;
        }
    };
}

