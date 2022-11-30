#include "ViewportWidget.h"

ViewportWidget::ViewportWidget(QWidget* parent)
{
}

ViewportWidget::~ViewportWidget()
{
}

void ViewportWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}

void ViewportWidget::resizeGL(int w, int h)
{
}

void ViewportWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
}