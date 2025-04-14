#include <SFML/Graphics.hpp>
#include "Game.h"

#include <iostream>

namespace Checkers {
	Game* game = nullptr;
	void start();
	void loop();
	void end();
};

void Checkers::start() {
	game = new Game();
	game->init();
}

void Checkers::loop() {
	sf::RenderWindow* mainWindow = game->getMainWindow();
	mainWindow->setFramerateLimit(60);

	while (mainWindow->isOpen()) {
		sf::Event evt;

		while (mainWindow->pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				mainWindow->close();
			}

			if (evt.type == sf::Event::Resized) {
				game->resize();
				mainWindow->setView(sf::View(sf::FloatRect(0.f, 0.f, (float) mainWindow->getSize().x, (float) mainWindow->getSize().y)));
			}

			if (evt.type == sf::Event::MouseButtonPressed) {
				if (evt.mouseButton.button == sf::Mouse::Left) {
					game->updateBoard();
				}
			}
		}

		mainWindow->clear(sf::Color(57, 56, 52));
		game->update();
		mainWindow->display();
	}
}

void Checkers::end() {
	delete game;
}