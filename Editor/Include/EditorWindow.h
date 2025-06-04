#pragma once

#include <QMainWindow>
#include <QPointer>

#include "ui_Editor.h"
#include <SettingsDialog.h>

class NodePropertiesWidget;
class Game;
class VulkanWindow;

class EditorWindow : public QMainWindow
{
    

public:
    EditorWindow(VulkanWindow *pVkWindow, Game* pGame, QWidget *parent = nullptr);
    ~EditorWindow();

public slots:
    void showSettingsDialog();

private:
    Ui::Editor ui;
    VulkanWindow* pVkWindow;
    Game* pGame;
    NodePropertiesWidget* pNodeProperties;
};
