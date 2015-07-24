#pragma once
#include <CL/cl.h>
#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>
#include <fstream>

#include "KeysGroup.h"

// Core of applicatoin. Generates image.
class Kernel
{
	unsigned int keys_size; // size of temporary array with points
	int nKeys; // number of key elements
	float *keys; // array with elements. size = 4 * keys. Each key has 4 floats, each by each
	unsigned int width; // output image width
	unsigned int height; // output image height

	cl_mem *mem_a; // pointer to GPU data of array with initial image
	cl_mem *mem_b; // pointer to GPU data of array with final image

	cl_mem *mem_tmp_p; // pointer to GPU data of temporary array with stuff can be calculated before parallel calculations
	cl_mem *mem_tmp_q; // pointer to GPU data of temporary array with stuff can be calculated before parallel calculations
	cl_mem *mem_tmp_c; // pointer to GPU data of temporary array with stuff can be calculated before parallel calculations

	cl_mem *mem_keys; // pointer to GPU data of  array with keys. float *keys;
	cl_mem *mem_output; // pointer to GPU data of output image

	// OpenCL stuff
	cl_platform_id platform_id;
	cl_device_id device_id;
	cl_context context ;
	cl_command_queue command_queue;
	cl_program program;
	cl_kernel kernel;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret;
	cl_image_format img_fmt;
	size_t* origin;
	size_t* region;

	float* data;//
	float* data_a; // inital image
	float* data_b; // final image
	
	float* tmp_p; // temporary data
	float* tmp_q; // temporary data
	float* tmp_c; // temporary data

	sf::Image output; // sfml output

	//void updateImage(cl_mem* mem, sf::Image *image, float* img_data); // not working
	void test(int r, int l); // debug function
public:
	Kernel(unsigned int width, unsigned int height, sf::Image *image_a, sf::Image *image_b, std::vector<KeysGroup> *data, int nKeys);
	~Kernel();

	
	void updateImageA(sf::Image *image_a); // change initial image
	void updateImageB(sf::Image *image_b); // change final image
	void updateKeys(std::vector<KeysGroup> *data, int nKeys); // change of keys data

	const sf::Image& run(float lambda); // get image at labda%
	
};

