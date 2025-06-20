#pragma once

#include <QMainWindow>
#include <QPointer>

#include "ui_Editor.h"
#include <SettingsDialog.h>

class NodePropertiesWidget;
class Game;
class RWindow_Qt;

class EditorWindow : public QMainWindow
{
    

public:
    EditorWindow(RWindow_Qt *pVkWindow, Game* pGame, QWidget *parent = nullptr);
    ~EditorWindow();

public slots:
    void showSettingsDialog();

private:
    Ui::Editor ui;
    RWindow_Qt* pVkWindow;
    Game* pGame;
    NodePropertiesWidget* pNodeProperties;
};
