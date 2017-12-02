#include<cassert>
#include<boost/compute/core.hpp>
#include<boost/compute/lambda.hpp>
#include<boost/compute/algorithm/copy.hpp>
#include<boost/compute/algorithm/transform.hpp>
#include<boost/compute/algorithm/reduce_by_key.hpp>
#include<boost/compute/iterator/counting_iterator.hpp>
#include"clmat.h"
using namespace std;
namespace compute = boost::compute;

CLMat::CLMat(unsigned w, unsigned h) : Matrix<float>{w, h}
{
	repeat(1);
	mode(RIGHT);
}

CLMat::CLMat(Matrix<float> r) : Matrix<float>{r}
{
	repeat(1);
	mode(RIGHT);
}

CLMat::CLMat(CLMat&& r) : Matrix<float>{static_cast<Matrix<float>&&>(r)}
{
	mode_ = r.mode_;
	sync_ = r.sync_;
	repeat_ = r.repeat_;
	v_ = move(r.v_);
}

CLMat::CLMat(const CLMat& r) : CLMat{static_cast<Matrix<float>>(r)}
{
	mode_ = r.mode_;
	sync_ = r.sync_;
	repeat_ = r.repeat_;
	v_ = r.v_;
}	

CLMat CLMat::operator=(const CLMat& r)
{
	if(width * height != r.width * r.height) {
		delete [] arr;
		arr = new float[width * height];
	}
	width = r.width; height = r.height;
	mode_ = r.mode_;
	sync_ = r.sync_;
	repeat_ = r.repeat_;
	v_.resize(r.v_.size());
	if(r.sync_ == CPU) this->Matrix<float>::operator=(static_cast<Matrix<float>>(r));
	else v_ = r.v_, sync_ = GPU;
	return *this;
}

float* CLMat::operator[](int x)
{
	sync_ = CPU;
	return Matrix<float>::operator[](x);
}

float* CLMat::operator[](int x) const
{
	return Matrix<float>::operator[](x);
}

CLMat CLMat::operator*(CLMat& r)
{
	assert(width == r.height);
	repeat(r.width); r.repeat(height);
	mode(LEFT); r.mode(RIGHT);
	if(sync_ == CPU) cpu2gpu(); if(r.sync_ == CPU) r.cpu2gpu();
	CLMat rt{height, r.width};
	int sz = v_.size();
	compute::vector<float> v(sz), key(sz / width), value(sz / width);
	compute::transform(v_.begin(), v_.end(),
			r.v_.begin(), v.begin(), compute::lambda::_1 * compute::lambda::_2);
	compute::reduce_by_key(
			compute::make_transform_iterator(compute::make_counting_iterator<int>(0), compute::lambda::_1 / width),
			compute::make_transform_iterator(compute::make_counting_iterator<int>(sz), compute::lambda::_1 / width),
			v.begin(), key.begin(), value.begin()
	);
	compute::copy_n(value.begin(), height * r.width, rt.v_.begin());
	rt.mode_ = LEFT;
	rt.sync_ = GPU;
	return move(rt);
}

void CLMat::sync()
{
	if(sync_ == CPU) cpu2gpu();
	else if(sync_ == GPU) gpu2cpu();
}

bool CLMat::repeat(int n)
{//repeat 
	if(n == repeat_) return false;
	v_.resize(width * height * n);
	if(sync_ != CPU) for(int i=0; i<n-repeat_; i++)//gpu copy, if cpu lazy 
		compute::copy_n(v_.begin(), width * height, 
						v_.begin() + width * height * (repeat_ + i));
	repeat_ = n;
	return true;
}

void CLMat::show()
{
	compute::copy(v_.begin(), v_.end(), ostream_iterator<float>(cout, ", "));
	cout << endl;
}

bool CLMat::mode(Mode m)
{//LEFT is when matrix is left side of multiplication
	if(m == mode_) return false;
	if(sync_ != CPU) gpu2cpu();
	mode_ = m;
	sync_ = CPU;
	return true;
}

void CLMat::cpu2gpu()
{
	if(mode_ == LEFT) {
		*this = transpose();
		cout << "HH"; show();
		for(int j=0; j<height; j++) for(int i=0; i<repeat_; i++) 
			compute::copy_n(data() + width * j, width, v_.begin()+width*(height*i+j));
		cout << "H2"; show();
		*this = transpose();
	} else {
		compute::copy_n(data(), width * height, v_.begin());
		for(int i=1; i<repeat_; i++)
			compute::copy_n(v_.begin(), width * height, v_.begin() + i*width*height);
	}
	sync_ = SYNC;
}

void CLMat::gpu2cpu()
{//becaust Matrix is column major
	if(mode_ == RIGHT) compute::copy_n(v_.begin(), width * height, data());
	else for(int i=0; i<height; i++) 
		compute::copy_n(v_.begin(), width, data() + i * width * repeat_);
	sync_ = SYNC;
}

ostream& operator<<(ostream& o, CLMat& r)
{
	if(r.sync_ == CLMat::GPU) r.gpu2cpu();
	o << static_cast<Matrix<float>>(r);
	return o;
}

