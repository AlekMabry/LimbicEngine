#include <ViewWidget.h>
#include <Game.h>
#include <Renderer/RWindow.h>

#include <QWindow>
#include <QVBoxLayout>

ViewWidget::ViewWidget(Game* pGame, RWindow* pRWindow, QWidget* pParent)
	: pGame(pGame), pRWindow(pRWindow), QWidget(pParent)
{
	setPalette(QColor("#FF0000"));
	setWindowTitle("Main View");
	setLayout(new QVBoxLayout());
	layout()->setContentsMargins(0, 0, 0, 0);
	layout()->setSpacing(0);

	pContainerWindow = QWindow::fromWinId(WId(pRWindow->GetHandle()));
	pContainerWidget = createWindowContainer(pContainerWindow, this);
	pContainerWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	layout()->addWidget(pContainerWidget);
}

ViewWidget::~ViewWidget()
{
}

QSize ViewWidget::minimumSizeHint() const
{
	return {640, 480};
}

QSize ViewWidget::sizeHint() const
{
	return {640, 480};
}

void ViewWidget::paintEvent(QPaintEvent* event)
{
	pGame->Draw();
	QWidget::paintEvent(event);
}

void ViewWidget::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	pGame->Draw();
}
