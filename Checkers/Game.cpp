#include "Game.h"
#include <map>

Game::Game() {
	board = new Board();
	mainWindow = new sf::RenderWindow(sf::VideoMode(800, 800), "Damas!");
	createPieces();
}

Game::~Game() {
	destroyPieces();
	delete board;
	delete mainWindow;
}

void Game::createPieces() {
	int count = 0;
	float squareSize = board->getSquareSize();

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (count == 24) {
				return;
			}

			if (i > 2 && i < 5) {
				piecesMAP.insert(std::make_pair(std::make_pair(i, j), -1));
				continue;
			}

			if (!((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0))) {
				sf::Color fillColor, outlineColor;

				if (count < 12) {
					fillColor = BLACK;
					outlineColor = WHITE;
				}

				else {
					fillColor = WHITE;
					outlineColor = BLACK;
				}

				pieces[count] = new Piece(std::make_pair(i, j), fillColor);
				pieces[count]->setCircleShape(sf::CircleShape(squareSize / 2.5));
				pieces[count]->setColor(fillColor);
				
				sf::CircleShape& shape = pieces[count]->getCircleShape();

				shape.setOutlineThickness(squareSize / 25);
				shape.setOutlineColor(outlineColor);
				shape.setPosition(sf::Vector2f(60 + (squareSize / 2) + (j * squareSize) - shape.getRadius(), 
				60 + (squareSize / 2)  + (i * squareSize) - shape.getRadius()));
				
				piecesMAP.insert(std::make_pair(std::make_pair(i, j), count));
				count++;
			}

			else {
				piecesMAP.insert(std::make_pair(std::make_pair(i, j), -1));
			}
		}
	}
}

void Game::destroyPieces() {
	for (int i = 0; i < 24; ++i) {
		delete pieces[i];
	}
}

void Game::init() {
	running = true;
}

sf::RenderWindow*& Game::getMainWindow() {
	return mainWindow;
}

void Game::resize() {
	board->resize(mainWindow->getSize());
	resizePieces();
}

void Game::update() {
	board->draw(mainWindow);
	drawPieces();
}

void Game::drawPieces() {
	for (int i = 0; i < 24; ++i) {
		if (pieces[i] != nullptr) {
			mainWindow->draw(pieces[i]->getCircleShape());
			mainWindow->draw(pieces[i]->getCrown());
		}
	}
}

void Game::resizePieces() {
	int count = 0;
	float squareSize = board->getSquareSize();
	float radius = 0.00f;

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (count == 24) {
				return;
			}

			if (i > 2 && i < 5) {
				continue;
			}

			if (!((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0))) {
				if (pieces[count] != nullptr) {
					sf::CircleShape& shape = pieces[count]->getCircleShape();
					sf::Sprite& crown = pieces[count]->getCrown();
					const std::pair<int, int> pos = pieces[count]->getPos();

					shape.setOutlineThickness(squareSize / 25);
					shape.setRadius(squareSize / 2.5);
					radius = shape.getRadius();
					shape.setPosition(sf::Vector2f(60 + (squareSize / 2) + pos.second * squareSize - radius,
						60 + (squareSize / 2) + pos.first * squareSize - radius));

					crown.setScale(0.0005 * squareSize, 0.0005 * squareSize);
					crown.setPosition(sf::Vector2f(60 + (squareSize / 3.25) + (pos.second * squareSize),
						60 + (squareSize / 3.25) + pos.first * squareSize));
				}

				count++;
			}
		}
	}
}

bool Game::boardClicked(sf::Vector2i mousePos) {
	float squareSize = board->getSquareSize();
	const int ROW = static_cast<int> ((static_cast<float>(mousePos.y) - 60) / squareSize);
	const int COL = static_cast<int> ((static_cast<float>(mousePos.x) - 60) / squareSize);

	bool ROW_INVALID = !(ROW >= 0 && ROW <= 7); 
	bool COL_INVALID = !(COL >= 0 && COL <= 7);

	if (ROW_INVALID || COL_INVALID) {
		return false;
	}

	else {
		squareSelected = piecesMAP[std::make_pair(ROW, COL)];
		squareClicked = std::make_pair(ROW, COL);
		return true;
	}
}

static void deSelectPieceIfNeeded(Board*& board, Piece*& piece, std::map<std::pair<int, int>, int>& piecesMAP, int& prevSquare, int& currentSquare) {
	const short int FREE_SPACE = -1;
	bool prevMoveHasLegit = prevSquare != FREE_SPACE;

	if (prevMoveHasLegit) {
		board->select(piece, piecesMAP, false);
		prevSquare = FREE_SPACE;
		currentSquare = FREE_SPACE;
	}
}

void Game::movePiece() {
	const int FREE_SPACE = -1;
	Piece*& selectedPiece = pieces[previusSquareSelected];
	std::pair<int, int> pos = selectedPiece->getPos();

	if (selectedPiece->mustCapture(piecesMAP)) {
		removeCapturedPieces();
	}

	piecesMAP[pos] = FREE_SPACE;
	board->select(selectedPiece, piecesMAP, false);
	selectedPiece->setPos(squareClicked);
	pos = selectedPiece->getPos();

	sf::CircleShape& shape = selectedPiece->getCircleShape();
	const float squareSize = board->getSquareSize(), radius = shape.getRadius();
	shape.setPosition(sf::Vector2f(60 + (squareSize / 2) + (pos.second * squareSize) - radius,
		60 + (squareSize / 2) + (pos.first * squareSize) - radius));

	if ((squareClicked.first == 0 && selectedPiece->getColor() == WHITE ||
		squareClicked.first == 7 && selectedPiece->getColor() == BLACK) &&
		(selectedPiece->isQueen() == false)) {

		selectedPiece->setQueen();
	}

	if (selectedPiece->isQueen()) {
		sf::Sprite& crown = selectedPiece->getCrown();
		crown.setScale(0.0005 * squareSize, 0.0005 * squareSize);
		crown.setPosition(sf::Vector2f(60 + (squareSize / 3.25) + (pos.second * squareSize),
			60 + (squareSize / 3.25) + (pos.first * squareSize)));
	}

	piecesMAP[squareClicked] = previusSquareSelected;
	squareSelected = -1;
	lastMovedPiece = previusSquareSelected;
}

void Game::updateBoard() { 
	const short int FREE_SPACE = -1, POSS_MOVE = -2;
	bool noPiecesSelected = squareSelected == previusSquareSelected && squareSelected == FREE_SPACE;

	if (!boardClicked(sf::Mouse::getPosition(*mainWindow)) || noPiecesSelected) {
		deSelectPieceIfNeeded(board, pieces[previusSquareSelected], piecesMAP, previusSquareSelected, squareSelected);
		return;
	}

	if (!noPiecesSelected && squareSelected != POSS_MOVE) {  
		/*if (pieces[lastMovedPiece]->mustCapture(piecesMAP) && pieces[squareSelected]->getColor() != pieces[lastMovedPiece]->getColor()) {
			for (piece : pieces) {
				Para toda peça que não deverá ser movida no momento (visto que a última peça que capturou ainda pode fazer outra captura)
				só marcar a peça, mas não mostrar os movimentos
			}
			return;
		}*/

		if (pieces[lastMovedPiece]->getColor() == pieces[squareSelected]->getColor()) {
			deSelectPieceIfNeeded(board, pieces[previusSquareSelected], piecesMAP, previusSquareSelected, squareSelected);
			return;
		}
	}

	if (squareSelected == POSS_MOVE) {
		movePiece();
	}
	
	else {
		if (squareSelected == previusSquareSelected && squareSelected != FREE_SPACE) {
			board->select(pieces[squareSelected], piecesMAP, false);
			previusSquareSelected = FREE_SPACE;
			return;
		}

		if (previusSquareSelected != FREE_SPACE) {
			board->select(pieces[previusSquareSelected], piecesMAP, false);
		}

		if (squareSelected != FREE_SPACE) {
			board->select(pieces[squareSelected], piecesMAP, false);
		}
	}

	previusSquareSelected = squareSelected;
}

template<typename T>
static void removeItemFromVector(std::vector<T>& vector, T value) {
	vector.erase(std::remove(vector.begin(), vector.end(), value));
}

//Ocorrem crashes no programa. Motivo: Ao capturar com damas, 
//o algoritmo nem sempre alcança o lugar correto para prosegguir

//Possível solução:
/*
Se por acaso houver mais de um movimento na mesma direção, desconsiderar as casas que não me levam ao objetivo
*/

#include <iostream>

void Game::removeCapturedPieces() {
	const std::pair<int, int> directions[4] = { {1, -1}, {1, 1}, {-1, -1}, {-1, 1} };
	std::pair<int, int> coord = pieces[previusSquareSelected]->getPos();
	int colorDir = -1;

	if (pieces[previusSquareSelected]->getColor() == BLACK) {
		colorDir = 1;
	}

	std::vector<Mark> marks = board->getMarks();
	std::vector<std::pair<int, int>> path;
	std::vector <std::pair<int, int>> stepsDirs;
	std::vector<std::pair<int, int>> positions;
	std::map<std::pair<int, int>, bool> visitedCoords;
	std::vector<sf::Vector2i> capturedPiecesCoords;

	int countAdjacentPosMoves = 0;
	bool validDirs[4] = { true, true, true, true };

	for (const Mark mark : marks) {
		positions.push_back(mark.pos);
		visitedCoords.insert({ mark.pos, false });
	}

	path.push_back(coord);

	while (coord != squareClicked) {
		int pieceFoundID = -1;
		countAdjacentPosMoves = 0;

		for (int i = 0; i < 4; ++i) {
			validDirs[i] = 1;
		}

		for (int i = 0; validDirs[0] || validDirs[1] || validDirs[2] || validDirs[3]; ++i) {
			if (pieces[previusSquareSelected]->isQueen() == false && i > 0) {
				break;
			}

			for (int j = 0; j < 4; ++j) {
				std::pair<int, int> posAux = { coord.first + ((2 + i) * colorDir * directions[j].first), coord.second + ((2 + i) * colorDir * directions[j].second) };
				std::pair<int, int> posTmp = { posAux.first - (colorDir * directions[j].first), posAux.second - (colorDir * directions[j].second) };
				bool outofbounds = (posTmp.first < 0 || posTmp.first > 7 || posTmp.second < 0 || posTmp.second > 7);
			
				if (outofbounds) {
					validDirs[j] = 0;
					continue;
				}

				if (piecesMAP[posTmp] != -1 && piecesMAP[posTmp] != -2) {
					bool sameColor = colorDir == 1 && piecesMAP[posTmp] < 11 || colorDir == -1 && piecesMAP[posTmp] > 11;
					
					if (!sameColor) {
						pieceFoundID = piecesMAP[posTmp];
					}
				}

				bool canCapture = pieceFoundID != -1;

				if (isPairInVector(positions, posAux) && !visitedCoords[posAux] && canCapture) {
					countAdjacentPosMoves++;
					visitedCoords[posAux] = true;
					path.push_back(posAux);
					stepsDirs.push_back(directions[j]);
					capturedPiecesCoords.push_back({ pieces[pieceFoundID]->getPos().first, pieces[pieceFoundID]->getPos().second });
					coord = posAux;
					i = 0;
					pieceFoundID = -1;
					break;
				}
			}
		}

		if (countAdjacentPosMoves == 0) {
			removeItemFromVector(path, path[path.size() - 1]);
			removeItemFromVector(stepsDirs, stepsDirs[stepsDirs.size() - 1]);
			removeItemFromVector(capturedPiecesCoords, capturedPiecesCoords[capturedPiecesCoords.size() - 1]);
			coord = path[path.size() - 1];
		}
	}

	removeItemFromVector(path, path[0]);


	std::cout << "\nImprimindo o array da sequência de capturas:" << std::endl;
	for (int i = 0; i < path.size(); ++i) {
		std::cout << "(" << path[i].first << ", " <<path[i].second<<" )" << std::endl;
	}

	std::cout << "\nImprimindo o array de peças que serão capturadas:" << std::endl;
	for (int i = 0; i < path.size(); ++i) {
		std::cout << "(" << capturedPiecesCoords[i].x << ", " << capturedPiecesCoords[i].y << " )" << std::endl;
	}

	for (int i = 0; i < path.size(); ++i) {
		sf::Vector2i posOfPieceToRem = capturedPiecesCoords[i];
		int pieceToRemoveID = piecesMAP[{posOfPieceToRem.x, posOfPieceToRem.y}];

		piecesMAP[{posOfPieceToRem.x, posOfPieceToRem.y}] = -1;

		std::cout << "pieceToRemoveID: " << pieceToRemoveID << "\n";

		if (pieces[pieceToRemoveID] != nullptr) {
			delete pieces[pieceToRemoveID];
			pieces[pieceToRemoveID] = nullptr;
		}
	}
}

/*
void Game::removeCapturedPieces() {
	const std::pair<int, int> directions[4] = { {1, -1}, {1, 1}, {-1, -1}, {-1, 1} };
	std::pair<int, int> coord = pieces[previusSquareSelected]->getPos();
	int colorDir = -1;

	if (pieces[previusSquareSelected]->getColor() == BLACK) {
		colorDir = 1;
	}

	std::vector<Mark> marks = board->getMarks();
	std::vector<std::pair<int, int>> path;
	std::vector <std::pair<int, int>> stepsDirs;
	std::vector<std::pair<int, int>> positions;
	std::map<std::pair<int, int>, bool> visitedCoords;

	int countAdjacentPosMoves = 0;

	for (const Mark mark : marks) {
		positions.push_back(mark.pos);
		visitedCoords.insert({mark.pos, false});
	}

	path.push_back(coord);

	while (coord != squareClicked) {
		std::pair<int, int> posAux;
		countAdjacentPosMoves = 0;

		for (std::pair<int, int> dir : directions) { //substituir o 2 para algum valor incremental
			posAux = std::make_pair(coord.first + (2 * colorDir * dir.first), coord.second + (2 * colorDir * dir.second));
			bool canCapture = piecesMAP[{posAux.first - (colorDir * dir.first), posAux.second - (colorDir * dir.second)}] != -1;

			if (isPairInVector(positions, posAux) && visitedCoords[posAux] == false && canCapture) {
				countAdjacentPosMoves++;
				visitedCoords[posAux] = true;
				path.push_back(posAux);
				stepsDirs.push_back(dir);
				coord = posAux;
				break;
			}
		}

		if (countAdjacentPosMoves == 0) {
			removeItemFromVector(path, path[path.size() - 1]);
			removeItemFromVector(stepsDirs, stepsDirs[stepsDirs.size() - 1]);
			coord = path[path.size() - 1];
		}
	}

	removeItemFromVector(path, path[0]);

	for (int i = 0; i < path.size(); ++i) {
		std::pair<int, int> posOfPieceToRem = std::make_pair(path[i].first - (stepsDirs[i].first * colorDir),
			path[i].second - (stepsDirs[i].second * colorDir));
		int pieceToRemoveID = piecesMAP[posOfPieceToRem];

		piecesMAP[posOfPieceToRem] = -1;
		delete pieces[pieceToRemoveID];
		pieces[pieceToRemoveID] = nullptr;
	}
}

bool Game::isAjacent(Piece piece, sf::Vector2i position) {
	sf::Vector2i directions[4] = { {1, -1}, {1, 1}, {-1, -1}, {-1, 1} };
	bool validDirs[4] = { true, true, true, true };
	int colorDir = -1;

	if (pieces[previusSquareSelected]->getColor() == BLACK) {
		colorDir = 1;
	}

	for (int i = 1; validDirs[0] || validDirs[1] || validDirs[2] || validDirs[3]; ++i) {
		if (!piece.isQueen() && i > 1) {
			break;
		}

		for (int j = 0; j < 4; ++j) {
			std::pair<int, int> nextPosition = { piece.getPos().first + ((i+1) * directions[j].x * colorDir),
				piece.getPos().second + (i * directions[j].y * colorDir) };

			bool validPos = piecesMAP.find(nextPosition) != piecesMAP.end();

			if (validPos && piecesMAP[nextPosition] == -2) {
				if (nextPosition.first == position.x && nextPosition.second == position.y) {
					return true;
				}
			}

			else {
				validDirs[j] = 0;
			}
		}
	}

	return false;
}
*/