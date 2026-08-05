#include "Matrix.h"

int main() {

    squere_Matrix_real m2(5, 1);
    squere_Matrix_real m3(5, matrix_type::UP_TRIANG, 2);


    m3.init_random();
    m3.show();
    float c = m3.determinant();
    std::cout << c;

    return 0;
}