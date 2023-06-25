#pragma once

#include <QMainWindow>
#include <QPointer>

#include "ui_Editor.h"
#include <SettingsDialog.h>

class EditorWindow : public QMainWindow
{
    

public:
    EditorWindow(QWidget *parent = nullptr);
    ~EditorWindow();

public slots:
    void showSettingsDialog();

private:
    Ui::Editor ui;
};
