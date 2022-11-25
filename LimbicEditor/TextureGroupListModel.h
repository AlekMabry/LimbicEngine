#pragma once

#include <QAbstractListModel>
#include "ResourceManager.h"

class TextureGroupListModel : public QAbstractListModel
{
public:
    TextureGroupListModel(ResourceManager *resources, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private slots:
    void updatedTextureGroups();

private:
    ResourceManager *resources;
};