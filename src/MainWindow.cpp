#pragma once
#include "MainWindow.h"
#include <wx/wx.h>
#include <wx/msgdlg.h>
#include <wx/event.h>
#include <format>
#include <cstdlib>
#include <time.h>


MainWindow::MainWindow(const wxString &title) : wxFrame(nullptr, wxID_ANY, title) {
    MainMenuPanel = new wxPanel(this);
    GamePanel = new wxPanel(this);
    
    AddControlsAndSizer_ForMainMenu();
    BindEventHandlers_ForMainMenu();

    SetupWindowSizer();
}

MainWindow::~MainWindow() {
    if (CellsArray != nullptr){
        delete [] CellsArray;
    }
    
    if (Mines != nullptr){
        delete [] Mines;
    }
}


void MainWindow::SetupWindowSizer() {
    GamePanel -> Show(false);
    
    WindowSizer = new wxBoxSizer(wxHORIZONTAL);
    WindowSizer -> Add(GamePanel, wxSizerFlags().Proportion(1).Expand());
    WindowSizer -> Add(MainMenuPanel, wxSizerFlags().Proportion(1).Expand());

    this -> SetSizer(WindowSizer);
    WindowSizer -> SetSizeHints(this); // The frame now respects the min size of WindowSizer
}




//------------------------ UI and functions for the Main Menu Window ------------------------------

void MainWindow::AddControlsAndSizer_ForMainMenu() {
    // 0 means choose a suitable width.
	// 110 means the font is 110 pixels tall.
    MenuTitleFont = new wxFont(wxFontInfo(FromDIP(wxSize(0, 110)) ) );
    MainFont = new wxFont(wxFontInfo(FromDIP(wxSize(0, 26)) ) );
    
    
    MainMenuPanel -> SetFont(*MainFont);

    MenuTitle = new wxStaticText(MainMenuPanel, wxID_ANY, "Mine Sweeper", 
                        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    MenuTitle -> SetFont(*MenuTitleFont);

    DifficultyText = new wxStaticText(MainMenuPanel, wxID_ANY, "Choose Difficulty :- ", 
                        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);

    EasyMode = new wxButton(MainMenuPanel, wxID_ANY, "Easy", wxDefaultPosition, 
                                (FromDIP(wxSize(220, 20)) ), wxBORDER_NONE);
    MediumMode = new wxButton(MainMenuPanel, wxID_ANY, "Medium", wxDefaultPosition, 
                                (FromDIP(wxSize(220, 20)) ), wxBORDER_NONE);
    HardMode = new wxButton(MainMenuPanel, wxID_ANY, "Hard", wxDefaultPosition, 
                                (FromDIP(wxSize(220, 20)) ), wxBORDER_NONE);


    MenuOuterSizer = new wxBoxSizer(wxVERTICAL);

    MenuSizer = new wxBoxSizer(wxVERTICAL);
    MenuSizer -> Add(MenuTitle, wxSizerFlags().CenterHorizontal().Proportion(2));

    DifficultySizer = new wxBoxSizer(wxHORIZONTAL);
    DifficultySizer -> AddStretchSpacer(2);
    DifficultySizer -> Add(DifficultyText, wxSizerFlags().Center().Proportion(2));
    DifficultySizer -> AddStretchSpacer(5);

    MenuSizer -> Add(DifficultySizer, wxSizerFlags().CenterHorizontal());
    MenuSizer -> AddSpacer(15);
    MenuSizer -> Add(EasyMode, wxSizerFlags().CenterHorizontal().Proportion(1));
    MenuSizer -> AddSpacer(5);
    MenuSizer -> Add(MediumMode, wxSizerFlags().CenterHorizontal().Proportion(1));
    MenuSizer -> AddSpacer(5);
    MenuSizer -> Add(HardMode, wxSizerFlags().CenterHorizontal().Proportion(1));

    MenuOuterSizer -> Add(MenuSizer, wxSizerFlags().Border(wxALL, 25).Proportion(1).Expand());
    
    MainMenuPanel -> SetSizer(MenuOuterSizer);
    MenuOuterSizer -> SetSizeHints(this); // The frame now respects the min size of MenuOuterSizer
    

    MenuTitle -> SetFocus();
}


void MainWindow::BindEventHandlers_ForMainMenu() {
    EasyMode -> Bind(wxEVT_BUTTON, &MainWindow::SetDifficulty_Easy, this);
    MediumMode -> Bind(wxEVT_BUTTON, &MainWindow::SetDifficulty_Medium, this);
    HardMode -> Bind(wxEVT_BUTTON, &MainWindow::SetDifficulty_Hard, this);
}



inline void MainWindow::SetDifficulty_Easy(wxCommandEvent &evt) {
    RowsInGrid = 10;
    TotalMines = 13;
    GotoGameScreen();
}


inline void MainWindow::SetDifficulty_Medium(wxCommandEvent &evt) {
    RowsInGrid = 16;
    TotalMines = 40;
    GotoGameScreen();
}


inline void MainWindow::SetDifficulty_Hard(wxCommandEvent &evt) {
    RowsInGrid = 24;
    TotalMines = 99;
    GotoGameScreen();
}



void MainWindow::GotoGameScreen() {
    wxSize WindowSize = this -> GetClientSize();
    wxPoint WindowPosition = this -> GetPosition();
    
    AddControlsAndSizer_ForGameScreen();
    BindEventHandlers();
    
    
    MainMenuPanel -> Show(false);
    GamePanel -> Show();

    WindowSizer -> SetSizeHints(this); // The frame now respects the min size of WindowSizer
    
    
    this -> SetClientSize(WindowSize);
    this -> SetPosition(WindowPosition);
}




//------------------------ UI and functions for the Game Window ---------------------------------------

void MainWindow::AddControlsAndSizer_ForGameScreen() {    
    // Since our grid is a square matrix, hence rows
    // columns will be same.
    GridSize = RowsInGrid * RowsInGrid;

    FlagCount = TotalMines;
    TotalMoves = GridSize - TotalMines;
    
    
    
    GamePanel -> SetFont(*MainFont);

    OuterSizer = new wxBoxSizer(wxVERTICAL);
    
    MainSizer = new wxBoxSizer(wxVERTICAL);
    MainSizer -> AddSpacer(5);

    InnerSizer = new wxBoxSizer(wxHORIZONTAL);

    StatusBar = new wxStaticText(GamePanel, wxID_ANY, std::format("  Number of Mines:- {}", FlagCount));
    StopWatch = new wxStaticText(GamePanel, wxID_ANY, std::format("Stopwatch:- {}   ", TimerCount), 
                                    wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
    
    InnerSizer -> Add(StatusBar, wxSizerFlags().Proportion(1));
    InnerSizer -> Add(StopWatch, wxSizerFlags().Proportion(1));

    MainSizer -> Add(InnerSizer, wxSizerFlags().Expand());
    MainSizer -> AddSpacer(15);

    GridSizer = new wxGridSizer(RowsInGrid);

    CellsArray = new wxButton* [GridSize];
    for (int i = 0; i < GridSize; i++){
        CellsArray[i] = new wxButton(GamePanel, 1000 + i, "", wxDefaultPosition, FromDIP(wxSize(40, 30)) );
        CellsArray[i] -> SetBackgroundColour(*wxLIGHT_GREY);
        GridSizer -> Add(CellsArray[i], wxSizerFlags().Proportion(1).Expand());
    }

    MainSizer -> Add(GridSizer, wxSizerFlags().Proportion(1).Expand());
    

    Timer = new wxTimer(GamePanel);

    GameOverWindow = new wxMessageDialog(this, "Ooops!!! You stepped on a mine. Game Over !! :( ", "Game Over");
    UserWonWindow = new wxMessageDialog(this, "Congratualations!!! You have uncovered all the mines. :) ", "You Won !!!");

    OuterSizer -> Add(MainSizer, wxSizerFlags().Border(wxALL, 10). Proportion(1).Expand());

    GamePanel -> SetSizer(OuterSizer);
    OuterSizer -> SetSizeHints(this); // The frame now respects the min size of OuterSizer.
}


void MainWindow::BindEventHandlers() {
    GamePanel -> Bind(wxEVT_TIMER, &MainWindow::AfterEverySecond, this);

    for (int i = 0; i < GridSize; i++){
        CellsArray[i] -> Bind(wxEVT_BUTTON, &MainWindow::OnLeftButtonClick, this);
        CellsArray[i] -> Bind(wxEVT_RIGHT_DOWN, &MainWindow::OnRightButtonClick, this);
    }
}


void MainWindow::UnbindEventHandlers() {
    GamePanel -> Unbind(wxEVT_TIMER, &MainWindow::AfterEverySecond, this);

    for (int i = 0; i < GridSize; i++){
        CellsArray[i] -> Unbind(wxEVT_BUTTON, &MainWindow::OnLeftButtonClick, this);
        CellsArray[i] -> Unbind(wxEVT_RIGHT_DOWN, &MainWindow::OnRightButtonClick, this);
    }
}



void MainWindow::OnLeftButtonClick(wxCommandEvent &evt) {
    int Cell_Index = evt.GetId() - 1000;
    StatusBar -> SetFocus();
    
    if (IsItFirstButtonClick) {
        GenerateMines(Cell_Index);
        IsItFirstButtonClick = false;
    }

    if (IsThereAnyMine(Cell_Index)) {
        GameOver();
        return;
    }

    RevealCell(Cell_Index);

    evt.Skip();
}


void MainWindow::OnRightButtonClick(wxMouseEvent &evt) {
    int Cell_Index = evt.GetId() - 1000;

    if (CellsArray[Cell_Index] -> GetLabel() == "") {
        CellsArray[Cell_Index] -> SetLabel("M");
        FlagCount--;
        
    }
    else{
        CellsArray[Cell_Index] -> SetLabel("");
        FlagCount++;
    }

    UpdateStausBar();
}



void MainWindow::GenerateMines(int FirstCell_Index) {
    // Use current time as seed for random generator .
    srand(time(0));

    // This array store the location of mines.
    // If any index has -ve value, that means a mine is present at the cell
    // located on that index.
    // If the value is >= 0, that represents the number of mines around the cell.
    Mines = new int[GridSize] ();

    int TotalMines_Copy = TotalMines;
    while (TotalMines_Copy > 0) {
        int Cell_Index = rand() % GridSize;

        if (Cell_Index != FirstCell_Index && !(IsThereAnyMine(Cell_Index)) ) {
            Mines[Cell_Index] = -1;
            TotalMines_Copy--;

            UpdateNeighbourCellsMinesCount(Cell_Index);
        }
    }

    Timer -> Start(1000);  // Generates Timer events every 1s.
}


void MainWindow::UpdateNeighbourCellsMinesCount(int Cell_Index) {
    // Transforming 1D coordinates into 2D coordinates.
    int x = Cell_Index % RowsInGrid;
    int y = Cell_Index / RowsInGrid;

    for (int i = -1; i <= 1; i++){
        for (int j = -1; j <= 1; j++) {

            if (i == 0 && j == 0) {
                // Adjacent Cell Index is same as Cell_Index.
                // So skip this part.
                continue;
            }

            int NeighbourCellx = x + j;
            int NeighbourCelly = y + i;
            
            // Converting 2D coordinates back to 1D coordinates.
            int NeighbourCellIndex = NeighbourCellx + (RowsInGrid * NeighbourCelly);

            if (IsItValidButton(NeighbourCellx, NeighbourCelly) && !(IsThereAnyMine(NeighbourCellIndex)) ) {
                Mines[NeighbourCellIndex]++;
            }
        }
    }
}


void MainWindow::RevealCell(int Cell_Index) {
    // If the cell has already been revealed, then don't do it again.
    if (CellsArray[Cell_Index] -> GetBackgroundColour() == *wxWHITE) {
        return;
    }
    
    
    CellsArray[Cell_Index] -> SetBackgroundColour(*wxWHITE);
    CellsArray[Cell_Index] -> Unbind(wxEVT_BUTTON, &MainWindow::OnLeftButtonClick, this);
    CellsArray[Cell_Index] -> Unbind(wxEVT_RIGHT_DOWN, &MainWindow::OnRightButtonClick, this);

    // When revealing a cell, remove the flag 'M', if marked by the player.
    RemoveFlag_M(Cell_Index);

    
    int MinesCount = GetMinesCountAroundCell(Cell_Index);

    if (MinesCount > 0) {     
        CellsArray[Cell_Index] -> SetLabel(std::to_string(MinesCount));
    }
    else{
        // When count == 0, that means there are no mines around the cell.
        // This cell is called a whitespace.
        RevealAllNeighbourCells(Cell_Index);
    }


    TotalMoves--;
    if (TotalMoves == 0) {
        UserWon();
    }
}


void MainWindow::RevealAllNeighbourCells(int Cell_Index) {
    // Transforming 1D coordinates into 2D coordinates.
    int x = Cell_Index % RowsInGrid;
    int y = Cell_Index / RowsInGrid;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {

            if (i == 0 && j == 0) {
                // Adjacent Cell Index is same as Cell_Index.
                // So skip this part.
                continue;
            }

            int NeighbourCellx = x + j;
            int NeighbourCelly = y + i;
            
            // Converting 2D coordinates back to 1D coordinates.
            int NeighbourCellIndex = NeighbourCellx + (RowsInGrid * NeighbourCelly);

            if (IsItValidButton(NeighbourCellx, NeighbourCelly)) {
                RevealCell(NeighbourCellIndex);
            }
        }
    }
}


void MainWindow::RemoveFlag_M(int Cell_Index) {
    // Removes the flag 'M' marked by the player on potential mines.
    if (CellsArray[Cell_Index] -> GetLabel() == "M"){
        CellsArray[Cell_Index] -> SetLabel("");
        FlagCount++;
    }

    UpdateStausBar();
}



inline void MainWindow::UpdateStausBar() {
    StatusBar -> SetLabel(std::format("  Number of Mines:- {}", FlagCount));
}


inline void MainWindow::UpdateStopWatch() {
    StopWatch -> SetLabel(std::format("Stopwatch:- {}   ", TimerCount));
}


inline void MainWindow::AfterEverySecond(wxTimerEvent &evt) {
    TimerCount++;
    UpdateStopWatch();
}


inline int MainWindow::GetMinesCountAroundCell(int Cell_Index) const {
    // Returns the number of mines around the Cell.
    return Mines[Cell_Index];
}


inline bool MainWindow::IsThereAnyMine(int Cell_Index) const {
    return Mines[Cell_Index] < 0;
}


inline bool MainWindow::IsItValidButton(int x, int y) const {
    return (x >= 0 && x < RowsInGrid) && (y >= 0 && y < RowsInGrid); 
}



void MainWindow::GameOver() {
    Timer -> Stop();
    GameOverWindow -> ShowModal();
    ResetGame();
}

void MainWindow::UserWon() {
    Timer -> Stop();
    UserWonWindow -> ShowModal();
    ResetGame();
}


void MainWindow::ResetGame() {
    delete [] Mines;
    Mines = nullptr;

    IsItFirstButtonClick = true;
    FlagCount = TotalMines;
    TotalMoves = GridSize - TotalMines;
    TimerCount = 0;

    for (int i = 0; i < GridSize; i++) {
        CellsArray[i] -> SetLabel("");
        CellsArray[i] -> SetBackgroundColour(*wxLIGHT_GREY);
    }
    
    UnbindEventHandlers();
    BindEventHandlers();
    UpdateStausBar();
    UpdateStopWatch();
}