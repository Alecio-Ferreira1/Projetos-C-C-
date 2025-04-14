#pragma onceT
#include <SFML/Graphics.hpp>
#include <array>
#include <map>
#include <vector>
#include "Board.h"
#include "Piece.h"

class Game {
private:
	bool running = false;
	Board* board = nullptr;
	std::array<Piece*, 24> pieces;
	sf::RenderWindow* mainWindow = nullptr;
	int previusSquareSelected = -1;
	int squareSelected = -1;
	std::map<std::pair<int, int>, int> piecesMAP;
	short int lastMovedPiece;
	std::pair<int, int> squareClicked = std::make_pair(0, 0);
	void createPieces();
	void destroyPieces();
	bool boardClicked(sf::Vector2i mousePos);
	void removeCapturedPieces();
	void movePiece();
public:
	Game();
	~Game();
	void init();
	void update();
	sf::RenderWindow*& getMainWindow();
	void resize();
	void drawPieces();
	void resizePieces();
	void updateBoard();
};

static void deSelectPieceIfNeeded(Board*& board, Piece*& piece, std::map<std::pair<int, int>, int>& piecesMAP, int& prevSquare, int& currentSquare);

template <typename T>
static void removeItemFromVector(std::vector<T>& vector, T value);