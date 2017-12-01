#include<cassert>
#include<boost/compute/core.hpp>
#include<boost/compute/lambda.hpp>
#include<boost/compute/algorithm/copy.hpp>
#include<boost/compute/algorithm/transform.hpp>
#include"clmat.h"
using namespace std;
namespace compute = boost::compute;

CLMat::CLMat(unsigned w, unsigned h) : Matrix<float>{w, h}
{ }

CLMat& CLMat::operator=(const CLMat& r)
{
	assert(r.width == width && r.height == height);
	if(r.sync_ != CPU) for(int i=0; i<repeat_; i++) 
		compute::copy_n(r.v_.begin(), width*height, v_.begin() + width*height*i);
	else {
		compute::copy_n(r.data(), width*height, v_.begin());
		for(int i=1; i<repeat_; i++)
			compute::copy_n(v_.begin(), width*height, v_.begin() + width*height*i);
	}
	sync_ = GPU;
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
	CLMat rt{width, height};
	compute::transform(v_.begin(), v_.end(), r.v_.begin(), rt.v_.begin(), compute::lambda::_1 * compute::lambda::_2);
	return rt;
}

void CLMat::sync()
{
	if(sync_ == CPU) cpu2gpu();
	else if(sync_ == GPU) compute::copy(v_.begin(), v_.end(), data());
	sync_ = SYNC;
}

bool CLMat::repeat(int n)
{
	if(n == repeat_) return false;
	v_.resize(width * height * n);
	if(sync_ != CPU) for(int i=0; i<n-repeat_; i++)//gpu copy, if cpu lazy 
		compute::copy_n(v_.begin(), width * height, 
						v_.begin() + width * height * (repeat_ + i));
	repeat_ = n;
	return true;
}

bool CLMat::mode(Mode m)
{
	if(m == mode_) return false;
	if(sync_ == GPU) {
		compute::copy_n(v_.begin(), width * height, data());
		if(mode_ == LEFT) transpose();//now CPU synced
	}
	mode_ = m;
	cpu2gpu();
	return true;
}

void CLMat::cpu2gpu()
{
	if(mode_ == LEFT) transpose();
	compute::copy_n(data(), width * height, v_.begin());
	if(mode_ == LEFT) transpose();
	for(int i=1; i<repeat_; i++)
		compute::copy_n(v_.begin(), width*height, v_.begin() + width * height * i);
	sync_ = SYNC;
}

void CLMat::gpu_fill()
{
	for(int i=1; i<repeat_; i++)
		compute::copy_n(v_.begin(), width*height, v_.begin() + width * height * i);
}

