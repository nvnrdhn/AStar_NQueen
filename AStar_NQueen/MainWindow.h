#pragma once
#include <vector>
#include "wx\wx.h"
#include "wx\dcbuffer.h"
#include "State.h"
class MainWindow :
	public wxWindow
{
public:
	MainWindow(wxFrame *frame);
	~MainWindow();
	void RenderFrame(wxPaintEvent &evt);
	void DrawQueen(wxAutoBufferedPaintDC &pdc);
	void RandomQueen();
	State AStar(State init);
	void RandomClick(wxCommandEvent &evt);
	void SolveClick(wxCommandEvent &evt);
	void SetN(wxCommandEvent &evt);
private:
	int N = 8;
	State init;
	wxChoice *Nbox;
	wxArrayString *choices;
	wxButton *solvebutton;
	wxButton *randombutton;
	wxStaticText *timelabel;
	DECLARE_EVENT_TABLE()
};

