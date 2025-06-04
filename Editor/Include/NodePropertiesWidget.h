#pragma once

#include <Entity/EEntity.h>
#include <VulkanWindow.h>

#include <QWidget>
#include <QVBoxLayout>

#include "ui_NodePropertiesWidget.h"

class NodePropertiesWidget : public QWidget
{
public:
	NodePropertiesWidget(VulkanWindow* pVkWindow, QWidget* pParent);
	~NodePropertiesWidget();

public slots:
	void onEntitySelected(EEntity* pEntity);

signals:
	void EntitiesUpdated();

private:
	std::unique_ptr<Ui::NodePropertiesWidget> pUi;
	VulkanWindow* pVkWindow;
	QVBoxLayout* pEntityLayout;
	QVBoxLayout* pTransformLayout;
	QVBoxLayout* pModelLayout;
	QVBoxLayout* pLightLayout;
};
