// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#pragma once

#include <iostream>
#include <cassert>

struct Dim
{
    int nrows, ncols;
};


// matrix multiplication: C = A * B
template <typename T>
void dotProd(T const* src1, Dim const& src1Dim, T const* src2, Dim const& src2Dim, T* dst)
{
    assert(src1Dim.ncols == src2Dim.nrows);

    for (int row = 0; row < src1Dim.nrows; ++row)
    {
        T const* src1El = &src1[row * src1Dim.ncols];

        for (int col = 0; col < src2Dim.ncols; ++col)
        {
            T const* src2El = &src2[col];

            T sum = 0.0f;
            for (int i = 0; i < src1Dim.ncols; ++i)
                sum += src1El[i] * src2El[i * src2Dim.ncols];

            dst[row * src2Dim.ncols + col] = sum;
        }
    }
}


template <typename T>
void printMatrix(T const* src, Dim const& dim)
{
    const T* el = src;
    for (int row = 0; row < dim.nrows; ++row)
    {
        for (int col = 0; col < dim.ncols; ++col)
            std::cout << *el++ << " ";
        std::cout << "\n";
    }
    std::cout << std::endl;
}
