#pragma once

#include <LimbicTypes.h>

#include <QWidget>


class RWindow;
class Game;

class ViewWidget : public QWidget
{
public:
	ViewWidget(Game* pRGame, const std::string& name, QWidget* pParent = nullptr);
	~ViewWidget();

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	void ConfigureMouseEvents();

	Game* pGame;
	RWindow* pRWindow;
	QWindow* pContainerWindow;
	QWidget* pContainerWidget;

	vec3 position;
	float pitch;
	float yaw;

	bool bFlying;	// Right-click drag to move view
	QPoint lastMousePosition;
};