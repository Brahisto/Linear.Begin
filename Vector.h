#include <vector>
#include <immintrin.h>
#include <iostream>


class Vector {
    public:     //only for scaling
    std::vector<float> coordinates;
    int n_;
    Vector(int) noexcept;
    Vector(int, float) noexcept;
    Vector(int, std::initializer_list<float>) noexcept;
    Vector operator*(const float) const;
    Vector operator+(const Vector &) const;
    Vector operator-(const Vector &) const;
};

struct vector_scalar_quad {
    Vector vector;
    float scalar_quadrat;
};
