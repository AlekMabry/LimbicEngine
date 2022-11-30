#include "EditorWindow.h"

EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent)
{
	// Window config
	setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	ui.setupUi(this);
	showMaximized();

	// Connect form to events
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