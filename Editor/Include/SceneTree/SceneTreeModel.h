#pragma once

#include <System/WorldSystem.h>

#include <QAbstractItemModel>

struct ENode
{
	EEntity* pEntity = nullptr;
	ENode* pParent = nullptr;
	QList<ENode> children;

	bool operator==(const ENode& rhs) const;
};

class SceneTreeModel : public QAbstractItemModel
{
public:
	SceneTreeModel(Game* pGame, QObject* pParent = nullptr);
	~SceneTreeModel();
	QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	QModelIndex parent(const QModelIndex& child) const override;
	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;

public slots:
	/* Reloads entire tree. */
	void onWorldUpdated();

private:
	WorldSystem* pWorld;
	ENode root;
};