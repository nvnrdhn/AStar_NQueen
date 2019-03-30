#include "MyApp.h"

bool MyApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;
	MyFrame *frame = new MyFrame("A* N-Queens");
	frame->SetInitialSize(wxSize(1280, 780));
	frame->Show(true);
	return true;
}
