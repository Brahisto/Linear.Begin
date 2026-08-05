

squere_Matrix_real::squere_Matrix_real(int n) noexcept {
    if (n > 0) {
        matrix_.assign(n*n, 0);
        n_ = n;
    }
    else {
        n_ = 0;
        std::cout << "The matrix size is 0. Try 'reset' with valid data...\n ";
    }
}

squere_Matrix_real::squere_Matrix_real(int n, float value) noexcept {
    if (n > 0) {
        matrix_.assign(n*n, value);
        n_ = n;
    }
    else {
        n_ = 0;
        std::cout << "The matrix size is 0. Try 'reset' with valid data...\n ";
    }
}

squere_Matrix_real::squere_Matrix_real(int n, matrix_type type) noexcept {
    if (n > 0) {
        matrix_.assign(n*n, 0);
        n_ = n;
    }
    else {
        n_ = 0;
        std::cout << "The matrix size is 0. Try 'reset' with valid data...\n ";
    }

    switch (type) {
        case matrix_type::LOW_TRIANG:
            for (int i{};i < n_; i++) {
                for (int j{}; j <= i; j++) {
                    matrix_[n_*i + j] = 1;
                }
            }
            break;

        case matrix_type::UP_TRIANG:
            for (int i{};i < n_; i++) {
                for (int j{}; j <= n_ - i - 1; j++) {
                    matrix_[n_*i + n_ - j - 1] = 1;
                }
            }
            break;

        case matrix_type::IDENTITY:
            for (int i{};i < n_; i++) matrix_[n*i + i] = 1;
            break;
    }
}

squere_Matrix_real::squere_Matrix_real(int n, matrix_type type, float value) noexcept {
    if (n > 0) {
        matrix_.assign(n*n, 0);
        n_ = n;
    }
    else {
        n_ = 0;
        std::cout << "The matrix size is 0. Try 'reset' with valid data...\n ";
    }

    switch (type) {
        case matrix_type::LOW_TRIANG:
            for (int i{};i < n_; i++) {
                for (int j{}; j <= i; j++) {
                    matrix_[n_*i + j] = value;
                }
            }
            break;

        case matrix_type::UP_TRIANG:
            for (int i{};i < n_; i++) {
                for (int j{}; j <= n_ - i - 1; j++) {
                    matrix_[n_*i + n_ - j - 1] = value;
                }
            }
            break;

        case matrix_type::IDENTITY:
            for (int i{};i < n_; i++) matrix_[n_*i + i] = value;
            break;
    }
}

squere_Matrix_real squere_Matrix_real::operator+(const squere_Matrix_real & matrix2) noexcept {
    squere_Matrix_real current(this->get_size());
    
    if (this->get_size() != matrix2.get_size()) {std::cout << "invalid size...\n"; return current;}

    int count = n_*n_ / 8;    //используем _m256, вмещающим 8 int and 8 float

    int remainder = (n_*n_) % 8;

    for (int i{}; i < count; i++) {
        __m256 vec_a = _mm256_loadu_ps(this->first_el() + 8*i);
        __m256 vec_b = _mm256_loadu_ps(matrix2.first_el() + 8*i);

        __m256 result = _mm256_add_ps(vec_a, vec_b);

        _mm256_storeu_ps(current.first_el_non_const() + 8*i, result);
    }
    
    for (int j{}; j < remainder; j++) {
        *(current.first_el_non_const() + 8*count + j) = *(this->first_el() + 8*count + j) + *(matrix2.first_el() + 8*count + j);
    }

    return current;
}

squere_Matrix_real squere_Matrix_real::operator-(const squere_Matrix_real & matrix2) noexcept {
    squere_Matrix_real current(this->get_size());
    
    if (this->get_size() != matrix2.get_size()) {std::cout << "invalid size...\n"; return current;}

    int count = n_*n_ / 8;    //используем _m256, вмещающим 8 int and 8 float

    int remainder = (n_*n_) % 8;

    for (int i{}; i < count; i++) {
        __m256 vec_a = _mm256_loadu_ps(this->first_el() + 8*i);
        __m256 vec_b = _mm256_loadu_ps(matrix2.first_el() + 8*i);

        __m256 result = _mm256_sub_ps(vec_a, vec_b);

        _mm256_storeu_ps(current.first_el_non_const() + 8*i, result);
    }
    
    for (int j{}; j < remainder; j++) {
        *(current.first_el_non_const() + 8*count + j) = *(this->first_el() + 8*count + j) - *(matrix2.first_el() + 8*count + j);
    }

    return current;
}

const squere_Matrix_real squere_Matrix_real::transpose_new() const {
    squere_Matrix_real current(this->get_size());
    for (int i{}; i < n_; i++) {
        for (int j{}; j < n_; j++) {
            *(current.first_el_iter() + n_*i + j) = matrix_[n_*j + i];
        }
    }
    return current;
}

squere_Matrix_real squere_Matrix_real::operator*(const squere_Matrix_real & matrix2) noexcept {
    squere_Matrix_real current(this->get_size());
    if (this->get_size() != matrix2.get_size()) {std::cout << "invalid size...\n"; return current;}
    squere_Matrix_real m2(matrix2.transpose_new());

    if (this->get_size() >= 8) {
        int count = n_ / 8;
        int r =  n_ % 8;

        for (int c{}; c < n_;c++) {
            for (int s{}; s < n_; s++) {
                float sum{};
                for (int i{}; i< count; i++) {
                    float * res_p = new float[8];
                    __m256 vec_a = _mm256_loadu_ps(this->first_el() + c*n_ + 8*i);
                    __m256 vec_b = _mm256_loadu_ps(m2.first_el() + s * n_ + 8*i);

                    __m256 result = _mm256_mul_ps(vec_a, vec_b);
                    
                    _mm256_storeu_ps(res_p, result);

                    for (int j{}; j < 8; j++) sum += *(res_p + j);

                }
                for (int j{}; j < r; j++) {
                    sum += matrix_[8*count + c*n_ + j] * (*(m2.first_el() + 8*count + s*n_ + j));
                }
                *(current.first_el_iter() + n_ * c + s) = sum;
            }   
        }
    }
    else if (this->get_size() >= 4 && this->get_size() < 8) {
        int count = n_ / 4;
        int r =  n_ % 4;

        for (int c{}; c < n_;c++) {
            float sum{};
            for (int s{}; s < n_; s++) {
                sum = 0.;
                for (int i{}; i< count; i++) {
                    float * res_p = new float[4];
                    __m128 vec_a = _mm_loadu_ps(this->first_el() + c*n_ + 4*i);
                    __m128 vec_b = _mm_loadu_ps(m2.first_el() + s * n_ + 4*i);

                    __m128 result = _mm_mul_ps(vec_a, vec_b);
                    
                    _mm_storeu_ps(res_p, result);

                    for (int j{}; j < 4; j++) sum += *(res_p + j);

                }
                for (int j{}; j < r; j++) {
                    sum += matrix_[4*count + c*n_ + j] * (*(m2.first_el() + 4*count + s*n_ + j));
                }
                *(current.first_el_iter() + n_ * c + s) = sum;
            }   
        }
    }
    else {
        for (int s{}; s < n_; s++) {
            float sum{};
            for (int i{}; i < n_; i++) {
                sum = 0.;
                for (int j{}; j < n_; j++) {
                    sum += matrix_[n_*s + j] * (*(m2.first_el() + n_*i + j));
                }
                *(current.first_el_iter() + n_*s + i) = sum;
            }
        }
    }
    return current;
}

float & squere_Matrix_real::operator()(int i, int j) {
    return this->matrix_[n_*(i-1) + j-1];
}

void squere_Matrix_real::show() const {
    for (int i{};i < n_; i++) {
        for (int j{}; j < n_; j++) {
            std::cout << std::setw(15) << matrix_[n_*i + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}


void squere_Matrix_real::transpose() {
	for (int i{}; i < n_; i++) {
		for (int j{i+1}; j < n_; j++) {
			std::swap(*(matrix_.begin() + n_ * i + j), *(matrix_.begin() + n_ * j + i));
 		}
	}
}


void squere_Matrix_real::swap_row(int i, int j) {
	std::swap_ranges(matrix_.begin() + n_ * i, matrix_.begin() + n_ * i + n_, matrix_.begin() + n_ * j);
}

void squere_Matrix_real::swap_col(int i, int j) {
	for (int row = 0; row < n_; ++row) {
		std::swap(matrix_[n_ * row + i], matrix_[n_ * row + j]);
	}
}  

float squere_Matrix_real::determinant() {
	for (int i{}; i < n_ - 1; i++) {   //в каждой строке ищем ведущий элемент
		auto max_el_iter = std::max_element(matrix_.cbegin() + n_ * i + i, matrix_.cbegin() + n_ * i + n_, [](float a, float b) {return std::abs(a) < std::abs(b); });
		int max_el_index = std::distance(matrix_.cbegin() + n_ * i, max_el_iter);     //i-й столбец мы будем умножать на отношение i-го элемента и ведущего, чтобы получить 0 на i-й позиции.
		if (max_el_index != i) this->swap_col(i, max_el_index);    //если ведущий элемент не на диагонали - меняем столбцы. 
 		this->transpose();   //транспонируем для реализации преобразования столбцов через непрерывные блоки, чтобы использовать SIMD. строка - это непрерывный блок
		for (int j{ i + 1 }; j < n_; j++) {    //текущий столбец мы не меняем, с помощью него меняем все остальные
			if (matrix_[n_ * i + i] < 1e-7) continue;    //если ведущий слишокм маленький, считаем, что весь столбец обнулен
			float rat = matrix_[n_ * j + i] / matrix_[n_ * i + i];   //олтношение текущего элемента и ведушего
			if (n_ - i >= 8) {    //условность только для выбора SIMD
				int count = (n_ - i) / 8;   //сколько целых 256 битных блоков занимае строка
				int remainder = (n_ - i) % 8;   //сколько значений остается
				for (int k{}; k < count; k++) {
					__m256 vec_a = _mm256_loadu_ps(&matrix_[n_ * i + i + 8*k]);
					__m256 multiplier = _mm256_set1_ps(rat);
					__m256 vec_b = _mm256_loadu_ps(&matrix_[n_ * j + i + 8*k]);
					__m256 vec_c = _mm256_sub_ps(vec_b, _mm256_mul_ps(vec_a, multiplier));
					_mm256_storeu_ps(&matrix_[n_ * j + i + 8*k], vec_c);
				}
				for (int k{}; k < remainder; k++) {
					matrix_[n_ * j + i + 8 * count + k] -= matrix_[n_ * i + i + 8 * count + k] * rat;
				}
			}
			else if ((n_ - i >= 4) && (n_ - i < 8)) {
				int count = (n_ - i) / 4;
				int remainder = (n_ - i) % 4;
				for (int k{}; k < count; k++) {
					__m128 vec_a = _mm_loadu_ps(&matrix_[n_ * i + i + 4 * k]);
					__m128 multiplier = _mm_set_ps1(rat);
					__m128 vec_b = _mm_loadu_ps(&matrix_[n_ * j + i + 4 * k]);
					__m128 vec_c = _mm_sub_ps(vec_b, _mm_mul_ps(vec_a, multiplier));
					_mm_storeu_ps(&matrix_[n_ * j + i + 4 * k], vec_c);
				}
				for (int k{}; k < remainder; k++) {
					matrix_[n_ * j + i + 4 * count + k] -= matrix_[n_ * i + i + 4 * count + k] * rat;
				}
			}
			else if (n_ - i <= 3) {
                for (int k{i}; k < n_;k++) {
                    matrix_[n_*j + k] -= matrix_[i*n_ + k] * rat;
                }
			}
		}
		this->transpose();
	}
    this->show();
	float det = 1.;
	for (int i{}; i < n_; i++) {
		det *= matrix_[n_ * i + i];
	}
	return det;

} 

void squere_Matrix_real::init_random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0, 1.);
    for (int i{}; i < n_; i++) {
        for (int j{}; j < n_; j++){
            matrix_[n_*i + j] = dist(gen);
        }
    }
}