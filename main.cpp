#include <iostream>
#include "matrix.h"

int main()
{
    Matrix<int, 0> matrix;
    for (int i = 0; i < 10; i++)
    {
        matrix[i][i] = i;
        matrix[i][9-i] = 9 - i;
    }

    for (int i = 1; i <= 8; i++)
    {
        for (int j = 0; j <= 8; j++)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << matrix.size() << std::endl;

    for(auto c: matrix)
    {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }
}