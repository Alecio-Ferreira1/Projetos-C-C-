#include "Piece.h"
#include <queue>
#include <array>

Piece::Piece(std::pair<int, int> pos, sf::Color color) {
	this->pos = pos;
	this->color = color;
	loadCrownTexture();
}

void Piece::loadCrownTexture() {
	std::string path;

	if (color == WHITE) {
		path = "assets\\imgs\\crown1.png";
	}

	else {
		path = "assets\\imgs\\crown2.png";
	}

	if (crownTexture.loadFromFile(path) == false) {
		exit(-1);
	}
}

void Piece::setCircleShape(sf::CircleShape shape) {
	this->shape = shape;
}

sf::CircleShape& Piece::getCircleShape() {
	return shape;
}

sf::Color Piece::getColor() {
	return color;
}

void Piece::setColor(sf::Color color) {
	this->color = color;
	shape.setFillColor(color);
}

std::pair<int, int> Piece::getPos() {
	return pos;
}

void Piece::setPos(std::pair<int, int> pos) {
	this->pos = pos;
}

std::vector<std::pair<int, int>> Piece::getMoves(std::map<std::pair<int, int>, int> piecesMap) {
	int colorDir = 1;
	std::vector<std::pair<int, int>> moves;

	if (color == WHITE) {
		colorDir = -1;
	}

	if (mustCapture(piecesMap)) {
		return getCaptures(piecesMap);
	}

	const std::pair<int, int> directions[4] = { {1, -1}, {1, 1}, {-1, -1}, {-1, 1} };
	bool validDirs[4] = { true, true, true, true };
	bool validCoord = true;

	for (int count = 1; (validDirs[0] || validDirs[1] || validDirs[2] || validDirs[3]); ++count) { 
		for (int i = 0; i < 4; ++i) {
			if (!isQueen() && (count > 1 || i > 1)) {
				return moves;
			}

			validCoord = piecesMap.find(std::make_pair(pos.first + (count * colorDir * directions[i].first),
				pos.second + (count * colorDir * directions[i].second))) != piecesMap.end();

			if (validCoord && piecesMap[std::make_pair(pos.first + (count * colorDir * directions[i].first),
				pos.second + (count * colorDir * directions[i].second))] == -1 && validDirs[i]) {
				moves.push_back(std::make_pair(pos.first + (count * colorDir * directions[i].first), 
					pos.second + (count * colorDir * directions[i].second)));
			}

			else {
				validDirs[i] = false;
			}
		}
	}

	return moves;
}

bool Piece::isQueen() {
	return queenMovesPossible;
}

void Piece::setQueen() {
	queenMovesPossible = true;
	crownSpr.setTexture(crownTexture);
}

sf::Sprite& Piece::getCrown() {
	return crownSpr;
}


bool Piece::mustCapture(std::map<std::pair<int, int>, int> piecesMap) {
	int colorDir = 1;
	
	if (color == WHITE) {
		colorDir = -1;
	}

	bool validCoord;
	const int pieceID = piecesMap[pos];
	int adjacentPieceID = -1;
	bool sameColor = false, validDirs[4] = { true, true, true, true };
	const std::pair<int, int> directions[4] = { {1, -1}, {1, 1}, {-1, -1}, {-1, 1} };

	for (int count = 1; validDirs[0] || validDirs[1] || validDirs[2] || validDirs[3]; ++count) {
		if (!isQueen() && count > 1) {
			return false;
		}

		for (int j = 0; j < 4; ++j) {
			std::pair<int, int> myPos = std::make_pair(pos.first + (count * colorDir * directions[j].first) - (colorDir * directions[j].first),
			pos.second + (count * colorDir * directions[j].second) - (colorDir * directions[j].second));
			validCoord = piecesMap.find(myPos) != piecesMap.end();
			bool currentPosIsOk = (validCoord && piecesMap[myPos] == -1) || (myPos == this->pos);

			if (!currentPosIsOk) {
				validDirs[j] = false;
				continue;
			}

			validCoord = piecesMap.find(std::make_pair(pos.first + (count * colorDir * directions[j].first),
			pos.second + (count * colorDir * directions[j].second))) != piecesMap.end();

			if (validCoord) {
				adjacentPieceID = piecesMap[std::make_pair(pos.first + (count * colorDir * directions[j].first),
					pos.second + (count * colorDir * directions[j].second))];

				if ((pieceID < 12 && adjacentPieceID < 12) || (pieceID >= 12 && adjacentPieceID >= 12)) {
					sameColor = true;
				}

				else {
					sameColor = false;
				}

				if (adjacentPieceID != -1 && adjacentPieceID != -2 && !sameColor && validDirs[j]) {
					validCoord = piecesMap.find(std::make_pair(pos.first + ((count + 1) * colorDir * directions[j].first),
					pos.second + ((count + 1) * colorDir * directions[j].second))) != piecesMap.end();

					if (validCoord && piecesMap[std::make_pair(pos.first + ((count + 1) * colorDir * directions[j].first),
						pos.second + ((count + 1) * colorDir * directions[j].second))] == -1 ||
						piecesMap[std::make_pair(pos.first + ((count + 1) * colorDir * directions[j].first),
						pos.second + ((count + 1) * colorDir * directions[j].second))] == -2) {

						return true;
					}

					validDirs[j] = false;
				}
			}

			else {
				validDirs[j] = false;
			}
		}
	}

	return false;
}

template <typename T>
bool isPairInVector(std::vector<T> Vector, T Item) {
	for (const auto& item : Vector) {
		if (Item == item) {
			return true;
		}
	}

	return false;
}

std::vector<std::pair<int, int>> Piece::getCaptures(std::map<std::pair<int, int>, int> piecesMap) {
	std::vector<std::pair<int, int>> moves, piecesCaptured;
	std::queue<std::pair<int, int>> captures;
	const std::pair<int, int> directions[4] = { {1, -1}, {1, 1}, {-1, -1}, {-1, 1} };
	bool validDirs[4] = { true, true, true, true };
	std::pair<int, int> current = pos;
	const int pieceID = piecesMap[pos];
	int adjacentPieceID = -1, colorDir = 1;
	bool sameColor = false, validCoord;

	if (color == WHITE) {
		colorDir = -1;
	}

	captures.push(current);

	while (!captures.empty()) {
		current = captures.front();
		captures.pop();

		for (int i = 0; i < 4; ++i) {
			validDirs[i] = true;
		}

		for (int count = 1; validDirs[0] || validDirs[1] || validDirs[2] || validDirs[3]; ++count) {
			if (!isQueen() && count > 1) {
				break;
			}

			for (int j = 0; j < 4; ++j) {
				validCoord = piecesMap.find(std::make_pair(current.first + (count * colorDir * directions[j].first),
					current.second + (count * colorDir * directions[j].second))) != piecesMap.end();

				if (validCoord) {
					adjacentPieceID = piecesMap[std::make_pair(current.first + (count * colorDir * directions[j].first),
						current.second + (count * colorDir * directions[j].second))];

					if ((pieceID < 12 && adjacentPieceID < 12) || (pieceID >= 12 && adjacentPieceID >= 12)) {
						sameColor = true;
					}

					else {
						sameColor = false;
					}

					if (adjacentPieceID != -1 && adjacentPieceID != -2 && !sameColor && validDirs[j] &&
						!isPairInVector(piecesCaptured, std::make_pair(current.first + (count * colorDir * directions[j].first),
						current.second + (count * colorDir * directions[j].second)))) {

						validCoord = piecesMap.find(std::make_pair(current.first + ((count + 1) * colorDir * directions[j].first),
						current.second + ((count + 1) * colorDir * directions[j].second))) != piecesMap.end();

						piecesCaptured.push_back(std::make_pair(current.first + (count * colorDir * directions[j].first),
						current.second + (count * colorDir * directions[j].second)));

						for (int countAux = (count + 1); validCoord; ++countAux) {
							validCoord = piecesMap.find(std::make_pair(current.first + (countAux * colorDir * directions[j].first),
							current.second + (countAux * colorDir * directions[j].second))) != piecesMap.end();

							if (piecesMap[std::make_pair(current.first + (countAux * colorDir * directions[j].first),
								current.second + (countAux * colorDir * directions[j].second))] == -1 &&
								!isPairInVector(moves, std::make_pair(current.first + (countAux * colorDir * directions[j].first),
								current.second + (countAux * colorDir * directions[j].second)))) {

								moves.push_back(std::make_pair(current.first + (countAux * colorDir * directions[j].first),
								current.second + (countAux * colorDir * directions[j].second)));

								captures.push(std::make_pair(current.first + (countAux * colorDir * directions[j].first),
								current.second + (countAux * colorDir * directions[j].second)));
							}

							else {
								validDirs[j] = false;
								break;
							}

							if (isQueen() == false) {
								break;
							}
						}
					}
				}

				else {
					validDirs[j] = false;
				}
			}
		}
	}

	return moves;
}

bool Piece::sameColor(sf::Color pieceColor) {
	return (color == pieceColor);
}