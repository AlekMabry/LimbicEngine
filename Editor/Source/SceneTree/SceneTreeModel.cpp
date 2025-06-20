
#include <SceneTree/SceneTreeModel.h>
#include <Game.h>
#include <Entity/EStaticWorldGeometry.h>

#include <QIcon>

bool ENode::operator==(const ENode& rhs) const
{
	return this == &rhs;
}

SceneTreeModel::SceneTreeModel(Game* pGame, QObject* pParent)
	: QAbstractItemModel(pParent), pWorld(pGame->GetWorldSystem())
{
	onWorldUpdated();

	auto pWorld = pGame->GetWorldSystem();
	pWorld->eMapUpdated.append([this]() { onWorldUpdated(); });
}

SceneTreeModel::~SceneTreeModel()
{
}

QModelIndex SceneTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return {};

	const ENode* parentNode;

	if (!parent.isValid())
		parentNode = &root;
	else
		parentNode = static_cast<ENode*>(parent.internalPointer());

	if (row >= 0 && row < parentNode->children.count())
	{
		const auto childNode = &parentNode->children[row];
		return createIndex(row, column, childNode);
	}

	return {};
}

QModelIndex SceneTreeModel::parent(const QModelIndex& child) const
{
	if (!child.isValid())
		return {};

	const auto pChildNode = static_cast<ENode*>(child.internalPointer());
	const auto pParentNode = pChildNode->pParent;

	if (pParentNode == &root || !pParentNode)
		return {};

	const auto childIndex = pParentNode->children.indexOf(*pChildNode);
	return createIndex(childIndex, 0, pParentNode);
}

int SceneTreeModel::rowCount(const QModelIndex& parent) const
{
	const ENode* parentNode;
	if (!parent.isValid())
		parentNode = &root;
	else
		parentNode = static_cast<ENode*>(parent.internalPointer());

	return parentNode->children.size();
}

int SceneTreeModel::columnCount(const QModelIndex& /*parent*/) const
{
	return 1;
}

QVariant SceneTreeModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return {};

	const auto pNode = static_cast<ENode*>(index.internalPointer());

	switch (role)
	{
		case Qt::DisplayRole:
			return QString::fromStdString(pNode->pEntity->name);

		case Qt::DecorationRole:
			if (dynamic_cast<EStaticWorldGeometry*>(pNode->pEntity))
			{
				QIcon icon(":/Icons/SceneTree-Prop");
				return QVariant::fromValue(icon);
			}
			return QIcon(":/Icons/SceneTree-Point");

		default:
			return {};
	}
}

void SceneTreeModel::onWorldUpdated()
{
	root.children.clear();
	for (const auto& entity : pWorld->GetEntities())
	{
		root.children.emplace_back(ENode{entity, &root, {}});
	}
}
