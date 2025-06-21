#include <EditorWindow.h>
#include <QMdiSubWindow>
#include <QWindow>
#include <Game.h>

#include <Renderer/RWindow_GLFW.h>
#include <NodePropertiesWidget.h>
#include <SceneTree/SceneTreeModel.h>
#include <ViewWidget.h>

EditorWindow::EditorWindow(Game* pGame, QWidget* parent) : QMainWindow(parent), pGame(pGame)
{
	// Window config
	setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	ui.setupUi(this);
	showMaximized();

	ui.mapTree->setModel(new SceneTreeModel(pGame, this));

	pNodeProperties = new NodePropertiesWidget(pGame, this);
	ui.nodePropertiesDockLayout->layout()->addWidget(pNodeProperties);
	connect(ui.mapTree, &QAbstractItemView::clicked, [=](const QModelIndex& index)
	{
		auto* pENode = static_cast<ENode*>(index.internalPointer());
		pNodeProperties->onEntitySelected(pENode->pEntity);
	});

	connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
}

EditorWindow::~EditorWindow()
{
}

void EditorWindow::AttachGameWindow(RWindow_GLFW* pWindow)
{
	auto pViewWidget = new ViewWidget(pGame, pWindow, this);
	setCentralWidget(pViewWidget);
	pViewWidget->show();
}

void EditorWindow::showSettingsDialog()
{
	SettingsDialog* settingsDialog = new SettingsDialog(this);
	settingsDialog->setModal(true);
	settingsDialog->show();
}
