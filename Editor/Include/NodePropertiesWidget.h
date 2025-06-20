#pragma once

#include <Entity/EEntity.h>
#include <RWindow_Qt.h>

#include <QWidget>
#include <QVBoxLayout>

#include "ui_NodePropertiesWidget.h"

class NodePropertiesWidget : public QWidget
{
public:
	NodePropertiesWidget(RWindow_Qt* pVkWindow, QWidget* pParent);
	~NodePropertiesWidget();

public slots:
	void onEntitySelected(EEntity* pEntity);

signals:
	void EntitiesUpdated();

private:
	std::unique_ptr<Ui::NodePropertiesWidget> pUi;
	RWindow_Qt* pVkWindow;
	QVBoxLayout* pEntityLayout;
	QVBoxLayout* pTransformLayout;
	QVBoxLayout* pModelLayout;
	QVBoxLayout* pLightLayout;
};
