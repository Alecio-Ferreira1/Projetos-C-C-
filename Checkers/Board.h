#pragma once
#include <SFML/graphics.hpp>
#include <map>
#include <utility>
#include <vector>
#include "Piece.h"

struct BoardText {
	sf::Text rows[8];
	sf::Text columns[8];
};

struct Mark {
	sf::CircleShape shape;
	std::pair<int, int> pos;
};

class Board {
private:
	sf::RectangleShape* squares[8][8];
	float squareSize = 80.f;
	BoardText text;
	sf::Font textFont;
	std::vector<Mark> marks;
public:
	Board();
	~Board();
	void draw(sf::RenderWindow*& window);
	void resize(sf::Vector2u windowSize);
	float getSquareSize();
	sf::RectangleShape& getSquare(int i, int j);
	void select(Piece*& piece, std::map<std::pair<int, int>, int>& piecesMAP, bool forceDontShowMarks);
	std::vector<Mark> getMarks();
};