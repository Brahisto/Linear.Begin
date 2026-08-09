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


using matrix = std::vector<float>;
using start_pos = matrix::iterator;

enum class matrix_type {UP_TRIANG, LOW_TRIANG, IDENTITY, ZERO};

class squere_Matrix_real {
    int n_;
    matrix matrix_;
    float determinant = 0;    //default value
public:
    squere_Matrix_real(int) noexcept;                 //заполнение нулями 
    squere_Matrix_real(int, float) noexcept;        //заполнение указанным значением
    squere_Matrix_real(int, matrix_type) noexcept;  //по умолчанию единицы
    squere_Matrix_real(int, matrix_type, float) noexcept;  //пользовательские значения
    squere_Matrix_real(int, std::initializer_list<float>) noexcept;

    squere_Matrix_real(const squere_Matrix_real &) = default;
    squere_Matrix_real& operator=(const squere_Matrix_real&) = default;

    squere_Matrix_real(squere_Matrix_real &&) = default;
    squere_Matrix_real& operator=(squere_Matrix_real&&) = default;

    squere_Matrix_real operator+(const squere_Matrix_real &) noexcept;
    squere_Matrix_real operator-(const squere_Matrix_real &) noexcept;
    squere_Matrix_real operator*(const squere_Matrix_real &) noexcept;

    const float& operator()(int, int) const;

    const squere_Matrix_real transpose_new() const;
    void transpose();
    float determinant_1();
    float determinant_2();
    int to_up_triang();    //make up triang matrix from input by equivalent transforming. return swap_count

    squere_Matrix_real reverse();

    void swap_row(int, int);
    void swap_col(int, int);
    void init_random();

    squere_Matrix_real except_ij_row_col(int, int) const;

    const std::vector<squere_Matrix_real> LU_decomposition();
    size_t get_size() const {return n_;}
    float get_det() {return determinant;}

    const float * first_el() const {return &matrix_[0];}
    float * first_el_non_const() {return &matrix_[0];}
    start_pos first_el_iter() {return matrix_.begin();}

    void show() const;
};

struct algebraic_addition {
    int signum;
    float minor;
};


#include "Matrix.tpp"

