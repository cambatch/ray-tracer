#pragma once

#include "Vector3.hpp"

#include <iostream>
#include <stdint.h>


namespace Math
{

template <size_t ROWS, size_t COLS>
struct Matrix
{
    alignas(16) float data[ROWS][COLS];

    float& operator()(const size_t row, const size_t col) { return data[row][col]; }
    const float& operator()(const size_t row, const size_t col) const { return data[row][col]; }

    Matrix<ROWS, COLS>() { std::memset(data, 0, sizeof(data)); }
    // We assume there is an array of the appropriate size...
    Matrix<ROWS, COLS>(float* vals)
    {
        for(size_t i = 0; i < ROWS; ++i) {
            for(size_t j = 0; j < COLS; ++j) {
                data[i][j] = vals[i * COLS + j];
            }
        }
    }

    // Identity matrix
    Matrix<ROWS, COLS>(float val) {
        std::memset(data, 0, sizeof(data));
        for(size_t i = 0; i < COLS; ++i)
            data[i][i] = val;
    }


    float Determinant() const;
    Matrix<ROWS, COLS> RotateX(float radians) const;
    Matrix<ROWS, COLS> RotateY(float radians) const;
    Matrix<ROWS, COLS> RotateZ(float radians) const;
    Matrix<ROWS, COLS> Scale(const Vector3& v) const;
    Matrix<ROWS, COLS> Translate(const Vector3& v) const;

    Vector3 operator*(const Vector3& v) const;


    Matrix<ROWS, COLS>& Transpose() {
        for(size_t i = 0; i < ROWS; ++i) {
            for(size_t j = 0; j < COLS; ++j) {
                data[j][i] = data[i][j];
            }
        }
        return *this;
    }


    Matrix<ROWS, COLS>& operator*=(const Matrix<ROWS, COLS>& other) {
        Matrix<ROWS, COLS> result;
        for(size_t i = 0; i < ROWS; ++i)
            for(size_t j = 0; j < COLS; ++j)
                for(size_t k = 0; k < ROWS; ++k)
                    data[i][j] += data[i][k] * other.data[k][j];
        return *this;
    }

    Matrix<ROWS, COLS> operator*(const Matrix<ROWS, COLS>& other) const {
        Matrix<4, 4> result;
        for(size_t i = 0; i < ROWS; ++i)
            for(size_t j = 0; j < COLS; ++j)
                for(size_t k = 0; k < ROWS; ++k)
                    result.data[i][j] += data[i][k] * other.data[k][j];
        return result;
    }

    bool operator==(const Matrix<ROWS, COLS>& other) const {
        for(size_t row = 0; row < ROWS; ++row) {
            for(size_t col = 0; col < COLS; ++col) {
                if(data[row][col] != other.data[row][col])
                    return false;
            }
        }
        return true;
    }

    bool operator!=(const Matrix<ROWS, COLS>& other) {
        return !(*this == other);
    }

    void Print(std::ostream& o = std::cout) const {
        for(size_t i = 0; i < ROWS; ++i) {
            for(size_t j = 0; j < COLS; ++j) {
                o << data[i][j] << ' ';
            }
            o << '\n';
        }
        o << std::endl;
    }
};

using Mat4 = Matrix<4, 4>;
using Mat3 = Matrix<3, 3>;
using Mat2 = Matrix<2, 2>;

template<>
inline float Matrix<2, 2>::Determinant() const {
    return data[0][0] * data[1][1] - data[0][1] * data[1][0];
}

template<>
inline float Matrix<3, 3>::Determinant() const {
    float result = 0.0f;
    result =  data[0][0] * (data[1][1]*data[2][2] - data[2][1]*data[1][2]);
    result -= data[0][1] * (data[1][0]*data[2][2] - data[2][0]*data[1][2]);
    result += data[0][2] * (data[1][0]*data[2][1] - data[2][0]*data[1][1]);
    return result;
}

template<>
inline float Matrix<4, 4>::Determinant() const {
    float result = 0.0f;
    result = data[0][0] * (data[1][1] * (data[2][2]*data[3][3] - data[3][2]*data[2][3])
                           - data[1][2] * (data[2][1]*data[3][3] - data[3][1]*data[2][3])
                           + data[1][3] * (data[2][1]*data[3][2] - data[3][1]*data[2][2]));
    result -= data[0][1] * (data[1][0] * (data[2][2]*data[3][3] - data[3][2]*data[2][3])
                            - data[1][2] * (data[2][0]*data[3][3] - data[3][0]*data[2][3])
                            + data[1][3] * (data[2][0]*data[3][2] - data[3][0]*data[2][2]));
    result += data[0][2] * (data[1][0] * (data[2][1]*data[3][3] - data[3][1]*data[2][3])
                            - data[1][1] * (data[2][0]*data[3][3] - data[3][0]*data[2][3])
                            + data[1][3] * (data[2][0]*data[3][1] - data[3][0]*data[2][1]));
    result -= data[0][3] * (data[1][0] * (data[2][1]*data[3][2] - data[3][1]*data[2][2])
                            - data[1][1] * (data[2][0]*data[3][2] - data[3][0]*data[2][2])
                            + data[1][2] * (data[2][0]*data[3][1] - data[3][0]*data[2][1]));
    return result;
}

template<>
inline Matrix<4, 4> Matrix<4, 4>::RotateX(float radians) const {
    return *this * RotateX(radians);
}

template<>
inline Matrix<4, 4> Matrix<4, 4>::RotateY(float radians) const {
    return *this * RotateY(radians);
}

template<>
inline Matrix<4, 4> Matrix<4, 4>::RotateZ(float radians) const {
    return *this * RotateZ(radians);
}

template<>
inline Matrix<4, 4> Matrix<4, 4>::Scale(const Vector3& v) const {
    return *this * Scale(v);
}

template<>
inline Matrix<4, 4> Matrix<4, 4>::Translate(const Vector3& v) const {
    return *this * Translate(v);
}

template<>
inline Vector3 Matrix<4, 4>::operator*(const Vector3& v) const {
    return Vector3(
        data[0][0] * v.x + data[0][1] * v.y + data[0][2] * v.z + data[0][3],
        data[1][0] * v.x + data[1][1] * v.y + data[1][2] * v.z + data[1][3],
        data[2][0] * v.x + data[2][1] * v.y + data[2][2] * v.z + data[2][3]
    );
}

// Square matrices ONLY
template<size_t N>
void SetMatrix(Matrix<N, N>& mat, float* vals) {
    for(size_t ct = 0; ct < N*N; ++ct) {
        int i = ct / N;
        int j = ct % N;
        mat.data[i][j] = vals[ct];
    }
}

template<size_t ROWS, size_t COLS>
Matrix<ROWS, COLS> Transpose(const Matrix<ROWS, COLS>& mat) {
    Matrix<ROWS, COLS> result;
    for(size_t i = 0; i < ROWS; ++i) {
        for(size_t j = 0; j < COLS; ++j) {
            result.data[j][i] = mat.data[i][j];
        }
    }
    return result;
}

template<size_t N>
Matrix<N-1, N-1> SubMatrix(const Matrix<N, N>& mat, int32_t row, int32_t col) {
    Matrix<N-1, N-1> result;
    for(size_t i = 0, resRow = 0; i < N; ++i) {
        if(i == row) continue;
        for(size_t j = 0, resCol = 0; j < N; ++j) {
            if(j == col) continue;
            result.data[resRow][resCol] = mat.data[i][j];
            ++resCol;
        }
        ++resRow;
    }
    return result;
}

template<size_t N>
float MatrixMinor(const Matrix<N, N>& mat, int32_t row, int32_t col) {
    Matrix<N-1, N-1> res = SubMatrix(mat, row, col);
    return res.Determinant();
}

template<size_t N>
float MatrixCofactor(const Matrix<N, N>& mat, int32_t row, int32_t col) {
    float result = MatrixMinor(mat, row, col);
    return ((row + col) % 2) ? -result : result;
}

template<size_t N>
Matrix<N, N> MatrixInverse(Matrix<N, N>& mat) {
    float determinant = mat.Determinant();

    if(determinant == 0) return Matrix<N, N>();

    Matrix<N, N> result;
    for(size_t row = 0; row < N; ++row) {
        for(size_t col = 0; col < N; ++col) {
            float cofactor = MatrixCofactor(mat, row, col);
            result.data[col][row] = cofactor / determinant;
        }
    }
    return result;
}

} // End namespace Math
