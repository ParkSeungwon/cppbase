#pragma once
#include<vector>
#include <boost/compute/algorithm/copy.hpp>
#include <boost/compute/container/vector.hpp>
#include"matrix.h"

class Neural
{//neural network
public:
	Neural(std::vector<unsigned> v);
	void forward_feed();
	bool back_propagation();
	void update_layer(float alpha = 0.01);
	void show();

protected:
	std::vector<Matrix<float>> weights_, layers_;
	std::vector<boost::compute::vector<float>> cl_weights_, cl_layers_;

private:
	boost::compute::device device_;// = compute::system::default_device();
	boost::compute::context context_;//(device);
	boost::compute::command_queue queue_;//(context, device);
};


