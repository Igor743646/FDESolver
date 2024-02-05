#include <mfdes.hpp>

namespace NEquationSolver {

    TModifiedFDES::TModifiedFDES(const TSolverParameters& params) : IEquationSolver(params) {}
    TModifiedFDES::TModifiedFDES(TSolverParameters&& params) : IEquationSolver(std::move(params)) {}


    TModifiedFDES::TResult TModifiedFDES::Solve() const {
        TModifiedFDES::TResult result(Parameters.TimeCount + 1, std::vector<double>(Parameters.SpaceCount + 1, 0.0));



        return result;
    }
}