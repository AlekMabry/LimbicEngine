#pragma once

#include <QWidget>

class RWindow;
class Game;

class ViewWidget : public QWidget
{
public:
	ViewWidget(Game* pRGame, RWindow* pRWindow, QWidget* pParent = nullptr);
	~ViewWidget();

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	Game* pGame;
	RWindow* pRWindow;
	QWindow* pContainerWindow;
	QWidget* pContainerWidget;
};