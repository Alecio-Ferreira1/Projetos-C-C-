#include "Game.h"
#define INVALID_COORDS std::make_pair(-1, -1);

enum optionsFor_getKeyMoveANDgeneratePlay {
	TRY_VICTORY = 1,
	BLOCK_VICTORY = 2,
};

bool Game::isRunning() const{
	return running;
}

void Game::reset() {
	running = true;
	plays = 0;
	lastSymbol = None;
	AITurn = rand() % 2;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			spaces[i][j] = None;
		}
	}
}

void Game::update(std::pair<int, int> coords, std::array<std::array<wxBitmapButton*, 3>, 3>& squares) {
	bool firstPlay = plays == 0;

	if (plays >= 9 || victory()) {
		running = false;
		return;
	}

	if (!firstPlay && spaces[coords.first][coords.second] != None) {
		return;
	}

	wxImage* img = nullptr;

	if (lastSymbol == None || lastSymbol == X) {
		spaces[coords.first][coords.second] = O;
		lastSymbol = O;

		img = new wxImage("assets/O.png", wxBITMAP_TYPE_PNG);
	}

	else {
		spaces[coords.first][coords.second] = X;
		lastSymbol = X;

		img = new wxImage("assets/X.png", wxBITMAP_TYPE_PNG);
	}


	*img = img->Scale(90, 90);
	squares[coords.first][coords.second]->SetBitmap(*img);
	squares[coords.first][coords.second]->Refresh();

	plays++;
	delete img;

	if (victory() || plays >= 9) {
		running = false;
		return;
	}
}

std::pair<int, int> Game::generatePlay(){
	if (plays >= 9){
		return (std::make_pair(0, 0));
	}

	bool corner = spaces[0][0] == O || spaces[0][2] == O || spaces[2][0] == O || spaces[2][2] == O;

	if (plays == 1 && AITurn == 1 && corner) {
		return std::make_pair(1, 1);
	}

	optionsFor_getKeyMoveANDgeneratePlay sequence_of_instructions[] = {TRY_VICTORY, BLOCK_VICTORY};
	std::pair<int, int> coords;

	for (const auto instruction : sequence_of_instructions) {
		coords = getKeyMove(instruction);

		if (coords.first != -1 && coords.second != -1) {
			return coords;
		}
	}

	int i = rand() % 3, j = rand() % 3;

	while (spaces[i][j] != None) {
		i = rand() % 3;
		j = rand() % 3;
	}
	
	return (std::make_pair(i, j));
}

std::pair<int, int> Game::getKeyMove(int situation) const{
	Symbol AIsymbol = None, playerSimbol = None;
	Symbol symbolForComparasion = None;

	if (getAITurn() == 0) {
		AIsymbol = O;
		playerSimbol = X;
	}

	else {
		AIsymbol = X;
		playerSimbol = O;
	}

	if (situation == TRY_VICTORY) {
		symbolForComparasion = AIsymbol;
	}

	else {
		symbolForComparasion = playerSimbol;
	}

	int countSymbolsMainDiag = 0;
	int countSymbolsSecDiag = 0;
	std::pair<int, int> aux[4] = { {0, 0}, {0, 0}, {0, 0}, {0, 0} };

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (spaces[i][j] == symbolForComparasion) {
				int countSymbolsRow = 0;
				int countSymbolsCol = 0;

				for (int k = 0; k < 3; ++k) {
					if (spaces[i][k] == symbolForComparasion) {
						countSymbolsRow++;
					}

					else {
						aux[0] = std::make_pair(i, k);
					}

					if (spaces[k][j] == symbolForComparasion) {
						countSymbolsCol++;
					}

					else {
						aux[1] = std::make_pair(k, j);
					}
				}

				if (countSymbolsRow == 2 && spaces[aux[0].first][aux[0].second] == None) {
					return aux[0];
				}

				if (countSymbolsCol == 2 && spaces[aux[1].first][aux[1].second] == None) {
					return aux[1];
				}
			}
		}

		if (spaces[i][i] == symbolForComparasion) {
			countSymbolsMainDiag++;
		}

		else {
			aux[2] = std::make_pair(i, i);
		}

		if (spaces[i][2 - i] == symbolForComparasion) {
			countSymbolsSecDiag++;
		}

		else {
			aux[3] = std::make_pair(i, 2 - i);
		}
	}

	if (countSymbolsMainDiag == 2 && spaces[aux[2].first][aux[2].second] == None) {
		return aux[2];
	}

	if (countSymbolsSecDiag == 2 && spaces[aux[3].first][aux[3].second] == None) {
		return aux[3];
	}

	return INVALID_COORDS;
}

void Game::activeAutoPlays() {
	player2 = false;
}

void Game::turnOffAutoPlays() {
	player2 = true;
}

bool Game::autoPlaysON() const{
	return (player2 == false);
}

int Game::getAITurn() const {
	return AITurn;
}

void Game::start() {
	running = true;
}

bool Game::hasRowCompleted() const{
	for (int i = 0; i < 3; ++i) {
		int count = 0;

		for (int j = 0; j < 3; ++j) {
			if (spaces[i][j] == spaces[i][0] && spaces[i][0] != None) {
				count++;
			}

			if (count == 3) {
				return true;
			}
		}
	}

	return false;
}

bool Game::hasColumnCompleted() const{
	for (int i = 0; i < 3; ++i) {
		int count = 0;

		for (int j = 0; j < 3; ++j) {
			if (spaces[j][i] == spaces[0][i] && spaces[0][i] != None) {
				count++;
			}

			if (count == 3) {
				return true;
			}
		}
	}

	return false;
}

bool Game::hasMainDiagonal() const {
	int count = 0;

	for (int i = 0; i < 3; ++i) {
		if (spaces[i][i] == spaces[0][0] && spaces[0][0] != None) {
			count++;
		}

		else {
			break;
		}
	}

	if (count == 3) {
		return true;
	}

	else return false;
}

bool Game::hasSecDiagonal() const {
	int count = 0;

	for (int i = 0; i < 3; ++i) {
		if (spaces[i][2 - i] == spaces[0][2] && spaces[0][2] != None) {
			count++;
		}

		else {
			break;
		}
	}

	if (count == 3) {
		return true;
	}

	else return false;
}

bool Game::victory() const{
	bool rowComplete = this->hasRowCompleted();
	bool columnComplete = this->hasColumnCompleted();
	bool mainDiagonal = this->hasMainDiagonal();
	bool secDiagonal = this->hasSecDiagonal();

	if (rowComplete || columnComplete || mainDiagonal || secDiagonal){
		return true;
	}

	else return false;
}

int Game::quantPlays() const {
	return plays;
}