#pragma once
#include <wx/wx.h>
#include "Game.h"
#include <array>
#define WIDTH 640
#define HEIGHT 480

class Frame : public wxFrame {
private:
	std::array<std::array<wxBitmapButton*, 3>, 3> buttons;
	Game* game = new Game;
	wxMessageDialog* message = new wxMessageDialog(this, "", "Fim de jogo!", wxOK_DEFAULT | wxCENTRE);
	wxString prevSelection = "1 v 1";
public:
	Frame(wxString title);
	~Frame();
	void createButtons(wxPanel*& panel);
	void onButtonClick(wxEvent& evt);
	void choiceBoxChange(wxEvent& evt);
	void resetButtons();
	void displayEndgameWindow();
};