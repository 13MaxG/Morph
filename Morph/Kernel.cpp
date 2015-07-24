#include "Kernel.h"


Kernel::Kernel(unsigned int width, unsigned int height, sf::Image *image_a, sf::Image *image_b, std::vector<KeysGroup> *groups, int nKeys)
{
	this->width = width;
	this->height = height;
	data = new float[width * height * 4];
	data_a = new float[width * height * 4];
	data_b = new float[width * height * 4];

	output.create(width, height, sf::Color::Black);
	keys_size = 4 * 2000;
	keys = new float[keys_size];
	tmp_p = new float[keys_size / 2 + 1];
	tmp_q = new float[keys_size / 2 + 1];
	tmp_c = new float[keys_size / 2 + 1];

	this->nKeys = nKeys;

	platform_id = NULL;
	device_id = NULL;
	context = NULL;
	command_queue = NULL;
	program = NULL;
	kernel = NULL;

	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms); test(ret, __LINE__);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices); test(ret, __LINE__);
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret); test(ret, __LINE__);
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret); test(ret, __LINE__);

	img_fmt.image_channel_order = CL_RGBA;
	img_fmt.image_channel_data_type = CL_FLOAT;
	origin = new size_t[3]; origin[0] = 0; origin[1] = 0; origin[2] = 0;
	region = new size_t[3]; region[0] = width; region[1] = height; region[2] = 1;


	for (int i = 0; i < 4 * width*height; i++)
		data[i] = 0;

	mem_output = new cl_mem;
	*mem_output = clCreateImage2D(context, CL_MEM_READ_WRITE, &img_fmt, width, height, 0, 0, &ret); test(ret, __LINE__);
	//ret = clEnqueueWriteImage(command_queue, *mem_output, CL_TRUE, origin, region, 4 * sizeof(float), 0, data, 0, NULL, NULL);
	test(ret, __LINE__);

	updateImageA(image_a);
	updateImageB(image_b);
	updateKeys(groups, nKeys);


	std::fstream file;
	file.open("./morph.cl", std::ios_base::in);
	size_t source_size;
	const char *source_str;
	std::string source;
	std::string buffer;
	while (std::getline(file, buffer)) source.append(buffer + "\n");
	source_size = source.length();
	source_str = source.c_str();
	std::cout << source << std::endl;
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret); test(ret, __LINE__);
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL); test(ret, __LINE__);
	kernel = clCreateKernel(program, "morph", &ret); test(ret, __LINE__);



}

void Kernel::updateKeys(std::vector<KeysGroup> *data, int nKeys)
{
	this->nKeys = nKeys;
	bool x = false;
	while (nKeys * 4 > keys_size)
	{
		keys_size += 2 * 4 * keys_size;
		x = true;
	}
	if (x)
	{
		delete keys;
		delete tmp_p;
		delete tmp_q;
		delete tmp_c;
		keys = new float[keys_size];
		tmp_p = new float[keys_size / 2 + 1];
		tmp_q = new float[keys_size / 2 + 1];
		tmp_c = new float[keys_size / 2 + 1];
	}

	int p = 0;
	for (int i = 0; i < data->size(); i++)
	{
		for (int j = 0; j < (*data)[i].getSize(); j++)
		{
			keys[p++] = (*data)[i].get(j).first.first * width;
			keys[p++] = (*data)[i].get(j).first.second* height;
			keys[p++] = (*data)[i].get(j).second.first* width;
			keys[p++] = (*data)[i].get(j).second.second* height;


		}
	}

	if (mem_keys != NULL)
	{
		ret = clReleaseMemObject(*mem_keys); test(ret, __LINE__);
		delete mem_keys;

		ret = clReleaseMemObject(*mem_tmp_p); test(ret, __LINE__);
		delete mem_tmp_p;
		ret = clReleaseMemObject(*mem_tmp_q); test(ret, __LINE__);
		delete mem_tmp_q;
		ret = clReleaseMemObject(*mem_tmp_c); test(ret, __LINE__);
		delete mem_tmp_c;
	}
	mem_keys = new cl_mem;
	std::cout << "KEYS SIZE: " << keys_size << std::endl;
	*mem_keys = clCreateBuffer(context, CL_MEM_READ_ONLY, keys_size * sizeof(float), NULL, &ret); test(ret, __LINE__);
	clEnqueueWriteBuffer(command_queue, *mem_keys, CL_TRUE, 0, keys_size * sizeof(float), keys, 0, NULL, NULL); test(ret, __LINE__);

	mem_tmp_p = new cl_mem;
	*mem_tmp_p = clCreateBuffer(context, CL_MEM_READ_ONLY, 1 + keys_size / 2 * sizeof(float), NULL, &ret); test(ret, __LINE__);
	clEnqueueWriteBuffer(command_queue, *mem_tmp_p, CL_TRUE, 0, 1 + keys_size / 2 * sizeof(float), tmp_p, 0, NULL, NULL); test(ret, __LINE__);

	mem_tmp_q = new cl_mem;
	*mem_tmp_q = clCreateBuffer(context, CL_MEM_READ_ONLY, 1 + keys_size / 2 * sizeof(float), NULL, &ret); test(ret, __LINE__);
	clEnqueueWriteBuffer(command_queue, *mem_tmp_q, CL_TRUE, 0, 1 + keys_size / 2 * sizeof(float), tmp_q, 0, NULL, NULL); test(ret, __LINE__);

	mem_tmp_c = new cl_mem;
	*mem_tmp_c = clCreateBuffer(context, CL_MEM_READ_ONLY, 1 + keys_size / 2 * sizeof(float), NULL, &ret); test(ret, __LINE__);
	clEnqueueWriteBuffer(command_queue, *mem_tmp_c, CL_TRUE, 0, 1 + keys_size / 2 * sizeof(float), tmp_c, 0, NULL, NULL); test(ret, __LINE__);


}

/*
void Kernel::updateImage(cl_mem* mem, sf::Image *image, float* img_data)
{
	int p = 0;
	for (int y = 0; y < image->getSize().y; y++)
	{
		for (int x = 0; x < image->getSize().x; x++)
		{
			img_data[p++] = image->getPixel(x, y).r;
			img_data[p++] = image->getPixel(x, y).g;
			img_data[p++] = image->getPixel(x, y).b;
			img_data[p++] = image->getPixel(x, y).a;

		}
	}
	if (mem != NULL)
	{
		ret = clReleaseMemObject(*mem); test(ret, __LINE__);
		delete mem;
	}
	mem = new cl_mem;
	*mem = clCreateBuffer(context, CL_MEM_READ_ONLY, 4 * image->getSize().x* image->getSize().y*sizeof(float), NULL, &ret);							// clCreateImage2D(context, CL_MEM_READ_WRITE, &img_fmt_a, image_a.getSize().x, image_a.getSize().y, 0, 0, &ret);
	test(ret, __LINE__);
	clEnqueueWriteBuffer(command_queue, *mem, CL_TRUE, 0, 4 * sizeof(float)* image->getSize().x* image->getSize().y, img_data, 0, NULL, NULL);
	test(ret, __LINE__);
	std::cout << "hejka" << std::endl;
}*/

void Kernel::updateImageA(sf::Image *image)
{
	int p = 0;
	for (int y = 0; y < image->getSize().y; y++)
	{
		for (int x = 0; x < image->getSize().x; x++)
		{
			data_a[p++] = image->getPixel(x, y).r;
			data_a[p++] = image->getPixel(x, y).g;
			data_a[p++] = image->getPixel(x, y).b;
			data_a[p++] = image->getPixel(x, y).a;

		}
	}
	if (mem_a != NULL)
	{
		ret = clReleaseMemObject(*mem_a); test(ret, __LINE__);
		delete mem_a;
	}
	mem_a = new cl_mem;

	*mem_a = clCreateImage2D(context, CL_MEM_READ_WRITE, &img_fmt, width, height, 0, 0, &ret); test(ret, __LINE__);
	ret = clEnqueueWriteImage(command_queue, *mem_a, CL_TRUE, origin, region, 4 * width * sizeof(float), 0, data_a, 0, NULL, NULL);


	std::cout << "hejka" << std::endl;
}

void Kernel::updateImageB(sf::Image *image)
{
	int p = 0;
	for (int y = 0; y < image->getSize().y; y++)
	{
		for (int x = 0; x < image->getSize().x; x++)
		{
			data_b[p++] = image->getPixel(x, y).r;
			data_b[p++] = image->getPixel(x, y).g;
			data_b[p++] = image->getPixel(x, y).b;
			data_b[p++] = image->getPixel(x, y).a;

		}
	}
	if (mem_b != NULL)
	{
		ret = clReleaseMemObject(*mem_b); test(ret, __LINE__);
		delete mem_b;
	}
	mem_b = new cl_mem;
	*mem_b = clCreateImage2D(context, CL_MEM_READ_WRITE, &img_fmt, width, height, 0, 0, &ret); test(ret, __LINE__);
	ret = clEnqueueWriteImage(command_queue, *mem_b, CL_TRUE, origin, region, 4 * width * sizeof(float), 0, data_b, 0, NULL, NULL);

}
const sf::Image& Kernel::run(float lambda)
{

	int p = 0;
	for (int i = 0; i < keys_size; i += 4)
	{
		tmp_c[p] = (1.0 - lambda) *keys[i] + lambda *  keys[i + 2];
		tmp_c[p + 1] = (1.0 - lambda) *keys[i + 1] + lambda *  keys[i + 3];

		tmp_p[p] = keys[i] - tmp_c[p];
		tmp_p[p + 1] = keys[i + 1] - tmp_c[p + 1];
		tmp_q[p] = keys[i + 2] - tmp_c[p];
		tmp_q[p + 1] = keys[i + 3] - tmp_c[p+1];

		p += 2;
	}
	// punkt przejœciowy


	clEnqueueWriteBuffer(command_queue, *mem_tmp_p, CL_TRUE, 0,  keys_size / 2 * sizeof(float), tmp_p, 0, NULL, NULL); test(ret, __LINE__);
	clEnqueueWriteBuffer(command_queue, *mem_tmp_q, CL_TRUE, 0,  keys_size / 2 * sizeof(float), tmp_q, 0, NULL, NULL); test(ret, __LINE__);
	clEnqueueWriteBuffer(command_queue, *mem_tmp_c, CL_TRUE, 0,  keys_size / 2 * sizeof(float), tmp_c, 0, NULL, NULL); test(ret, __LINE__);

	




	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)mem_output); test(ret, __LINE__);
	ret = clSetKernelArg(kernel, 1, sizeof(unsigned int), &width); test(ret, __LINE__);
	ret = clSetKernelArg(kernel, 2, sizeof(unsigned int), &height); test(ret, __LINE__);
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)mem_a); test(ret, __LINE__);
	ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)mem_b); test(ret, __LINE__);
	ret = clSetKernelArg(kernel, 5, sizeof(float), &lambda); test(ret, __LINE__);
	ret = clSetKernelArg(kernel, 6, sizeof(int), &nKeys); test(ret, __LINE__);
	ret = clSetKernelArg(kernel, 7, sizeof(cl_mem), (void *)mem_keys); test(ret, __LINE__);
	ret = clSetKernelArg(kernel, 8, sizeof(cl_mem), (void *)mem_tmp_p); test(ret, __LINE__);
	ret = clSetKernelArg(kernel, 9, sizeof(cl_mem), (void *)mem_tmp_q); test(ret, __LINE__);
	ret = clSetKernelArg(kernel, 10, sizeof(cl_mem), (void *)mem_tmp_c); test(ret, __LINE__);
	size_t global_work_size[3] = { width, height, 0 };
	size_t local_work_size[3] = { -1, 1, 0 };

	/* Execute OpenCL kernel */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL); test(ret, __LINE__);


	/* Transfer result from the memory buffer */
	ret = clEnqueueReadImage(command_queue, *mem_output, CL_TRUE, origin, region, 0, 0, data, 0, NULL, NULL); test(ret, __LINE__);

	ret = clFlush(command_queue); test(ret, __LINE__);


	p = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			output.setPixel(x, y, sf::Color((data[p]), (data[1 + p]), (data[2 + p])));
			p += 4;
		}
	}

	return output;
}
Kernel::~Kernel()
{
	ret = clFlush(command_queue); test(ret, __LINE__);
	ret = clFinish(command_queue); test(ret, __LINE__);
	ret = clReleaseKernel(kernel); test(ret, __LINE__);
	ret = clReleaseProgram(program); test(ret, __LINE__);
	ret = clReleaseMemObject(*mem_a); test(ret, __LINE__);
	ret = clReleaseMemObject(*mem_b); test(ret, __LINE__);
	ret = clReleaseMemObject(*mem_keys); test(ret, __LINE__);
	ret = clReleaseMemObject(*mem_tmp_p); test(ret, __LINE__);
	ret = clReleaseMemObject(*mem_tmp_q); test(ret, __LINE__);
	ret = clReleaseMemObject(*mem_tmp_c); test(ret, __LINE__);
	ret = clReleaseMemObject(*mem_output); test(ret, __LINE__);
	ret = clReleaseCommandQueue(command_queue); test(ret, __LINE__);
	ret = clReleaseContext(context); test(ret, __LINE__);
}

void Kernel::test(int r, int l)
{
	if (r != 0)
	{
		std::cout << "PROBLEM: " << r << " @" << l << std::endl;
		int x; std::cin >> x;
	}
}