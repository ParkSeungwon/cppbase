#include<random>
#include"neural.h"
using namespace std;
namespace compute = boost::compute;

Neural::Neural(vector<unsigned> v) 
	: device_{compute::system::default_device()}, context_{device_}, 
	  queue_{context_, device_}
{//N, Feature, Hidden1, H2, H3, ..., O 
	for(int i=1; i<v.size()-1; i++) {
		weights_.push_back(Matrix<float>{v[i+1], v[i]+1});
		layers_.push_back(Matrix<float>{v[i]+1, v[0]});
	}
	layers_.push_back(Matrix<float>{v[v.size()-1], v[0]});//output
	layers_.push_back(Matrix<float>{v[v.size()-1], v[0]});//label

	uniform_real_distribution<float> di{-1, 1}; random_device rd;
	for(auto& m : weights_) for(int i=1, w=m.get_width(), h=m.get_height(); i<=w; i++) 
		for(int j=1; j<=h; j++) m[i][j] = di(rd);//initialize weights
	for(auto& m : layers_)//for threshold multiply
		for(int i=1, w=m.get_width(), h=m.get_height(); i<=h; i++) m[w][i] = 1;
	for(const auto& m : weights_) cl_weights_.push_back(compute::vector<float>(m.data(), m.data() + m.get_width() * m.get_height()));
	for(const auto& m : layers_) cl_layers_.push_back(compute::vector<float>(m.data(), m.data() + m.get_width() * m.get_height()));
}

void Neural::show()
{
	cout << device_.name() << endl;
	for(const auto& m : layers_) cout << m << endl;//operator<< Matrix bug when 1 line
	for(const auto& m : weights_) cout << m << endl;
	cout << "vector: [ ";
	boost::compute::copy(
			cl_layers_.begin()->begin(), cl_layers_.begin()->end(),
			std::ostream_iterator<float>(std::cout, ", ")
			);
	cout << "]" << endl;
}

