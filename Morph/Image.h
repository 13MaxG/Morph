#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>


// Image is class which containts original image and image after translations
class Image
{
	std::string path; // path to image
	sf::Image originalImage; // orginal, non edited image
	sf::Image image; // image after transormations

	

	unsigned int width; // width of output image
	unsigned int height; // height of output image
public:
	std::pair<double, double> scale;  // todo
	std::pair<double, double> position; // todo
	double angle; // todo

	Image();
	Image(unsigned int width, unsigned int height, std::string path, std::pair<double, double> scale = std::make_pair(1.0, 1.0), std::pair<double, double> position = std::make_pair(0.0, 0.0), double angle = 0);

	void update();
	sf::Image& get();
	sf::Image& getOriginalImage();

	std::string getPath();

	~Image();
};

