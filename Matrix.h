#pragma once
#include <vector>
#include <array>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <immintrin.h>
#include <algorithm>
#include <random>

using matrix = std::vector<float>;
using start_pos = matrix::iterator;

enum class matrix_type {UP_TRIANG, LOW_TRIANG, IDENTITY};

class squere_Matrix_real {
    int n_;
    matrix matrix_;
public:
    squere_Matrix_real(int) noexcept;                 //заполнение нулями 
    squere_Matrix_real(int, float) noexcept;        //заполнение указанным значением
    squere_Matrix_real(int, matrix_type) noexcept;  //по умолчанию единицы
    squere_Matrix_real(int, matrix_type, float) noexcept;  //пользовательские значения

    squere_Matrix_real(const squere_Matrix_real &) = default;
    squere_Matrix_real& operator=(const squere_Matrix_real&) = default;

    squere_Matrix_real(squere_Matrix_real &&) = default;
    squere_Matrix_real& operator=(squere_Matrix_real&&) = default;

    squere_Matrix_real operator+(const squere_Matrix_real &) noexcept;
    squere_Matrix_real operator-(const squere_Matrix_real &) noexcept;
    squere_Matrix_real operator*(const squere_Matrix_real &) noexcept;

    float& operator()(int, int);

    const squere_Matrix_real transpose_new() const;
    void transpose();
    float determinant_1();
    float determinant_2();
    squere_Matrix_real reverse();

    void swap_row(int, int);
    void swap_col(int, int);
    void init_random();

    const std::vector<squere_Matrix_real> LU_decomposition();
    size_t get_size() const {return n_;}

    const float * first_el() const {return &matrix_[0];}
    float * first_el_non_const() {return &matrix_[0];}
    start_pos first_el_iter() {return matrix_.begin();}

    void show() const;
};

#include "Matrix.tpp"

