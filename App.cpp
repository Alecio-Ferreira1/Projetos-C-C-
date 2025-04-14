#include "App.h"
#include "Frame.h"

bool App::OnInit() {
	Frame* mainframe = new Frame("Jogo da Velha");

	mainframe->SetClientSize(WIDTH, HEIGHT);
	mainframe->Center();
	mainframe->SetWindowStyle(wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER & ~wxMAXIMIZE_BOX);
	mainframe->Show();

	return true;
}

wxIMPLEMENT_APP(App);