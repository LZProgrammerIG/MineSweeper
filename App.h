#pragma once
#include <wx/wx.h>
#include "MainWindow.h"

class App : public wxApp {
public:
    virtual bool OnInit();

    MainWindow* window = nullptr;
};