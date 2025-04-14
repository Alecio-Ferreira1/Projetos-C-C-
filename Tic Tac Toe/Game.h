#pragma once
#include <wx/wx.h>
#include <utility>
#include <array>

enum Symbol{
	None, O, X
};

class Game {
private:
	Symbol spaces[3][3] = { {None, None, None}, {None, None, None}, {None, None, None} };
	bool running = false;
	Symbol lastSymbol = None;
	int plays = 0;
	bool player2 = true;
	int AITurn = rand() % 2;
	std::pair<int, int> getKeyMove(int situation) const;
public:
	Game(){}
	~Game(){}
	bool isRunning() const;
	void reset();
	void update(std::pair<int, int> coords, std::array<std::array<wxBitmapButton*, 3>, 3>& squares);
	void start();
	bool victory() const;
	bool hasRowCompleted() const;
	bool hasColumnCompleted() const;
	bool hasMainDiagonal() const;
	bool hasSecDiagonal() const;
	int quantPlays() const;
	void activeAutoPlays();
	void turnOffAutoPlays();
	bool autoPlaysON() const;
	int getAITurn() const;
	std::pair<int, int> generatePlay();
};