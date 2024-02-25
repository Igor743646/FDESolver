#pragma once

#include <utils/utils.hpp>
#include <fstream>

namespace PFDESolver {
    class TMatrix;
}

namespace NLinalg {
    class TMatrix {
    public:

        using TPluResult = std::tuple<std::vector<usize>, TMatrix>;

        TMatrix();
        TMatrix(usize, usize, f64 = 0.0);
        TMatrix(usize, usize, const std::vector<f64>&);
        TMatrix(const TMatrix&);
        TMatrix(TMatrix&&);
        TMatrix(const std::vector<f64>&);
        TMatrix(usize);
        TMatrix& operator=(const TMatrix&);
        TMatrix& operator=(TMatrix&&);
        ~TMatrix();

        std::pair<usize, usize> Shape() const;
        void SwapRows(usize, usize);
        void SwapColumns(usize, usize);

        static TMatrix E(usize);
        TPluResult LUFactorizing();

        /// @brief Метод решения линейного матричного уравнения через PLU разложение \\
        /// @brief Ax = b => PLUx = b => LUx = P^(-1)b = P^(T)b
        /// @param b вектор значений с правой стороны уравнения
        /// @exception Метод вызывает исключение, если матрица не квадратная
        /// @return Возвращает x - решение системы уравнений вида Ax = b 
        std::optional<std::vector<f64>> Solve(const std::vector<f64>&);
        static std::optional<std::vector<f64>> Solve(const TPluResult&, const std::vector<f64>&);

        const f64* operator[](usize) const;
        f64* operator[](usize);
        friend TMatrix operator*(const TMatrix&, const TMatrix&);
        friend std::vector<f64> operator*(const std::vector<f64>&, const TMatrix&);

        friend std::ostream& operator<<(std::ostream& out, const TMatrix& m);
        PFDESolver::TMatrix ToProto() const;

    private:

        usize Rows, Columns;
        f64* Matrix;
    };
}
