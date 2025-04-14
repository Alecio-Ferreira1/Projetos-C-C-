#include "Mainframe.h"
#include "GameApp.h"

void Mainframe::onButtonClick(wxEvent& evt) {
	wxBitmapButton* square = (wxBitmapButton*)evt.GetEventObject();

	if (game->isRunning() && square->GetName() != "opened") {
		char row = square->GetName()[0];
		char col = square->GetName()[1];

		game->updateBoard(std::make_pair(row - '0', col - '0'), buttons);
		Update();
	}

	if(!game->isRunning()){
		wxString text;

		if (game->victory()) {
			text = "Parabéns, você venceu!";
		}

		else {
			text = "Você perdeu, tente novamente!";
		}

		endGameWindow->SetMessage(text);
		endGameWindow->ShowModal();
		game->restart();
		restartBoard();
	}
}

void Mainframe::rightButtonClick(wxEvent& evt) {
	if (!game->isRunning()) {
		return;
	}

	wxBitmapButton* square = (wxBitmapButton*) evt.GetEventObject();
	wxImage* img = nullptr;

	if (game->getFlags() < ((16 * ROWS * COLS) / 100) && !square->GetName().Contains("flag") && !square->GetName().Contains("opened")) {
		game->putFlag();
		
		img = new wxImage("imgs/flag.png", wxBITMAP_TYPE_PNG);
		square->SetName(square->GetName() + "flag");
		*img = img->Scale(WIDTH / COLS, WIDTH / COLS);
		square->SetBitmap(*img);
		square->Refresh();
		
		return;
	}

	if (square->GetName().Contains("flag")) {
		game->removeFlag();

		img = new wxImage("imgs/square_not_opened.png", wxBITMAP_TYPE_PNG);
		*img = img->Scale(WIDTH / COLS, WIDTH / COLS);
		square->SetBitmap(*img);
		square->SetName(wxString::Format("%c%c", square->GetName()[0], square->GetName()[1]));
		square->Refresh();
	}
}

void Mainframe::createBoard(wxFrame *father) {
	wxInitAllImageHandlers();

	wxImage* img = new wxImage("imgs/square_not_opened.png", wxBITMAP_TYPE_PNG);
	*img = img->Scale(WIDTH / COLS, WIDTH / COLS);

	for (int i = 0; i < ROWS; ++i) {
		buttons[i] = new wxBitmapButton* [COLS];

		for (int j = 0; j < COLS; ++j) {
			buttons[i][j] = new wxBitmapButton(father, wxID_ANY, *img, wxPoint((WIDTH / COLS) * j,
				(HEIGHT / ROWS) * i), wxSize(WIDTH / COLS, WIDTH / COLS));
			buttons[i][j]->Bind(wxEVT_BUTTON, &Mainframe::onButtonClick, this);
			buttons[i][j]->Bind(wxEVT_RIGHT_DOWN, &Mainframe::rightButtonClick, this);
			buttons[i][j]->SetName(wxString::Format("%i%i", i, j));
		}
	}
}

void Mainframe::restartBoard() {
	wxImage* img = new wxImage("imgs/square_not_opened.png", wxBITMAP_TYPE_PNG);
	*img = img->Scale(WIDTH / COLS, WIDTH / COLS);

	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			buttons[i][j]->SetBitmap(*img);
			buttons[i][j]->SetName(wxString::Format("%i%i", i, j));
		}
	}
}

Mainframe::Mainframe(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	buttons = new wxBitmapButton** [ROWS];
	wxPanel* panel = new wxPanel(this, wxID_ANY);
	wxIcon* favicon = new wxIcon("imgs/favicon.ico", wxBITMAP_TYPE_ICO);
	
	endGameWindow = new wxMessageDialog(panel, "", "Fim de Jogo!");

	SetIcon(*favicon);

	createBoard((wxFrame*) panel);
	game->start();
}