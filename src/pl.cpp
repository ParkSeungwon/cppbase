#include <iostream>
#include <boost/compute/algorithm/copy.hpp>
#include <boost/compute/algorithm/transform.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/functional/operator.hpp>
#include "matrix.h"

namespace compute = boost::compute;

// this example demonstrates how to use Boost.Compute's STL
// implementation to add two vectors on the GPU
int main()
{
    // setup input arrays
    float a[] = { 1, 2, 3, 4 };
    float b[] = { 5, 6, 7, 8 };
	Matrix<float> m{10, 10};
	Matrix<float> n{10, 10};
	Matrix<float> k{10, 10};


    // make space for the output
    float c[] = { 0, 0, 0, 0 };

    // create vectors and transfer data for the input arrays 'a' and 'b'
    compute::vector<float> vector_a(m.data(), m.data() + 100);
    compute::vector<float> vector_b(n.data(), n.data() + 100);
    compute::vector<float> vector_c(k.data(), k.data() + 100);

    // create vector for the output array

    // add the vectors together
    compute::transform(
        vector_a.begin(),
        vector_a.end(),
        vector_b.begin(),
        vector_c.begin(),
        compute::plus<float>()
    );

    // transfer results back to the host array 'c'
    compute::copy(vector_c.begin(), vector_c.end(), vector_a.begin());

    // print out results in 'c'
	std::cout << k << std::endl;

    return 0;
}
