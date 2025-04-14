#pragma once;
#include <SFML/Graphics.hpp>
#include <utility>
#include <map>
#include <vector>

#define WHITE sf::Color(255, 255, 255)
#define BLACK sf::Color(0, 0, 0)

class Piece {
private:
	std::pair<int, int> pos = {0 , 0};
	bool queenMovesPossible = false;
	sf::CircleShape shape;
	sf::Color color;
	sf::Sprite crownSpr;
	sf::Texture crownTexture;
public:
	Piece(std::pair<int, int> pos, sf::Color color);
	void setCircleShape(sf::CircleShape shape);
	sf::CircleShape& getCircleShape();
	sf::Color getColor();
	void setColor(sf::Color color);
	std::pair<int, int> getPos();
	void setPos(std::pair<int, int> pos);
	std::vector<std::pair<int, int>> getMoves(std::map<std::pair<int, int>, int> piecesMap);
	bool isQueen();
	void setQueen();
	sf::Sprite& getCrown();
	void loadCrownTexture();
	bool mustCapture(std::map<std::pair<int, int>, int> piecesMap);
	std::vector<std::pair<int, int>> getCaptures(std::map<std::pair<int, int>, int> piecesMap);
	bool sameColor(sf::Color pieceColor);
};

template <typename T>
bool isPairInVector(std::vector<T> Vector, T Item);