#include "Engine.h"


Engine::Engine(std::string configPath, bool editor)
{
	this->configPath = configPath;
	this->editor = editor;
	this->mode = DEFAULT;
	this->tool = LINE;

	std::fstream file;
	file.open(configPath, std::ios_base::in);

	std::string buffer, tmp;

	getline(file, buffer);
	outputName = buffer.substr(0, buffer.find("#"));
	std::cout << "outputName: " << outputName << std::endl;

	getline(file, buffer);
	workingDirecory = buffer.substr(0, buffer.find("#"));
	std::cout << "workingDirecory: " << workingDirecory << std::endl;

	getline(file, buffer);
	avconvPath = buffer.substr(0, buffer.find("#"));
	std::cout << "avconvPath: " << avconvPath << std::endl;

	getline(file, buffer);
	buffer = buffer.substr(0, buffer.find("#"));
	width = atoi(buffer.c_str());
	std::cout << "width: " << width << std::endl;

	getline(file, buffer);
	buffer = buffer.substr(0, buffer.find("#"));
	height = atoi(buffer.c_str());
	std::cout << "height: " << height << std::endl;

	getline(file, buffer);
	std::string pathImg1 = buffer.substr(0, buffer.find("#"));
	std::cout << "pathImg1: " << pathImg1 << std::endl;

	getline(file, buffer);
	std::string pathImg2 = buffer.substr(0, buffer.find("#"));
	std::cout << "pathImg2: " << pathImg2 << std::endl;

	getline(file, buffer);
	buffer = buffer.substr(0, buffer.find("#"));
	tmp = buffer.substr(0, buffer.find(" "));
	double posx1 = atof(tmp.c_str());
	tmp = buffer.substr(buffer.find(" "), buffer.size() - buffer.find(" ") + 1);
	double posy1 = atof(tmp.c_str());
	std::cout << "transition1: " << posx1 << " " << posy1 << std::endl;

	getline(file, buffer);
	buffer = buffer.substr(0, buffer.find("#"));
	tmp = buffer.substr(0, buffer.find(" "));
	double posx2 = atof(tmp.c_str());
	tmp = buffer.substr(buffer.find(" "), buffer.size() - buffer.find(" ") + 1);
	double posy2 = atof(tmp.c_str());
	std::cout << "transition2: " << posx2 << " " << posy2 << std::endl;

	getline(file, buffer);
	buffer = buffer.substr(0, buffer.find("#"));
	tmp = buffer.substr(0, buffer.find(" "));
	double scalex1 = atof(tmp.c_str());
	tmp = buffer.substr(buffer.find(" "), buffer.size() - buffer.find(" ") + 1);
	double scaley1 = atof(tmp.c_str());
	std::cout << "scale1: " << scalex1 << " " << scaley1 << std::endl;

	getline(file, buffer);
	buffer = buffer.substr(0, buffer.find("#"));
	tmp = buffer.substr(0, buffer.find(" "));
	double scalex2 = atof(tmp.c_str());
	tmp = buffer.substr(buffer.find(" "), buffer.size() - buffer.find(" ") + 1);
	double scaley2 = atof(tmp.c_str());
	std::cout << "scale12 " << scalex2 << " " << scaley2 << std::endl;


	getline(file, buffer);
	buffer = buffer.substr(0, buffer.find("#"));
	double rot1 = atof(buffer.c_str());
	std::cout << "rot1: " << rot1 << std::endl;


	getline(file, buffer);
	buffer = buffer.substr(0, buffer.find("#"));
	double rot2 = atof(buffer.c_str());
	std::cout << "rot2: " << rot2 << std::endl;

	getline(file, buffer);
	buffer = buffer.substr(0, buffer.find("#"));
	duration = atof(buffer.c_str());
	std::cout << "duration: " << duration << std::endl;

	getline(file, buffer);
	buffer = buffer.substr(0, buffer.find("#"));
	fps = atof(buffer.c_str());
	std::cout << "fps: " << fps << std::endl;

	img1 = Image(width, height, pathImg1, std::make_pair(scalex1, scaley1), std::make_pair(posx1, posy1));
	img2 = Image(width, height, pathImg2, std::make_pair(scalex1, scaley1), std::make_pair(posx1, posy1));

	getline(file, buffer);
	buffer = buffer.substr(0, buffer.find("#"));
	int numberOfGroups = atoi(buffer.c_str());
	std::cout << "numberOfGroups: " << numberOfGroups << std::endl;

	for (int i = 0; i < numberOfGroups; i++)
	{
		getline(file, buffer);
		buffer = buffer.substr(0, buffer.find("#"));
		int size = atoi(buffer.c_str());
		std::cout << "| size: " << size << std::endl;
		groups.push_back(KeysGroup()); 
		nKeys += size;
		for (int j = 0; j < size; j++)
		{
			
			getline(file, buffer);
			buffer = buffer.substr(0, buffer.find("#"));
			size_t Y = 0;
			size_t X = buffer.find(" ");
			tmp = buffer.substr(Y, X);
			double a = atof(tmp.c_str());
			Y = X + 1;
			X = buffer.find(" ", Y);
			tmp = buffer.substr(Y, X);
			double b = atof(tmp.c_str());
			Y = X + 1;
			X = buffer.find(" ", Y);
			tmp = buffer.substr(Y, X);
			double c = atof(tmp.c_str());
			Y = X + 1;
			X = buffer.find(" ", Y);
			tmp = buffer.substr(Y, X);
			double d = atof(tmp.c_str());

			groups.back().add(a, b, c, d);
			std::cout << "|	|adding " << a << " " << b<< " "<<c<<" "<<d << std::endl;
		}
	}
	file.close();

	///
	//saveConfig();
	///

	window = new sf::RenderWindow(sf::VideoMode(width, height), "Morph");
	kernel = new Kernel(width, height, &img1.get(), &img2.get(), &groups, nKeys);
	editor1 = new Editor(&img1);
	editor2 = new Editor(&img2);

	lambda = 0;
	fullscreen = false;
}

void Engine::saveConfig()
{

	std::fstream file;
	file.open(configPath, std::ios_base::out);

	file << outputName << "#output namefile" << std::endl;
	file << workingDirecory << "#working directory" << std::endl;
	file << avconvPath << "#path to avconv" << std::endl;
	file << width << "#output width" << std::endl;
	file << height << "#output height" << std::endl;
	file << img1.getPath() << "#path to img1" << std::endl;
	file << img2.getPath() << "#path to img1" << std::endl;
	file << img1.position.first << " " << img1.position.second << "#position of img1" << std::endl;
	file << img2.position.first << " " << img2.position.second << "#position of img2" << std::endl;
	file << img1.scale.first << " " << img1.scale.second << "#scale of img1" << std::endl;
	file << img2.scale.first << " " << img2.scale.second << "#scale of img2" << std::endl;
	file << img1.angle << "#rotation of img1" << std::endl;
	file << img2.angle << "#rotation of img2" << std::endl;
	file << duration << "#duration of animation" << std::endl;
	file << fps << "#frames per second" << std::endl;
	file << groups.size() << "#number of groups" << std::endl;

	for (unsigned int i = 0; i < groups.size(); i++)
	{
		file << groups[i].getSize() << "#size of "<<i+1<<". group" << std::endl;
		for (unsigned int j = 0; j < groups[i].getSize(); j++)
		{
			file<< groups[i].get(j).first.first<< " "
				<< groups[i].get(j).first.second << " "
				<< groups[i].get(j).second.first << " "
				<< groups[i].get(j).second.second << "#keypoint " << j+1<<". in "<<i+1<<". group" << std::endl;
		}
	}



	file.close();
	
}


Engine::~Engine()
{
	delete editor1;
	delete editor2;
	delete window;
}

void  Engine::run()
{
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();

			input(&event);
		}

		calc();

		window->clear(sf::Color::Black);

		display();

		window->display();
	}
}

void  Engine::display()
{
	if (mode == DEFAULT)
	{
		sf::Texture texture;
		texture.loadFromImage(kernel->run(lambda));

		sf::Sprite sprite;
		sprite.setTexture(texture);
		sprite.setPosition(0, 0);

		window->draw(sprite);

	}

	if (mode == EDITOR_1)
	{
		editor1->display(window);

		for (int i = 0; i < groups.size(); i++)
		{
			for (int j = 0; j < groups[i].getSize(); j++)
			{
				

				sf::CircleShape shape(2);
				shape.setOrigin(2, 2);
				shape.setFillColor(sf::Color(100, 250, 50));
				shape.setPosition(groups[i].get(j).first.first * window->getSize().x, groups[i].get(j).first.second* window->getSize().y);
				window->draw(shape);
			}
		}

	}

	if (mode == EDITOR_2)
	{
		editor2->display(window);

		for (int i = 0; i < groups.size(); i++)
		{
			for (int j = 0; j < groups[i].getSize(); j++)
			{
				

				sf::CircleShape shape(2);
				shape.setOrigin(2, 2);
				shape.setFillColor(sf::Color(100, 250, 50));
				shape.setPosition(groups[i].get(j).second.first * window->getSize().x, groups[i].get(j).second.second* window->getSize().y);
				window->draw(shape);
			}
		}
	}
}

void Engine::calc()
{
	if (editor1->isReady() && editor2->isReady())
	{
		createGroup(editor1->read(), editor2->read());
		editor1->clean();
		editor2->clean();
	}

	if (mode == DEFAULT)
	{
		lambda = (double)sf::Mouse::getPosition(*window).x / (double)window->getSize().x;
		if (lambda < 0) lambda = 0;
		if (lambda > 1) lambda = 1;
	}
}



void  Engine::input(sf::Event *event)
{
	bool a = sf::Keyboard::isKeyPressed(sf::Keyboard::A); bool b = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	if (a && !b) setMode(EDITOR_1); else
		if (!a && b) setMode(EDITOR_2); else
			setMode(DEFAULT);

	bool n1 = false; bool n2 = false;
	if (event->type == sf::Event::KeyReleased)
	{
		if (event->key.code == sf::Keyboard::Num1) n1 = true;
		if (event->key.code == sf::Keyboard::Num2) n2 = true;

		if (event->key.code == sf::Keyboard::Z) Undo();
		if (event->key.code == sf::Keyboard::Y) Redo();
		if (event->key.code == sf::Keyboard::Space) saveConfig();

		if (event->key.code == sf::Keyboard::Return) renderToFile();

		if (event->key.code == sf::Keyboard::P)  fullOrWindowScreen();
	}

	
	if (a && !b) setMode(EDITOR_1); else
		if (!a && b) setMode(EDITOR_2); else
			setMode(DEFAULT);

	if (n1 && !n2) setTool(LINE); else
		if (!n1 && n2) setTool(CIRCLE);


	if (mode == EDITOR_1)
		editor1->input(event, window);

	if (mode == EDITOR_2)
		editor2->input(event, window);
}
void Engine::fullOrWindowScreen()
{
	

	if (fullscreen)
	{
		delete window;
		window = new sf::RenderWindow(sf::VideoMode(width, height), "Morph");
	}
	else
	{
		delete window;
		window = new sf::RenderWindow(sf::VideoMode(width, height), "Morph", sf::Style::Fullscreen);
	}

	fullscreen = !fullscreen;
}

void Engine::setMode(Mode mode)
{
	this->mode = mode;
}
void Engine::setTool(Tool tool)
{
	this->tool = tool;
	editor1->setTool(tool);
	editor2->setTool(tool);

	if (tool == LINE) std::cout << "set line" << std::endl;
	if (tool == CIRCLE) std::cout << "set circle" << std::endl;

}


void Engine::createGroup(std::pair<std::pair<double, double>, std::pair<double, double> > a, std::pair<std::pair<double, double>, std::pair<double, double> > b)
{
	groups.push_back(KeysGroup()); 

	
	double len = std::sqrt(
			std::max(
			(a.first.first*window->getSize().x - a.second.first*window->getSize().x) * (a.first.first*window->getSize().x - a.second.first*window->getSize().x) + (a.first.second *window->getSize().y - a.second.second*window->getSize().y) * (a.first.second*window->getSize().y - a.second.second*window->getSize().y),
			(b.first.first*window->getSize().x - b.second.first*window->getSize().x) * (b.first.first*window->getSize().x - b.second.first*window->getSize().x) + (b.first.second*window->getSize().y - b.second.second*window->getSize().y) * (b.first.second*window->getSize().y - b.second.second*window->getSize().y))
		);
	int n = ceil(len   / 10.0);
	std::cout << len << std::endl;
	nKeys += n + 1;
	for (int i = 0; i <= n; i++)
	{
		
		double percent = (double)i / (double)n;


		groups.back().add(a.first.first*(1 - percent) + percent * a.second.first, 
			a.first.second*(1 - percent) + percent * a.second.second, 
			b.first.first*(1 - percent) + percent * b.second.first, 
			b.first.second*(1 - percent) + percent * b.second.second);


	}
	Cleardo();
	kernel->updateKeys(&groups, nKeys);

}

void Engine::Undo()
{
	if (groups.size()  > 0)
	{
		hiddenGroups.push_back(groups.back());
		nKeys -= groups.back().getSize();
		groups.pop_back();
	}
}

void Engine::Redo()
{
	if (hiddenGroups.size()  > 0)
	{
		groups.push_back(hiddenGroups.back());
		nKeys += hiddenGroups.back().getSize();
		hiddenGroups.pop_back();
	}
}
void Engine::Cleardo()
{
	hiddenGroups.erase(hiddenGroups.begin(), hiddenGroups.end());
}


void Engine::renderToFile()
{

	int frames = duration * fps;

	sf::Clock mainc;
	for (int i = 1; i <= frames; i++)
	{
		double render_time = 0;
		double draw_time = 0;
		double write_time = 0;
		
		std::string file = workingDirecory + "/" + outputName + to_str(i, 8) + ".png";

		sf::Clock clock;
		clock.restart();
		double p = (double)i / (double)frames;
		sf::Image image = kernel->run(p);
		render_time = clock.getElapsedTime().asSeconds();

		clock.restart();

	
		sf::Texture texture;
		texture.loadFromImage(image);

		sf::Sprite sprite;
		sprite.setTexture(texture);
		sprite.setPosition(0, 0);

		window->clear();
		window->draw(sprite);
		window->display();

		render_time = clock.getElapsedTime().asSeconds();
		clock.restart();

		image.saveToFile(file);

		write_time = clock.getElapsedTime().asSeconds();
		clock.restart();
		std::cout << "frame: " << i << "/" << frames << ". render: "<<render_time<<". draw: "<<draw_time<<". write: "<<write_time<<".  remaining: "<<
			(mainc.getElapsedTime().asSeconds() * (1.0 - p) / p )
			<<std::endl;

		
 	}


	std::cout << "creating video from images" << std::endl;
	std::string command = " \"\"" + avconvPath + "\" " + " -y -i \"" + workingDirecory + "/" + outputName + "%08d.png\" -r " + std::to_string((int)fps) + " -q 1 -c:v libx264 \"" + workingDirecory + "/" + outputName + ".mp4\" \"";;
	std::cout <<"command: "<< command.c_str() << std::endl;
	
	sf::Clock clock;
	clock.restart();
//	WinExec(command.c_str(), SW_SHOW);
	system(command.c_str());
	std::cout << "image merging: " << clock.getElapsedTime().asSeconds() << std::endl;

	std::cout << "total time: " << mainc.getElapsedTime().asSeconds() << std::endl;
	std::cout << "outut file: " << workingDirecory + "/" + outputName + ".png" << std::endl;

}

std::string Engine::to_str(int number, int k)
{
	std::string x = std::to_string(number);
	while (x.length() < k)
		x = "0" + x;
	return x;
}