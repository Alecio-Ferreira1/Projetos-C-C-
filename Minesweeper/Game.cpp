#include "Game.h"
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>

void Game::generateMines() {
    int randRow = rand() % ROWS;
    int randCol = rand() % COLS;

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            board[i][j] = -1;
        }
    }

    for (int i = 0; i < amountMines; ++i) {
        while (board[randRow][randCol] == 0) {
            randRow = rand() % ROWS;
            randCol = rand() % COLS;
        }

        board[randRow][randCol] = 0;
    }
}

int Game::numBombNeighbors(std::pair<int, int> coords) const{
    int count = 0;
    int row = coords.first, col = coords.second;

    if (row - 1 >= 0) {
        if (board[row - 1][col] == 0) {
            count++;
        }
    }

    if (row + 1 < ROWS) {
        if (board[row + 1][col] == 0) {
            count++;
        }
    }

    if (col - 1 >= 0) {
        if (board[row][col - 1] == 0) {
            count++;
        }
    }

    if (col + 1 < COLS) {
        if (board[row][col + 1] == 0) {
            count++;
        }
    }

    if (row - 1 >= 0 && col + 1 < COLS) {
        if (board[row - 1][col + 1] == 0) {
            count++;
        }
    }

    if (row - 1 >= 0 && col - 1 >= 0) {
        if (board[row - 1][col - 1] == 0) {
            count++;
        }
    }

    if (row + 1 < ROWS && col - 1 >= 0) {
        if (board[row + 1][col - 1] == 0) {
            count++;
        }
    }

    if (row + 1 < ROWS && col + 1 < COLS) {
        if (board[row + 1][col + 1] == 0) {
            count++;
        }
    }

    return count;
}

std::queue<std::pair<int, int>> Game::getNeighbors(std::pair<int, int> coords) {
    int row = coords.first, col = coords.second;
    std::queue<std::pair<int, int>> neighbors;

    if (row - 1 >= 0) {
        neighbors.push(std::make_pair(row - 1, col));
    }

    if (row + 1 < ROWS) {
        neighbors.push(std::make_pair(row + 1, col));
    }

    if (col - 1 >= 0) {
        neighbors.push(std::make_pair(row, col - 1));
    }

    if (col + 1 < COLS) {
        neighbors.push(std::make_pair(row, col + 1));
    }

    if (row - 1 >= 0 && col + 1 < COLS) {
        neighbors.push(std::make_pair(row - 1, col + 1));
    }

    if (row - 1 >= 0 && col - 1 >= 0) {
        neighbors.push(std::make_pair(row - 1, col - 1));
    }

    if (row + 1 < ROWS && col - 1 >= 0) {
        neighbors.push(std::make_pair(row + 1, col - 1));
    }

    if (row + 1 < ROWS && col + 1 < COLS) {
        neighbors.push(std::make_pair(row + 1, col + 1));
    }

    return neighbors;
}

void Game::showAllMines(wxBitmapButton***& squares) const{
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (board[i][j] == 0) {
                wxImage* bomb = new wxImage("imgs/bomb.png", wxBITMAP_TYPE_PNG);
                squares[i][j]->SetBitmap(*bomb);
                squares[i][j]->Refresh();
            }
        }
    }
}

void Game::updateBoard(std::pair<int, int> coords, wxBitmapButton***&squares) {
    if (board[coords.first][coords.second] == 0) {
        running = false;
        showAllMines(squares);
        Sleep(500);
        return;
    }

    if (board[coords.first][coords.second] == -1) {
        std::queue<std::pair<int, int>> queue_of_coords;

        queue_of_coords.push(coords);
        storedSquares.push_back(coords);

        while (!queue_of_coords.empty()) {
            std::pair<int, int> front = queue_of_coords.front();
            int numAdjBombs = numBombNeighbors(front);

            if(numAdjBombs == 0) {
                std::queue<std::pair<int, int>> neighbors = getNeighbors(front);

                while (!neighbors.empty()) {
                    std::pair<int, int> coord = neighbors.front();
                    bool wasAlreadyBeeninserted = false;

                    for (auto element : storedSquares) {
                        if (element.first == coord.first && element.second == coord.second) {
                            wasAlreadyBeeninserted = true;
                            break;
                        }
                    }

                    if (!wasAlreadyBeeninserted) {
                        queue_of_coords.push(coord);
                        storedSquares.push_back(coord);
                    }

                    neighbors.pop();
                }
            }

            if (squares[front.first][front.second]->GetName().Contains("flag")) {
                removeFlag();
            }

            wxImage* img = new wxImage(wxString::Format("imgs/%i_bombs.png", numAdjBombs), wxBITMAP_TYPE_PNG);
            *img = img->Scale(WIDTH/COLS - 0.5, WIDTH/COLS - 0.5);
            squares[front.first][front.second]->SetBitmap(*img);
            squares[front.first][front.second]->Refresh();
            squares[front.first][front.second]->SetName("opened");

            board[front.first][front.second] = 1;
            squaresOpened++;
            queue_of_coords.pop();

            if (victory()) {
                running = false;
                break;
            }
        }
    }
}

bool Game::victory() const {
    return squaresOpened == ((ROWS * COLS) - amountMines);
}

bool Game::isRunning() const{
    return running;
}

void Game::start() {
    running = true;
    generateMines();
}

void Game::restart() {
    start();
    storedSquares.clear();
    squaresOpened = 0;
    flags = 0;
}

int Game::getFlags() const{
    return flags;
}

void Game::putFlag() {
    flags++;
}

void Game::removeFlag() {
    flags--;
}