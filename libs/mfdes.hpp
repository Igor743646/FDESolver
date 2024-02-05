#pragma once

#include <equation_solver.hpp>
#include <linalg/matrix.hpp>

namespace NEquationSolver {

    class TModifiedFDES : public IEquationSolver {
    protected:
        using IEquationSolver::TResult;

    public:

        TModifiedFDES(const TSolverParameters&);
        TModifiedFDES(TSolverParameters&&);
        virtual TResult Solve() const;
        
    };

}