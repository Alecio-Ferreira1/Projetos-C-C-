#include <wx/wx.h>
#include <vector>
#include <queue>
#define ROWS 10
#define COLS 10
#define WIDTH 400
#define HEIGHT 400

class Game {
private:
    int board[ROWS][COLS];
    std::vector<std::pair<int, int>> storedSquares;
    bool running = false;
    int squaresOpened = 0;
    int amountMines = (16 * ROWS * COLS) / 100;
    int flags = 0;
public:
    Game() {}
    ~Game() {}
    bool isRunning() const;
    void start();
    void restart();
    void updateBoard(std::pair<int, int> coords, wxBitmapButton***& button);
    void generateMines();
    void showAllMines(wxBitmapButton***& squares) const;
    int numBombNeighbors(std::pair<int, int> coords) const;
    std::queue<std::pair<int, int>> getNeighbors(std::pair<int, int> coords);
    int getFlags() const;
    void putFlag();
    void removeFlag();
    bool victory() const;
};
