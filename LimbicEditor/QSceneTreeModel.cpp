#include "QSceneTreeModel.h"

QSceneTreeModel::QSceneTreeModel(Node *root, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->root = root;
}

QSceneTreeModel::~QSceneTreeModel() {}

QModelIndex QSceneTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Node *parentNode;

    if (!parent.isValid())
        parentNode = root;
    else
        parentNode = static_cast<Node *>(parent.internalPointer());

    Node *childNode = parentNode->getChild(row);
    if (childNode)
        return createIndex(row, column, childNode);

    return QModelIndex();
}

QModelIndex QSceneTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    Node *childNode = static_cast<Node *>(index.internalPointer());
    Node *parentNode = childNode->getParent();

    if (parentNode == root)
        return QModelIndex();

    return createIndex(parentNode->getIndex(), 0, parentNode);
}

int QSceneTreeModel::rowCount(const QModelIndex &parent) const
{
    Node *parentNode;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentNode = root;
    else
        parentNode = static_cast<Node *>(parent.internalPointer());

    return parentNode->getChildCount();
}

int QSceneTreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant QSceneTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Node *node = static_cast<Node *>(index.internalPointer());

    switch (role) {
    case Qt::DisplayRole:
        return QString::fromStdString(node->name);
        break;

    case Qt::DecorationRole:
        switch (node->getType()) {
        case SNodeType_Point:
            return QIcon(":/Icons/SceneTree-Point");
            break;

        case SNodeType_Prop:
            return QIcon(":/Icons/SceneTree-Prop");
            break;

        case SNodeType_PointLight:
            return QIcon(":/Icons/SceneTree-PointLight");
            break;
        }
        break;
    }

    return QVariant();
}

Qt::ItemFlags QSceneTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

Qt::DropActions QSceneTreeModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

bool QSceneTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Node *parentNode;
    if (!parent.isValid())
        parentNode = root;
    else
        parentNode = static_cast<Node *>(parent.internalPointer());

    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; i++) {
        Node *childNode = parentNode->getChild(row);
        parentNode->deleteChild(childNode);
    }
    endRemoveRows();

    return true;
}

QStringList QSceneTreeModel::mimeTypes() const {
    QStringList types;
    types << "limbiceditor/nodetree";
    return types;
}

bool QSceneTreeModel::canDropMimeData(const QMimeData *data,
                                      Qt::DropAction action,
                                      int row,
                                      int column,
                                      const QModelIndex &parent) const
{
    if (!data->hasFormat("limbiceditor/nodetree"))
        return false;

    return true;
}

QMimeData *QSceneTreeModel::mimeData(const QModelIndexList &indexes) const 
{
    QMimeData *data = new QMimeData;
    QByteArray *buffer = new QByteArray();
    QDataStream *stream = new QDataStream(buffer, QIODevice::ReadWrite);

    for (int i = 0; i < indexes.size(); i++)
    {
        QModelIndex index = indexes.at(i);
        Node *node = static_cast<Node *>(index.internalPointer());
        if (node == nullptr)
            continue;

        Map::serializeTree(node, *stream);
    }

    data->setData("limbiceditor/nodetree", *buffer);
    return data;
}

bool QSceneTreeModel::dropMimeData(
    const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Node *parentNode = static_cast<Node *>(parent.internalPointer());
    if (parentNode == nullptr)
        return false;

    QByteArray buffer = data->data("limbiceditor/nodetree");
    QDataStream *stream = new QDataStream(&buffer, QIODevice::ReadWrite);

    switch (action) {
    case (Qt::CopyAction):
    case (Qt::MoveAction):
        Map::deserializeTree(parentNode, *stream);
        break;
    }

    return true;
}