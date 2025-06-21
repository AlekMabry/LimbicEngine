#include <NodePropertiesWidget.h>
#include <SceneTree/MapTreeModel.h>
#include <Game.h>
#include <Entity/EEntity.h>
#include <LimbicTypes.h>

#include "ui_NodePropertiesWidget.h"

#include <qspinbox.h>

#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

NodePropertiesWidget::NodePropertiesWidget(Game* pGame, QWidget* pParent)
	: QWidget(pParent), pUi(std::make_unique<Ui::NodePropertiesWidget>()), pGame(pGame)
{
	pUi->setupUi(this);

	pEntityLayout = new QVBoxLayout();
	pUi->entityScrollAreaContents->setLayout(pEntityLayout);
	pTransformLayout = new QVBoxLayout();
	pUi->transformScrollAreaContents->setLayout(pTransformLayout);
	pModelLayout = new QVBoxLayout();
	pUi->modelScrollAreaContents->setLayout(pModelLayout);
	pLightLayout = new QVBoxLayout();
	pUi->lightScrollAreaContents->setLayout(pLightLayout);
}

NodePropertiesWidget::~NodePropertiesWidget()
{
}

QSize NodePropertiesWidget::minimumSizeHint() const
{
	return {200, 300};
}

void NodePropertiesWidget::onEntitySelected(EEntity* pEntity)
{
	while (auto* pItem = pTransformLayout->takeAt(0))
	{
		if (auto* pWidget = pItem->widget())
			pWidget->deleteLater();
	}

	if (!pEntity)
	{
		//pUi->transformTab->hide();
		//pUi->modelTab->hide();
		//pUi->lightTab->hide();
		return;
	}

	auto properties = pEntity->GetPropertyInfo();
	for (auto& propPair : properties)
	{
		auto& prop = propPair.second;

		switch (prop.type)
		{
			case ePropertyTypeVec3:
				{
				auto pGroupBox = new QGroupBox(QString::fromStdString(prop.label));
				auto pLayout = new QFormLayout();
				pGroupBox->setLayout(pLayout);
				auto pVec = static_cast<glm::vec3*>(prop.handle);

				auto xBox = new QDoubleSpinBox();
				xBox->setValue(pVec->x);
				connect(xBox, &QDoubleSpinBox::valueChanged,
					[=](double d)
					{
						pVec->x = d;
						pGame->Draw();
					});
				pLayout->insertRow(0, "X:", xBox);

				auto yBox = new QDoubleSpinBox();
				yBox->setValue(pVec->y);
				connect(yBox, &QDoubleSpinBox::valueChanged,
					[=](double d)
					{
						pVec->y = d;
						pGame->Draw();
					});
				pLayout->insertRow(0, "Y:", yBox);

				auto zBox = new QDoubleSpinBox();
				zBox->setValue(pVec->z);
				connect(zBox, &QDoubleSpinBox::valueChanged,
					[=](double d)
					{
						pVec->z = d;
						pGame->Draw();
					});
				pLayout->insertRow(0, "Z:", zBox);
				pTransformLayout->addWidget(pGroupBox);
				}
				break;
				
			default:
				break;
		}
	}
	pTransformLayout->addStretch();

}
