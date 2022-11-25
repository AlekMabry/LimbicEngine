#include "EditorWindow.h"

EditorWindow::EditorWindow(QWidget *parent) : QMainWindow(parent)
{
	// Window config
	setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	ui.setupUi(this);
	showMaximized();

	// Context menus
    nodeMenu = new QMenu("Node Options", this);
    nodeMenu->setContextMenuPolicy(Qt::ActionsContextMenu);
    nodeMenu_new = new QAction(tr("New"), this);
    nodeMenu_delete = new QAction(tr("Delete"), this);
    nodeMenu->addAction(nodeMenu_new);
    nodeMenu->addAction(nodeMenu_delete);

	scene = new Map();
	sceneTreeModel = new QSceneTreeModel(scene->getRoot());
	ui.sceneTree->setModel(sceneTreeModel);

    resources = std::make_unique<ResourceManager>(this);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(onCustomContextMenu(const QPoint &)));

	connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
    connect(ui.actionTexture_Manager, SIGNAL(triggered()), this, SLOT(showTextureManager()));
}

EditorWindow::~EditorWindow()
{}

void EditorWindow::showSettingsDialog() {
    SettingsDialog *settingsDialog = new SettingsDialog(this);
    settingsDialog->setModal(true);     // Fire and forget
    settingsDialog->show();
}

void EditorWindow::showTextureManager() {
    if (textureManagerWidget.isNull())
    {
        textureManagerWidget = new TextureManagerWidget(resources.get(), this);
        ui.mdiArea->addSubWindow(textureManagerWidget);
    }

    textureManagerWidget->show();
}

void EditorWindow::onCustomContextMenu(const QPoint &point)
    {
    //QModelIndex index = ui.sceneTree->indexAt(point);
    //if (index.isValid() && index.row() % 2 = 0)
	//{
    //nodeMenu->exec(mapToGlobal(point)); //ui.sceneTree->viewport()->mapToGlobal(point));
	//}
}