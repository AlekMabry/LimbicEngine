#pragma once

#include <Entity/EEntity.h>
#include <QWidget>
#include <QVBoxLayout>

#include "ui_NodePropertiesWidget.h"

class NodePropertiesWidget : public QWidget
{
public:
	NodePropertiesWidget(Game* pGame, QWidget* pParent);
	~NodePropertiesWidget();
	QSize minimumSizeHint() const override;

public slots:
	void onEntitySelected(EEntity* pEntity);

signals:
	void EntitiesUpdated();

private:
	std::unique_ptr<Ui::NodePropertiesWidget> pUi;
	Game* pGame;
	QVBoxLayout* pEntityLayout;
	QVBoxLayout* pTransformLayout;
	QVBoxLayout* pModelLayout;
	QVBoxLayout* pLightLayout;
};
