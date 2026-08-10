#include "Matrix.h"

int main() {

    std::initializer_list<float> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    squere_Matrix_real m2(3, a);


    std::vector<squere_Matrix_real> LU {squere_Matrix_real (3), squere_Matrix_real (3)};

    LU = m2.LU_decomposition();

    squere_Matrix_real m4(3);
    m4 = LU[0] * LU[1];

    LU[0].show();
    LU[1].show();

    m4.show();

    return 0;
}