#include <EditorWindow.h>
#include "RWindow_Qt.h"
#include <QMdiSubWindow>
#include <RView_Qt.h>

#include <NodePropertiesWidget.h>
#include <SceneTree/SceneTreeModel.h>

EditorWindow::EditorWindow(RWindow_Qt* pVkWindow, Game* pGame, QWidget* parent) : QMainWindow(parent), pGame(pGame), pVkWindow(pVkWindow)
{
	// Window config
	setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	ui.setupUi(this);
	showMaximized();

	ui.mapTree->setModel(new SceneTreeModel(pGame, this));

	pNodeProperties = new NodePropertiesWidget(pVkWindow, this);
	ui.nodePropertiesDockLayout->layout()->addWidget(pNodeProperties);
	connect(ui.mapTree, &QAbstractItemView::clicked, [=](const QModelIndex& index)
	{
		auto* pENode = static_cast<ENode*>(index.internalPointer());
		pNodeProperties->onEntitySelected(pENode->pEntity);
	});

	// Game window
	//connect(pNodeProperties, &NodePropertiesWidget::EntitiesUpdated, [=]() {
	//	pVkWindow->RequestUpdate();
	//});
	auto* vkWidget = createWindowContainer(pVkWindow);
	vkWidget->setMinimumSize(640, 480);
	setCentralWidget(vkWidget);

	connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
}

EditorWindow::~EditorWindow()
{
}

void EditorWindow::showSettingsDialog()
{
	SettingsDialog* settingsDialog = new SettingsDialog(this);
	settingsDialog->setModal(true);	   // Fire and forget, it will delete itself
	settingsDialog->show();
}