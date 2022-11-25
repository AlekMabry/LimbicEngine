#pragma once

#include <QMainWindow>
#include <QPointer>

#include "ui_Editor.h"
#include "SettingsDialog.h"
#include "TextureManagerWidget.h"
#include "ViewPortWidget.h"
#include "Map.h"
#include "QSceneTreeModel.h"
#include "ResourceManager.h"

class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    EditorWindow(QWidget *parent = nullptr);
    ~EditorWindow();

public slots:
    void showSettingsDialog();
    void showTextureManager();

private:
    Map *scene;
    QSceneTreeModel *sceneTreeModel;
    std::unique_ptr<ResourceManager> resources;
    Ui::Editor ui;

    QPointer<TextureManagerWidget> textureManagerWidget;

    QMenu *nodeMenu;
    QAction *nodeMenu_new;
    QAction *nodeMenu_delete;

    void onCustomContextMenu(const QPoint &);
};
