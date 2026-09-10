#include "Vector.h"

Vector::Vector(int n) noexcept {
    if (n > 0) {
        coordinates.assign(n, 0);
        n_ = n;
    }
    else std::cout << "invalid input\n";
}
Vector::Vector(int n, float v) noexcept {
    if (n > 0) {
        coordinates.assign(n, v);
        n_ = n;
    }
    else std::cout << "invalid input\n";
}

Vector::Vector(int n, std::initializer_list<float> values) noexcept {
    if (n > 0) {
        coordinates.assign(n, 0);
        int count{};
        count = (n > values.size() ? values.size() : n);
        for (int i{}; i < count; i++) {
            *(coordinates.begin() + i) = *(values.begin() + i);
        }
        n_ = n;
    }
    else std::cout << "invalid input\n";
}

Vector Vector::operator*(const float s) const {
    Vector current (n_);
    if (coordinates.size() >= 8) {
        int count = coordinates.size() / 8;
        int remainder = coordinates.size() % 8;
        float * d = new float[8];
        for (int i{}; i < count; i++) {
            __m256 vec_a = _mm256_loadu_ps(&coordinates[8*i]);
            __m256 scale = _mm256_set1_ps(s);
            __m256 res = _mm256_mul_ps(vec_a, scale);
            _mm256_storeu_ps(d, res);

            for (int j{}; j < 8; j++) current.coordinates[8*i + j] = *(d + j);
        }
        delete[] d;
        for (int i{}; i < remainder; i++) {
            current.coordinates[8*count + i] = coordinates[8*count + i]*s;
        }
    }
    else if (coordinates.size() >= 4) {
        int count = coordinates.size() / 4;
        int remainder = coordinates.size() % 4;
        float * d = new float[4];
        for (int i{}; i < count; i++) {
            __m128 vec_a = _mm_loadu_ps(&coordinates[4*i]);
            __m128 scale = _mm_set1_ps(s);
            __m128 res = _mm_mul_ps(vec_a, scale);
            _mm_storeu_ps(d, res);

            for (int j{}; j < 4; j++) current.coordinates[4*i + j] = *(d + j);
        }
        for (int i{}; i < remainder; i++) {
            current.coordinates[4*count + i] = coordinates[4*count + i]*s;
        }
    }
    else {
        for (int i{}; i < n_; i++) {
            current.coordinates[i] = coordinates[i]*s;
        }
    }
    return current;
}

Vector Vector::operator+(const Vector & vec) const {
    Vector result(this->coordinates.size());
    if (coordinates.size() >= 8) {
        int count = coordinates.size() / 8;
        int remainder = coordinates.size() % 8;
        float * d = new float[8];
        for (int i{}; i < count; i++) {
            __m256 vec_a = _mm256_loadu_ps(&coordinates[8*i]);
            __m256 vec_b = _mm256_loadu_ps(&(vec.coordinates[8*i]));
            __m256 res = _mm256_add_ps(vec_a, vec_b);
            _mm256_storeu_ps(d, res);

            for (int j{}; j < 8; j++) result.coordinates[8*i + j] = *(d + j);
        }
        delete[] d;
        for (int i{}; i < remainder; i++) {
            result.coordinates[8*count + i] = vec.coordinates[8*count + i] + this->coordinates[8*count + i];
        }
    }
    else if (coordinates.size() >= 4) {
        int count = coordinates.size() / 4;
        int remainder = coordinates.size() % 4;
        float * d = new float[4];
        for (int i{}; i < count; i++) {
            __m128 vec_a = _mm_loadu_ps(&coordinates[4*i]);
            __m128 vec_b = _mm_loadu_ps(&(vec.coordinates[4*i]));
            __m128 res = _mm_add_ps(vec_a, vec_b);
            _mm_storeu_ps(d, res);

            for (int j{}; j < 4; j++) result.coordinates[4*i + j] = *(d + j);
        }
        delete[] d;
        for (int i{}; i < remainder; i++) {
            result.coordinates[4*count + i] = vec.coordinates[4*count + i] + this->coordinates[4*count + i];
        }
    }
    else {
        for (int i{}; i < n_; i++) {
            result.coordinates[i] = this->coordinates[i] + vec.coordinates[i];
        }
    }
    return result;
}

Vector Vector::operator-(const Vector & vec) const {
    Vector result(this->coordinates.size());
    if (coordinates.size() >= 8) {
        int count = coordinates.size() / 8;
        int remainder = coordinates.size() % 8;
        float * d = new float[8];
        for (int i{}; i < count; i++) {
            __m256 vec_a = _mm256_loadu_ps(&coordinates[8*i]);
            __m256 vec_b = _mm256_loadu_ps(&(vec.coordinates[8*i]));
            __m256 res = _mm256_sub_ps(vec_a, vec_b);
            _mm256_storeu_ps(d, res);

            for (int j{}; j < 8; j++) result.coordinates[8*i + j] = *(d + j);
        }
        delete[] d;
        for (int i{}; i < remainder; i++) {
            result.coordinates[8*count + i] = vec.coordinates[8*count + i] - this->coordinates[8*count + i];
        }
    }
    else if (coordinates.size() >= 4) {
        int count = coordinates.size() / 4;
        int remainder = coordinates.size() % 4;
        float * d = new float[4];
        for (int i{}; i < count; i++) {
            __m128 vec_a = _mm_loadu_ps(&coordinates[4*i]);
            __m128 vec_b = _mm_loadu_ps(&(vec.coordinates[4*i]));
            __m128 res = _mm_sub_ps(vec_a, vec_b);
            _mm_storeu_ps(d, res);

            for (int j{}; j < 4; j++) result.coordinates[4*i + j] = *(d + j);
        }
        delete[] d;
        for (int i{}; i < remainder; i++) {
            result.coordinates[4*count + i] = vec.coordinates[4*count + i] - this->coordinates[4*count + i];
        }
    }
    else {
        for (int i{}; i < n_; i++) {
            result.coordinates[i] = this->coordinates[i] - vec.coordinates[i];
        }
    }
    return result;
}
