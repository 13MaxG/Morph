#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#include <Windows.h>//tylko dla winexec

#include "Image.h"
#include "KeysGroup.h"
#include "Editor.h"
#include "Kernel.h"

class Engine
{
// config
	std::string configPath;
	bool editor;

	std::string outputName;
	std::string workingDirecory;
	
	std::string avconvPath;
	unsigned int width;
	unsigned int height;
	double fps;
	double duration; 

	Image img1;
	Image img2;

	int nKeys;
	std::vector<KeysGroup> groups;
	std::vector<KeysGroup> hiddenGroups;
	void saveConfig();
// ---


	sf::RenderWindow *window;
	bool fullscreen;
	void fullOrWindowScreen();// toggle fullscreen. 

	void renderToFile(); // render wideo

	void display(); // VIEW
	void input(sf::Event *event); // CONTROLLER
	void calc(); // MODEL


	void createGroup(std::pair<std::pair<double, double>, std::pair<double, double> > a, std::pair<std::pair<double, double>, std::pair<double, double> > b); // add new group. TODO: should be changed with tools

	void Undo(); // undo
	void Redo(); // redo
	void Cleardo(); // reset stack of undo/redo

	enum Mode{ DEFAULT, EDITOR_1, EDITOR_2 };
	Mode mode; // DEFAULT - view mode. Editor_1 - edit on initial image, EDITOR_2 edit on final image
	void setMode(Mode mode); // change mode


	Editor* editor1; // editor of initial image
	Editor* editor2; // editor of final image

	Kernel* kernel; // Kerel which generaes images

	double lambda; // current percent of animation. for draw current frame. based on mouse position.

	Tool tool; // Current tool. has to be here to synchronize information bewtween Editor1 and Editor2
	void setTool(Tool tool);

	std::string to_str(int number, int k); // temporaty function, becouse avconv is a bit supid. to_str(1337, 8) == "00001337";
public:

	Engine(std::string configPath, bool editor = true); // todo: render instanly

	void run();
	~Engine();
};

