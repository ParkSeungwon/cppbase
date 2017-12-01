#pragma once
#include<vector>
#include <boost/compute/algorithm/copy.hpp>
#include <boost/compute/container/vector.hpp>
#include"matrix.h"

class CLMat : public Matrix<float>
{
	enum Mode {LEFT, RIGHT} mode_;
	enum Sync {CPU = -1, SYNC, GPU} sync_;

public:
	CLMat(unsigned w, unsigned h);
	CLMat& operator=(const CLMat& r);
	float* operator[](int x);
	float* operator[](int x) const;
	CLMat operator*(CLMat& r);

	bool mode(Mode m); Mode mode() const; 
	bool repeat(int n); int repeat() const;

protected:
	boost::compute::vector<float> v_;

private:
	void sync();
	int repeat_ = 1;

	void cpu2gpu();
	void gpu_fill();
};
