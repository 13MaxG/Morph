#pragma once
#include <SFML/Graphics.hpp>
#include "Image.h"
#include "KeysGroup.h"
#include <algorithm>
#include <iostream>


enum Tool{ LINE, CIRCLE };

class Editor
{
	Image *img; // current image
	Tool tool; // current tool. TODO

	sf::Texture texture; // sfml drawing stuff
	sf::Sprite sprite; // sfml drawing stuff

	
	bool ready; // if editor is DISPLAYED
	bool active; // if line is CREATING

	
	std::pair<std::pair<double, double>, std::pair<double, double> > data; // data for only tool -  line. repsesent begin and end of line. in 

public:

	void input(sf::Event *event, sf::RenderWindow *window);
	void display(sf::RenderWindow *window);

	void setTool(Tool tool); // change tool. TODO

	bool isReady(); //if editor is DISPLAYED
	void clean(); // clean after displaying


	std::pair<std::pair<double, double>, std::pair<double, double> > read();// get data. TODO: diffrent with tools


	Editor(Image *img);
	~Editor();
};

