#include "TextureGroupListModel.h"

TextureGroupListModel::TextureGroupListModel(ResourceManager* resources, QObject* parent)
    : QAbstractListModel(parent)
{
    this->resources = resources;
    connect(resources, SIGNAL(updatedTextureGroups()), this, SLOT(updatedTextureGroups()));
}

int TextureGroupListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0 || !parent.isValid())
        return 0;
    else
        return resources->getTextureGroupCount();
}

QVariant TextureGroupListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        QString editorPath;
        if (!resources->getTextureGroupPath(index.row(), editorPath));
            return QVariant(editorPath);
    }
    return QVariant();
}

void TextureGroupListModel::updatedTextureGroups() {
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(resources->getTextureGroupCount(), 0);
    emit dataChanged(topLeft, bottomRight);
}