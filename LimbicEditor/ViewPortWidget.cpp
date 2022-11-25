#include "ViewPortWidget.h"

ViewPortWidget::ViewPortWidget(QWidget* parent)
{
}

ViewPortWidget::~ViewPortWidget()
{
}

void ViewPortWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	
}

void ViewPortWidget::resizeGL(int w, int h)
{
}

void ViewPortWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
}