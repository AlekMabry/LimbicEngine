#include <EditorWindow.h>
#include "VulkanWindow.h"
#include <QMdiSubWindow>

EditorWindow::EditorWindow(VulkanWindow *pVkWindow, QWidget* parent) : QMainWindow(parent), pVkWindow(pVkWindow)
{
	// Window config
	setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	ui.setupUi(this);
	showMaximized();

	auto *vkWidget = createWindowContainer(pVkWindow);
	vkWidget->setMinimumSize(640, 480);
	setCentralWidget(vkWidget);
	//pVkWindow->setTitle("Scene View");
	//auto subWindow = ui.mdiArea->addSubWindow(pVkWindow);

	//subWindow->show();

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