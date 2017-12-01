#include <vector>
#include <algorithm>

#include <boost/compute/algorithm/transform.hpp>
#include <boost/compute/algorithm/accumulate.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/functional/math.hpp>
#include <iostream>
namespace compute = boost::compute;

int main()
{
    // get default device and setup context

    // generate random data on the host
    std::vector<float> host_vector(1000000);
    std::generate(host_vector.begin(), host_vector.end(), rand);
	for(int i=0; i<100; i++) std::cout << host_vector[i] << ' ';

    // create a vector on the device
    compute::vector<float> device_vector(host_vector.size());

    // transfer data from the host to the device
    compute::copy(
        host_vector.begin(), host_vector.end(), device_vector.begin()
    );
	
	auto add_four = compute::make_function_from_source<float(float,float)>("add_four", 
			"float add_four(float f,float g) { return cos(f) + exp(-f) + sin(f) + g / 1e+9; }");

    // calculate the square-root of each element in-place
    compute::transform(
        device_vector.begin(),
        device_vector.end(),
        device_vector.begin(),
        device_vector.begin(),
        add_four//compute::plus<float>()
    );

	float f; compute::reduce(device_vector.begin(), device_vector.end(), &f);
	std::cout << "accum " << f << std::endl;
    // copy values back to the host
    compute::copy(
        device_vector.begin(), device_vector.end(), host_vector.begin()
    );
	std::cout << std::endl;
	for(int i=0; i<100; i++) std::cout << host_vector[i] << ' ';

    return 0;
}
