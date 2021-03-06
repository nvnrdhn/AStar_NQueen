#include "MainWindow.h"
#include <time.h>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>

BEGIN_EVENT_TABLE(MainWindow, wxWindow)
EVT_PAINT(MainWindow::RenderFrame)
EVT_CHOICE(wxID_ANY, MainWindow::SetN)
EVT_BUTTON(1000, MainWindow::SolveClick)
EVT_BUTTON(1001, MainWindow::RandomClick)
END_EVENT_TABLE()


MainWindow::MainWindow(wxFrame *frame)
	: wxWindow(frame, wxID_ANY)
{
	srand(time(NULL));
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	SetBackgroundColour(wxColor(*wxWHITE));
	choices = new wxArrayString();
	for (int i = 4; i <= 20; i++) {
		std::string temp = std::to_string(i);
		choices->Add(temp);
	}
	Nbox = new wxChoice(this, this->GetId(), wxPoint(770, 20), wxSize(450, 300));
	Nbox->Set(*choices);
	solvebutton = new wxButton(this, 1000, "Solve", wxPoint(770, 100), wxSize(100, 100));
	solvebutton = new wxButton(this, 1001, "Random", wxPoint(770, 250), wxSize(100, 100));
	timelabel = new wxStaticText(this, wxID_ANY, "Searching time: ", wxPoint(770, 400));
	timelabel->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	timelabel->SetBackgroundColour(*wxBLACK);
	timelabel->SetForegroundColour(*wxWHITE);
	heurlabel = new wxStaticText(this, wxID_ANY, "Heuristic value: ", wxPoint(770, 450));
	heurlabel->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	heurlabel->SetBackgroundColour(*wxBLACK);
	heurlabel->SetForegroundColour(*wxWHITE);
	RandomQueen();
}


MainWindow::~MainWindow()
{
	delete choices;
	delete Nbox;
	delete solvebutton;
	delete randombutton;
	delete timelabel;
}

void MainWindow::RenderFrame(wxPaintEvent & evt)
{
	wxAutoBufferedPaintDC pdc(this);
	bool black = 1;
	int size = 720 / N;
	for (int y = 0; y < 720; y += size) {
		bool temp = black;
		for (int x = 0; x < 720; x += size) {
			if (black)
				pdc.SetBrush(*wxBLACK_BRUSH);
			else
				pdc.SetBrush(*wxWHITE_BRUSH);
			pdc.DrawRectangle(wxPoint(x, y), wxSize(x + size, y + size));
			black = black ? 0 : 1;
		}
		black = temp ? 0 : 1;
	}
	pdc.SetBrush(*wxBLACK_BRUSH);
	pdc.SetPen(*wxBLACK_PEN);
	pdc.DrawRectangle(wxPoint(720, 0), wxSize(1000, 1000));
	pdc.DrawRectangle(wxPoint(0, 720), wxSize(1000, 1000));
	DrawQueen(pdc);
}

void MainWindow::DrawQueen(wxAutoBufferedPaintDC & pdc)
{
	pdc.SetBrush(*wxRED_BRUSH);
	pdc.SetPen(*wxBLACK_PEN);
	int size = 720 / N;
	for (auto it : init.queens)
		pdc.DrawCircle(wxPoint(it.x*size + (size / 2), it.y*size + (size / 2)), size / 4);

}

void MainWindow::RandomQueen()
{
	timelabel->SetLabel("Searching time: ");
	init.hn = init.fn = init.step = 0;
	init.solution = std::queue<pii>();
	init.ld.clear();
	init.col.clear();
	init.col.resize(N, 0);
	init.rd.clear();
	init.rd.resize(N * 2, 0);
	init.queens.clear();
	for (int i = 0; i < N; i++) {
		Queen q = Queen(rand() % N, i);
		init.queens.push_back(q);
		init.hn += init.col[q.x];
		init.hn += init.ld[q.x - q.y];
		init.hn += init.rd[q.x + q.y];
		init.col[q.x]++;
		init.ld[q.x - q.y]++;
		init.rd[q.x + q.y]++;
	}
	heurlabel->SetLabel("Heuristic value: " + std::to_string(init.hn));
}

State MainWindow::AStar(State init)
{
	std::priority_queue<State, std::vector<State>, std::greater<std::vector<State>::value_type>> pq;
	pq.push(init);
	while (!pq.empty()) {
		State current = pq.top();
		pq.pop();
		if (current.hn == 0)
			return current;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (j != current.queens[i].x) {
					State next = current;

					next.solution.push(pii(i, j));

					next.col[next.queens[i].x]--;
					next.ld[next.queens[i].x - next.queens[i].y]--;
					next.rd[next.queens[i].x + next.queens[i].y]--;

					next.hn -= next.col[next.queens[i].x];
					next.hn -= next.ld[next.queens[i].x - next.queens[i].y];
					next.hn -= next.rd[next.queens[i].x + next.queens[i].y];

					next.queens[i].x = j;

					next.hn += next.col[next.queens[i].x];
					next.hn += next.ld[next.queens[i].x - next.queens[i].y];
					next.hn += next.rd[next.queens[i].x + next.queens[i].y];

					next.col[next.queens[i].x]++;
					next.ld[next.queens[i].x - next.queens[i].y]++;
					next.rd[next.queens[i].x + next.queens[i].y]++;

					next.step++;
					next.fn = next.hn + next.step;
					pq.push(next);
				}
			}
		}
	}
	return init;
}

void MainWindow::RandomClick(wxCommandEvent & evt)
{
	RandomQueen();
	Refresh();
}

void MainWindow::SolveClick(wxCommandEvent & evt)
{
	auto start = std::chrono::high_resolution_clock::now();
	State goal = AStar(init);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	timelabel->SetLabel("Searching time: " + std::to_string(duration.count()/1000000.0) + " seconds");
	while (!goal.solution.empty()) {
		pii temp = goal.solution.front();
		goal.solution.pop();

		init.col[init.queens[temp.first].x]--;
		init.ld[init.queens[temp.first].x - init.queens[temp.first].y]--;
		init.rd[init.queens[temp.first].x + init.queens[temp.first].y]--;

		init.hn -= init.col[init.queens[temp.first].x];
		init.hn -= init.ld[init.queens[temp.first].x - init.queens[temp.first].y];
		init.hn -= init.rd[init.queens[temp.first].x + init.queens[temp.first].y];

		init.queens[temp.first].x = temp.second;

		init.hn += init.col[init.queens[temp.first].x];
		init.hn += init.ld[init.queens[temp.first].x - init.queens[temp.first].y];
		init.hn += init.rd[init.queens[temp.first].x + init.queens[temp.first].y];

		init.col[init.queens[temp.first].x]++;
		init.ld[init.queens[temp.first].x - init.queens[temp.first].y]++;
		init.rd[init.queens[temp.first].x + init.queens[temp.first].y]++;

		heurlabel->SetLabel("Heuristic value: " + std::to_string(init.hn));

		Refresh();
		Update();
		Sleep(1000);
	}
	init = goal;
}

void MainWindow::SetN(wxCommandEvent & evt)
{
	std::string temp = Nbox->GetString(Nbox->GetCurrentSelection());
	int n = std::stoi(temp);
	N = n;
	RandomQueen();
	Refresh();
}
