#define BOOST_TEST_MODULE tests
#include "matrix.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( TestSuite )

    struct cout_redirect {
        cout_redirect( std::streambuf * new_buffer )
                : old( std::cout.rdbuf( new_buffer ) )
        { }

        ~cout_redirect( ) {
            std::cout.rdbuf( old );
        }

    private:
        std::streambuf * old;
    };

BOOST_AUTO_TEST_CASE(testMatrix1) {
        Matrix<int, -1> matrix(0);
        BOOST_CHECK_EQUAL(matrix.size(), 0); // все ячейки свободны
        BOOST_CHECK_EQUAL(matrix[0][0], -1);
        BOOST_CHECK_EQUAL(matrix.size(), 0);
        matrix[100][100] = 314;
        BOOST_CHECK_EQUAL(matrix[100][100], 314);
        BOOST_CHECK_EQUAL(matrix.size(), 1);
        // 100100314
        boost::test_tools::output_test_stream output;
        cout_redirect guard(output.rdbuf());
        for (auto c: matrix) {
            std::cout << c.GetRow() << c.GetColumn() << c.GetVal() << std::endl;
        }
        BOOST_CHECK(output.is_equal("100100314\n"));
    }
BOOST_AUTO_TEST_SUITE_END()


