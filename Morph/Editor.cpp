#include "Editor.h"


Editor::Editor(Image *img)
{
	this->img = img;

	texture.loadFromImage((*img).getOriginalImage());
	sprite.setTexture(texture);

	active = false;
}


void Editor::input(sf::Event *event, sf::RenderWindow *window)
{
	
	bool newActive = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	if (active == false && newActive == true)
	{
		data.first = std::make_pair((double)sf::Mouse::getPosition(*window).x / (double)window->getSize().x, (double)sf::Mouse::getPosition(*window).y / (double)window->getSize().y);
		ready = false;
	}

	active = newActive;
	

	if ( event->type == sf::Event::MouseButtonReleased)
	{
		if (event->mouseButton.button == sf::Mouse::Left)
		{
			data.second = std::make_pair((double)sf::Mouse::getPosition(*window).x / (double)window->getSize().x, (double)sf::Mouse::getPosition(*window).y / (double)window->getSize().y);
			active = false;
			ready = true;
		}
		
	} 

	
}
void Editor::display(sf::RenderWindow *window)
{
	window->draw(sprite);






	if (active)
	{
		sf::Vertex line[] =
		{
			sf::Vertex( sf::Vector2f( data.first.first * window->getSize().x, data.first.second * window->getSize().y )),
			sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
		};

		window->draw(line, 2, sf::Lines);
	}

	if (ready)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(data.first.first * window->getSize().x, data.first.second * window->getSize().y), sf::Color::Red),
			sf::Vertex(sf::Vector2f(data.second.first * window->getSize().x, data.second.second * window->getSize().y), sf::Color::Red)
		};

		window->draw(line, 2, sf::Lines);
	}
}



void Editor::setTool(Tool tool)
{
	this->tool = tool;
}

void Editor::clean()
{
	ready = false;
	active = false;
}
bool  Editor::isReady()
{
	return ready;
}
std::pair<std::pair<double, double>, std::pair<double, double> >  Editor::read()
{
	return data;
}

Editor::~Editor()
{
}
