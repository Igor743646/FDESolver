#pragma once

#include <utils/utils.hpp>

namespace NLinalg {
    class TMatrix {
    public:

        TMatrix(usize, usize, double = 0.0);
        TMatrix(const TMatrix&);
        TMatrix(TMatrix&&);
        TMatrix(const std::vector<double>&);
        TMatrix(std::vector<double>&&);
        TMatrix(usize);
        ~TMatrix();

        std::pair<usize, usize> Shape() const;
        void SwapRows(usize, usize);
        void SwapColumns(usize, usize);

        static TMatrix E(usize);
        std::tuple<TMatrix, TMatrix, TMatrix> LUFactorizing();
        std::optional<std::vector<double>> Solve(const std::vector<double>&);
        const double* operator[](usize) const;
        double* operator[](usize);
        friend TMatrix operator*(const TMatrix&, const TMatrix&);
        friend std::vector<double> operator*(const std::vector<double>&, const TMatrix&);

        friend std::ostream& operator<<(std::ostream& out, const TMatrix& m) {
            out << std::fixed;
            out.precision(2);
            out << "[";
            for (usize i = 0; i < m.Rows; i++) {
                out << ((i == 0) ? "[" : " [");
                for (usize j = 0; j < m.Columns; j++) {
                    out << m[i][j];
                    out << ((j + 1 < m.Columns) ? " " : "");
                }

                out << ((i + 1 < m.Rows) ? "]\n" : "]");
            }
            out << "]";
            return out;
        }

    private:

        usize Rows, Columns;
        double* Matrix;
    };
}
