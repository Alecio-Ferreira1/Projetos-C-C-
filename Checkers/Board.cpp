#include "Board.h"

#define LIGHT_COLOR sf::Color(237, 214, 176)
#define DARK_COLOR sf::Color(184, 135, 98)

Board::Board() {
	if (textFont.loadFromFile("assets\\fonts\\arial.ttf") == false) {
		exit(-1);
	}

	char letter = ' ';

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			squares[i][j] = new sf::RectangleShape(sf::Vector2f(squareSize, squareSize));
			squares[i][j]->setPosition(sf::Vector2f(60 + (j * squareSize), 60 + (i * squareSize)));

			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0)) {
				squares[i][j]->setFillColor(LIGHT_COLOR);
			}

			else {
				squares[i][j]->setFillColor(DARK_COLOR);
			}
		}

		text.rows[i].setString(std::to_string(8 - i));
		text.rows[i].setFont(textFont);
		text.rows[i].setCharacterSize(0.25 * squareSize);
		text.rows[i].setFillColor(sf::Color::Red);
		text.rows[i].setPosition(sf::Vector2f(63, 63 + (i * squareSize)));

		letter = 97 + i;
		text.columns[i].setString(letter);
		text.columns[i].setFont(textFont);
		text.columns[i].setCharacterSize(0.25 * squareSize);
		text.columns[i].setFillColor(sf::Color::Red);
		text.columns[i].setPosition(sf::Vector2f(60 + (i * squareSize) + (0.7 * squareSize), 60 + (7 * squareSize) + (0.7 * squareSize)));
	}
}

Board::~Board() {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			delete squares[i][j];
		}
	}
}

void Board::draw(sf::RenderWindow*& window) {  //COLOCAR EM GAME (PENSAR SOBRE ISSO)
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			window->draw(*squares[i][j]);
			window->draw(text.columns[j]);
		}

		window->draw(text.rows[i]);
	}

	for (const auto mark : marks) {
		window->draw(mark.shape); 
	}
}

void Board::resize(sf::Vector2u windowSize) {  //COLOCAR EM GAME (PENSAR SOBRE ISSO)
	squareSize = (std::min(windowSize.x, windowSize.y)) * 0.1;

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			squares[i][j]->setSize(sf::Vector2f(squareSize, squareSize));
			squares[i][j]->setPosition(60 + (j * squareSize), 60 + (i * squareSize));
		}

		text.rows[i].setPosition(sf::Vector2f(63, 63 + (i * squareSize)));
		text.rows[i].setCharacterSize(0.25 * squareSize);

		text.columns[i].setPosition(sf::Vector2f(60 + (i * squareSize) + (0.7 * squareSize), 60 + (7 * squareSize) + (0.7 * squareSize)));
		text.columns[i].setCharacterSize(0.25 * squareSize);
	}

	for (auto& mark : marks) {
		mark.shape.setRadius(0.15 * squareSize);
		mark.shape.setPosition(60 + (squareSize / 2) + (mark.pos.second * squareSize) - mark.shape.getRadius(), 
		60 + (squareSize / 2) + (mark.pos.first * squareSize) - mark.shape.getRadius());
	}
}

float Board::getSquareSize() {
	return squareSize;
}

sf::RectangleShape& Board::getSquare(int i, int j) {
	return *squares[i][j];
}

void Board::select(Piece*& piece, std::map<std::pair<int, int>, int>& piecesMAP, bool forceDontShowMarks) {
	std::pair<int, int> coords = piece->getPos();
	const sf::Color lightYellow = sf::Color(246.f, 235.f, 114.f);
	const sf::Color darkYellow = sf::Color(220.f, 195.f, 75.f);
	bool showMarks = false;

	if (squares[coords.first][coords.second]->getFillColor() == LIGHT_COLOR) {
		squares[coords.first][coords.second]->setFillColor(lightYellow);
		showMarks = true;
	}

	else if (squares[coords.first][coords.second]->getFillColor() == DARK_COLOR) {
		squares[coords.first][coords.second]->setFillColor(darkYellow);
		showMarks = true;
	}

	else if (squares[coords.first][coords.second]->getFillColor() == lightYellow) {
		squares[coords.first][coords.second]->setFillColor(LIGHT_COLOR);
	}

	else {
		squares[coords.first][coords.second]->setFillColor(DARK_COLOR);
	}

	if (forceDontShowMarks) {
		return;
	}

	if (showMarks) {
		std::vector<std::pair<int, int>> moves = piece->getMoves(piecesMAP);
		sf::CircleShape circle;
		Mark mark;

		for (const auto move : moves) {
			circle = sf::CircleShape(0.15 * squareSize);
			circle.setFillColor(sf::Color(158.f, 116.f, 84.f));
			circle.setPosition(60 + (squareSize / 2) + (move.second * squareSize) - circle.getRadius(), 
			60 + (squareSize / 2) + (move.first * squareSize) - circle.getRadius());
			
			mark.shape = circle;
			mark.pos = move;
			piecesMAP[move] = -2;

			marks.push_back(mark);
		}
	}

	else {
		for (Mark& mark : marks) {
			piecesMAP[mark.pos] = -1;
		}

		marks.clear();
	}
}

std::vector<Mark> Board::getMarks() {
	return marks;
}