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

enum class matrix_type {UP_TRIANG, LOW_TRIANG, IDENTITY, ZERO, SQUERE, RECTANG};

class Matrix {
    matrix_type type;   //squere or rectang
};



class squere_Matrix_real : public Matrix {
    int n_;
    matrix matrix_;
    float determinant = 0;    //default value
    matrix_type type;
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
    size_t get_size() const {return n_;}
    float get_det() {return determinant;}

    const float * first_el() const {return &matrix_[0];}
    float * first_el_non_const() {return &matrix_[0];}
    start_pos first_el_iter() {return matrix_.begin();}

    squere_Matrix_real construct_transform_matrix(int, int, float);
    std::vector<squere_Matrix_real> LU_decomposition();

    void show() const;
};

class rectang_Matrix : public Matrix {
    int n_;
    int m_;
    matrix matrix_;
    matrix_type type;
public:
    rectang_Matrix(int, int) noexcept;                 //заполнение нулями 
    rectang_Matrix(int, int, std::initializer_list<float>) noexcept;

    rectang_Matrix(const rectang_Matrix &) = default;
    rectang_Matrix& operator=(const rectang_Matrix&) = default;

    rectang_Matrix(rectang_Matrix &&) = default;
    rectang_Matrix& operator=(rectang_Matrix&&) = default;

    rectang_Matrix operator+(const rectang_Matrix &) noexcept;
    rectang_Matrix operator-(const rectang_Matrix &) noexcept;
    rectang_Matrix operator*(const rectang_Matrix &) noexcept;

    const float& operator()(int, int) const;

    const rectang_Matrix transpose_new() const;

    void swap_row(int, int);
    void swap_col(int, int);
    void init_random();

    std::vector<int> get_size() const {return std::vector<int> ({n_, m_});};
    const float * first_el() const {return &matrix_[0];}
    float * first_el_non_const() {return &matrix_[0];}
    void show() const;
};

struct algebraic_addition {
    int signum;
    float minor;
};


#include "Matrix.tpp"

