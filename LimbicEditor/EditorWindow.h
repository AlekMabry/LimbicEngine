#pragma once

#include <QMainWindow>
#include <QPointer>

#include "ui_Editor.h"
#include "SettingsDialog.h"
#include "ViewPortWidget.h"

class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    EditorWindow(QWidget *parent = nullptr);
    ~EditorWindow();

public slots:
    void showSettingsDialog();

private:
    Ui::Editor ui;
};
