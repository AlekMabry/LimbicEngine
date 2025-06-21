#include <Game.h>
#include <Renderer/RView.h>
#include <Renderer/RWindow.h>
#include <ViewWidget.h>

#include <QMouseEvent>
#include <QVBoxLayout>
#include <QWindow>

#define GLM_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

ViewWidget::ViewWidget(Game* pGame, const std::string& name, QWidget* pParent)
	: pGame(pGame), pRWindow(pRWindow), QWidget(pParent), position({-2.0f, 0.0f, 1.8f}), pitch(0.0f), yaw(0.0f), bFlying(false)
{
	setPalette(QColor("#FF0000"));
	setWindowTitle("Main View");
	setLayout(new QVBoxLayout());
	layout()->setContentsMargins(0, 0, 0, 0);
	layout()->setSpacing(0);

	pGame->CreateWin(name, 640, 480);
	pRWindow = pGame->GetWin(name);
	auto win32Handle = pRWindow->GetHandle();

	pContainerWindow = QWindow::fromWinId(WId(win32Handle));
	pContainerWidget = createWindowContainer(pContainerWindow, this);
	pContainerWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	layout()->addWidget(pContainerWidget);

	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);

	ConfigureMouseEvents();
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
	float pitchRad = glm::radians(pitch);
	float yawRad = glm::radians(yaw);
	vec3 direction{
		cos(yawRad) * cos(pitchRad),	// Forward (+X)
		sin(yawRad) * cos(pitchRad),	// Right (+Y)
		sin(pitchRad)					// Up (+Z)
	};
	vec3 right{cos(glm::radians(yaw - 90.0f)), sin(glm::radians(yaw - 90.0f)), 0.0f};
	vec3 up = glm::cross(right, direction);

	mat4 view = glm::lookAt(position, position + direction, up);
	auto pRView = pRWindow->GetRenderView();
	if (pRView)
	{
		pRView->SetViewTransform(view);
		pRView->SetProjection(0.1f, 128.0f, 120.0f);
	}

	pGame->Draw();
	QWidget::paintEvent(event);
}

void ViewWidget::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	pGame->Draw();
}

void ViewWidget::ConfigureMouseEvents()
{
	pRWindow->AddMouseButtonCallback(
		[this](RWindow* window, int button, int action, int /*mods*/)
		{
			if (button == GLFW_MOUSE_BUTTON_RIGHT)
			{
				const auto size = window->GetWindowSize();
				const auto centerX = size.first / 2;
				const auto centerY = size.second / 2;

				if (action == GLFW_PRESS)
				{
					bFlying = true;
					window->SetMousePos(centerX, centerY);
				}
				else
				{
					bFlying = false;
					window->SetMousePos(centerX, centerY);
				}
			}
		});

	pRWindow->AddMousePosCallback(
		[this](RWindow* window, double x, double y)
		{
			const auto size = window->GetWindowSize();
			const auto centerX = size.first / 2;
			const auto centerY = size.second / 2;

			// Call is a repeat when mouse is centered
			if (x == static_cast<double>(centerX) && y == static_cast<double>(centerY))
			{
				return;
			}
			if (bFlying)
			{
				auto dX = centerX - x;
				auto dY = centerY - y;
				yaw += dX * 1.25 * 0.022;
				while (yaw >= 360.0f)
				{
					yaw -= 360.0f;
				}
				while (yaw <= -180.0f)
				{
					yaw += 360.0f;
				}
				pitch += dY * 1.25 * 0.022;
				pitch = std::clamp(pitch, -89.0f, 89.0f);
				update();
				window->SetMousePos(centerX, centerY);
			}
		});
}
