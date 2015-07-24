#pragma once

#include <algorithm>
#include <vector>

// Little cover of Vector class
class KeysGroup
{
	std::vector< std::pair<std::pair<double, double>, std::pair<double, double> >  > data; // List of elements
	static unsigned int totalSize;// total number of active keys
public:
	
	unsigned int getSize(); 
	static unsigned int getTotalSize();
	void add(double x1, double y1, double x2, double y2);
	std::pair<std::pair<double, double>, std::pair<double, double> >& get(int i);
	void drop();

	KeysGroup();
	~KeysGroup();
};

