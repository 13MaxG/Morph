#include "KeysGroup.h"

unsigned int KeysGroup::totalSize = 0;

KeysGroup::KeysGroup()
{
}


KeysGroup::~KeysGroup()
{
	drop();
}

unsigned int KeysGroup::getSize()
{
	return data.size();
}
unsigned int KeysGroup::getTotalSize()
{
	return totalSize;
}
void KeysGroup::add(double x1, double y1, double x2, double y2)
{
	totalSize++;
	data.push_back(std::make_pair(std::make_pair(x1, y1), std::make_pair(x2, y2)));
}
void KeysGroup::drop()
{
	totalSize -= data.size();

	data.erase(data.begin(), data.end());
}

std::pair<std::pair<double, double>, std::pair<double, double> >& KeysGroup::get(int i)
{
	return data[i];
}