#pragma once
#include <vector>
#include <array>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <immintrin.h>
#include <algorithm>
#include <random>
#include <initializer_list>
#include "Vector.h"

using start_pos = std::vector<float>::const_iterator;

enum class matrix_type {UP_TRIANG, LOW_TRIANG, IDENTITY, ZERO, SQUERE, RECTANG, COMMON, DIAGONAL};


class Matrix {
    int n_;
    int m_;
    std::vector<float> matrix_;
    float determinant = 0;    //default value
    matrix_type type_figure;
    matrix_type type_struct;
    bool is_compute_det;
public:
    Matrix(int) noexcept;                 //заполнение нулями 
    Matrix(int, float) noexcept;        //заполнение указанным значением
    Matrix(int, matrix_type) noexcept;  //по умолчанию единицы
    Matrix(int, matrix_type, float) noexcept;  //пользовательские значения
    Matrix(int, std::initializer_list<float>) noexcept;
    Matrix(int, int) noexcept;                 //заполнение нулями 
    Matrix(int, int, std::initializer_list<float>) noexcept;


    Matrix(const Matrix &) = default;
    Matrix& operator=(const Matrix&) = default;

    Matrix(Matrix &&) = default;
    Matrix& operator=(Matrix&&) = default;

    Matrix operator+(const Matrix &) noexcept;
    Matrix operator-(const Matrix &) noexcept;
    Matrix operator*(const Matrix &) noexcept;
    void operator*(const float) noexcept;

    const float& operator()(int, int) const;

    const Matrix transpose_new() const;
    float det();
    int to_up_triang();    //make up triang matrix from input by equivalent transforming. return swap_count

    Matrix reverse();

    void swap_row(int, int);
    void swap_col(int, int);
    void init_random();

    Matrix except_ij_row_col(int, int) const;
    size_t get_size() const {return n_;}
    float get_det() {return determinant;}

    const float * first_el() const {return &matrix_[0];}
    float * first_el_non_const() {return &matrix_[0];}
    start_pos first_el_iter() const {return matrix_.begin();}
    std::vector<Matrix> LU_decomposition();

    void show() const;
    
    int row_count() const {return n_;}
    int col_count() const {return m_;}
    void orthogonalization(const Matrix &);  //inside realization is based on row-transforming. If start basis presented as 
    //vectors-columns you need to transpose this matrix.

    float ort_check(int, int, const Matrix &);
};

float scalar_product(const std::vector<float> &, const Matrix &, const std::vector<float> &);     // the arguments is vectors. Similar realization is motivated by
                                                              //using methods of matrixes dot inside.

struct algebraic_addition {
    int signum;
    float minor;
};


