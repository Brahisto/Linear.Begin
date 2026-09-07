

Matrix::Matrix(int n) noexcept {
    is_compute_det = false;
    if (n > 0) {
        matrix_.assign(n*n, 0);
        n_ = n;
        m_ = n;
        type_figure = matrix_type::SQUERE;
        type_struct = matrix_type::COMMON;
        determinant = 0;
    }
    else {
        n_ = 0;
        m_ = 0;
        std::cout << "The matrix size is 0. Try 'reset' with valid data...\n ";
    }
}

Matrix::Matrix(int n, float value) noexcept {
    is_compute_det = false;
    if (n > 0) {
        matrix_.assign(n*n, value);
        n_ = n;
        m_ = n;
        type_figure = matrix_type::SQUERE;
        type_struct = matrix_type::COMMON;
        determinant = 0;
    }
    else {
        n_ = 0;
        m_ = 0;
        std::cout << "The matrix size is 0. Try 'reset' with valid data...\n ";
    }
}

Matrix::Matrix(int n, matrix_type type) noexcept {
    is_compute_det = false;
    if (n > 0) {
        matrix_.assign(n*n, 0);
        n_ = n;
        m_ = n;
        type_figure = matrix_type::SQUERE;
    }
    else {
        n_ = 0;
        m_ = 0;
        std::cout << "The matrix size is 0. Try 'reset' with valid data...\n ";
    }

    switch (type) {
        case matrix_type::LOW_TRIANG:
            for (int i{};i < n_; i++) {
                for (int j{}; j <= i; j++) {
                    matrix_[n_*i + j] = 1;
                }
            }
            type_struct = matrix_type::LOW_TRIANG;
            break;

        case matrix_type::UP_TRIANG:
            for (int i{};i < n_; i++) {
                for (int j{}; j <= n_ - i - 1; j++) {
                    matrix_[n_*i + n_ - j - 1] = 1;
                }
            }
            type_struct = matrix_type::UP_TRIANG;
            break;

        case matrix_type::IDENTITY:
            for (int i{};i < n_; i++) matrix_[n*i + i] = 1;
            type_struct = matrix_type::IDENTITY;
            break;
        case matrix_type::ZERO:
            for (auto &i : matrix_) i = 0;
            type_struct = matrix_type::ZERO;
            break;
    }
}

Matrix::Matrix(int n, matrix_type type, float value) noexcept {
    is_compute_det = false;
    if (n > 0) {
        matrix_.assign(n*n, 0);
        n_ = n;
        m_ = n;
        type_figure = matrix_type::SQUERE;
    }
    else {
        n_ = 0;
        m_ = 0;
        std::cout << "The matrix size is 0. Try 'reset' with valid data...\n ";
    }

    switch (type) {
        case matrix_type::LOW_TRIANG:
            for (int i{};i < n_; i++) {
                for (int j{}; j <= i; j++) {
                    matrix_[n_*i + j] = value;
                }
            }
            type_struct = matrix_type::LOW_TRIANG;
            break;

        case matrix_type::UP_TRIANG:
            for (int i{};i < n_; i++) {
                for (int j{}; j <= n_ - i - 1; j++) {
                    matrix_[n_*i + n_ - j - 1] = value;
                }
            }
            type_struct = matrix_type::UP_TRIANG;
            break;

        case matrix_type::DIAGONAL:
            for (int i{};i < n_; i++) matrix_[n*i + i] = value;
            type_struct = matrix_type::DIAGONAL;
            break;
    }
}

Matrix::Matrix(int n, std::initializer_list<float> values) noexcept {
    is_compute_det = false;
    if (n > 0) {
        n_ = n;
        m_ = n;
        matrix_.assign(n*n, 0);
        matrix_ = values;
        type_figure = matrix_type::SQUERE;
        type_struct = matrix_type::COMMON;        
    }
    else {
        n_ = 0;
        m_ = 0;
        std::cout << "The matrix size is 0. Try 'reset' with valid data...\n ";
    }
}

Matrix::Matrix(int n, int m) noexcept {
    is_compute_det = false;
    if (n > 0 && m > 0) {
        matrix_.assign(m*n, 0);
        n_ = n; m_ = m;
        type_figure = matrix_type::RECTANG;
        type_struct = matrix_type::COMMON;
    }
    else  {
        std::cout << "invalid size.../n";
        m_ = 0;
        n_ = 0;
    }
}

Matrix::Matrix(int n, int m, std::initializer_list<float> values) noexcept {
    is_compute_det = false;
    if (n > 0 && m > 0) {
        matrix_.assign(m*n, 0);
        int count{};
        count = (m*n > values.size() ? values.size() : m*n);
        for (int i{}; i < count; i++) {
            *(matrix_.begin() + i) = *(values.begin() + i);
        }
        n_ = n; m_ = m;
        type_figure = matrix_type::RECTANG;
        type_struct = matrix_type::COMMON;
    }
    else  {
        std::cout << "invalid size.../n";
        m_ = 0;
        n_ = 0;
    }
}


Matrix Matrix::operator*(const Matrix & right) noexcept {
    Matrix result(n_, right.m_);

    if (m_ == right.n_) {
        if (m_ >= 8) {
            int count = m_ / 8;
            int remainder = m_ % 8;
            float sum = 0;
            float * res = new float[8];

            for (int i{}; i < n_;i++) {
                for (int k{}; k < right.n_;k++) {
                    for (int j{}; j < count; j++) {
                        __m256 vec_a = _mm256_loadu_ps(this->first_el() + m_*i + j*8);
                        __m256 vec_b = _mm256_loadu_ps(right.first_el() + right.m_*k + j*8);
                        __m256 multiply = _mm256_mul_ps(vec_a, vec_b);
                        _mm256_storeu_ps(res, multiply);
                        for (int q{}; q < 8; q++) sum += *(res + q);
                    }

                    for (int j{}; j < remainder; j++) {
                        sum += matrix_[m_*i + count*8 + j] * right.matrix_[right.m_ * k + count*8 + j];
                    }
                    result.matrix_[right.n_*i + k] = sum;
                }
            }
            delete[] res;
        }

        else if (m_ >= 4 && m_ < 8) {
            int count = m_ / 4;
            int remainder = m_ % 4;
            float sum = 0;
            float * res = new float[4];

            for (int i{}; i < n_;i++) {
                for (int k{}; k < right.n_;k++) {
                    for (int j{}; j < count; j++) {
                        __m128 vec_a = _mm_loadu_ps(this->first_el() + m_*i + j*4);
                        __m128 vec_b = _mm_loadu_ps(right.first_el() + right.m_*k + j*4);
                        __m128 multiply = _mm_mul_ps(vec_a, vec_b);
                        _mm_storeu_ps(res, multiply);
                        for (int q{}; q < 4; q++) sum += *(res + q);
                    }

                    for (int j{}; j < remainder; j++) {
                        sum += matrix_[m_*i + count*4 + j] * right.matrix_[right.m_ * k + count*4 + j];
                    }
                    result.matrix_[right.n_*i + k] = sum;
                }
            }
            delete[] res;
        }

        else {
            float sum{};
            for (int i{}; i < n_; i++) {
                for (int j{}; j < right.m_; j++) {
                    sum = 0;
                    for (int k{}; k < m_; k++) {
                        sum += matrix_[m_ * i + k] * right.matrix_[right.m_ * j + k];
                    }
                    result.matrix_[right.n_ * i + j] = sum;
                }
            }
        }
    }

    else std::cout << "impossible to multiplicate...\n";

    return result;
}

Matrix Matrix::operator+(const Matrix & matrix2) noexcept {
    Matrix current(*this);
    
    if (this->get_size() == matrix2.get_size()) {
        int count = n_*m_ / 8;    //используем _m256, вмещающим 8 int and 8 float

        int remainder = (n_*m_) % 8;

        for (int i{}; i < count; i++) {
            __m256 vec_a = _mm256_loadu_ps(this->first_el() + 8*i);
            __m256 vec_b = _mm256_loadu_ps(matrix2.first_el() + 8*i);

            __m256 result = _mm256_add_ps(vec_a, vec_b);

            _mm256_storeu_ps(current.first_el_non_const() + 8*i, result);
        }
    
        for (int j{}; j < remainder; j++) {
            *(current.first_el_non_const() + 8*count + j) = *(this->first_el() + 8*count + j) + *(matrix2.first_el() + 8*count + j);
        }
    }

    return current;
}

Matrix Matrix::operator-(const Matrix & matrix2) noexcept {
    Matrix current(*this);
    
    if (this->get_size() == matrix2.get_size()) {
        int count = n_*m_ / 8;    //используем _m256, вмещающим 8 int and 8 float

        int remainder = (n_*m_) % 8;

        for (int i{}; i < count; i++) {
            __m256 vec_a = _mm256_loadu_ps(this->first_el() + 8*i);
            __m256 vec_b = _mm256_loadu_ps(matrix2.first_el() + 8*i);

            __m256 result = _mm256_sub_ps(vec_a, vec_b);

            _mm256_storeu_ps(current.first_el_non_const() + 8*i, result);
        }
    
        for (int j{}; j < remainder; j++) {
            *(current.first_el_non_const() + 8*count + j) = *(this->first_el() + 8*count + j) - *(matrix2.first_el() + 8*count + j);
        }
    }
    return current;
}


const Matrix Matrix::transpose_new() const {
    Matrix current(m_, n_);
    for (int i{}; i < n_; i++) {
        for (int j{}; j < m_; j++) {
            current.matrix_[n_*j + i] = matrix_[m_*i + j];
        }
    }
    return current;
}

const float & Matrix::operator()(int i, int j) const {
    return this->matrix_[m_*i + j];
}

void Matrix::show() const {
    for (int i{};i < n_; i++) {
        for (int j{}; j < m_; j++) {
            std::cout << std::setw(15) << matrix_[m_*i + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}


void Matrix::swap_row(int i, int j) {
	std::swap_ranges(matrix_.begin() + m_ * i, matrix_.begin() + m_ * i + m_, matrix_.begin() + m_ * j);
}

void Matrix::swap_col(int i, int j) {
    for (int row = 0; row < n_; ++row) {
		std::swap(matrix_[m_ * row + i], matrix_[m_ * row + j]);
	}
}

int Matrix::to_up_triang() {
    if (type_figure == matrix_type::SQUERE) {
        int count_swap = 0;
        std::vector<float> i_element_row(n_, 0.);

        for (int i{}; i < n_-1; i++) {
            for (int j{}; j < n_-i; j++) {
                i_element_row[i + j] = matrix_[n_ * i + i + n_ * j];  //j задает номер строки
            }

            auto max_element_row_iter = std::max_element(i_element_row.begin() + i, i_element_row.end(), [](float a, float b) {return std::abs(a) < std::abs(b); });
            int max_element_row_num = std::distance(i_element_row.begin(), max_element_row_iter);
            if (i != max_element_row_num) {
                this->swap_row(i, max_element_row_num);   //привели к ведущему элементу.
                count_swap++;   
            }
            if (std::abs(matrix_[n_ * i + i]) < 1e-11) continue; //{determinant = 0.; return determinant;}  //если ведущий слишокм маленький, считаем, что весь столбец обнулен
            for (int j{ i + 1 }; j < n_; j++) {    //текущую строку мы не меняем, с помощью нее меняем все остальные
                float rat = matrix_[n_ * j + i] / matrix_[n_ * i + i];   //отношение текущего элемента и ведушего
                if (n_ - i >= 8) {    //условность только для выбора SIMD
                    int count = (n_ - i) / 8;   //сколько целых 256 битных блоков занимае строка
                    int remainder = (n_ - i) % 8;   //сколько значений остается
                    for (int k{}; k < count; k++) {
                        __m256 vec_a = _mm256_loadu_ps(&matrix_[n_ * i + i + 8 * k]);
                        __m256 multiplier = _mm256_set1_ps(rat);
                        __m256 vec_b = _mm256_loadu_ps(&matrix_[n_ * j + i + 8 * k]);
                        __m256 vec_c = _mm256_sub_ps(vec_b, _mm256_mul_ps(vec_a, multiplier));
                        _mm256_storeu_ps(&matrix_[n_ * j + i + 8 * k], vec_c);
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
                else {
                    for (int k{ i }; k < n_; k++) {
                        matrix_[n_ * j + k] -= matrix_[n_*i + k] * rat;
                    }
                }
            }
        }
        return count_swap;
    }
    else {
        std::cout << "cannot transform rectangular matrix...\n";
        return 0;
    }
} 

float Matrix::det() {
    if (type_figure == matrix_type::SQUERE) {
        Matrix current = *this;
        int c = current.to_up_triang();

        determinant = 1.;
        for (int i{}; i < n_; i++) {
            determinant *= current.matrix_[n_ * i + i];
        }
        if (c % 2 == 1 && (determinant)) determinant *= -1;
        is_compute_det = true;
        return determinant;
    }
    else {
        std::cout << "cannot find a det for rectangular matrix...\n";
        return 0;
    }
    
}

void Matrix::init_random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0, 1.);
    for (int i{}; i < n_; i++) {
        for (int j{}; j < m_; j++){
            matrix_[m_*i + j] = dist(gen);
        }
    }
}

Matrix Matrix::except_ij_row_col(int i, int j) const {
    if (type_figure == matrix_type::SQUERE) {
        Matrix current (n_-1, (float)0.);
        for (int g{}; g < current.get_size();g++) {
            for (int h{}; h < current.get_size(); h++) {
                if (h < j and g < i) *(current.first_el_iter() + (n_-1)*g + h) = this->operator()(g, h);
                else if (h < j and g >= i) *(current.first_el_iter() + (n_-1)*g + h) = this->operator()(g+1, h);
                else if (h >= j and g < i) *(current.first_el_iter() + (n_-1)*g + h) = this->operator()(g, h+1);
                else if (h >= j and g >= i) *(current.first_el_iter() + (n_-1)*g + h) = this->operator()(g+1, h+1);
            }
        }
        return current;
    }
    else {
        std::cout << "only for squere matrixes\n";
        return *this;
    }
}


Matrix Matrix::reverse() {
    if (type_figure == matrix_type::SQUERE) {
        Matrix reverse_matrix (n_, (float)0.);
        if (is_compute_det == false) this->det();
        float d = this->determinant; 
        if (d != 0) {
        algebraic_addition M;
        for (int i{}; i < n_; i++) {
            for (int j{}; j< n_; j++) {

                M.signum = ((i + j) % 2 == 0 ? 1 : -1 );
                M.minor = (this->except_ij_row_col(i, j)).det();

                reverse_matrix.matrix_[n_*i + j] = M.signum * M.minor; 
            }
        }
        reverse_matrix = reverse_matrix.transpose_new();
        for (auto & i : reverse_matrix.matrix_) {i*= 1/d;};
        }
        else std::cout << "error.The determinant is zero...\n";
        return reverse_matrix;
    }
    else {
        std::cout << "reverse matrix is exist only for squere matrixes...\n";
        return *this;
    }
}

std::vector<Matrix> Matrix::LU_decomposition() {
    if (type_figure == matrix_type::SQUERE) {
        std::vector<Matrix> a{Matrix (n_, matrix_type::IDENTITY), *this};
        for (int i{}; i < n_ - 1; i++) {
            for (int j{ i + 1 }; j < n_; j++) {    //текущую строку мы не меняем, с помощью нее меняем все остальные
                if (std::abs((a[1]).matrix_[n_ * i + i]) < 1e-11) continue;//если ведущий слишокм маленький, считаем, что весь столбец обнулен
                float rat = (a[1]).matrix_[n_ * j + i] / (a[1]).matrix_[n_ * i + i];   //олтношение текущего элемента и ведушего
                if (n_ - i >= 8) {    //условность только для выбора SIMD
                    int count = (n_ - i) / 8;   //сколько целых 256 битных блоков занимае строка
                    int remainder = (n_ - i) % 8;   //сколько значений остается
                    for (int k{}; k < count; k++) {
                        __m256 vec_a = _mm256_loadu_ps(&((a[1]).matrix_[n_ * i + i + 8 * k]));
                        __m256 multiplier = _mm256_set1_ps(rat);
                        __m256 vec_b = _mm256_loadu_ps(&((a[1]).matrix_[n_ * j + i + 8 * k]));
                        __m256 vec_c = _mm256_sub_ps(vec_b, _mm256_mul_ps(vec_a, multiplier));
                        _mm256_storeu_ps(&((a[1]).matrix_[n_ * j + i + 8 * k]), vec_c);

                        vec_a = _mm256_loadu_ps(&((a[0]).matrix_[n_ * i + i + 8 * k]));
                        multiplier = _mm256_set1_ps(rat);
                        vec_b = _mm256_loadu_ps(&((a[0]).matrix_[n_ * j + i + 8 * k]));
                        vec_c = _mm256_sub_ps(vec_b, _mm256_mul_ps(vec_a, multiplier));
                        _mm256_storeu_ps(&((a[0]).matrix_[n_ * j + i + 8 * k]), vec_c);
                    }
                    for (int k{}; k < remainder; k++) {
                        (a[1]).matrix_[n_ * j + i + 8 * count + k] -= (a[1]).matrix_[n_ * i + i + 8 * count + k] * rat;
                        (a[0]).matrix_[n_ * j + i + 8 * count + k] -= (a[0]).matrix_[n_ * i + i + 8 * count + k] * rat;
                    }
                }
                else if ((n_ - i >= 4) && (n_ - i < 8)) {
                    int count = (n_ - i) / 4;
                    int remainder = (n_ - i) % 4;
                    for (int k{}; k < count; k++) {
                        __m128 vec_a = _mm_loadu_ps(&((a[1]).matrix_[n_ * i + i + 4 * k]));
                        __m128 multiplier = _mm_set_ps1(rat);
                        __m128 vec_b = _mm_loadu_ps(&((a[1]).matrix_[n_ * j + i + 4 * k]));
                        __m128 vec_c = _mm_sub_ps(vec_b, _mm_mul_ps(vec_a, multiplier));
                        _mm_storeu_ps(&((a[1]).matrix_[n_ * j + i + 4 * k]), vec_c);

                        vec_a = _mm_loadu_ps(&((a[0]).matrix_[n_ * i + i + 4 * k]));
                        multiplier = _mm_set_ps1(rat);
                        vec_b = _mm_loadu_ps(&((a[0]).matrix_[n_ * j + i + 4 * k]));
                        vec_c = _mm_sub_ps(vec_b, _mm_mul_ps(vec_a, multiplier));
                        _mm_storeu_ps(&((a[0]).matrix_[n_ * j + i + 4 * k]), vec_c);
                    }
                    for (int k{}; k < remainder; k++) {
                        (a[1]).matrix_[n_ * j + i + 4 * count + k] -= (a[1]).matrix_[n_ * i + i + 4 * count + k] * rat;
                        (a[0]).matrix_[n_ * j + i + 4 * count + k] -= (a[0]).matrix_[n_ * i + i + 4 * count + k] * rat;
                    }
                }
                else {
                    for (int k{ i }; k < n_; k++) {
                        (a[1]).matrix_[n_ * j + k] -= (a[1]).matrix_[n_ * i + k] * rat;
                    }
                    for (int k{}; k < n_; k++) {
                        (a[0]).matrix_[n_ * j + k] -= (a[0]).matrix_[n_ * i + k] * rat;
                    }
                }
            }
        }
        return a;
    }

    else {
        std::cout << "only for squere matrixes...\n";
        return std::vector<Matrix> ({0, 0});
    }
}


float scalar_product(const std::vector<float> & x, Matrix& G, const std::vector<float> & y) {  //invoking matrix as Gram's matrix.
    if (x.size() == y.size() && x.size() == G.col_count()) {    //input data - vectors 1xn
        int n{x.size()};
        float dot{};
        for (int i{}; i < n; i++) {
            for (int j{}; j < n; j++) {
                dot += x[j] * (*(G.first_el_iter() + n*i + j)) * y[i];  
            }
        }
        return dot;
    }
    else {
        std::cout << "dimentions is not equal\n";
        return 0;
    }
}


