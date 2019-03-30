#include "MyFrame.h"

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString & title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER & ~wxMAXIMIZE_BOX)
{
#if wxUSE_MENUS
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(helpMenu, "&Help");
	SetMenuBar(menuBar);
#endif
	mainwindow = new MainWindow(this);
	mainwindow->SetInitialSize(wxSize(1280, 720));
}
void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxString::Format
	(
		"Welcome to A* N-Queens program!\n"
		"\n"
		"This program was made by Novan Ardhana\n"
		"using %s.",
		wxVERSION_STRING
	),
		"About this program",
		wxOK | wxICON_INFORMATION,
		this);
}