#pragma once

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>

class ViewportWidget : public QOpenGLWidget, public QOpenGLExtraFunctions
{
	Q_OBJECT

public:
	ViewportWidget(QWidget* parent = nullptr);
	~ViewportWidget();

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
};
