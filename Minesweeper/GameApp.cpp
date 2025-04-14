#include <wx/wx.h>
#include <time.h>
#include "GameApp.h"
#include "Mainframe.h"

bool GameApp::OnInit() {
	srand(time(NULL));

	Mainframe* mainframe = new Mainframe("Campo Minado");
	mainframe->SetClientSize(WIDTH, HEIGHT);
	mainframe->Center();
	mainframe->SetWindowStyle(wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER & ~wxMAXIMIZE_BOX);
	mainframe->Show();

	return true;
}

wxIMPLEMENT_APP(GameApp);