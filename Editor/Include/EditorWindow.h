#pragma once

#include <QMainWindow>
#include <QPointer>

#include "ui_Editor.h"
#include <SettingsDialog.h>

class NodePropertiesWidget;
class Game;
class RWindow_GLFW;

class EditorWindow : public QMainWindow
{
    

public:
    EditorWindow(Game* pGame, QWidget *parent = nullptr);
    ~EditorWindow();

public slots:
    void showSettingsDialog();

private:
    Ui::Editor ui;
    RWindow_GLFW* pWindow;
    Game* pGame;
    NodePropertiesWidget* pNodeProperties;
};
