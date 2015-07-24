#include "Image.h"

Image::Image()
{
}

Image::Image(unsigned int width, unsigned int height, std::string path, std::pair<double, double> scale, std::pair<double, double> position, double angle)
{
	this->path = path;

	originalImage.loadFromFile(path);

	this->width = width;
	this->height = height;

	
	this->scale = scale;
	this->position = position;
	this->angle = angle;
	update();

}

void Image::update()
{
	image.create(width, height, sf::Color::Black);
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			if (x < originalImage.getSize().x && y < originalImage.getSize().y)
			image.setPixel(x, y, originalImage.getPixel(x, y));
		}
	}
	
}

sf::Image& Image::get()
{
	return image;
}
sf::Image& Image::getOriginalImage()
{
	return originalImage;
}

std::string Image::getPath()
{
	return path;
}
Image::~Image()
{
}
