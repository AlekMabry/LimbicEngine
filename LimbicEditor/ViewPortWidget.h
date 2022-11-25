#pragma once

#include <QOpenGLExtraFunctions>
#include <qopenglwidget.h>

class ViewPortWidget : public QOpenGLWidget, public QOpenGLExtraFunctions
{
	Q_OBJECT

public:
	ViewPortWidget(QWidget *parent = nullptr);
	~ViewPortWidget();

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
};

