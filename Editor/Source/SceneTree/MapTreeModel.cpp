
#include <SceneTree/MapTreeModel.h>
#include <Game.h>
#include <Entity/EStaticWorldGeometry.h>

#include <QIcon>

bool ENode::operator==(const ENode& rhs) const
{
	return this == &rhs;
}

MapTreeModel::MapTreeModel(Game* pGame, QObject* pParent)
	: QAbstractItemModel(pParent), pWorld(pGame->GetWorldSystem())
{
	onWorldUpdated();

	auto pWorld = pGame->GetWorldSystem();
	pWorld->eMapUpdated.append([this]() { onWorldUpdated(); });
}

MapTreeModel::~MapTreeModel()
{
}

QModelIndex MapTreeModel::index(int row, int column, const QModelIndex& parent) const
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

QModelIndex MapTreeModel::parent(const QModelIndex& child) const
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

int MapTreeModel::rowCount(const QModelIndex& parent) const
{
	const ENode* parentNode;
	if (!parent.isValid())
		parentNode = &root;
	else
		parentNode = static_cast<ENode*>(parent.internalPointer());

	return parentNode->children.size();
}

int MapTreeModel::columnCount(const QModelIndex& /*parent*/) const
{
	return 1;
}

QVariant MapTreeModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return {};

	const auto pNode = static_cast<ENode*>(index.internalPointer());

	switch (role)
	{
		case Qt::DisplayRole:
			if (pNode->pEntity->GetName().empty())
			{
				QString entityNamePlaceholder = {"Entity "};
				entityNamePlaceholder += QString::number(pNode->pEntity->GetID());
				return entityNamePlaceholder;
			}
			return QString::fromStdString(pNode->pEntity->GetName());

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

void MapTreeModel::onWorldUpdated()
{
	root.children.clear();
	for (const auto& entity : pWorld->GetEntities())
	{
		root.children.emplace_back(ENode{entity, &root, {}});
	}
}
