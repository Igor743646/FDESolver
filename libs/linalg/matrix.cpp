#include <matrix.hpp>
#include <cassert>
#include <optional>

namespace NLinalg {

    TMatrix::TMatrix(usize rows, usize columns, double fill) 
    : Rows(rows), Columns(columns) {
        Matrix = new double[rows * columns](fill);
    }
    
    TMatrix::TMatrix(const TMatrix& matrix) 
    : TMatrix(matrix.Rows, matrix.Columns) {
        std::memcpy(Matrix, matrix.Matrix, Rows * Columns * sizeof(double));
    }

    TMatrix::TMatrix(TMatrix&& matrix) 
    : Rows(matrix.Rows), Columns(matrix.Columns) {
        Matrix = matrix.Matrix;
        matrix.Matrix = nullptr;
    }

    TMatrix::TMatrix(usize rows) 
    : TMatrix(rows, rows) {
    }

    TMatrix::TMatrix(const std::vector<double>& v) 
    : TMatrix(1, v.size()) {
        std::memcpy(Matrix, v.data(), Rows * Columns * sizeof(double));
    }

    TMatrix::~TMatrix() {
        if (Matrix) {
            delete[] Matrix;
        }
    }

    const double* TMatrix::operator[](usize i) const {
        assert(i < Rows);
        return static_cast<const double*>(Matrix + i * Columns);
    }

    double* TMatrix::operator[](usize i) {
        assert(i < Rows);
        return (Matrix + i * Columns);
    }

    TMatrix operator*(const TMatrix& lhs, const TMatrix& rhs) {
        assert(lhs.Columns == rhs.Rows);

        TMatrix result(lhs.Rows, rhs.Columns);

        for (usize i = 0; i < lhs.Rows; i++) {
            for (usize j = 0; j < rhs.Columns; j++) {
                for (usize k = 0; k < lhs.Columns; k++) {
                    result[i][j] += lhs[i][k] * rhs[k][j];
                }
            }
        }

        return result;
    }

    std::vector<double> operator*(const std::vector<double>& lhs, const TMatrix& rhs) {
        assert(lhs.size() == rhs.Rows);

        std::vector<double> result(rhs.Columns, 0.0);
        for (usize i = 0; i < rhs.Columns; i++) {
            for (usize j = 0; j < lhs.size(); j++) {
                result[i] += rhs[j][i] * lhs[j];
            }
        }

        return result;
    }

    std::pair<usize, usize> TMatrix::Shape() const {
        return {Rows, Columns};
    }

    void TMatrix::SwapRows(usize i, usize j) {
        assert(i < Rows && j < Rows);

        if (i == j)
            return;

        for (usize idx = 0; idx < Columns; idx++) {
            std::swap(Matrix[i * Columns + idx], Matrix[j * Columns + idx]);
        }
    }

    void TMatrix::SwapColumns(usize i, usize j) {
        assert(i < Columns && j < Columns);

        if (i == j)
            return;

        for (usize idx = 0; idx < Rows; idx++) {
            std::swap(Matrix[idx * Columns + i], Matrix[idx * Columns + j]);
        }
    }

    std::tuple<TMatrix, TMatrix, TMatrix> TMatrix::LUFactorizing() {
        assert(Columns == Rows);

        std::tuple<TMatrix, TMatrix, TMatrix> result(E(Rows), E(Rows), TMatrix(*this));
        TMatrix& P = std::get<0>(result);
        TMatrix& L = std::get<1>(result);
        TMatrix& U = std::get<2>(result);

        for (usize colId = 0; colId < Columns; colId++) {

            // 1. Находим строку с максимальным по модулю элементом.
            {
                usize rowMax = colId;
                for (usize rowId = colId + 1; rowId < Rows; rowId++) {
                    if (std::abs(U[rowId][colId]) > std::abs(U[rowMax][colId])) {
                        rowMax = rowId;
                    }
                }

                if (U[rowMax][colId] == 0) {
                    continue;
                }

                // 2. Меняем строки в U и обновляем L.
                if (rowMax != colId) {
                    P.SwapColumns(colId, rowMax);
                    L.SwapRows(colId, rowMax);
                    L.SwapColumns(colId, rowMax);
                    U.SwapRows(colId, rowMax);
                }
            }

            // 3. Алгоритм Гаусса
            for (usize rowId = colId + 1; rowId < Rows; rowId++) {
                double koef = U[rowId][colId] / U[colId][colId];

                U[rowId][colId] = 0;
                L[rowId][colId] = koef;

                for (usize t = colId + 1; t < Columns; t++) {
                    U[rowId][t] -= koef * U[colId][t];
                }
            }
        }

        return result;
    }

    std::optional<std::vector<double>> TMatrix::Solve(const std::vector<double>& b) {
        assert(Rows == Columns && Columns == b.size());

        // 1. Делаем LU - разложение
        auto [P, L, U] = LUFactorizing();

        // 2. Вычисляем P^(T)b = bP = y (1 x n)
        auto y = b * P;

        // 3. Вычисляем L * z = y;
        std::vector<double> z(Columns, 0.0);

        for (usize i = 0; i < Rows; i++) {
            z[i] = y[i];

            for (usize j = 0; j < i; j++) {
                z[i] -= L[i][j] * z[j];
            }

            z[i] /= L[i][i];
        }

        // 4. Вычисляем U * x = z
        std::vector<double> x(Columns, 0.0);

        for (long i = Rows - 1; i >= 0; i--) {
            x[i] = z[i];

            for (usize j = i + 1; j < Columns; j++) {
                x[i] -= U[i][j] * x[j];
            }

            if (std::abs(U[i][i]) < EPSILON) 
                return std::nullopt;

            x[i] /= U[i][i];
        }

        return x;
    }

    TMatrix TMatrix::E(usize n) {
        TMatrix matrix(n);

        for (usize i = 0; i < n; i++) {
            matrix[i][i] = 1.0;
        }

        return matrix;
    }

    std::ostream& operator<<(std::ostream& out, const TMatrix& m) {
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

    void TMatrix::WriteText(std::ofstream& out) const {
        out << Rows << " " << Columns << Endl;
        for (usize i = 0; i < Rows; i++) {
            for (usize j = 0; j < Columns; j++) {
                out << Matrix[i * Columns + j] << ' ';
            }

            out << Endl;
        }
    }

    void TMatrix::WriteBinary(std::ofstream& out) const {
        out << Rows << Columns;
        out.write(reinterpret_cast<char*>(Matrix), sizeof(double) * Rows * Columns);
    }
}