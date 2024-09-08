#pragma once
#include <wx/wx.h>
#include <wx/msgdlg.h>


class MainWindow : public wxFrame {
public:
    MainWindow(const wxString &title);
    ~MainWindow();


private:
    wxSizer* WindowSizer = nullptr;

    // Since our grid is a square matrix, hence rows
    // columns will be same.
    int RowsInGrid;
    int TotalMines;


public:
    void SetupWindowSizer();



//------------------------ UI and functions for the Main Menu Window ------------------------------

private:
    wxFont* MenuTitleFont = nullptr;
    wxFont* MainFont = nullptr;
    
    wxPanel* MainMenuPanel = nullptr;

    wxSizer* MenuOuterSizer = nullptr;
    wxSizer* MenuSizer = nullptr;
    wxSizer* DifficultySizer = nullptr;
    
    wxStaticText* MenuTitle = nullptr;
    wxStaticText* DifficultyText = nullptr;
    wxButton* EasyMode = nullptr;
    wxButton* MediumMode = nullptr;
    wxButton* HardMode = nullptr;


public:
    void AddControlsAndSizer_ForMainMenu();
    void BindEventHandlers_ForMainMenu();

    inline void SetDifficulty_Easy(wxCommandEvent &evt);
    inline void SetDifficulty_Medium(wxCommandEvent &evt);
    inline void SetDifficulty_Hard(wxCommandEvent &evt);

    void GotoGameScreen();



//------------------------ UI and functions for the Game Window ------------------------------------

private:
    wxPanel* GamePanel = nullptr;
    
    wxSizer* OuterSizer = nullptr;
    wxSizer* MainSizer = nullptr;
    wxSizer* InnerSizer = nullptr;
    wxSizer* GridSizer = nullptr;
    
    wxButton** CellsArray = nullptr;
    wxStaticText* StatusBar = nullptr;
    wxStaticText* StopWatch = nullptr;
    wxMessageDialog* GameOverWindow = nullptr;
    wxMessageDialog* UserWonWindow = nullptr;
    wxTimer* Timer = nullptr;


    int GridSize;

    int* Mines = nullptr;
    bool IsItFirstButtonClick = true;
    int FlagCount;
    int TimerCount = 0;
    int TotalMoves;


public:
    void AddControlsAndSizer_ForGameScreen();
    void BindEventHandlers();
    void UnbindEventHandlers();

    void OnLeftButtonClick(wxCommandEvent &evt);
    void OnRightButtonClick(wxMouseEvent &evt);

    void GenerateMines(int FirstCell_Index);
    void UpdateNeighbourCellsMinesCount(int Cell_Index);
    void RevealCell(int Cell_Index);
    void RevealAllNeighbourCells(int Cell_Index);
    void RemoveFlag_M(int Cell_Index);
    
    inline void UpdateStausBar();
    inline void UpdateStopWatch();
    inline void AfterEverySecond(wxTimerEvent &evt);
    inline  int GetMinesCountAroundCell(int Cell_Index) const;
    inline bool IsThereAnyMine(int Cell_Index) const;
    inline bool IsItValidButton(int x, int y) const;

    void GameOver();
    void UserWon();

    void ResetGame();
};