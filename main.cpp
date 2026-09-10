#include "Matrix.h"

int main() {

    Matrix some (3, {1, 0, 0, 0, 1, 0, 0, 0, 1});
    some.show();
    Matrix Gram (3, {3, 0, 2, 0, 5, 3, 2, 3, 6});
    some.orthogonalization(Gram);

    float c = some.ort_check(2, 3, Gram);

    std::cout << c;

    return 0;
}