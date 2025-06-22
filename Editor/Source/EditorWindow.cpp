#include <EditorWindow.h>
#include <QMdiSubWindow>
#include <QWindow>
#include <Game.h>

#include <NodePropertiesWidget.h>
#include <MapTreeModel.h>
#include <ViewWidget.h>

EditorWindow::EditorWindow(Game* pGame, QWidget* parent) : QMainWindow(parent), pGame(pGame)
{
	// Window config
	setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	ui.setupUi(this);
	showMaximized();

	ui.mapTree->setModel(new MapTreeModel(pGame, this));

	pNodeProperties = new NodePropertiesWidget(pGame, this);
	ui.nodePropertiesDockLayout->layout()->addWidget(pNodeProperties);
	connect(ui.mapTree, &QAbstractItemView::clicked, [=](const QModelIndex& index)
	{
		auto* pENode = static_cast<ENode*>(index.internalPointer());
		pNodeProperties->onEntitySelected(pENode->pEntity);
	});

	connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));

	auto pViewWidget = new ViewWidget(pGame, "perspective", this);
	setCentralWidget(pViewWidget);
	pViewWidget->show();

	pGame->LoadMap("");
}

EditorWindow::~EditorWindow()
{
}

void EditorWindow::showSettingsDialog()
{
	SettingsDialog* settingsDialog = new SettingsDialog(this);
	settingsDialog->setModal(true);
	settingsDialog->show();
}
