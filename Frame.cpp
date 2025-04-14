#include "Frame.h"

Frame::Frame(wxString title) : wxFrame(nullptr, wxID_ANY, title){
	wxPanel* panel = new wxPanel(this);
	wxArrayString options;

	options.Add("1 v 1");
	options.Add("I.A");

	wxFont font = wxFont(20, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRABOLD);

	wxChoice* optionsToPlay = new wxChoice(panel, wxID_ANY, wxPoint(10, 10), wxSize(100, 50), options);
	optionsToPlay->SetFont(font);
	optionsToPlay->Select(0); 
	optionsToPlay->Bind(wxEVT_CHOICE, &Frame::choiceBoxChange, this);

	font.SetPointSize(13);
	wxButton* resetButton = new wxButton(panel, wxID_ANY, "REINICIAR", wxPoint(WIDTH - 110, HEIGHT - 60), wxSize(100, 50));
	resetButton->SetFont(font);
	resetButton->Bind(wxEVT_BUTTON, &Frame::onButtonClick, this);
	resetButton->SetName("reset");

	SetBackgroundColour(wxColour(37, 7, 253));
	createButtons(panel);
	game->start();
}

Frame::~Frame() {
	delete game;
	delete message;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			delete buttons[i][j];
		}
	}
}

void Frame::createButtons(wxPanel*& panel) {
	wxInitAllImageHandlers();

	const int btn_size = 100;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			buttons[i][j] = new wxBitmapButton(panel, wxID_ANY, wxBitmap(), wxPoint((25 * WIDTH)/100 + (j * btn_size), 
			(18 * HEIGHT) / 100 + (i * btn_size)), wxSize(btn_size, btn_size));
			buttons[i][j]->Bind(wxEVT_BUTTON, &Frame::onButtonClick, this);
			buttons[i][j]->SetName(wxString::Format("%i%i", i, j));
		}
	}
}

void Frame::resetButtons() {
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			buttons[i][j]->SetBitmap(wxBitmap());
			buttons[i][j]->Refresh();
		}
	}

}

void Frame::onButtonClick(wxEvent& evt) {
	wxButton* button = (wxButton*)evt.GetEventObject();

	if (button->GetName() == "reset") {
		game->reset();
		resetButtons();

		if (game->autoPlaysON() && game->getAITurn() == 0) {
			game->update(game->generatePlay(), buttons);
		}
	}

	else {		
		if (game->isRunning()) {
			wxString buttonName = button->GetName();
			char i = buttonName[0], j = buttonName[1];
			int count = game->quantPlays();
		
			game->update(std::make_pair(i - '0', j - '0'), buttons);

			if (count != game->quantPlays() && game->autoPlaysON()) {
				game->update(game->generatePlay(), buttons);
			}
		}

		if (!game->isRunning()) {
			displayEndgameWindow();
		}
	}
}

void Frame::choiceBoxChange(wxEvent& evt) {
	wxChoice* options = (wxChoice*)evt.GetEventObject();

	if (prevSelection != options->GetStringSelection()){
		game->reset();
		resetButtons();

		if (options->GetStringSelection() == "I.A") {
			game->activeAutoPlays();

			if (game->getAITurn() == 0) {
				game->update(game->generatePlay(), buttons);
			}
		}

		else {
			game->turnOffAutoPlays();
		}

		prevSelection = options->GetStringSelection();
	}
}

void Frame::displayEndgameWindow() {
	wxString text = "";

	if (game->victory()) {
		if (game->autoPlaysON()) {
			if (game->quantPlays() % 2 != game->getAITurn()) {
				text = "O computador venceu! Reinicie o Jogo.";
			}

			else {
				text = "Você venceu! Reinicie o Jogo.";
			}
		}

		else {
			if (game->quantPlays() % 2 != 0) {
				text = "O Jogador 1 venceu! Reinicie o Jogo.";
			}

			else {
				text = "O Jogador 2 venceu! Reinicie o Jogo.";
			}
		}
	}

	else {
		text = "O jogo empatou! Reinicie o Jogo.";
	}

	message->SetMessage(text);
	message->ShowModal();
}