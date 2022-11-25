#pragma once

#include "Node.h"
#include "Map.h"
#include <QAbstractItemModel>
#include <QIcon>
#include <QMimeData>
#include <QIODevice>

class QSceneTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    QSceneTreeModel(Node *root, QObject *parent = nullptr);
 
    ~QSceneTreeModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
 
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    Qt::DropActions supportedDropActions() const;

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    QStringList mimeTypes() const;

    bool canDropMimeData(const QMimeData *data,
                         Qt::DropAction action,
                         int row,
                         int column,
                         const QModelIndex &parent) const;

    QMimeData *mimeData(const QModelIndexList &indexes) const;

    bool dropMimeData(const QMimeData *data,
                      Qt::DropAction action,
                      int row,
                      int column,
                      const QModelIndex &parent);

private:
    Node *root;
};
