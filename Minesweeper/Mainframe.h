#pragma once
#include <wx/wx.h>
#include "Game.h"

class Mainframe : public wxFrame {
private:
	Game* game = new Game;
	wxBitmapButton*** buttons = nullptr;
	wxMessageDialog* endGameWindow = nullptr;
public:
	Mainframe(const wxString& title);
	~Mainframe() { delete game; }
	void onButtonClick(wxEvent &evt);
	void createBoard(wxFrame *father);
	void rightButtonClick(wxEvent& evt);
	void restartBoard();
};