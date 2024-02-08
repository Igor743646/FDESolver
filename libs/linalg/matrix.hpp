#pragma once

#include <utils/utils.hpp>
#include <fstream>

namespace NLinalg {
    class TMatrix {
    public:

        using TPluResult = std::tuple<std::vector<usize>, TMatrix>;

        TMatrix();
        TMatrix(usize, usize, double = 0.0);
        TMatrix(usize, usize, const std::vector<double>&);
        TMatrix(const TMatrix&);
        TMatrix(TMatrix&&);
        TMatrix(const std::vector<double>&);
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
        std::optional<std::vector<double>> Solve(const std::vector<double>&);
        static std::optional<std::vector<double>> Solve(const TPluResult&, const std::vector<double>&);

        const double* operator[](usize) const;
        double* operator[](usize);
        friend TMatrix operator*(const TMatrix&, const TMatrix&);
        friend std::vector<double> operator*(const std::vector<double>&, const TMatrix&);

        friend std::ostream& operator<<(std::ostream& out, const TMatrix& m);
        void WriteText(std::ofstream& out) const;
        void WriteBinary(std::ofstream& out) const;

    private:

        usize Rows, Columns;
        double* Matrix;
    };
}
