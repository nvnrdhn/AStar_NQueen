#pragma once
#include "wx/wxprec.h"
#include "wx/wx.h"
#include "MainWindow.h"
class MyFrame :
	public wxFrame
{
public:
	MyFrame(const wxString& title);
	void OnAbout(wxCommandEvent& event);
private:
	MainWindow *mainwindow;
	wxDECLARE_EVENT_TABLE();
};
